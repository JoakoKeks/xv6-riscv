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
sys_mprotect(void) {
  uint64 addr;
  int len;
  struct proc *p = myproc();

//Extraemos los argumentos de la llamada del sistema
  argaddr(0, &addr);
  argint(1, &len);
  
//Verificamos si ocurrió algún error al obtener dirección y tamaño del archivo
  if (len <= 0 || addr <= 0)  
    return -1;
  
//Generamos el código para deshabilitar escritura y habilitar solo lectura
  for(int i = 0; i < len; i++)
  {
    pte_t *pte = walk(p->pagetable, addr + i * PGSIZE, 0);
    if(pte == 0 || (*pte & PTE_V) == 0)
      return -1;

    *pte |= PTE_R;   //Habilitamos solo lectura
    *pte &= ~PTE_W;; //Deshabilitamos escritura
    
  }
  
  sfence_vma();
  return 0;
}

uint64 
sys_munprotect(void) {
  uint64 addr;
  int len;
  struct proc *p = myproc();

//Extraemos los argumentos de la llamada del sistema
  argaddr(0, &addr);
  argint(1, &len);

//Verificamos si ocurrió algún error al obtener dirección y tamaño del archivo  
  if (len <= 0 || addr <= 0)  
    return -1;
  if(addr % PGSIZE != 0 || len <= 0)
    return -1;
  
//Generamos el código que nos permite rehabilitar la escritura
  for(int i = 0; i < len; i++) 
  {
    pte_t *pte = walk(p->pagetable, addr + i * PGSIZE, 0);
    if(pte == 0 || (*pte & PTE_V) == 0)
      return -1;

    *pte |= PTE_W;  // Habilitar escritura
  }
  
  sfence_vma();
  return 0;
}
