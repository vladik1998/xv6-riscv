# Assignment 1 Theoretical Questions

## Task 2
### Q1
- Before the allocation the process is using 16384 bytes
- After the allocation the process is using 81920 bytes

### Q2
No difference

### Q3
There is no difference in the memory size before and after freeing it, because the operating system prefers to keep this memory inside the heap for various efficiency reasons:

- **System Calls Overhead**: Returning memory to the operating system involves the system call `sbrk()`, which can be expensive in terms of performance. To avoid frequent system calls, the runtime library often keeps the memory in the heap for future allocations instead of returning it immediately.

- **Heap Management**: The heap is managed by the xv6 runtime library, which maintains its own free list of memory blocks. When you call `free()`, the block of memory is added to this free list and can be reused by subsequent `malloc()` calls within the same process.
