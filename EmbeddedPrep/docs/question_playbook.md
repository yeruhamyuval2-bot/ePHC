# Bitmask Coding Playbook

## 0  How to Use This Playbook

* Purpose and audience
* Interactive workflow (Socratic Q\&A)

## 1  Bitwise Foundations

### 1.1 Binary representation refresher

* Binary is base-2: each digit is a bit (0 or 1).
* Left-shifting multiplies by 2; right-shifting divides by 2 (for unsigned).
* Useful for compact representation, toggles, masks.

### 1.2 Core operators (`&`, `|`, `^`, `~`, `<<`, `>>`) – one-liner summary

* `a & b`: AND – only bits set in both `a` and `b` remain `1`
* `a | b`: OR – bits set in either `a` or `b` become `1`
* `a ^ b`: XOR – bits set in `a` or `b` but not both become `1`
* `~a`: NOT – inverts all bits in `a`
* `a << n`: shift left – move bits `n` positions left (multiply by `2^n`)
* `a >> n`: shift right – move bits `n` positions right (unsigned: divide by `2^n`)

### 1.3 Operator precedence cheatsheet

* `~`, `<<`, `>>` have higher precedence than `&`
* `&` > `^` > `|` in evaluation order
* Use parentheses to clarify complex expressions

### 1.4 Signed vs unsigned shifts

* Right shift (`>>`) on signed numbers is implementation-defined (arithmetic vs logical)
* Always use unsigned types for portable shift behavior

### 1.5 Common pitfalls

* Shifting by ≥ bit-width of the type (undefined behavior)
* Negative shift counts
* Sign extension with `>>` on signed values
* Off-by-one errors in bitmasks (e.g., `1 << 31` vs `1U << 31`)

## 2  Essential Idioms & Micro‑Patterns

### 2.1 Test / set / clear / toggle bit

* **Test bit k**: `(x & (1 << k)) != 0`
* **Set bit k**: `x | (1 << k)`
* **Clear bit k**: `x & ~(1 << k)`
* **Toggle bit k**: `x ^ (1 << k)`

### 2.2 Isolate lowest set bit

* **Extract LSB mask**: `x & -x`
* This yields a binary mask where only the least significant `1` bit in `x` is preserved.
* Useful for algorithms involving subsets, tries, or priority encoding.
* Relies on two's complement: `-x` flips all bits of `x` and adds 1, so the lowest `1` bit in `x` survives.

### 2.3 Check if only one bit is set (power of two)

* **Check**: `(x & (x - 1)) == 0` and `x != 0`
* A number with exactly one bit set (e.g., powers of two) will pass this check.
* Works because `x - 1` clears the lowest set bit, so ANDing it with `x` zeroes everything.

### 2.4 Count set bits (population count)

* **Loop with LSB clearing**: `while (x) { x &= (x - 1); count++; }`
* **Lookup table**: sum precomputed values for each byte
* **Bit hacks**: use masks/shifts to sum bit groups (e.g., Hamming weight algorithm)

### 2.5 Clear lowest set bit

* `x & (x - 1)`
* Drops the lowest `1` bit in `x`.

### 2.6 Set lowest unset bit

* `x | (x + 1)`
* Turns on the lowest `0` bit in `x`.

### 2.7 Mask all bits below the lowest set bit

* `x ^ (x & -x)`
* This creates a mask of all bits **lower** than the lowest `1` bit in `x`.

### 2.8 Compute parity (even/odd count of 1s)

* XOR fold method: `x ^= x >> 1; x ^= x >> 2; x ^= x >> 4; x ^= x >> 8; x ^= x >> 16; parity = x & 1`

### 2.9 Next number with same number of 1s

* Generate the lexicographically next number with same bit count.

* ```c
  c = x & -x;
  r = x + c;
  next = (((r ^ x) >> 2) / c) | r;
  ```

---

## 3  LeetCode Study Path for Embedded Engineers

