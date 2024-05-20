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
If the message is longer than 32 chars, `argstr()` function in the `sys_exit` system call will return -1 (because thats what happens where the string arg is longer than `max`). This will cause the `sys_exit` function to call `exit()` with `0` for the second argument and the exit message will be set to defult "No exit message". Implementing this system call like this can assure that long messages won't cause errors and will be ignored.

### Q3
If the message is shorter than 32 chars, `argstr()` function in the `sys_exit` system call will return the length of the message. This will cause the `sys_exit` function to call `exit()` with `buf` that conatins the message for the second argument. The exit message will be set to the message in the buffer. For the case when the argument is `0` we call the `exit()` with `0` for the second argument which saves the defult exit message in the proccess.

### Q4
**The flow of the exit message logic goes like this:**
- The shell creates a new child proccess and after running it, calls `wait(0, exit_msg_buf);`. This function will loop until the child status will become `ZOMBIE`.
- The child executes his code and after finishing he calls `exit(0, exit_msg)`. This line will call the `kernel/sysproc.c/sys_exit()` system call.
- In the system call implementation we copy the exit message from the user space to the kernel space using the `kernel/proc.c/argstr()` function. This is the `1st` copy of the exit message.
- After the copy, we know what is the message length and if we need to do the default behaviour (in case when longer than 32 char or 0).
- We call the `kernel/proc.c/exit()` function with the proper arguments for each case.
- In the implementation of this function we added a condition check that if it is satisfied we are in the default behavior and if not we have a real exit message.
- In any case we copy the proper exit message to the PCB of the proccess using `kernel/string.c/safestrcpy()` function. This is the `2nd` copy of the exit message that is done inside the kernel space.
- After the copy, we change the proccess state to `ZOMBIE` and the shell that we waiting, continues its wait logic.
- Inside the `kernel/proc.c/wait()` function we use the `kernel/vm.c/copyout()` function to copy the exit message from the kernel space (PCB) to the user space. This is the `3rd` copy.
- After doing so, the shell continues its execution and prints the exit message of the child proccess.
Overall, we copied the exit message 3 times.

### Q5
In the shell we added a new `static char exit_msg_buf[32];` variable. This is the place holder to the exit message. When calling `wait()` we give this static address as an argument. As we explained in Q4 when the child proccess will finish his execution, the exit message will be copied from its PCB to the address that we gave to the `wait()` function using the `kernel/vm.c/copyout()` function. In this way, after the child will finish, the shell has the exit message placed in the `exit_msg_buf` and can use it as it likes. For example we can print the message as we were asked to do in this task.

### Q6
The message is saved inside a buffer that is part of the shell class. If the shell will decide to modify it after the message is received it won't cause any error.

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
