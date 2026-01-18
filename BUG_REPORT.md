# ePHC Project - Bug Report

## Critical Issues

### 1. **CRITICAL: Mutex Release Outside Scope in StartTaskHALT** 
**Location:** [main.c](main.c#L925-L939)  
**Severity:** CRITICAL - Data Race Condition

```c
void StartTaskHALT(void *argument)
{
	// ...
	for(;;)
	{
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
		if (osMutexAcquire(SIgnalMutexHandle, osWaitForever) == osOK)
		{
			// Process here...
		}
		osMutexRelease(SIgnalMutexHandle);  // ← BUG: Released OUTSIDE if block!
	}
}
```

**Problem:** The mutex is released outside the `if` block, which means:
- If `osMutexAcquire()` fails, the mutex is still released (double release)
- This can cause undefined behavior and data races

**Fix:** Move `osMutexRelease()` inside the `if` block:
```c
if (osMutexAcquire(SIgnalMutexHandle, osWaitForever) == osOK)
{
	// Process here...
	osMutexRelease(SIgnalMutexHandle);
}
```

---

### 2. **CRITICAL: Potential Buffer Overflow in DipSwitchStatus()**
**Location:** [Utilies.c](Utilies.c#L190-L196)  
**Severity:** CRITICAL - Memory Corruption

```c
void DipSwitchStatus()
{
	for(int i = 0; i < 12; i++)
	{
		u8DipSwitchesStatus |= HAL_GPIO_ReadPin(sDipSwitchesPin[i].Port, sDipSwitchesPin[i].PinNumber) << i;
	}
}
```

**Problem:** 
- `u8DipSwitchesStatus` is an 8-bit value (uint8_t)
- Bitwise shift `<< i` where `i` goes up to 11 shifts beyond 8 bits
- This causes bits to be lost and creates undefined behavior
- When `i=8` and beyond, shifting a uint8_t by 8+ bits is undefined behavior in C

**Fix:** Use a uint16_t instead:
```c
extern uint16_t u8DipSwitchesStatus;  // In MSGlobals.h

void DipSwitchStatus()
{
	for(int i = 0; i < 12; i++)
	{
		u8DipSwitchesStatus |= (uint16_t)HAL_GPIO_ReadPin(sDipSwitchesPin[i].Port, sDipSwitchesPin[i].PinNumber) << i;
	}
}
```

---

### 3. **HIGH: Uninitialized State Variable in communication.c**
**Location:** [communication.c](communication.c#L85-L160)  
**Severity:** HIGH - Logic Error / Undefined Behavior

```c
void ReceiveFromAcs()
{
    uint8_t currentByte = au8RxBufAcs.uaiDataArray[u8ReadIdxAcs];
	// switch(eReturnCodeState) is used but eReturnCodeState is NEVER initialized
    switch(eReturnCodeState)
    {
		case IDLE:
		// ...
    }
}
```

**Problem:**
- `eReturnCodeState` is a global static variable that tracks the receive state machine
- It's declared in `communication.c` but never explicitly initialized in code
- On first call, it has undefined/garbage value
- Should be initialized to `IDLE` state

**Fix:** Initialize in `ResetCommunication()` function (line 240), which is good, but also add explicit initialization or initialize in Globals.c. Currently missing proper initialization at startup.

---

### 4. **HIGH: Race Condition in UART Flag Handling**
**Location:** [main.c](main.c#L755-L787)  
**Severity:** HIGH - Race Condition

In `StartTaskUART1()`:
```c
if(flag == EVENT_DATA_READY)
{
	vPortEnterCritical();
	uint8_t sizeToCopy = au8RxBufAcs.uSize;
	memcpy(au8TxBufCmm.uaiDataArray,(uint8_t *)au8RxBufAcs.uaiDataArray,sizeToCopy);
	vPortExitCritical();
	// ...
	HAL_UART_Receive_IT(&huart6, (uint8_t *)&au8RxBufAcs.uaiDataArray[0], 1);
```

**Problem:**
- The critical section is too short - only protects memcpy
- But `HAL_UART_Receive_IT()` is called outside critical section
- ISR callbacks can fire between memcpy exit and Receive_IT call, causing buffer corruption
- Buffer state is inconsistent during this window

---

### 5. **HIGH: Type Mismatch in SendData() Return**
**Location:** [communication.c](communication.c#L215-L225)  
**Severity:** HIGH - Type Error

```c
bool SendData(UART_HandleTypeDef* uart_, uint8_t* txBuffer_, uint8_t size_)
{
	if (uart_->gState == HAL_UART_STATE_READY)
	{
		return HAL_UART_Transmit_DMA(uart_, txBuffer_, size_) == HAL_OK;
	}
	else
	{
		return HAL_BUSY;  // ← BUG: Returning HAL_StatusTypeDef as bool!
	}
}
```

**Problem:**
- Function returns `bool` but returns `HAL_BUSY` which is `HAL_StatusTypeDef` (int)
- `HAL_BUSY = 1`, which equals `true`, but semantic meaning is wrong
- Caller in [main.c#L768](main.c#L768): `if(SendData(...))` interprets `HAL_BUSY` as true
- This is misleading and can cause logic errors

**Fix:** Either change return type or handle the conversion:
```c
// Option 1: Change return type
HAL_StatusTypeDef SendData(UART_HandleTypeDef* uart_, uint8_t* txBuffer_, uint8_t size_)
{
	if (uart_->gState == HAL_UART_STATE_READY)
	{
		return HAL_UART_Transmit_DMA(uart_, txBuffer_, size_);
	}
	else
	{
		return HAL_BUSY;
	}
}

// Option 2: Keep bool and fix return
bool SendData(UART_HandleTypeDef* uart_, uint8_t* txBuffer_, uint8_t size_)
{
	if (uart_->gState == HAL_UART_STATE_READY)
	{
		return HAL_UART_Transmit_DMA(uart_, txBuffer_, size_) == HAL_OK;
	}
	return false;  // Not ready
}
```

---

### 6. **MEDIUM: Bitwise AND Used Instead of Equality Check**
**Location:** [main.c](main.c#L805-L810)  
**Severity:** MEDIUM - Logic Error

```c
void StartTaskUART6(void *argument)
{
	// ...
	flag = osThreadFlagsWait(EVENT_DATA_READY | EVENT_ERROR, osFlagsWaitAny, osWaitForever);

	if(flag & EVENT_ERROR)  // ← Using & instead of ==
	{
		ResetCommunication();
	}
	else if(flag & EVENT_DATA_READY)  // ← Using & instead of ==
	{
		// Process...
	}
}
```

vs. [main.c](main.c#L755-L760) in StartTaskUART1 which uses `==` correctly.

**Problem:**
- `osFlagsWaitAny` means one of the flags triggered
- `flag` variable contains the flag that was set, not a bitmask
- Using `&` instead of `==` is inconsistent with StartTaskUART1
- Could mask multiple bits unintentionally

**Fix:** Use `==` for consistency:
```c
if(flag == EVENT_ERROR)
{
	ResetCommunication();
}
else if(flag == EVENT_DATA_READY)
{
	// Process...
}
```

---

### 7. **MEDIUM: Missing NULL Check for Thread Handles**
**Location:** [main.c](main.c#L215-L227)  
**Severity:** MEDIUM - Defensive Programming

```c
TaskUART1Handle = osThreadNew(StartTaskUART1, NULL, &TaskUART1_attributes);
TaskUART6Handle = osThreadNew(StartTaskUART6, NULL, &TaskUART6_attributes);
// ... no error checking ...
osKernelStart();
```

**Problem:**
- If thread creation fails, handles are NULL
- No error checking - code proceeds anyway
- This can cause NULL pointer dereferences later when setting flags

**Fix:** Add error checking:
```c
TaskUART1Handle = osThreadNew(StartTaskUART1, NULL, &TaskUART1_attributes);
if (TaskUART1Handle == NULL) {
	Error_Handler();
}
// ... for all thread creations ...
```

---

### 8. **MEDIUM: Incomplete ResetCommunication() Logic**
**Location:** [communication.c](communication.c#L240-L255)  
**Severity:** MEDIUM - State Management

```c
void ResetCommunication()
{
	HAL_UART_AbortTransmit(&huart1);
	HAL_UART_AbortTransmit(&huart6);
	u8ReadIdxCmm = 0;
	u8ReadIdxAcs = 0;
	eReturnCodeState = IDLE;
	HAL_UART_Receive(&huart1, ...);  // ← Blocking receive!
	HAL_UART_Receive(&huart6, ...);  // ← Blocking receive!
	HAL_UART_Receive_IT(&huart1, ...);
	HAL_UART_Receive_IT(&huart6, ...);
}
```

**Problem:**
- Calls both `HAL_UART_Receive()` (blocking) and `HAL_UART_Receive_IT()` (interrupt) sequentially
- Blocking receive with 100ms timeout can freeze the system during error recovery
- The blocking receive is immediately replaced by interrupt receive - redundant

**Fix:** Remove blocking receive calls:
```c
void ResetCommunication()
{
	HAL_UART_AbortTransmit(&huart1);
	HAL_UART_AbortTransmit(&huart6);
	u8ReadIdxCmm = 0;
	u8ReadIdxAcs = 0;
	eReturnCodeState = IDLE;
	// Remove blocking HAL_UART_Receive calls
	HAL_UART_Receive_IT(&huart1, (uint8_t *)&au8RxBufCmm.uaiDataArray[0], 1);
	HAL_UART_Receive_IT(&huart6, (uint8_t *)&au8RxBufAcs.uaiDataArray[0], 1);
}
```

---

### 9. **MEDIUM: Stack Overflow Risk - Small Task Stack Sizes**
**Location:** [main.c](main.c#L58-L94)  
**Severity:** MEDIUM - Resource Management

```c
const osThreadAttr_t TaskUART1_attributes = {
  .name = "TaskUART1",
  .stack_size = 128 * 4,  // Only 512 bytes!
  .priority = (osPriority_t) osPriorityLow,
};
```

**Problem:**
- All tasks allocated only 512 bytes stack
- STM32F401 has 64KB RAM total
- Each task calls functions that may use stack:
  - `memcpy()` with local variables
  - `vPortEnterCritical()` / `vPortExitCritical()`
  - Multiple function calls in thread
- Insufficient stack can cause silent corruption

**Recommendation:** Increase stack sizes:
```c
.stack_size = 512 * 4,  // 2048 bytes per task - more reasonable
```

Monitor actual stack usage during testing.

---

### 10. **LOW: Inconsistent NULL Pointer Usage**
**Location:** [main.c](main.c#L215-L227)  
**Severity:** LOW - Code Consistency

Thread creation passes `NULL` for argument:
```c
TaskUART1Handle = osThreadNew(StartTaskUART1, NULL, &TaskUART1_attributes);
```

But function signature shows:
```c
void StartTaskUART1(void *argument)
{
  (void)argument;  // Suppresses unused parameter warning
```

This is OK but could be cleaner. Consider passing meaningful context data if needed.

---

### 11. **LOW: Missing Guard Against NULL Pointer in Arrays**
**Location:** [Utilies.c](Utilies.c#L187-L196)  
**Severity:** LOW - Defensive Programming

```c
void DipSwitchStatus()
{
	for(int i = 0; i < 12; i++)
	{
		u8DipSwitchesStatus |= HAL_GPIO_ReadPin(sDipSwitchesPin[i].Port, sDipSwitchesPin[i].PinNumber) << i;
	}
}
```

**Problem:**
- No validation that `sDipSwitchesPin[i].Port` is not NULL
- If `Initialize()` wasn't called, Port pointer could be uninitialized
- Would cause hard fault

**Fix:** Add assert or validation:
```c
for(int i = 0; i < 12; i++)
{
	if (sDipSwitchesPin[i].Port != NULL) {
		u8DipSwitchesStatus |= (uint16_t)HAL_GPIO_ReadPin(sDipSwitchesPin[i].Port, sDipSwitchesPin[i].PinNumber) << i;
	}
}
```

---

## Summary Table

| # | Issue | File | Line | Severity | Type |
|---|-------|------|------|----------|------|
| 1 | Mutex release outside scope | main.c | 925 | CRITICAL | Data Race |
| 2 | Buffer overflow in DipSwitch | Utilies.c | 192 | CRITICAL | Memory |
| 3 | Uninitialized state variable | communication.c | 85 | HIGH | Logic |
| 4 | Race condition in UART | main.c | 755 | HIGH | Concurrency |
| 5 | Type mismatch in SendData() | communication.c | 220 | HIGH | Type Error |
| 6 | Bitwise AND vs equality | main.c | 805 | MEDIUM | Logic |
| 7 | Missing NULL checks | main.c | 215 | MEDIUM | Defensive |
| 8 | Blocking in ResetComm() | communication.c | 245 | MEDIUM | Performance |
| 9 | Small stack sizes | main.c | 58 | MEDIUM | Resource |
| 10 | NULL argument usage | main.c | 215 | LOW | Style |
| 11 | Missing array validation | Utilies.c | 192 | LOW | Defensive |

---

## Recommended Priority Order

1. **Fix Issue #1** - Mutex release (data corruption risk)
2. **Fix Issue #2** - Buffer overflow (memory corruption)
3. **Fix Issue #5** - SendData type mismatch (logic errors)
4. **Fix Issue #4** - UART race condition (data corruption)
5. **Fix Issue #3** - Initialize eReturnCodeState
6. **Fix Issue #8** - Remove blocking calls
7. **Fix Issue #6** - Use == instead of &
8. **Fix Issue #9** - Increase stack sizes
9. **Fix Issues #7, #10, #11** - Defensive programming improvements

