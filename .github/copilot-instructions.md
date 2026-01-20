# ePHC Project - AI Coding Agent Instructions

## Project Overview
**ePHC** is an embedded STM32F401 firmware project for a precision measurement controller board. It communicates with Renishaw ACS encoders via UART, manages DIP switch configurations, power states, and coordinates multiple real-time tasks using FreeRTOS.

**Target Hardware:** STM32F401VBTX microcontroller (64KB RAM, Cortex-M4)

## Architecture: Multi-Task Communication System

### Core Components & Data Flow
- **[main.c](Core/Src/main.c)** (984 lines) - FreeRTOS task scheduler and initialization
  - 5 concurrent tasks: `TaskUART1`, `TaskUART6`, `TaskReadDs` (DIP switches), `TaskHALT`, `TaskSYNC`
  - Each task processes events via thread notifications and semaphores
  - Stack size critical: increased to 512*4 (2048B) to prevent overflow in nested calls

- **[communication.c](Core/Src/communication.c)** (267 lines) - UART/DMA communications
  - Handles Renishaw command protocol via three UART channels (USART1, USART2, USART6)
  - RX callbacks populate circular buffers (`au8RxBufAcs`, `au8RxBufEph`)
  - TX uses DMA with semaphore signaling for completion

- **[Utilies.c](Core/Src/Utilies.c)** - GPIO utilities and DIP switch reading
  - DIP switches map to voltage levels (V5_0, V5_6, V6_1, V6_6)
  - Power pin management for 3×3 grid of outputs

- **[EEprom.c](Core/Src/EEprom.c)** - SPI-based persistent configuration storage

- **Globals in [MSGlobals.h](Core/Inc/MSGlobals.h)** - Shared state (106 volatile variables)
  - Thread-safe access via `SIgnalMutexHandle` (Renishaw signal processing)
  - Event buffers: `au8RxBufAcs`, `au8TxBufAcs`, `au8RxBufEph`
  - Parser state: `u8ReadIdxCmm`, `u8ParsingDone`, `u8IsFirstHaltArrived`

### Critical Data Types
```c
// [Types.h] - Message structure (1 byte packed)
typedef struct {
    GPIO_TypeDef* Port;
    uint16_t PinNumber;
} sPinConfig;

typedef struct {
    uint8_t uaiDataArray[RENISHAW_CMD_MAX_LENGTH];  // ~256 bytes
    uint8_t uSize;
} sCommunicationReceive;

// [MSGlobals.h] - Renishaw protocol enums
enum RENISHAW_COMMAND { A, B, S, U, M, N, C, E, F, H, I, J, O, T, V, X, K, Q, R, XON=17, XOFF=19, CR='\r' }
enum COMMAND_PROCESS_TYPE { IDLE_STATE, READ_STATE, SEND_STATE, RESET_UART=0xFF }
enum RENISHAW_RC_STATES { IDLE, WAIT_FOR_XON, WAIT_FOR_CR }
```

## Build & Development Workflow

### CMake Build System
- **Configure:** `cmake --preset Debug` or `Release`
- **Build:** `ninja -C build/Debug` or `cmake --build build/Debug`
- **Output:** Executable at `build/Debug/ePHC`
- **Toolchain:** ARM Cortex-M4 (gcc-arm-none-eabi.cmake)
- **STM32CubeMX generated code:** Imported via `cmake/stm32cubemx/CMakeLists.txt`

### Key Build Commands (Windows/PowerShell)
```pwsh
cd "c:\Globus\ACS-Boards\STM boards\ePHC"
cmake --preset Debug
cmake --build build/Debug --config Debug
```

### Debugging
- VS Code launch config: [ePHC Debug.launch](ePHC%20Debug.launch) - Requires ST-LINK adapter
- Breakpoints in FreeRTOS tasks may cause RTOS internal state issues; use watch variables instead

## Critical Code Patterns & Known Issues

### ✅ Fixes Already Implemented
1. **Mutex Release Bug (main.c:925)** - CRITICAL: Moved `osMutexRelease()` inside `if` block to prevent double-release
2. **Buffer Overflow (Utilies.c:190)** - Changed `u8DipSwitchesStatus` from `uint8_t` to `uint16_t` (12-bit shift requires 16-bit)
3. **Thread Creation Safety (main.c:215)** - NULL checks added after each `osThreadNew()` call
4. **SendData Return Type (communication.h)** - Returns `HAL_StatusTypeDef` (not `bool`); callers must check `== HAL_OK`

