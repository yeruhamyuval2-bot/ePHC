# Questions I did

## Leetcode

- 328. Odd Even Linked List
- 393. UTF-8 Validation
- 2502. Design Memory Allocator
- 150. Evaluate Reverse Polish Notation
- 190. Reverse Bits (not allow to shift to overflow)
- 338. Counting Bits (shift is like 2**)
- 762. Prime Number of Set Bits in Binary Representation
- 8. String to Integer (atoi)

## Non leetcode

### Implement sizeof

Write a macro or inline function `MY_SIZEOF(x)` in C that computes the size (in bytes) of any variable or type `x`, similar to the built‑in `sizeof`, *without using the `sizeof` operator itself*. It should work for primitive types, arrays, structs, etc., and return a compile-time constant.

**Example:**

```c
int a[10];
struct Point { double x, y; } p;

printf("bytes in a: %zu\n", MY_SIZEOF(a));   // expected 10 * sizeof(int)
printf("bytes in p: %zu\n", MY_SIZEOF(p));   // expected size of Point struct
```

Be careful of Packed structs and unpack using `memcpy` that is byte by byte or real loop of chars. Also it is possible to do sizeof(arr) when arr was declared static before but it can lead to dangerous behavior.

### 🔒 Problem: Design Thread‑Safe Queue (C)

**Description**
Implement a threadsafe queue supporting concurrent producers and consumers using POSIX threads (`pthread`). Your queue must satisfy:

- **FIFO** behavior for enqueue/dequeue.
- **Multiple producers** may call `enqueue()` concurrently.
- **Multiple consumers** may call `dequeue()` concurrently.
- **Blocking on dequeue:** If the queue is empty, `dequeue()` should block until an item is available.
- **Blocking on enqueue (optional bonus):** If a capacity limit is set, `enqueue()` should block when full.

**API Skeleton (C)**

```c
typedef struct ts_queue ts_queue_t;

// Create a queue with optional capacity (0 = unbounded)
ts_queue_t* ts_queue_create(size_t capacity);

// Destroy queue and free resources
void ts_queue_destroy(ts_queue_t* q);

// Enqueue an item (blocks if full)
void ts_enqueue(ts_queue_t* q, void* item);

// Dequeue an item (blocks if empty)
void* ts_dequeue(ts_queue_t* q);
```

**Requirements**

1. Use `pthread_mutex_t` and `pthread_cond_t`.
2. Ensure correct signaling so dequeuers wake when items arrive, and enqueuers wake when space frees (if bounded).
3. Use REACTO: Repeat the prompt; then step through approach, code, test, optimize.

Remember that queue needs tail and head to make it o(1). Also on cond wait we need a condition to signal to and mutex to unlock for other tasks, and to wrap the cond with a while, to validate that we still get the first condition as we wanted.

### 🎯 **Interview Prompt – Aligned Malloc in C**

> “Write two functions in C:
> `void *aligned_malloc(size_t bytes, size_t alignment);`
> `void aligned_free(void *ptr);`
>
> - `alignment` is guaranteed to be a power of two (e.g., 8, 16, 64).
> - `aligned_malloc` must return a memory address that’s a multiple of `alignment`, with at least `bytes` usable beyond that.
> - `aligned_free` must correctly free the original allocation.”
>
> *(Commonly asked in interviews and described in many C/C++ programming resources.)* ([vayudoot.org][1], [tianrunhe.wordpress.com][2])

The main idea of this question is that we need to align the address that we get from the malloc back, so we need to ask for the maximum size of bytes to be able to handle all returned addresses. Then we will be able to calculate the aligned offset with binary operation that closet aligned address.
We need to remember to cast correct (void * does not allow arithmetics because it doesn't have explicit size).

#### 🧠 TL;DR Rules

| Pointer type | Add/subtract integers? | Subtract pointers?      | Dereference?      |
| ------------ | ---------------------- | ----------------------- | ----------------- |
| `T *`        | ✅ yes                  | ✅ if same array         | ✅ within bounds   |
| `void *`     | ❌ not allowed          | ❌ illegal               | ❌ must cast first |
| `char *`     | ✅ fine-grained         | ✅ useful for raw memory | ✅                 |

### **Interview Prompt – Return endianness**

**Problem Statement:**
Write a C function that returns 1 if the system is little‑endian, and 0 if it’s big‑endian. Provide a short `main()` that calls this and prints the result.

> **Constraints:**
>
> - Use only standard C (e.g., `<stdio.h>`, no platform‑specific libs).
> - Avoid compiler macros (e.g., no `__BYTE_ORDER__`).
> - Keep it concise and explain your reasoning clearly.

### 📋 **Problem: Implement a simple `malloc` with a fixed-size array in C**

You have a fixed-size memory buffer, for example:

```c
#define HEAP_SIZE 1024
static uint8_t heap[HEAP_SIZE];
```

Implement two functions:

```c
void* my_malloc(size_t size);
void my_free(void* ptr);
```

Your allocator should:

1. Use the `heap[]` array as the only source of memory.
2. Return `NULL` if the requested `size` cannot be allocated.
3. Support multiple allocations and frees.
4. Optionally, handle fragmentation (this can be a stretch goal).

---

### 🔎 Socratic Exploration

Let’s begin with a question:

**1. What data structures or metadata do we need to store in the heap to manage allocations and frees?** (Hint: think about how to know which parts are free and which are occupied.)
I found a fitting LeetCode‑style challenge that aligns with your criteria:

### 🧠 **Find all bit‑aligned occurrences of a 32‑bit pattern inside a byte buffer.**

**"Find all bit‑aligned occurrences of a 32‑bit pattern inside a byte buffer."**

- **Input:** a buffer (e.g., `uint8_t buf[]`) and a 32‑bit pattern (`uint32_t pat`).
- **Goal:** return the starting **bit** positions (not bytes!) where `pat` appears.

This is less common than byte-wise search, but discussions like this one on Stack Overflow discuss exactly that:

> “Given a source (8, 16, 24, or 32 bits)… find how many times the bit‑pattern occurs as we shift one bit at a time.” ([youtube.com][1], [reddit.com][2], [stackoverflow.com][3])
