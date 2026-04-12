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
  argint(0, &n);
  exit(n);
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
  argaddr(0, &p);
  return wait(p);
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
sys_memsize(void){
  struct proc *p = myproc();
  return p->sz;
}

uint64 sys_co_yield(void)
{
  int pid, value;
  argint(0, &pid);
  argint(1, &value);

  struct proc *p = myproc();

  // Reject invalid PID or self-yield
  if(pid <= 0 || p->pid == pid){
    return -1;
  }

  // Find target process by PID
  struct proc *target = find_proc_by_pid(pid);
  if(target == 0){
    return -1;
  }

  // Maybe dead lock condition TODO!!!!!!!!!!!!!!!!!!!!!!!!!!!!1
  // Acquire locks to safely access process state
  acquire(&p->lock);
  acquire(&target->lock);

  // Validate target process state
  if(target->state == UNUSED || target->state == USED || target->state == ZOMBIE || target->killed){
    release(&target->lock);
    release(&p->lock);
    return -1;
  }

  // =========================
  // CASE 1: Target is already waiting for us
  // =========================
  if(target->state == SLEEPING && target->chan == p){
    uint64 other_val = target->trapframe->a0;

    target->trapframe->a0 = value;
    p->trapframe->a0 = other_val;

    release(&target->lock);
    wakeup(p);

    release(&p->lock);
    return p->trapframe->a0;
  }

  // =========================
  // CASE 2: Target is not waiting (we are first)
  // =========================

  p->trapframe->a0 = value;

  release(&target->lock);

  sleep(target, &p->lock);

  release(&p->lock);
  return p->trapframe->a0;
}