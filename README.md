# Protección de Memoria en xv6

## Resumen
Se modificó el programador de xv6 para incluir un sistema de comunicación de proceso. La conexión utiliza una cola de mensajes dentro del kernel.

## Cambios realizados:
Se realizaron cambios en diversos archivos del kernel para agregar las deficiones y condiciones de las funciones. Como tal la estructura de las funciones está completa, junto con sus llamadas y el archivo de prueba.
## Dificultades
Como tal no logré completar el código, tuve problemas con valores en sysproc dónde se devolvía el siguiente error:
```
kernel/sysproc.c:100:5: error: void value not ignored as it ought to be
  100 | if (argint(0, &pid) < 0) {
      |     ^~~~~~~~~~~~~~~
kernel/sysproc.c:108:9: error: 'msg_queue' undeclared (first use in this function)
  108 |     if (msg_queue.size >= MAX_QUEUE_SIZE) {
      |         ^~~~~~~~~
kernel/sysproc.c:108:9: note: each undeclared identifier is reported only once for each function it appears in
kernel/sysproc.c: In function 'sys_receive':
kernel/sysproc.c:135:14: error: 'msg_queue' undeclared (first use in this function)
  135 |     acquire(&msg_queue.lock);
      |              ^~~~~~~~~
make: *** [<builtin>: kernel/sysproc.o] Error 1
```
Este error se debe a malas declaraciones y ajustes ineficientes a los archivos del kernel, por faltas de tiempo no logré solucionarlos.