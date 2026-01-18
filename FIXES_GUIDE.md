 # ePHC Project - Bug Fixes Implementation Guide

## Fix #1: Mutex Release in StartTaskHALT (CRITICAL)

**File:** `Core/Src/main.c` (Lines 920-939)

**Current Code:**
```c
void StartTaskHALT(void *argument)
{
	(void)argument;
	for(;;)
	{
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
		if (osMutexAcquire(SIgnalMutexHandle, osWaitForever) == osOK)
		{
			if ((!u8IsFirstHaltArrived) && (u8IsInTp20Measurement) && (u161MilliSecondCounter >= SNYK_WAIT_TIME) && (u161MilliSecondCounter <= HALT_TIME_OFF))
			{
				u8IsFirstHaltArrived = TRUE;
				TriggerToAcsControl(GPIO_PIN_SET);
			}
		}
		osMutexRelease(SIgnalMutexHandle);  // ← MOVED OUTSIDE IF
	}
}
```

**Fixed Code:**
```c
void StartTaskHALT(void *argument)
{
	(void)argument;
	for(;;)
	{
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
		if (osMutexAcquire(SIgnalMutexHandle, osWaitForever) == osOK)
		{
			if ((!u8IsFirstHaltArrived) && (u8IsInTp20Measurement) && (u161MilliSecondCounter >= SNYK_WAIT_TIME) && (u161MilliSecondCounter <= HALT_TIME_OFF))
			{
				u8IsFirstHaltArrived = TRUE;
				TriggerToAcsControl(GPIO_PIN_SET);
			}
			osMutexRelease(SIgnalMutexHandle);  // ← MOVED INSIDE IF
		}
	}
}
```

---

## Fix #2: Buffer Overflow in DipSwitchStatus (CRITICAL)

**File:** `Core/Inc/MSGlobals.h` (Line ~90)

**Current Declaration:**
```c
extt uint8_t u8DipSwitchesStatus;  // Only 8 bits!
```

**Fixed Declaration:**
```c
extt uint16_t u8DipSwitchesStatus;  // 16 bits to hold all 12 switches
```

---

**File:** `Core/Src/Utilies.c` (Lines 190-196)

**Current Code:**
```c
void DipSwitchStatus()
{
	for(int i = 0; i < 12; i++)
	{
		u8DipSwitchesStatus |= HAL_GPIO_ReadPin(sDipSwitchesPin[i].Port, sDipSwitchesPin[i].PinNumber) << i;
	}
	return;
}
```

**Fixed Code:**
```c
void DipSwitchStatus()
{
	for(int i = 0; i < 12; i++)
	{
		u8DipSwitchesStatus |= (uint16_t)HAL_GPIO_ReadPin(sDipSwitchesPin[i].Port, sDipSwitchesPin[i].PinNumber) << i;
	}
	return;
}
```

---

## Fix #3: Initialize eReturnCodeState (HIGH)

**File:** `Core/Src/Utilies.c` (Lines 15-45)

**Current Code - Add to Initialize():**
```c
void Initialize()
{
	// ... existing code ...
	
	eReturnCodeState = IDLE;  // Already present here
	eTask1State = IDLE_STATE;
	eTask2State = IDLE_STATE;
	
	// ... rest of code ...
}
```

This is already done correctly in Initialize(). Ensure Initialize() is called in main() before RTOS starts (which it is at line 172).

**Verification:** Confirm in `main.c` line 172 shows:
```c
Initialize();  // ← Call this BEFORE any UART operations
```

---

## Fix #4: UART Race Condition (HIGH)

**File:** `Core/Src/main.c` (Lines 755-787)

**Current Code in StartTaskUART1:**
```c
if(flag == EVENT_DATA_READY)
{
	vPortEnterCritical();
	uint8_t sizeToCopy = au8RxBufAcs.uSize;
	memcpy(au8TxBufCmm.uaiDataArray,(uint8_t *)au8RxBufAcs.uaiDataArray,sizeToCopy);
	vPortExitCritical();

	if (osSemaphoreAcquire(Uart1TxSemHandle, osWaitForever) == osOK)
	{
		if(SendData(&huart1, au8TxBufCmm.uaiDataArray, sizeToCopy))
		{
			HAL_UART_Receive_IT(&huart6, (uint8_t *)&au8RxBufAcs.uaiDataArray[0], 1);
			HAL_UART_Receive_IT(&huart1, (uint8_t *)&au8RxBufCmm.uaiDataArray[0], 1);
		}
```

