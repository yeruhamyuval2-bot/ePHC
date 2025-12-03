1. What is the scheduler on MCU freeRTOS?
   1. priority based preeamptive scheduler. round robin with TIME_SLCING on the same priority.
2. Does every M4 arm has MPU?
3. how memory pool in implemented in freeRTOS?
4. What are the configured RTOS flags? 
   1. 
5. how can we know about stackoverflow?
   1. By checking the SP if it run of the limit.
   2. By putting known value at the end of stack and see if overridden.
6. What is special in newlib reentrant?
   1. They use gloabl var and can have race condition.
   2. Malloc by new lib uses critical section
7. Why can't high-priority ISEs call freeRTOS APIs?
   1. When using freeRTOS api, it uses critical section that masks 5-255 priorites but higher priorites are not protected from other high priorties ISR.
8. What kind of heap algorithems are there?
   1. Coalescing - mergin two adjacent free blocks. In malloc it goes over linked list and search for the next high free blocks that is big enough and split it. When free, it takes the block and find its place by address, add it to the list and check the adjact nodes. There is a magic marker to help detect corruption of double free.
   2. Doug Lea's malloc (for experts)
9. How can we make multiple timers using only one hardware timer?
   1.  Use linked list with relative deltas. every insert or timeout, we update all the timers in the list.
   2.  Min-heap/binary heap insert/remove O(log n)
   3.  Hierarchical timer wheel
10. What is buddy tree?
    1.  It is used when need to allocate memory into 2^n size. when free it, merge the two nearset cells. It does minimal external fregmentation but increase the internal fregmentation.
11. What is Slab cache?
    1.  like memory pool, it splits the memory into fixed size objects and save the resource allocations in a bitmap. we need to remember that in each cache, it saves the bitmap.
12. Count sempahore in FreeRTOS.
    1.  It saves the waiting tasks in a linked list.
13. How the DMA works? How many packets can it save?
14. 