| #  | Category / Tag                    | Focus Level | Why It Matters (Embedded)                           | Recommended Progression (Easy → Medium)                                                                                                                                                                    |
| -- | --------------------------------- | ----------- | --------------------------------------------------- | ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| 1  | **Bit Manipulation**              | ⭐⭐⭐⭐        | Register bits, masks, protocol flags                | **191. Number of 1 Bits (Easy)** → **136. Single Number (Easy)** → **268. Missing Number (Easy‑Medium)** → **389. Find the Difference (Easy)** → 89. Gray Code (Medium) → **201. Bitwise AND of Numbers Range (Medium)**       |
| 2  | **Array & Pointer Logic**         | ⭐⭐⭐⭐        | Memory‑centric operations, DMA buffers              | **26. Remove Duplicates from Sorted Array (Easy)** → **283. Move Zeroes (Easy)** → **88. Merge Sorted Array (Easy)** → **153. Find Minimum in Rotated Sorted Array (Medium)** → **238. Product of Array Except Self (Medium)** |
| 3  | **Sliding Window / Two Pointers** | ⭐⭐⭐         | Real‑time stream windows, overlap handling          | 121. Best Time to Buy and Sell Stock (Easy) → 169. Majority Element (Easy) → **209. Min Size Subarray Sum (Medium)** → 567. Permutation in String (Medium)                                                     |
| 4  | **Greedy**                        | ⭐⭐⭐         | Scheduling, power‑saving slots, resource allocation | 455. Assign Cookies (Easy) → 392. Is Subsequence (Easy) → 860. Lemonade Change (Easy) → **134. Gas Station (Medium)** → 406. Queue Reconstruction by Height (Medium)                                           |
| 5  | **Math (Modulo / Pow‑of‑2)**      | ⭐⭐⭐         | Timing, counters, power‑of‑two operations           | 7. Reverse Integer (Easy) → 231. Power of Two (Easy) → 202. Happy Number (Easy) → **172. Factorial Trailing Zeroes (Medium)**           |
| 6  | **Stack**                         | ⭐⭐          | Nested state machines, expression parsing           | 20. Valid Parentheses (Easy) → 155. Min Stack (Easy) → 496. Next Greater Element I (Easy) → **739. Daily Temperatures**(Medium)                                                                               |
| 7  | **Queue / Deque**                 | ⭐⭐          | Task queues, producer‑consumer buffers              | 933. Number of Recent Calls (Easy) → 622. Design Circular Queue (Medium) → 641. Design Circular Deque (Medium)                                                                                             |
| 8  | **Linked List**                   | ⭐⭐          | Packet chains, custom allocators                    | 206. Reverse Linked List (Easy) → 21. Merge Two Sorted Lists (Easy) → 141. Linked List Cycle (Easy) → 234. Palindrome Linked List (Medium)                                                                 |
| 9  | **Trie / Prefix Structures**      | ⭐⭐          | Command parsers, prefix lookup                      | 208. Implement Trie (Medium) → 211. Design Add and Search Words DS (Medium)                                                                                                                                |
| 10 | **Prefix Sum / Difference**       | ⭐⭐          | Real‑time stats, histograms                         | 724. Find Pivot Index (Easy) → 560. Subarray Sum Equals K (Medium)                                                                                                                                         |

*Progression order:* Work row‑wise from #1 to #10. Within each category, solve problems left→right. Move to next category when comfortable.

Here’s a single, **easy-→-hard progression** of well-known synchronisation / deadlock / starvation problems.
Only problems with publicly available statements are included; nothing was invented.

| ▲  | Problem & Site                                             | Core Topic                     | Site Tag\*    | Why it’s placed here / main trap                                                                                                       |
| -- | ---------------------------------------------------------- | ------------------------------ | ------------- | -------------------------------------------------------------------------------------------------------------------------------------- |
| 1  | **Print in Order — LeetCode #1114**                      | Simple ordering (1→2→3)        | Easy          | Smallest mutex/flag exercise; perfect warm-up. ([LeetCode][1])                                                                         |
| 2  | **Fizz Buzz Multithreaded — LeetCode #1195**               | 4-thread coordination          | Easy          | Adds multiple mutually-exclusive outputs without shared-resource contention. ([LeetCode][2])                                           |
| 3  | **Print FooBar Alternately — LeetCode #1115**              | Two-thread ping-pong           | Medium        | First true alternation pattern; needs hand-off logic. ([LeetCode][3])                                                                  |
| 4  | **Print Zero Even Odd — LeetCode #1116**                   | Cyclic sequencing (3 threads)  | Medium        | Requires robust condition-wait loop & spurious-wake handling. ([LeetCode][4])                                                          |
| 5  | **Producer - Consumer (Semaphores) — GeeksforGeeks**       | Bounded buffer, no deadlock    | — (Med)       | Classic full/empty count; introduces starvation-free resource sharing. ([GeeksforGeeks][5])                                            |
| 6  | **Sleeping Barber — GeeksforGeeks**                        | Blocking + wake-up races       | — (Med)       | Adds limited waiting chairs and barber sleep logic; easy to deadlock if mis-signalled. ([GeeksforGeeks][6])                            |
| 7  | **Building H₂O — LeetCode #1117**                          | Barrier + fairness             | Medium        | Must group 2 H + 1 O atomically; fairness starts to bite. ([LeetCode][7])                                                              |
| 8  | Readers–Writers (Writer-priority) — GeeksforGeeks      | Starvation scenario            | — (Med-Hard)  | Shows how prioritising writers can starve readers; fairness fixes get subtle. ([GeeksforGeeks][8])                                     |
| 9  | **Dining Philosophers** — LeetCode #1226                   | Deadlock + starvation          | Medium        | Canonical shared-resource ring; forces lock ordering / try-lock solutions. ([LeetCode][9])                                             |
| 10 | Traffic Light Controlled Intersection — LeetCode #1279 | Mutual exclusion + fairness    | Hard          | Real-world intersection model; demands O(1) hand-off & starvation-free policy. ([Leetcode][10])                                        |
| 11 | **Cigarette Smokers** — Wikipedia                          | Conditional resource combo     | — (Hard)      | Three ingredients, one agent; naïve semaphore use deadlocks/livelocks. ([Wikipedia][11])                                               |
| 12 | Unisex Bathroom — Educative.io                         | Capacity + two-group exclusion | — (Hard)      | Must bar mixed occupancy **and** cap total users ≤ 3, without starving either gender. ([Educative][12])                                |
| 13 | Banker’s Algorithm (Safety) — GeeksforGeeks            | Deadlock avoidance             | — (Hard)      | Teaches system-wide safe-state checks before resource grants. ([GeeksforGeeks][13])                                                    |
| 14 | Deadlock Detection & Recovery — GeeksforGeeks          | Post-facto detection           | — (Hard)      | Full resource-wait-graph analysis + recovery policies. ([GeeksforGeeks][14])                                                           |
| 15 | Santa Claus Problem — Tufts PDF                        | Group formation + priority     | — (Very Hard) | Combines barriers, multiple condition vars, priority (reindeer > elves); graduate-level monitor design. ([Tufts Computer Science][15]) |

