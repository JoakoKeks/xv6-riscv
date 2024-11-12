#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main() {
    int pages = 1;  // Número de páginas a proteger
    int size = pages * 4096;  // Tamaño en bytes (1 página = 4096 bytes)
    
    // Solicita memoria adicional usando sbrk
    void *mem = sbrk(size);
    if ((uint64)mem == (uint64)-1) {
        printf("Error al solicitar memoria\n");
        exit(1);
    }

    printf("Memoria asignada en %p\n", mem);

    // Llama a mprotect para marcar la página como solo lectura
    if (mprotect(mem, pages) == -1) {
        printf("Error al aplicar mprotect\n");
        exit(1);
    }
    printf("mprotect aplicado. Memoria marcada como solo lectura.\n");

    // Intenta escribir en la memoria protegida (esto debería fallar y terminar el proceso)
    printf("Intentando escribir en memoria protegida...\n");
    *(char *)mem = 'A';  // Esta escritura debería causar un fallo de protección y terminar el proceso

    // Este mensaje no debería imprimirse si mprotect funciona correctamente
    printf("ERROR: Escribió en memoria protegida sin fallo.\n");

    // Llama a munprotect para permitir la escritura
    if (munprotect(mem, pages) == -1) {
        printf("Error al aplicar munprotect\n");
        exit(1);
    }
    printf("munprotect aplicado. Memoria ahora es de lectura/escritura.\n");

    // Intenta escribir en la memoria desprotegida (esto debería funcionar)
    printf("Intentando escribir en memoria desprotegida...\n");
    *(char *)mem = 'B';  // Esta escritura debería ser exitosa
    printf("Escritura exitosa en memoria desprotegida.\n");

    printf("Prueba completada.\n");
    exit(0);
}
