#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "types.h"

char * calls_name [] = { " sys_call0 " , " sys_fork " , " sys_exit " ,
" sys_wait " , " sys_pipe " , " sys_read " , " sys_kill " , " sys_exec " ,
" sys_fstat " , " sys_chdir " , " sys_dup " ," sys_getpid " ," sys_sbrk " ,
" sys_sleep " , " sys_uptime " ," sys_open " " sys_write " ," sys_mknod " ,
" sys_unlink " , " sys_link " , " sys_mkdir " , " sys_close " ,
" sys_print_count " , " sys_toggle " , " sys_add " , " sys_ps " ,
" sys_send " , " sys_recv " , " sys_send_multi " };

extern int calls_count[syscall_len];

int toggle_on = 0;

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
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
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int
sys_print_count(void)
{
  for (int i = 1; i < syscall_len; i++)
  {
    if (calls_count[i] > 0)
      cprintf("%s %d\n", calls_name[i], calls_count[i]);
  }
  return 0;
}

int
sys_toggle(void)
{
  int old_toggle = toggle_on;
  if (toggle_on == 0)
  {
    for (int i = 0; i < syscall_len; i++)
    {
      calls_count[i] = 0;
    }
    toggle_on = 1;
  }
  else {
    toggle_on = 0;
  }
  cprintf("toggle_on switched from %d to %d\n", old_toggle, toggle_on);
  return 0;
}

int
sys_add(int a, int b)
{
  argint(0, &a);
  argint(1, &b);
  cprintf("add() get %d\n", a + b);
  return (a + b);
}

extern void print_pid(void);

int
sys_ps(void)
{
  print_pid();
  return 0;
}