\* LeetCode rows list the platform’s official difficulty; GeeksforGeeks/Wikipedia/Educative don’t tag, so the level shown reflects typical interview consensus.

### Suggested study path

Work **top-to-bottom**. Each rung introduces exactly one new wrinkle—ordering → cyclic hand-off → bounded buffers → fairness → deadlock → system-level avoidance/detection.
Apply your usual **REACTO** cadence, time-boxing roughly:

* Easy rows (1-3): ≤ 20 min each
* Medium (4-7): 30–40 min
* Med-Hard (8-10): 45–60 min
* Hard (11-15): 60–90 min (allow more for Santa).

When you’re ready, tell me the number you’d like to tackle next, and we’ll start at the **“Repeat-the-Question”** stage.

[1]: https://leetcode.com/problems/print-in-order/?utm_source=chatgpt.com "Print in Order - LeetCode"
[2]: https://leetcode.com/problems/fizz-buzz-multithreaded/?utm_source=chatgpt.com "Fizz Buzz Multithreaded - LeetCode"
[3]: https://leetcode.com/problems/print-foobar-alternately/?utm_source=chatgpt.com "Print FooBar Alternately - LeetCode"
[4]: https://leetcode.com/problems/print-zero-even-odd/?utm_source=chatgpt.com "Print Zero Even Odd - LeetCode"
[5]: https://www.geeksforgeeks.org/producer-consumer-problem-using-semaphores-set-1/?utm_source=chatgpt.com "Producer Consumer Problem using Semaphores | Set 1"
[6]: https://www.geeksforgeeks.org/sleeping-barber-problem-in-process-synchronization/?utm_source=chatgpt.com "Sleeping Barber problem in Process Synchronization | GeeksforGeeks"
[7]: https://leetcode.com/problems/building-h2o/?utm_source=chatgpt.com "Building H2O - LeetCode"
[8]: https://www.geeksforgeeks.org/readers-writers-problem-writers-preference-solution/?utm_source=chatgpt.com "Readers-Writers Problem | Writers Preference Solution"
[9]: https://leetcode.com/problems/the-dining-philosophers/?utm_source=chatgpt.com "The Dining Philosophers - LeetCode"
[10]: https://leetcode.ca/all/1279.html?utm_source=chatgpt.com "1279. Traffic Light Controlled Intersection - leetcode.ca"
[11]: https://en.wikipedia.org/wiki/Cigarette_smokers_problem?utm_source=chatgpt.com "Cigarette smokers problem"
[12]: https://www.educative.io/courses/java-multithreading-for-senior-engineering-interviews/unisex-bathroom-problem?utm_source=chatgpt.com "Unisex Bathroom Problem - Educative.io"
[13]: https://www.geeksforgeeks.org/bankers-algorithm-in-operating-system-2/?utm_source=chatgpt.com "Banker's Algorithm in Operating System | GeeksforGeeks"
[14]: https://www.geeksforgeeks.org/deadlock-detection-recovery/?utm_source=chatgpt.com "Deadlock Detection And Recovery | GeeksforGeeks"
[15]: https://www.cs.tufts.edu/comp/21/hw/solutions/puzzlesextra/LBoS5.5-Santa-Claus-solution.pdf?utm_source=chatgpt.com "[PDF] 5.5.2 Santa problem solution"
