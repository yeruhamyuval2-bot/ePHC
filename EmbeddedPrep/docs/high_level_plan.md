### 0 → What Apple is (quietly) looking for

Recent postings for **Wi‑Fi MAC / Embedded‑FW roles** at Apple emphasise three recurring themes:

* **Own the MAC‑layer firmware**: architect, implement, bring‑up and tune it on pre‑production silicon. ([Apple][1], [Apple][2])
* **Cross‑layer system thinking**: debug PHY↔MAC↔IP interactions, model new scheduling / power‑save algorithms, and drive 802.11 ax/be features into silicon. ([Apple][3], [Apple][4])
* **Write production‑grade embedded C** that ships in hundreds of millions of devices, with rigorous code reviews, unit‑tests, and field diagnostics. ([Fairygodboss][5], [Apple][6])

---

## 1 | Skill Matrix & Depth‑check

| Domain                      | Depth Apple Expects                                                                   | Self‑check prompts                                                                                                      |
| --------------------------- | ------------------------------------------------------------------------------------- | ----------------------------------------------------------------------------------------------------------------------- |
| **Embedded C**              | MISRA‑style safety, zero‑cost abstractions, DMA/ISR‑safe code, UB traps               | Can you write a lock‑free ring buffer that survives ISR pre‑emption without `volatile` misuse?                          |
| **Wi‑Fi MAC (802.11 a–be)** | DCF/EDCA timing, RTS/CTS, Block‑Ack, TWT, OFDMA scheduling, power‑save state machines | Walk through every frame‑exchange when a client roams from AP 1 to AP 2 under 11ax with BSS Coloring enabled.           |
| **System design**           | Partitioning FW tasks, buffer ownership, cross‑layer metrics, field‑upgrade safety    | Design a MAC scheduler that guarantees ≤4 ms latency for AirPods *and* ≥600 Mb/s bulk throughput on the same SoC radio. |
| **Debug/Bring‑up**          | JTAG/T32, Wireshark, spectrum analysers, on‑board logs → cloud dashboards             | Given random TX stalls after 37 min of heavy traffic, outline your dump/trace strategy.                                 |
| **Algorithms**              | Rate adaptation, CWmin/CWmax tuning, airtime fairness, interference detection         | Explain how Minstrel‑HT chooses the next MCS and how you’d change it for U‑NII‑4 congestion.                            |
| **Behavioural**             | Cross‑disciplinary teamwork, bias‑for‑action, customer focus                          | Describe a time you pushed a late‑cycle FW fix and how you de‑risked it.                                                |

---

## 2 | Four‑Week Prep Plan (adjust pacing as needed)

| Week                                    | Focus                                                                                                                                                                                                          | Must‑do artefacts                                                               |
| --------------------------------------- | -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------- |
| **1 – Refresh C & RT basics**           | *Morning*: Re‑read ***Effective C*** (van der Linden), implement 5 pointer‑heavy LeetCode problems each day.<br>*Afternoon*: Write an ISR‑safe circular buffer & unit‑test it (Ceedling).                      | ✅ Git repo with unit‑tested ring buffer, bit‑ops utils.                         |
| **2 – Core 802.11 MAC**                 | Annotate the 802.11‑2020 state machine for scan→auth→assoc→data. Code a Beacon/AssocReq parser in C.<br>Evenings: watch Stanford EE 379C Wi‑Fi 6 lectures (free on YouTube).                                   | ✅ Cheat‑sheet of all inter‑frame timings (SIFS/DIFS/PIFS/EIFS).                 |
| **3 – Advanced features & algorithms**  | Study 11ax TWT, OFDMA RU allocation, BSS Coloring. Simulate CSMA/CA back‑off in Python and graph throughput vs CWmin.<br>Re‑implement Minstrel‑HT decision logic in C.                                         | ✅ Jupyter notebook + README explaining algorithm trade‑offs.                    |
| **4 – System‑design & mock interviews** | Twice‑weekly 1‑hr whiteboard sessions (peer or Pramp) on: <br>• Power‑save architecture for Apple Watch <br>• Multi‑radio coexistence (BT/Wi‑Fi/UWB) arbitration <br>• Secure FW upgrade flow with A/B images. | ✅ Slide deck (5–7 pp) per design; record a 5‑min pitch video for self‑critique. |

---

## 3 | Representative Interview‑style Questions & How to Practise

### 3.1 Coding (C, 30–45 min each)