### ⚠️ Active Patterns to Maintain
- **Thread Flags for Task Wakeup:** Use `osThreadFlagsSet(TaskHandle, EVENT_TYPE)` not semaphores for simple signaling
- **Enum Over Bitwise:** Already refactored to use `if(flag == EVENT_ERROR)` not `if(flag & EVENT_ERROR)`
- **DMA with Semaphores:** TX completion handled by `HAL_UART_TxCpltCallback()` → `osSemaphoreRelease(Uart1TxSemHandle)`
- **Pragma pack(push,1):** Used in [Types.h](Core/Inc/Types.h#L15) for hardware-aligned structs only

### Naming Conventions
- Variables: Hungarian notation (u8=uint8_t, u16=uint16_t, s=struct, p=pointer, e=enum)
  - Example: `uint16_t u16DipSwitchesStatus`, `sPinConfig sPowerPin[3][3]`
- Macros: UPPERCASE_SNAKE_CASE (see [Constant.h](Core/Inc/Constant.h))
- Functions: CamelCase with task prefix
  - Example: `StartTaskUART1()`, `ReceiveFromAcs()`

## Critical Constraints & Limitations

### Memory Management (64KB RAM)
- 5 FreeRTOS tasks × 2KB stack each = 10KB reserved
- UART buffers: ~512B each (3 channels) = ~1.5KB
- Heap limited; avoid large stack allocations in nested calls

### Renishaw Protocol State Machine
- Commands are **case-sensitive single characters** (A, B, S, U, M, N, C, E, F, H, I, J, O, T, V, X, K, Q, R)
- XON/XOFF flow control: ASCII 17/19 bytes (not characters)
- Response parsing state: `IDLE` → `WAIT_FOR_XON` → `WAIT_FOR_CR` (end with '\r')

### UART Constraints
- Baud rates configured via STM32CubeMX (see `.ioc` file)
- All RX uses interrupt-driven circular buffering (no polling)
- TX must check `HAL_StatusTypeDef` for `HAL_BUSY` state before sending

## File Organization & Key References

| Purpose | File | Key Structures |
|---------|------|-----------------|
| Main control loop | [Core/Src/main.c](Core/Src/main.c) | TaskUART1, TaskUART6, FreeRTOS config |
| UART/DMA communication | [Core/Src/communication.c](Core/Src/communication.c) | HAL_UART callbacks, `SendData()` |
| Global variables & Renishaw enums | [Core/Inc/MSGlobals.h](Core/Inc/MSGlobals.h) | `sCommunicationReceive`, voltage enums |
| Type definitions | [Core/Inc/Types.h](Core/Inc/Types.h) | `sPinConfig`, struct packing |
| Constants & Renishaw commands | [Core/Inc/Constant.h](Core/Inc/Constant.h) | Voltage levels, bit positions, ACK values |
| Utilities (GPIO, DIP switches) | [Core/Src/Utilies.c](Core/Src/Utilies.c) | `DipSwitchStatus()`, pin configuration |
| Persistent storage | [Core/Src/EEprom.c](Core/Src/EEprom.c) | SPI EEPROM read/write |

## When Making Changes

1. **Adding UART/serial communication:** Update both RX callback in [communication.c](Core/Src/communication.c) AND TX in [main.c](Core/Src/main.c)
2. **Modifying global state:** Check [MSGlobals.h](Core/Inc/MSGlobals.h) for mutex requirements; wrap in `osMutexAcquire()` if concurrent access
3. **DIP switch logic changes:** Remember `u16DipSwitchesStatus` is 16-bit (bits 0-11 only used)
4. **Memory-intensive operations:** Consider task stack size in [main.c](Core/Src/main.c) (TaskUART*_attributes)
5. **Build issues:** Run `cmake --preset Debug` before building if CMake caching issues occur

## References for Further Learning
- **FreeRTOS:** Kernel used for multitasking (osThreadNew, osMutexAcquire, osThreadFlagsSet)
- **STM32F4 HAL:** Hardware Abstraction Layer documentation for UART, SPI, GPIO, DMA functions
- **Renishaw Encoders:** See command enum patterns in [MSGlobals.h](Core/Inc/MSGlobals.h) (ASCII-based protocol)
