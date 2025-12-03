# State Machine Design Guide

## Deep Dive — Adding a New State vs. Keeping a Flag

### 1. The Four-Question Litmus Test ("S-M-O-T")

| Q-No. | Question                                                                                                           | **Yes →**     | **No →**    |
| ----- | ------------------------------------------------------------------------------------------------------------------ | ------------- | ----------- |
| **S** | *Is the **S**ystem's externally visible behavior different?* <br> ▸ outputs change <br> ▸ accepted commands change | **New State** | likely Flag |
| **M** | *Is the condition **M**utually exclusive?* <br> You cannot be in A **and** B simultaneously.                       | **New State** | Flag        |
| **O** | *Does it impose a temporary **O**utside-observable "mode"?* <br> e.g. "Maintenance", "Safe", "Low-Power"           | **New State** | Flag        |
| **T** | *Does it change **T**ransition rules themselves?* <br> Different set of legal next states / events.                | **New State** | Flag        |

If you answered **Yes** to any → favour a **state**; all **No** → keep a flag/variable.

### 2. Cost/Benefit Matrix

| Factor           | New State                             | Flag                                                |
| ---------------- | ------------------------------------- | --------------------------------------------------- |
| **Readability**  | + Clear diagrams, self-documenting    | – Requires discipline to avoid "flag soup"          |
| **Branch Count** | – More states to test                 | + Fewer states, but extra `if` paths                |
| **Scalability**  | + Scales; adding new mode = new state | – Flags compound (2 flags → up to 4 implicit modes) |
| **RAM/ROM**      | ± Slight increase (extra handler)     | + Zero extra code if reusing branch                 |
| **Runtime**      | + No per-tick checks (event driven)   | – Polling/branch may run every event                |
| **Debug Trace**  | + Easy: "A → B"                       | – Need extra prints for flag combinations           |

### 3. Classic Embedded Patterns

| Domain               | Good **State** Examples                | Good **Flag** Examples                 |
| -------------------- | -------------------------------------- | -------------------------------------- |
| **Communications**   | `WAIT_FOR_ACK`, `HANDSHAKE`, `ERROR`   | `retry_count`, `is_encrypted`          |
| **Power Management** | `SLEEP`, `DEEPSLEEP`, `ACTIVE`         | `low_battery_warning`                  |
| **Bootloaders**      | `CHECK_FW`, `ERASE`, `WRITE`, `VERIFY` | `crc_passed` flag inside VERIFY        |
| **UI Menu**          | `SETTINGS_MENU`, `CALIBRATION_MODE`    | `backlight_on` bit used in many states |
| **Motor Control**    | `HOMING`, `RUNNING`, `FAULT`           | `direction = DIR_FWD/REV`              |

### 4. Anti-Patterns & Smells

| Smell                                                                       | Why it hurts                                         | Fix                                                        |
| --------------------------------------------------------------------------- | ---------------------------------------------------- | ---------------------------------------------------------- |
| **Flag Explosion** <br> 3-4 booleans tracked everywhere                     | 8–16 implicit modes, impossible to test exhaustively | Collapse into explicit states                              |
| **Hidden Transitions** <br> Flag is set in ISR, a different module polls it | Hard to trace, introduces timing races               | Turn ISR write into queued event triggering a state change |
| **Mode + Sub-mode both flags** <br> e.g. `connected` & `streaming`          | "Are we Connected *and* Streaming?" unclear          | Make `STREAMING` a child or separate state                 |
| **Timeout handled in while-loop**                                           | Burns CPU and hides timing                           | Use timer-event → possibly new state (`WAIT_FOR_ACK`)      |

### 5. Design-Time Decision Tree

```
          ┌──────── Is behaviour externally different? ─────────┐
          │               (outputs / accepted cmds)             │
          └──── Yes ──►  Add a State  ◄── No ─────┬──────────────
                                                  │
                             Is the condition mutually exclusive?
                                                  │
                                       Yes  ──►  State
                                       No   ───► Continue
                                                  │
             Does it block or gate other events until cleared?
                                       │
                            Yes ──►  State      No ──► Flag
```

### 6. Real-World Embedded Example

**Scenario:**
A GNSS module is normally in `FIXED` or `SEARCHING`. During over-temperature it must cease RF transmission, show a warning, and refuse configuration commands until temperature returns to normal.

