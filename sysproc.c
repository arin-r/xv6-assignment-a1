#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int sys_fork(void)
{
  return fork();
}

int sys_exit(void)
{
  exit();
  return 0; // not reached
}

int sys_wait(void)
{
  return wait();
}

int sys_kill(void)
{
  int pid;

  if (argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int sys_getpid(void)
{
  return myproc()->pid;
}

int sys_sbrk(void)
{
  int addr;
  int n;

  if (argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if (growproc(n) < 0)
    return -1;
  return addr;
}

int sys_sleep(void)
{
  int n;
  uint ticks0;

  if (argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while (ticks - ticks0 < n)
  {
    if (myproc()->killed)
    {
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

extern int trace_state;
extern int syscall_count[NUM_SYSCALLS];

void sys_toggle(void)
{
  trace_state = 1 - trace_state; // Toggle the state

  if (trace_state == 1)
  {
    memset(syscall_count, 0, sizeof(syscall_count)); // Reset counts on TRACE ON
  }
}

// In syscall.c
void sys_print_trace(void)
{
  if (trace_state == 0)
  {
    cprintf("Tracing is OFF. No counts to display.\n");
    return;
  }

  const char *syscall_names[] = {
      // SYS 0 is usually unused or invalid
      "unused",
      "fork",
      "exit",
      "wait",
      "pipe",
      "read",
      "kill",
      "exec",
      "fstat",
      "chdir",
      "dup",
      "getpid",
      "sbrk",
      "sleep",
      "uptime",
      "open",
      "write",
      "mknod",
      "unlink",
      "link",
      "mkdir",
      "close",
      "toggle",
      "print_trace"};

  // Print in alphabetical order
  for (int i = 0; i < NUM_SYSCALLS; i++)
  {
    if (syscall_count[i] > 0)
    {
      if (strncmp(syscall_names[i], "(null)", 6) == 0)
      {
        cprintf("Unknown syscall %d\n", i);
      }
      cprintf("%s %d\n", syscall_names[i], syscall_count[i]);
    }
  }
}
