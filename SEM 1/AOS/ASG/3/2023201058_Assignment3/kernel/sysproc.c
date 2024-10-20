#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "date.h"

uint64
sys_exit(void)
{
    int n;
    argint(0, &n);
    exit(n);
    return 0; // not reached
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
sys_waitx(void)
{
    uint64 p, raddr, waddr;
    uint64 rtime, wtime;

    // if(argaddr(0, &p) < 0)
    //   return -1;
    // if (argaddr(1, &raddr) < 0)
    //   return -1;
    // if (argaddr(2, &waddr) < 0)
    //   return -1;
    argaddr(0, &p);
    argaddr(1, &raddr);
    argaddr(2, &waddr);

    int ret = waitx(p, &rtime, &wtime);
    struct proc *proc = myproc();
    if (copyout(proc->pagetable, raddr, (char *)&rtime, sizeof(int)) < 0)
        return -1;
    if (copyout(proc->pagetable, waddr, (char *)&wtime, sizeof(int)) < 0)
        return -1;
    return ret;
}

uint64
sys_sbrk(void)
{
    uint64 addr;
    int n;

    argint(0, &n);
    addr = myproc()->sz;
    if (growproc(n) < 0)
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
    while (ticks - ticks0 < n)
    {
        if (killed(myproc()))
        {
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

// int
uint64
sys_hello(void)
{
    // cprintf("Hello, world!\n");
    // return 0;
    printf("Hello, world!\n");
    return 12;
}

uint64
sys_trace(void)
{
    int n;
    argint(0, &n);
    trace(n);
    return 0;
}

uint64
sys_set_priority(void)
{
#ifndef PBS
    return -1;
#endif
    int priority, pid;
    int old = -1;
    argint(0, &priority);
    argint(1, &pid);
    set_priority(priority, pid, &old);
    return old;
}