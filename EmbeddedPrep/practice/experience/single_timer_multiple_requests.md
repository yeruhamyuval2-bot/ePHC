### 🕒 Multi-Timer Mechanisms with **One** Hardware Timer — Quick Reference

A distilled recap of our deep-dive (FreeRTOS → Zephyr → Linux) and the supporting data-structure patterns.

---

#### 1. Core Idea in One Sentence

> **Program the hardware timer for “next-soonest deadline”, run all due callbacks in its ISR or a service task, then re-arm it for the next deadline.**
> No absolute wall-clock required—only monotonically counting ticks.

---

#### 2. Canonical Software Data-Structures

| Pattern                          | Complexity (insert / expiry)   | Sweet-Spot                                     | Key Trade-Offs                                                |
| -------------------------------- | ------------------------------ | ---------------------------------------------- | ------------------------------------------------------------- |
| **Delta-encoded linked list**    | O(n) / O(1)                    | ≤ \~30 timers, tiny MCUs                       | Minimal RAM, but insertion cost grows with timer count        |
| **Min-heap / binary heap**       | O(log n) / O(log n) \*root pop | 10 – 100s of timers (FreeRTOS software timers) | Fast “next-soonest”; cancel/update needs pointer-to-index map |
| **Timer wheel** (array of lists) | O(1) / O(1)                    | Many short (< range) timers                    | Range limited by wheel size; periodic tick walk               |
| **Hierarchical timer wheel**     | Amortized O(1)                 | 1000s of timers, networking stacks             | Extra wheel levels add code & RAM                             |
| **Red-black tree**               | O(log n) / O(log n)            | High-res, arbitrary future deadlines           | More RAM; harder to implement, but O(log n) cancel/update     |

---

#### 3. Real-World OS Implementations

| OS / API                           | Data-Structure                               | Timer Source        | Notes                                                        |
| ---------------------------------- | -------------------------------------------- | ------------------- | ------------------------------------------------------------ |
| **FreeRTOS**<br>• `vTaskDelay()`   | Single-level **timer wheel** (list per tick) | `SysTick`           | Ultra-cheap task sleeps; range ≤ `configMAX_DELAY`           |
| **FreeRTOS**<br>• `xTimerCreate()` | **Min-heap** in `TimerServiceTask`           | Same tick           | Fine-grained user callbacks; periodic & one-shot             |
| **Zephyr RTOS**                    | **Red-black tree** (`sys_clock`)             | 1 HPET/RTC          | Tickless idle: tree’s leftmost node ⇒ delta for next wake    |
| **Linux (classic `timer_list`)**   | **Hierarchical wheel** (4 levels)            | per-CPU LAPIC       | Coarse “jiffies” timers for drivers, TCP, etc.               |
| **Linux `hrtimer`**                | **Per-CPU red-black tree**                   | HPET / TSC deadline | µs / ns granularity; powers POSIX `nanosleep`, RT scheduling |

---

#### 4. Choosing the Right Pattern

| When you need…                                         | Pick                       |
| ------------------------------------------------------ | -------------------------- |
| **Tiny RAM** & handful of timers                       | Delta list                 |
| **Deterministic O(1)** for 100s timers, short timeouts | (Hierarchical) timer wheel |
| **Arbitrary long delays** *and* cheap cancel/update    | Red-black tree             |
| **Middle-ground, moderate timer count**                | Min-heap                   |

---

#### 5. Practical Tips & Pitfalls

* **Wrap-around safety:** reload the down-counter each time ⇒ underlying 32-bit overflow is irrelevant.
* **High-frequency callbacks (< tick period):** Use a *separate* peripheral timer ISR, not the RTOS tick.
* **Power saving:** Tickless idle works best with structures that give “next deadline” quickly (heap or RB-tree).
* **Long-range timers on a single wheel:** add a *rotation counter* or move to hierarchical design.
* **Cancellation performance:** If frequent, prefer RB-tree (O(log n)) or maintain index maps for heaps.

---

#### 6. Quick Cheat-Sheet Code Snippets

<details>
<summary>🔽 Delta-list insert (O(n))</summary>

```c
void start_timer(SWTimer *t, uint32_t dt)
{
    SWTimer **p = &head;
    for (; *p && dt >= (*p)->delta; p = &(*p)->next)
        dt -= (*p)->delta;
    t->delta = dt;
    if (*p) (*p)->delta -= dt;
    t->next = *p;
    *p = t;
    program_hw_timer(head->delta);
}
```

</details>

<details>
<summary>🔽 Ready-queue bitmap trick (scheduler sibling)</summary>

```c
uint32_t ready_map;
inline void make_ready(TCB *t){
    t->next = prio_head[t->prio];
    prio_head[t->prio] = t;
    ready_map |= 1u << t->prio;
}
inline TCB *next_task(void){
    int p = __builtin_ctz(ready_map);
    TCB *t = prio_head[p];
    prio_head[p] = t->next;
    if (!prio_head[p]) ready_map &= ~(1u<<p);
    return t;
}
```

</details>

---

### 7. Key Take-Away

> **One lowly hardware timer + the right in-RAM data structure ⇒ unlimited software timers with deterministic behaviour.**
> Master the pattern, then pick Δ-list, heap, wheel, or RB-tree to fit your MCU, power, and scale constraints.

---

*Happy timing!*
