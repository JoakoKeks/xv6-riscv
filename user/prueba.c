#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

void print_result(int result, char *message) {
  if (result < 0) {
    printf("Error: %s\n", message);
  } else {
    printf("Cambios: %s\n", message);
  }
}

int main() {
  char *filename = "Prueba.txt";
  int fd;
  // Creamos el archivo con permisos de lectura y escritura
  fd = open(filename, O_CREATE | O_RDWR);
  
  if (fd < 0) {
    printf("No se pudo crear el archivo\n");
    exit(1);
  }
  printf("Archivo %s creado con permisos de lectura y escritura con éxito\n", filename);

  // Escribimos en el archivo
  if (write(fd, "Lorem ipsum dolor sit amet.", 11) != 11) {
    printf("No se pudo escribir en el archivo\n");
    exit(1);
  }
  printf("Se logró escribir en el archivo tal y como se esperaba\n");
  close(fd);

  // Cambiamos permisos del archivo a solo lectura
  print_result(chmod(filename, 1), "Archivo modificado a solo lectura");

  fd = open(filename, O_WRONLY);
  if (fd >= 0) {
    printf("Se abrió el archivo en modo escritura con permisos de solo lectura\n");
    close(fd);
    exit(1);
  } else {
    printf("EL archivo no se pudo abrir en modo escritura (Resultado esperado considerando el cambio)\n");
  }

  // Cambiamos los permisos de solo lectura a lectura y escritura
  print_result(chmod(filename, 3), "Archivo modificado a lectura y escritura");

  fd = open(filename, O_RDWR);
  if (fd < 0) {
    printf("No se pudo abrir el archivo\n");
    exit(1);
  }
  if (write(fd, "consectetur adipiscing elit. Aenean in.\n", 13) != 13) {
    printf("No se pudo escribir en el archivo después de restaurar los permisos\n");
    close(fd);
    exit(1);
  }
  printf("Luego de restaurar los permisos se logró la lectura y escritura en el archivo\n");
  close(fd);

  // Cambiamos a inmutable
  print_result(chmod(filename, 5), "Archivo modificado a inmutable");

  fd = open(filename, O_WRONLY);
  if (fd >= 0) {
    printf("Archivo abierto en modo escritura con permisos inmutables\n");
    close(fd);
    exit(1);
  } else {
    printf("El archivo no se pudo abrir (Resultado esperado considerando el cambio de permisos)\n");
  }

  // Intentamos cambiar los permisos de inmutable a lectura y escritura
  int chmod_result = chmod(filename, 3);
  if (chmod_result == 0) {
    printf("Se cambiaron los permisos de un archivo inmutable.\n");
    exit(1);
  } else {
    printf("No se pueden cambiar los permisos de un archivo inmutable, proceso denegado (Resultado esperado)\n");
  }

  printf("Las pruebas fueron realizadas con éxito\n");
  exit(0);
}