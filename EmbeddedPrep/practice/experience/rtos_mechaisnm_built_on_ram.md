### 🛠️ Seven “Work-Horse” RTOS / Firmware Mechanisms Built Only from RAM Data-Structures

*A concise cheat-sheet connecting each mechanism to its core software structure and the payoff you get—no special hardware required.*

| #     | Mechanism                                              | Primary Data-Structure(s)                                                                                                                             | What It Enables / Why It Matters                                                                                                                                                     |
| ----- | ------------------------------------------------------ | ----------------------------------------------------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| **1** | **Ready-Task Scheduler**                               | • **Array of linked-lists** (one per priority) <br>• **Bit-map** of non-empty priorities                                                              | O(1) “pick-next-task” with just a CLZ/CTZ instruction. Powers context switches in FreeRTOS, Zephyr, RTX, Linux RT-patch.                                                             |
| **2** | **Message / Event Queues**                             | • **Circular buffer** (ring) for fixed-size items <br>• **Linked list** for variable-size blocks                                                      | Zero-copy UART FIFOs, inter-task mailboxes, DMA descriptor chains. Separates producer/consumer without silicon FIFOs.                                                                |
| **3** | **Dynamic Memory Allocator**                           | • **Simple free list** (DLmalloc, `heap_4`) <br>• **Buddy tree** (glibc `ptmalloc`, Linux page-alloc) <br>• **Slab caches** (object pools + bit-maps) | `malloc()/free()` or pool allocators on bare metal. Object caches for TCBs, semaphores, network buffers—no MMU needed.                                                               |
| **4** | **Semaphores & Event-Flag Sets**                       | • **Counter + wait-list** <br>• **Bit-mask** for multi-flag groups                                                                                    | Classical counting/binary semaphores and multi-bit event groups (FreeRTOS `EventGroup`). All wake-ups handled in software lists.                                                     |
| **5** | **Deferred-Work / ISR Bottom-Halves**                  | • **Lock-free singly-linked list** (per-CPU) <br>• **Array of work queues**                                                                           | Lets ISRs stay \~5 µs: they push a function ptr; a low-prio “worker” task or softirq drains the list. Used by Linux `tasklet`, Zephyr `k_work`, FreeRTOS `xTimerPendFunctionCall()`. |
| **6** | **Resource Tracking** <br>(sockets, DMA chans, GPIOs…) | • **Bit-maps** (<32 resources) <br>• **Hash tables / open addressing** (>32)                                                                          | Constant-time *find-first-free* and O(1) membership tests. No hardware allocation table; just scan or CLZ on a bit-field.                                                            |
| **7** | **Software Watchdog & Tickless Idle Accounting**       | • **Delta-list** or **timer wheel** (same as multitimer trick)                                                                                        | Single hardware timer supervises *all* watchdog kicks, sleep compensation, and idle-time stats—keeps power low without RTC assist.                                                   |

---

#### Key Patterns to Remember

* **Bit-maps = O(1) allocation** when the set is small (≤ word width).
* **Linked lists** stay king for variable-size or FIFO ordering with minimal RAM.
* **Ring buffers** shine for producer/consumer streams—one head, one tail, no malloc.
* **Self-balancing trees / heaps** appear only when you need sorted access or arbitrary cancellation (timers, allocators).

> 💡 **Take-away:** With nothing more exotic than arrays, lists, heaps, and bit-maps, kernels orchestrate scheduling, messaging, memory, and timeouts—often driven by a *single* hardware interrupt. Master these patterns and you can build surprisingly feature-rich firmware on even the humblest MCUs.
