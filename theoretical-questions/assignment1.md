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

## Task 3
### Q1
As soon as we changed these signatures, there were a lot of errors in the code. The reason for this is the fact that many functions in our operation system used a signature with one argument.

### Q2


## Task 4
### Q1
The schedueling policy is implemented in the file `kernel/proc.c` inside the function `scheduler`.

### Q2
The policy is iterating over all processes in the CPU in an infinite loop and in the same order over and over again: when a process gives up it's cpu time - the next process will run.

This is an implementation of a non-preemprive round-robin (when the processes are expected to let go of their CPU time so other processes can run).

### Q3
When a new process is created using `fork()`, it creates a process that is a duplicate of the process that created it (the parent process).

A process is calling `sched()` (giving away CPU time) in the following scenarios:

1. When it waits for some IO invoking `sleep()`.
2. When it terminates successfully invoking `exit()`.
3. When an exception occured in the user space invoking `usertrap()`.
4. When an exception occured in the kernel space invoking `kerneltrap()`.

That is, a process will complete it's CPU burst before scheduling.

### Q4
When a process calls the `sleep()` system call, its state is changing from *RUNNING* to *SLEEPING*, and then scheduling is performed to let other processes run in the meanwhile. When the process state will be *RUNNABLE* (for instance, if the user invoked the `wakeup()` system call), the process will get again the CPU time, and the sleep function will clean up.
