
### **Weekly Active Learning Plan (6–8 Weeks Total)**

Each session blends **theory (30–40%)**, **practice (40–50%)**, and **reflection or debugging (10–20%)**.

| Week  | Session Focus                          | Learning Objectives                                                  | Exercises / Practice                                                                           |
| ----- | -------------------------------------- | -------------------------------------------------------------------- | ---------------------------------------------------------------------------------------------- |
| **1** | **Memory Safety & Embedded C**         | Bitwise operations, pointer arithmetic, memory layout, buffer safety | Implement a circular buffer (`buffer_init`, `push`, `pop`)<br>Code review of unsafe C snippets |
| **2** | **RTOS & Interrupts**                  | Task scheduling, ISRs, semaphores, queues, race conditions           | ADC interrupt → RTOS task using queue/semaphore<br>Write pseudocode and simulate timing        |
| **3** | **Peripheral Drivers (I2C/SPI/UART)**  | Peripheral init, register-level control, bus error handling          | Write an I2C driver for reading temperature from reg `0x05`<br>Mock transaction in C           |
| **4** | **Wi-Fi MAC Protocols (802.11ax)**     | DCF, backoff, OFDMA, MU-MIMO, TWT                                    | Frame exchange walkthrough (scan to IP assign)<br>Write state machine of MAC                   |
| **5** | **Debugging & Tools**                  | Use of JTAG, logic analyzer, UART debug, crash diagnosis             | Debugging scenario: Wi-Fi crash under load<br>Plan how to collect crash info, isolate          |
| **6** | **System Architecture**                | Task modularity, layering (HAL, services), ISR-task decoupling       | Design sensor hub: 5 sensors + Wi-Fi upload<br>Draw task diagram, interaction model            |
| **7** | **Optimization (Performance & Power)** | Reduce latency, DMA, low-power states, memory fragmentation          | Optimize O(n²) descriptor walk<br>Apply new structure, measure improvement                     |
| **8** | **Soft Skills + Mock Interviews**      | Ownership, communication, debugging storytelling, mentorship         | Review real examples from your past work<br>Simulate design questions / behavioral interview   |

---

### **How to Run Each Slot (1–2 hours)**

**1. Kickoff (10 min)**

* Review objective
* Watch short video or read a 2-pager primer if needed

**2. Concept Learning (25–40 min)**

* Use code samples, real-life diagrams, datasheet snippets
* Socratic questions to deepen understanding

**3. Live Exercise (45–60 min)**

* Code or pseudocode challenge
* Can be solo or paired if practicing with others

**4. Review & Reflect (10–15 min)**

* Discuss edge cases, risks, and real usage
* If time permits, code review

---

Would you like this plan delivered as a Notion/Markdown doc for tracking? I can also help prioritize based on your weaknesses or target company (e.g. Apple vs Qualcomm).