1. **Sliding‑window CRC32**: implement in ≤ O(N) with a 256‑entry LUT; discuss memory/latency trade‑offs.
2. **Thread‑safe message queue** for Wi‑Fi Tx descriptors with single producer (ISR) + single consumer (task).
3. Parse a concatenated stream of 802.11 frames and detect missing Block‑Ack bitmaps.

*Practice:* time‑box to 25 min; compile with `-Wall -Werror -fsanitize=address,undefined`. Unit‑test ⟶ code review for MISRA violations.

### 3.2 Wi‑Fi / MAC Theory

* Rapid‑fire:

  * What is NAV and how does it mitigate the hidden node problem?
  * Contrast EDCA AC\_VO vs AC\_BE contention parameters.
  * Explain TWT negotiation – who sends the TWT element and why?

* Deep dive (45 min whiteboard):

  * Design a **latency‑first scheduler** for AR/VR traffic over Wi‑Fi 7 (EHT). Cover station capability exchange, AP coordination, RU allocation, and back‑pressure to the TCP stack.

### 3.3 System‑design Scenarios

> “Your firmware must support seamless roaming for Apple Watch (<50 ms hand‑off) without noticeable audio glitches.”

Break down: hardware triggers, pre‑authentication, buffered frames, host‑driver hints, KPI logging, rollback plan.

### 3.4 Debug & Bring‑up

*Given: intermittent throughput dips every \~60 s in a chamber test.*
Outline a hypothesis tree, the instrumentation you’d add (MacStat counters, RF env capture), and how you’d bisect FW vs PHY causes.

---

## 4 | Resource Stack

| Topic                  | Resource                                                                                      |
| ---------------------- | --------------------------------------------------------------------------------------------- |
| **802.11 spec**        | IEEE Std 802.11‑2020 (skim Clause 10 MAC; Clause 11 MAC management)                           |
| **Wi‑Fi 6/6E/7 books** | *Wi‑Fi 6 Protocol and Network Architecture* (Cisco Press)                                     |
| **Embedded C**         | *Clean Code in C* (Miroslav Lach) & *Embedded C Coding Standard* (Barr Group)                 |
| **Algorithms**         | Minstrel‑HT source (Linux `mac80211/rate.c`)                                                  |
| **Tools**              | Wireshark + 802.11 dissector, **wmediumd** for MAC simulation, Saleae Logic for SDIO sniffing |
| **Mock‑interview**     | Interviewing.io (systems) + LeetCode “C language” tag                                         |

---

## 5 | Day‑of Interview Tips

* **Clarify use‑case first** – Apple loves customer impact framing (“AirPods battery life”, “HomePod latency”).
* **Show cross‑layer thinking** – tie MAC decisions to PHY constraints and application KPIs.
* **Think power & memory** – always mention SRAM budget, DVFS, deep‑sleep entry/exit.
* **Structured answers** – state approach → dive deep → summarise trade‑offs.
* **Behavioural** – pick STAR stories that show collaboration with silicon, RF, and iOS teams.

---

### Next step

If you’d like, share a specific job ID or any particular algorithm you feel weak in, and we can drill down with practice problems or a mock whiteboard session script. Good luck—Apple interviews are tough but very learnable with focused prep!

[1]: https://jobs.apple.com/en-us/details/200555904/wifi-embedded-software-engineer?utm_source=chatgpt.com "WiFi Embedded Software Engineer - Jobs - Careers at Apple"
[2]: https://jobs.apple.com/en-us/details/200603258/wifi-mac-systems-engineer?team=HRDWR&utm_source=chatgpt.com "WiFi MAC Systems Engineer - Jobs - Careers at Apple"
[3]: https://jobs.apple.com/en-us/details/200582955/wireless-systems-engineer-cross-layer-design?utm_source=chatgpt.com "Wireless Systems Engineer, Cross-Layer Design - Apple Careers"
[4]: https://jobs.apple.com/en-us/details/200589380/wi-fi-mac-standard-architect?team=SFTWR&utm_source=chatgpt.com "Wi-Fi MAC Standard Architect - Careers at Apple"
[5]: https://fairygodboss.com/jobs/apple/wifi-embedded-software-engineer-55162ddf765314217d261355f7a6adb4?utm_source=chatgpt.com "WiFi Embedded Software Engineer at Apple in Cupertino, CA"
[6]: https://jobs.apple.com/en-us/details/200597775/wifi-fw-engineer?team=HRDWR&utm_source=chatgpt.com "WiFi FW Engineer - Jobs - Careers at Apple"