**Fixed Code:**
```c
if(flag == EVENT_DATA_READY)
{
	vPortEnterCritical();
	uint8_t sizeToCopy = au8RxBufAcs.uSize;
	memcpy(au8TxBufCmm.uaiDataArray,(uint8_t *)au8RxBufAcs.uaiDataArray,sizeToCopy);
	// Re-enable UART RX interrupts while still in critical section
	HAL_UART_Receive_IT(&huart6, (uint8_t *)&au8RxBufAcs.uaiDataArray[0], 1);
	HAL_UART_Receive_IT(&huart1, (uint8_t *)&au8RxBufCmm.uaiDataArray[0], 1);
	vPortExitCritical();

	if (osSemaphoreAcquire(Uart1TxSemHandle, osWaitForever) == osOK)
	{
		if(SendData(&huart1, au8TxBufCmm.uaiDataArray, sizeToCopy))
		{
			// TX in progress, semaphore will be released in callback
		}
```

**Rationale:** Move UART_Receive_IT calls inside the critical section to prevent ISR from firing and modifying buffers between memcpy and re-enabling interrupts.

---

## Fix #5: SendData Return Type (HIGH)

**File:** `Core/Inc/communication.h` (Line ~32)

**Current Declaration:**
```c
bool SendData(UART_HandleTypeDef* uart_, uint8_t* txBuffer_, uint8_t size_);
```

**Option A - Change Return Type (RECOMMENDED):**
```c
HAL_StatusTypeDef SendData(UART_HandleTypeDef* uart_, uint8_t* txBuffer_, uint8_t size_);
```

---

**File:** `Core/Src/communication.c` (Lines 215-225)

**Current Code:**
```c
bool SendData(UART_HandleTypeDef* uart_, uint8_t* txBuffer_, uint8_t size_)
{
	if (uart_->gState == HAL_UART_STATE_READY)
	{
		return HAL_UART_Transmit_DMA(uart_, txBuffer_, size_) == HAL_OK;
	}
	else
	{
		return HAL_BUSY;  // ← BUG: wrong type
	}
}
```

**Fixed Code (Option A):**
```c
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
```

**File:** `Core/Src/main.c` - Update call sites (Lines 768, 819)

**Current Code:**
```c
if(SendData(&huart1, au8TxBufCmm.uaiDataArray, sizeToCopy))
```

**Fixed Code:**
```c
if(SendData(&huart1, au8TxBufCmm.uaiDataArray, sizeToCopy) == HAL_OK)
```

---

## Fix #6: Bitwise Operators (MEDIUM)

**File:** `Core/Src/main.c` (Lines 805-820)

**Current Code in StartTaskUART6:**
```c
flag = osThreadFlagsWait(EVENT_DATA_READY | EVENT_ERROR, osFlagsWaitAny, osWaitForever);

if(flag & EVENT_ERROR)  // ← Should be ==
{
	ResetCommunication();
	EmptyUartBuffers();
}

else if(flag & EVENT_DATA_READY)  // ← Should be ==
{
	// ...
}
```

**Fixed Code:**
```c
flag = osThreadFlagsWait(EVENT_DATA_READY | EVENT_ERROR, osFlagsWaitAny, osWaitForever);

if(flag == EVENT_ERROR)
{
	ResetCommunication();
	EmptyUartBuffers();
}

else if(flag == EVENT_DATA_READY)
{
	// ...
}
```

---

## Fix #7: Add NULL Checks on Thread Creation (MEDIUM)

**File:** `Core/Src/main.c` (Lines 215-227)

**Current Code:**
```c
TaskUART1Handle = osThreadNew(StartTaskUART1, NULL, &TaskUART1_attributes);
TaskUART6Handle = osThreadNew(StartTaskUART6, NULL, &TaskUART6_attributes);
TaskReadDsHandle = osThreadNew(StartTaskReadDs, NULL, &TaskReadDs_attributes);
TaskSYNCHandle = osThreadNew(StartTaskSYNC, NULL, &TaskSYNC_attributes);
TaskHALTHandle = osThreadNew(StartTaskHALT, NULL, &TaskHALT_attributes);
```

**Fixed Code:**
```c
TaskUART1Handle = osThreadNew(StartTaskUART1, NULL, &TaskUART1_attributes);
if (TaskUART1Handle == NULL) Error_Handler();

TaskUART6Handle = osThreadNew(StartTaskUART6, NULL, &TaskUART6_attributes);
if (TaskUART6Handle == NULL) Error_Handler();

TaskReadDsHandle = osThreadNew(StartTaskReadDs, NULL, &TaskReadDs_attributes);
if (TaskReadDsHandle == NULL) Error_Handler();

TaskSYNCHandle = osThreadNew(StartTaskSYNC, NULL, &TaskSYNC_attributes);
if (TaskSYNCHandle == NULL) Error_Handler();

TaskHALTHandle = osThreadNew(StartTaskHALT, NULL, &TaskHALT_attributes);
if (TaskHALTHandle == NULL) Error_Handler();
```

