#include "kernel/types.h"
#include "user/user.h"


int main(int argc, char *argv[]) {
    int num_processes = 20;
    int i;
    for (i = 0; i < num_processes; i++) {
        if (fork() == 0) {
            // Código del hijo
            int pid = getpid();
            printf("Proceso %d ejecutándose\n", pid);
            sleep(10);
            exit(0); // El hijo termina
        } else {
            // Código del padre: espera a que el hijo termine
            wait(0);
        }
    }
    exit(0);  // El padre también termina
}