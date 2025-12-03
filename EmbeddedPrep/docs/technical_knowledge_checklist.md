### 1️⃣ Technical-Knowledge Checklist

For each topic, you should be able to pass a test with questions at different levels of difficulty. Use the **Status** column to mark whether you ✅ "passed the test", ⚠️ "partial understanding", or ❌ "need to study".

| #  | Technical Topic                                                                        | Status | Test Description | Subtopics |
| -- | -------------------------------------------------------------------------------------- | ------ | ---------------- | --------- |
| 1  | Embedded C/C++ best practices (memory-safe code, buffer limits, pointer arithmetic)    |        | Test: Implement a thread-safe circular buffer with proper error handling and memory safety checks | • Memory safety patterns<br>• Buffer overflow prevention<br>• Pointer arithmetic safety<br>• Resource management<br>• Error handling patterns<br>• Thread safety mechanisms<br>• Memory alignment<br>• Volatile usage |
| 2  | Bit-level work on hardware registers & masks                                           |        | Test: Implement a bitmap allocator with atomic operations and proper bit manipulation | • Bit manipulation operations<br>• Register access patterns<br>• Atomic operations<br>• Bit fields<br>• Mask creation and usage<br>• Bit shifting and rotation<br>• Hardware register mapping<br>• Interrupt-safe bit operations |
| 3  | Minimal, ISR-safe interrupt design & deferring work to RTOS tasks                      |        | Test: Design an interrupt handler that safely defers work to a task | • ISR design principles<br>• Task deferral patterns<br>• Priority management<br>• Context switching<br>• Stack management<br>• Interrupt nesting<br>• Critical sections<br>• Timing constraints |
| 4  | RTOS fundamentals: tasks/threads, pre-emptive vs co-operative scheduling               |        | Test: Implement a priority-based task scheduler with proper context switching | • Task management<br>• Scheduling algorithms<br>• Priority levels<br>• Context switching<br>• Stack management<br>• Task synchronization<br>• Resource sharing<br>• Timing analysis |
| 5  | Task-synchronization primitives (semaphores, mutexes, event flags, queues)             |        | Test: Implement a producer-consumer pattern using synchronization primitives | • Semaphore types and usage<br>• Mutex implementation<br>• Event flag systems<br>• Message queues<br>• Priority inheritance<br>• Deadlock prevention<br>• Timeout handling<br>• Resource management |
| 6  | Debugging race conditions & priority inversion (incl. priority-inheritance)            |        | Test: Identify and fix a priority inversion issue in a multi-threaded system | • Race condition detection<br>• Priority inversion scenarios<br>• Debugging techniques<br>• Prevention strategies<br>• Testing methodologies<br>• Analysis tools<br>• Fix implementation<br>• Verification methods |
| 7  | Peripheral interfaces: **I²C**, **SPI**, **UART**, **GPIO** (config + driver bring-up) |        | Test: Implement a driver for a peripheral with proper error handling and timing | • Protocol fundamentals<br>• Timing requirements<br>• Error handling<br>• Interrupt handling<br>• DMA usage<br>• Power management<br>• Driver architecture<br>• Testing strategies |
| 8  | Logic-analyzer / scope use for digital bus timing & signal integrity                   |        | Test: Debug a timing issue using a logic analyzer and scope | • Signal measurement<br>• Timing analysis<br>• Protocol decoding<br>• Trigger setup<br>• Data capture<br>• Analysis techniques<br>• Debug strategies<br>• Tool configuration |
| 9  | Wi-Fi MAC protocol basics (802.11 a/b/g/n/ac/ax/be, CSMA/CA, beacons, ACK)             |        | Test: Implement a basic Wi-Fi connection sequence with proper state management | • Protocol versions<br>• Frame types<br>• Connection states<br>• Authentication methods<br>• Association process<br>• Power management<br>• Security protocols<br>• Error handling |
| 10 | Advanced Wi-Fi MAC: OFDMA & MU-MIMO (11ax/11be)                                        |        | Test: Implement an advanced Wi-Fi feature with proper protocol handling | • OFDMA principles<br>• MU-MIMO operation<br>• Resource allocation<br>• Beamforming<br>• Channel access<br>• Power management<br>• Performance optimization<br>• Implementation challenges |
| 11 | Target Wake Time (TWT) power-save scheduling                                           |        | Test: Implement a power-saving feature with proper timing and state management | • TWT operation modes<br>• Scheduling algorithms<br>• Power state management<br>• Timing synchronization<br>• Error recovery<br>• Performance impact<br>• Implementation strategies<br>• Testing methodologies |
| 12 | Management-frame & state-machine sequences (auth → assoc → 4-way handshake)            |        | Test: Implement a complete authentication and association sequence | • Frame types<br>• State transitions<br>• Security protocols<br>• Error handling<br>• Timing requirements<br>• Implementation patterns<br>• Testing strategies<br>• Debug techniques |
| 13 | JTAG/SWD debugging & crash triage on bare metal                                        |        | Test: Debug and fix a crash using JTAG/SWD debugging tools | • Debug interface setup<br>• Memory inspection<br>• Register access<br>• Breakpoint usage<br>• Watchpoints<br>• Crash analysis<br>• Core dumps<br>• Recovery strategies |
| 14 | Hardware bring-up tools (RF testers, packet sniffers, spectrum analyzers)              |        | Test: Use hardware tools to diagnose and fix a communication issue | • Tool selection<br>• Configuration<br>• Measurement techniques<br>• Data analysis<br>• Debug strategies<br>• Performance testing<br>• Compliance testing<br>• Documentation |
| 15 | System-architecture patterns: state machine, producer-consumer, layering               |        | Test: Design and implement a system using appropriate architectural patterns | • State machine design<br>• Producer-consumer patterns<br>• Layered architecture<br>• Interface design<br>• Error handling<br>• Performance optimization<br>• Testing strategies<br>• Documentation |
| 16 | Concurrency design for real-time deadlines                                             |        | Test: Implement a real-time system with proper deadline handling | • Deadline analysis<br>• Task scheduling<br>• Resource management<br>• Priority handling<br>• Error recovery<br>• Performance optimization<br>• Testing methodologies<br>• Documentation |
| 17 | Code-size & RAM optimisation techniques (static vs heap, bit-fields, DMA offload)      |        | Test: Optimize a system for memory usage and performance | • Memory allocation<br>• Stack management<br>• Heap usage<br>• DMA implementation<br>• Bit field optimization<br>• Code size reduction<br>• Performance tuning<br>• Resource management |
| 18 | Power-optimisation techniques (clock gating, sleep modes, no busy wait)                |        | Test: Implement power-saving features with proper timing | • Sleep modes<br>• Clock management<br>• Power states<br>• Wake-up strategies<br>• Timing requirements<br>• Performance impact<br>• Implementation patterns<br>• Testing methodologies |
| 19 | Profiling/tracing for hotspot detection & throughput tuning                            |        | Test: Profile and optimize a system for performance | • Profiling tools<br>• Performance metrics<br>• Bottleneck identification<br>• Optimization techniques<br>• Measurement strategies<br>• Analysis methods<br>• Documentation<br>• Testing approaches |

