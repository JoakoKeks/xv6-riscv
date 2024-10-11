#include "kernel/types.h"
#include "user/user.h"

int main() {
  for (int i = 0; i < 20; i++) {
    int pid = fork();
    if (pid == 0) {
      printf("Ejecutando proceso %d\n", i);
      sleep(2);  // Hacemos que el proceso duerma un par de segundos
      exit(0);
    }
  }

  for (int i = 0; i < 20; i++) {
    wait(0);  // Esperamos que terminen todos los procesos hijos
  }

  exit(0);
}