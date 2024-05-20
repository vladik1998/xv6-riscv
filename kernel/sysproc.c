#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  char buf[32];
  int is_null;
  argint(0, &n);
  // To copy the exit msg from userspace to kernal space.
  is_null = argstr(1, buf, sizeof(buf));
  printf("Size read: %d, Buffer: %d, the string is: %s\n", is_null, sizeof(buf), buf);
  if(is_null < 0){
    return -1;
  }
  if(!is_null){
    exit(n, 0);
  }
  exit(n, buf);
  return 0;  // not reached
}


uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  // A placeholder to save the exit msg.
  uint64 exit_msg_p;
  argaddr(0, &p);
  argaddr(1, &exit_msg_p);
  return wait(p, exit_msg_p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

uint64
sys_memsize(void)
{
  return myproc()->sz;
}
