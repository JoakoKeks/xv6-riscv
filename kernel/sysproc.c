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
  if(n < 0)
    n = 0;
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
sys_send(void) {
    int pid;
    char msg[MAX_MESSAGE_LENGTH];
    argint(0, &pid);
if (argint(0, &pid) < 0) {
    return -1;
}
argstr(1, msg, MAX_MESSAGE_LENGTH);
if (argstr(1, msg, MAX_MESSAGE_LENGTH) < 0) {
    return -1;
}

    if (msg_queue.size >= MAX_QUEUE_SIZE) {
        release(&msg_queue.lock);
        return -1; // Cola llena
    }

    message new_msg;
    new_msg.sender_pid = myproc()->pid;
    safestrcpy(new_msg.content, msg, MAX_MESSAGE_LENGTH);

    msg_queue.messages[msg_queue.tail] = new_msg;
    msg_queue.tail = (msg_queue.tail + 1) % MAX_QUEUE_SIZE;
    msg_queue.size++;

    wakeup(&msg_queue);

    release(&msg_queue.lock);
    return 0;
}


uint64
sys_receive(void) {
    char buffer[MAX_MESSAGE_LENGTH];
    if (argstr(0, buffer, MAX_MESSAGE_LENGTH) < 0)
      return -1;


    acquire(&msg_queue.lock);

    while (msg_queue.size == 0) {
        sleep(&msg_queue, &msg_queue.lock);
    }

    message received_msg = msg_queue.messages[msg_queue.head];
    msg_queue.head = (msg_queue.head + 1) % MAX_QUEUE_SIZE;
    msg_queue.size--;

    safestrcpy(buffer, received_msg.content, MAX_MESSAGE_LENGTH);

    release(&msg_queue.lock);
    return received_msg.sender_pid;
}