---

### 2️⃣ Evidence Planner – "Prove I'm Mid-Level"

Fill in concrete stories from your projects. Hiring teams love STAR-style bullets (Situation / Task / Action / Result).

| Topic / Competency                                        | My Concrete Example (project, bug, feature…) | Result / Impact | Status |
| --------------------------------------------------------- | -------------------------------------------- | --------------- | ------ |
| **Embedded C/C++ best practices**                         |                                              |                 |        |
| **RTOS task & ISR integration**                           |                                              |                 |        |
| **Peripheral-driver bring-up (I²C/SPI/UART/GPIO)**        |                                              |                 |        |
| **802.11 MAC feature implementation (e.g., TWT, OFDMA)**  |                                              |                 |        |
| **Throughput or latency optimisation**                    |                                              |                 |        |
| **Board bring-up & lab debugging (JTAG, scope, sniffer)** |                                              |                 |        |
| **System-level architecture / refactor**                  |                                              |                 |        |
| **Ownership of a module / feature**                       |                                              |                 |        |
| **Cross-functional collaboration (HW / QA / PM)**         |                                              |                 |        |
| **Mentoring & code reviews**                              |                                              |                 |        |
| **Risk assessment & decision-making**                     |                                              |                 |        |
| **Initiative / bias-for-action story**                    |                                              |                 |        |

---

#### How to use these tables

1. **Test Levels:**
   - Basic: Understanding of concepts and simple implementations
   - Intermediate: Complex implementations with error handling
   - Advanced: System-level design with optimization

2. **For each topic:**
   - Review the test description
   - Implement the test requirements
   - Mark your status based on your ability to complete the test
   - Document any specific areas that need improvement

3. **Study plan:**
   - Focus on topics marked ❌ first
   - Review ⚠️ topics to improve understanding
   - Practice ✅ topics to maintain proficiency

4. **Test Requirements:**
   - Each test should include:
     - Basic implementation
     - Error handling
     - Performance considerations
     - Documentation
     - Testing strategy

5. **Evidence Collection:**
   - For each competency in the Evidence Planner:
     - Document a specific project or feature you worked on
     - Quantify the impact (e.g., "reduced latency by 35%")
     - Use STAR format (Situation, Task, Action, Result)
     - Keep examples recent and relevant to the role
