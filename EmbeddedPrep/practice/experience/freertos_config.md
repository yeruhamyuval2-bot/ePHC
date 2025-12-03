# 📘 FreeRTOS Configuration Summary Sheet

> Tailored for embedded engineers configuring `FreeRTOSConfig.h`
> Based on Sony's `default_freertos_config.h` and CMSIS-RTOS2 requirements.

---

## ✅ 1. Task API & CMSIS-RTOS2 Compliance

| Macro                                            | Purpose                               |
| ------------------------------------------------ | ------------------------------------- |
| `INCLUDE_xSemaphoreGetMutexHolder`               | Get owning task of a mutex            |
| `INCLUDE_vTaskDelay`                             | Relative task delay (sleep)           |
| `INCLUDE_vTaskDelayUntil`                        | Periodic delay (no drift)             |
| `INCLUDE_vTaskDelete`                            | Allow task deletion                   |
| `INCLUDE_xTaskGetCurrentTaskHandle`              | Get current task handle               |
| `INCLUDE_xTaskGetSchedulerState`                 | Is the scheduler running?             |
| `INCLUDE_uxTaskGetStackHighWaterMark`            | Get stack usage low-watermark         |
| `INCLUDE_uxTaskPriorityGet` / `vTaskPrioritySet` | Get/set task priority                 |
| `INCLUDE_eTaskGetState`                          | Query a task's runtime state          |
| `INCLUDE_vTaskSuspend`                           | Suspend/resume tasks                  |
| `INCLUDE_xTimerPendFunctionCall`                 | Call function from timer/task context |

---

## ✅ 2. Kernel Features & Synchronization

| Macro                                                        | Purpose                                         |
| ------------------------------------------------------------ | ----------------------------------------------- |
| `configUSE_MUTEXES`                                          | Enable binary mutexes (w/ priority inheritance) |
| `configUSE_COUNTING_SEMAPHORES`                              | Enable counting semaphores                      |
| `configUSE_TASK_NOTIFICATIONS`                               | Enable fast task-to-task signaling              |
| `configUSE_TIMERS`                                           | Enable software timers                          |
| `configTIMER_TASK_PRIORITY` / `STACK_DEPTH` / `QUEUE_LENGTH` | Tune timer task                                 |
| `configUSE_RECURSIVE_MUTEXES`                                | Enable recursive mutexes                        |

---

## ✅ 3. Preemption, Scheduling, and Power

| Macro                                     | Purpose                                       |
| ----------------------------------------- | --------------------------------------------- |
| `configUSE_PREEMPTION`                    | Preemptive vs. cooperative scheduling         |
| `configMAX_PRIORITIES`                    | Max priority levels available                 |
| `configUSE_PORT_OPTIMISED_TASK_SELECTION` | Use hardware acceleration for task selection  |
| `configUSE_TIME_SLICING`                  | Enable round-robin among equal-priority tasks |
| `configUSE_TICKLESS_IDLE`                 | Suppress ticks during idle (save power)       |

---

## ✅ 4. Hook Functions & Error Handling

| Macro                                         | Purpose                                 |
| --------------------------------------------- | --------------------------------------- |
| `configUSE_IDLE_HOOK` / `configUSE_TICK_HOOK` | Call user hooks during idle/tick        |
| `configUSE_MALLOC_FAILED_HOOK`                | Call user hook on malloc failure        |
| `configCHECK_FOR_STACK_OVERFLOW`              | Enable stack overflow checks (mode 1/2) |
| `configASSERT(x)`                             | Halt if `x` fails (development only)    |
| `configRECORD_STACK_HIGH_ADDRESS`             | Record stack end in TCB for debug tools |

---

## ✅ 5. Memory & Thread-Safe C Library

| Macro                             | Purpose                                   |
| --------------------------------- | ----------------------------------------- |
| `configMINIMAL_STACK_SIZE`        | Default task stack (words)                |
| `configSUPPORT_STATIC_ALLOCATION` | Allow static task/queue creation          |
| `configUSE_NEWLIB_REENTRANT`      | Enable task-local `errno`, `strtok`, etc. |
| `configUSE_HEAP_SCHEME`           | Choose heap scheme (1–5) in `heap_X.c`    |

---

## ✅ 6. Interrupt Priority & Safety

| Macro                                  | Purpose                                                   |
| -------------------------------------- | --------------------------------------------------------- |
| `configPRIO_BITS`                      | Number of NVIC priority bits (e.g., 3 → 8 levels)         |
| `configKERNEL_INTERRUPT_PRIORITY`      | Kernel interrupt priority (lowest)                        |
| `configMAX_SYSCALL_INTERRUPT_PRIORITY` | Max priority from which FreeRTOS API can be called in ISR |

> 🧠 Only ISRs with priority **≥ MAX\_SYSCALL** can call FreeRTOS API

---

## ✅ 7. Debug, Statistics, and Task Local Storage

| Macro                                     | Purpose                                          |
| ----------------------------------------- | ------------------------------------------------ |
| `configNUM_THREAD_LOCAL_STORAGE_POINTERS` | Per-task storage slots (e.g., context ptrs)      |
| `configQUEUE_REGISTRY_SIZE`               | Register queues/semaphores by name (debug tools) |
| `configUSE_STATS_FORMATTING_FUNCTIONS`    | Enable `vTaskList()` / `vTaskGetRunTimeStats()`  |

---

## 📌 Typical Add-Ons (Not in Default, But Useful)

| Macro                              | Purpose                                        |
| ---------------------------------- | ---------------------------------------------- |
| `configGENERATE_RUN_TIME_STATS`    | Enable CPU usage per task (needs custom timer) |
| `configUSE_APPLICATION_TASK_TAG`   | Attach metadata to tasks (e.g., profiling)     |
| `configSUPPORT_DYNAMIC_ALLOCATION` | Enable dynamic object creation (default: 1)    |