| Criterion           | Evaluation                       | Decision  |
| ------------------- | -------------------------------- | --------- |
| Visible behaviour?  | Yes – RF OFF, error flag to host | New state |
| Mutually exclusive? | Can't be FIXED & OverTemp        | New state |
| Blocks commands?    | Yes – reject most AT cmds        | New state |
| Result              | **Add `OVERTEMP_PROTECT` state** |           |

*Implementation:*
`SEARCHING` → **`OVERTEMP_PROTECT`** (on `EVT_TEMP_HIGH`)
Entry: shut RF, start polling temp every 1 s
Internal: ignore nav events
Exit: re-enable RF, resume previous state

### 7. Checklist for Flags That *Are* Okay

1. **Doesn't change state diagram** (e.g., LED pending, debug prints).
2. **Read by exactly one region of code** (encapsulated).
3. **Orthogonal** – same flag applies in several states identically.
4. **No timing dependency** – it's fine if cleared a little later.
5. **Bounded lifetime** – e.g., local to one handler, not global forever.

If any violated → rethink as separate state(s).

### 8. Migrating from Flags to States Safely

1. **Catalogue current flag combos** (truth table).
2. **Group combos into sensible, exclusive modes**.
3. Create new states & entry/exit logic.
4. Route every event to *exactly one* handler per state.
5. Delete obsoleted flag checks to prevent "zombie paths."
6. Regression test previously impossible combos: they should now be *un-representable.*

### 9. Cheat-Sheet Summary

> *When behaviour–or–rules change, think "state."
> When only numbers or minor toggles vary, think "flag."*

Keep the number of states **minimal but sufficient**; keep flags **orthogonal and contained**.

---

# Comprehensive Guide to Event-Driven State Machines in C

---

## 1  Why State Machines?

* **Clarity:** Every mutually-exclusive mode is a state box, every trigger is an arrow
* **Predictability:** Deterministic reactions to well-defined events
* **Testability:** Inject events, assert next state and side-effects
* **Low power:** CPU sleeps until an event arrives (no busy loops)

---

## 2  New State vs. Internal Flag

| Ask yourself…                                     | If **YES** → **New State** | If **NO** → **Flag / variable**           |
| ------------------------------------------------- | -------------------------- | ----------------------------------------- |
| Does behaviour (allowed inputs/outputs) change?   | Connected → Wait For ACK   | `retry_count++` inside VERIFYING          |
| Is the condition mutually exclusive?              | Error vs. Normal           | Debug-enabled can coexist with any state  |
| Will you block/ pause until the condition clears? | Wait For User Ack          | Low-power indicator while still CONNECTED |
| Would you draw a new box on the diagram?          | Yes → state                | No → flag                                 |

**Heuristic:**

> *A new box → new state. A note on the box → flag.*

---

## 3  Event-Driven Philosophy

* **Only discrete events advance logic** – timer expiry, IRQ, message, button press
* **No polling inside states**

  ```c
  /* ❌ polling style */
  while (state == WAIT_FOR_ACK) {
      if (ack_received()) transition_to(CONNECTED);
  }

  /* ✅ event style */
  on_event(EVT_ACK_RECEIVED) { transition_to(CONNECTED); }
  ```

* **Outputs** = side-effects performed (sending frames, toggling GPIO, logging)
* **Inputs** = posted events; between events the FSM does *nothing*.

---

## 4  Transition Types

| Type                              | When to use             | Entry/Exit called?            |
| --------------------------------- | ----------------------- | ----------------------------- |
| **External** (`A → B`)            | Real mode change        | Yes (`A.exit` then `B.enter`) |
| **Internal** (handled *inside* A) | Small action, stay in A | No                            |
| **Self** (`A → A`)                | Need full re-init of A  | Yes (`A.exit`, `A.enter`)     |

---

## 5  Design Pattern with Timers (Descriptor Table)

### 5.1 Core Enums

```c
typedef enum { STATE_IDLE, STATE_CONNECTED, STATE_WAIT_ACK, STATE_ERROR, STATE_COUNT } State;
typedef enum { EVT_NONE, EVT_START, EVT_ACK_RX, EVT_TX_TIMEOUT, EVT_RETRY_TIMEOUT, EVT_FATAL } Event;
```

### 5.2 State "v-table"

```c
typedef struct StateMachine StateMachine;
typedef void (*StateFn)(StateMachine*, Event);
typedef void (*ActionFn)(StateMachine*);

typedef struct {
    State     id;
    StateFn   on_event;
    ActionFn  on_enter;
    ActionFn  on_exit;
    uint32_t  timeout_ms;     /* 0 = no auto-timeout            */
    Event     timeout_evt;    /* event fired when that timer hits*/
} StateDesc;
```