---

## Fix #8: Remove Blocking Calls (MEDIUM)

**File:** `Core/Src/communication.c` (Lines 240-255)

**Current Code:**
```c
void ResetCommunication()
{
	HAL_UART_AbortTransmit(&huart1);
	HAL_UART_AbortTransmit(&huart6);
	u8ReadIdxCmm = 0;
	u8ReadIdxAcs = 0;
	eReturnCodeState = IDLE;
	HAL_UART_Receive(&huart1, (uint8_t *)au8RxBufCmm.uaiDataArray, RENISHAW_CMD_MAX_LENGTH, 100);  // ← BLOCKING!
	HAL_UART_Receive(&huart6, (uint8_t *)au8RxBufAcs.uaiDataArray, RENISHAW_CMD_MAX_LENGTH, 100);  // ← BLOCKING!
	HAL_UART_Receive_IT(&huart1, (uint8_t *)&au8RxBufCmm.uaiDataArray[0], 1);
	HAL_UART_Receive_IT(&huart6, (uint8_t *)&au8RxBufAcs.uaiDataArray[0], 1);
	return;
}
```

**Fixed Code:**
```c
void ResetCommunication()
{
	HAL_UART_AbortTransmit(&huart1);
	HAL_UART_AbortTransmit(&huart6);
	u8ReadIdxCmm = 0;
	u8ReadIdxAcs = 0;
	eReturnCodeState = IDLE;
	// Removed blocking HAL_UART_Receive calls - use interrupt mode only
	HAL_UART_Receive_IT(&huart1, (uint8_t *)&au8RxBufCmm.uaiDataArray[0], 1);
	HAL_UART_Receive_IT(&huart6, (uint8_t *)&au8RxBufAcs.uaiDataArray[0], 1);
	return;
}
```

---

## Fix #9: Increase Stack Sizes (MEDIUM)

**File:** `Core/Src/main.c` (Lines 58-94)

**Current Code:**
```c
const osThreadAttr_t TaskUART1_attributes = {
  .name = "TaskUART1",
  .stack_size = 128 * 4,  // 512 bytes - TOO SMALL
  .priority = (osPriority_t) osPriorityLow,
};
```

**Fixed Code:**
```c
const osThreadAttr_t TaskUART1_attributes = {
  .name = "TaskUART1",
  .stack_size = 512 * 4,  // 2048 bytes - more reasonable
  .priority = (osPriority_t) osPriorityLow,
};
```

**Apply to all thread attributes:**
- TaskUART1_attributes
- TaskUART6_attributes
- TaskReadDs_attributes
- TaskSYNC_attributes
- TaskHALT_attributes

**Rationale:** 
- STM32F401 has 64KB RAM total
- Current 512 bytes per task × 5 tasks = 2.5KB used
- Proposed 2048 bytes per task × 5 tasks = 10KB used (~16% of RAM)
- Much safer with room for stack growth

---

## Fix #10: Add Array Bounds Validation (LOW)

**File:** `Core/Src/Utilies.c` (Lines 190-196)

**Current Code:**
```c
void DipSwitchStatus()
{
	for(int i = 0; i < 12; i++)
	{
		u8DipSwitchesStatus |= (uint16_t)HAL_GPIO_ReadPin(sDipSwitchesPin[i].Port, sDipSwitchesPin[i].PinNumber) << i;
	}
	return;
}
```

**Fixed Code (with validation):**
```c
void DipSwitchStatus()
{
	for(int i = 0; i < 12; i++)
	{
		if (sDipSwitchesPin[i].Port != NULL) {
			u8DipSwitchesStatus |= (uint16_t)HAL_GPIO_ReadPin(sDipSwitchesPin[i].Port, sDipSwitchesPin[i].PinNumber) << i;
		}
	}
	return;
}
```

---

## Testing Recommendations

After implementing fixes:

1. **Fix #1 & #4:** Test with multi-threaded stress test - send continuous data on UART1 and UART6 simultaneously
2. **Fix #2:** Verify DipSwitch status bits - should properly capture all 12 switches without overflow
3. **Fix #5:** Verify SendData return value handling - test with UART busy/ready conditions
4. **Fix #6:** Verify flag handling in StartTaskUART6 - test event filtering
5. **Fix #8:** Verify ResetCommunication doesn't block - measure execution time
6. **Fix #9:** Monitor task stack usage during operation to ensure no overflow

---

## Compilation Verification

After making all changes:

```bash
cd Debug
cmake --build .
```

Should compile without warnings related to:
- Type mismatches
- Operator precedence issues
- NULL pointer comparisons

---