### 5.3 FSM Context

```c
struct StateMachine {
    const StateDesc *tbl;     /* descriptor array               */
    State            cur;     /* current state                  */
    uint8_t          retries; /* user context                   */
    /* …buffers, handles, etc… */
};
```

### 5.4 Lightweight Timer → Event

```c
void timer_start(uint32_t ms, Event ev);   /* posts ev on expiry */
void timer_cancel(Event ev);
```

### 5.5 Handlers (only per-state logic)

```c
static void idle_ev(StateMachine *sm, Event e) {
    if (e == EVT_START) sm->cur = STATE_CONNECTED;
}

static void conn_enter(StateMachine *sm) {
    start_tx();
    timer_start(2000, EVT_TX_TIMEOUT);
}
static void conn_exit (StateMachine *sm) {
    timer_cancel(EVT_TX_TIMEOUT);
    stop_tx();
}
static void conn_ev(StateMachine *sm, Event e) {
    switch (e) {
        case EVT_ACK_RX: sm->retries = 0; break;
        case EVT_TX_TIMEOUT: sm->cur = STATE_WAIT_ACK; break;
        case EVT_FATAL: sm->cur = STATE_ERROR; break;
        default: break;
    }
}

static void wait_ev(StateMachine *sm, Event e) {
    if (e == EVT_ACK_RX)        sm->cur = STATE_CONNECTED;
    else if (e == EVT_RETRY_TIMEOUT) {
        if (++sm->retries >= 3) sm->cur = STATE_ERROR;
        else retransmit();
    }
}
```

### 5.6 Descriptor Array

```c
static const StateDesc g_states[STATE_COUNT] = {
 [STATE_IDLE]      = { STATE_IDLE,      idle_ev,  NULL,        NULL,        0,               EVT_NONE },
 [STATE_CONNECTED] = { STATE_CONNECTED, conn_ev,  conn_enter,  conn_exit,   2000,            EVT_TX_TIMEOUT },
 [STATE_WAIT_ACK]  = { STATE_WAIT_ACK,  wait_ev,  NULL,        NULL,        1000,            EVT_RETRY_TIMEOUT },
 /* STATE_ERROR … */
};
```

### 5.7 Central Dispatcher

```c
static void dispatch(StateMachine *sm, Event e)
{
    State old = sm->cur;
    g_states[old].on_event(sm, e);

    State new = sm->cur;
    if (new != old) {
        if (g_states[old].on_exit)  g_states[old].on_exit(sm);
        if (g_states[new].on_enter) g_states[new].on_enter(sm);
    }
}
```

### 5.8 Main Loop Skeleton

```c
int main(void)
{
    StateMachine sm = { .tbl = g_states, .cur = STATE_IDLE };
    if (g_states[STATE_IDLE].on_enter) g_states[STATE_IDLE].on_enter(&sm);

    for (;;) {
        Event e;
        if (event_get(&e)) dispatch(&sm, e);
        /* idle / sleep / background work */
    }
}
```

---

## 6  Testing & Debugging Checklist

1. **Unit tests**: feed synthetic event sequences, assert `sm.cur` and mock side-effects.
2. **Trace log** in `dispatch`: `"OLD --(EVT)--> NEW"` → perfect for UART/JTAG console.
3. **Coverage**: each state, each event, each timeout path.
4. **Static analysis**: make sure every `(state,event)` combo is handled (MISRA switch-exhaustive).

---

## 7  Choosing Your Implementation Style

| Project size / complexity                              | Recommended pattern                                  |
| ------------------------------------------------------ | ---------------------------------------------------- |
| 2-5 states, 8-bit µC                                   | Simple `switch(state)` inline                        |
| 5-20 states, timers, unit tests                        | **Descriptor table** (this guide)                    |
| Deep nesting, reuse of substates, safety certification | Hierarchical State Machine (HSM) library or code-gen |

---

## 8  Best-Practice Checklist

* One **owner** of `current_state` (the dispatcher).
* **Never** modify state directly in IRQs — just `event_post()`.
* **Entry/Exit** do all setup/cleanup; handlers stay focused.
* Treat **timers** as events; avoid while-loops waiting on flags.
* When in doubt: *"Would this be a new box?"* → new state.

---

**You now have a turnkey template**: copy the enums, table, dispatcher, and fill in your own events, timeouts, and side-effects.
Need further tweaks (logging macros, Ceedling tests, RTOS timer hook)? Just ask!
