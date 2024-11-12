# Protección de Memoria en xv6

## Resumen
Se modificó el programador de xv6 para incluir un sistema de protección de archivos. Esta protección se encarga de dar acceso y quitar acceso a la escritura y lectura de archivos, habilitando y deshabilitando permisos.


## Archivos modificados/creados:
    - memlayout.h
    - riscv.h
    - syscall.c
    - syscall.h
    - sysproc.c
    - trap.c
    - mprot_test.c
    - user.h
    - usys.pl
    - Makefile

## Cambios realizados:
### memlayout.h:
Se define 1 bit de lectura en la entrada de la página.
```
#define PTE_RO (1 << 9) //Se define un bit solo para lectura en la entrada de la tabla de páginas
```

### riscv.h:
Hacemos definiciones para indicar errores de lectura o de dirección.
```
#define CAUSE_LOAD_PAGE_FAULT 13
#define CAUSE_STORE_PAGE_FAULT 15
```

### syscall.c y syscall.h:
Agregamos las llamadas a sistema de mprotect y munprotect. Además se le asocian los números de llamada.
```
extern uint64 sys_mprotect(void);
extern uint64 sys_munprotect(void);
```

```
[SYS_mprotect]    sys_mprotect,
[SYS_munprotect]  sys_munprotect,
```
```
#define SYS_mprotect 22
#define SYS_munprotect 23
```

### sysproc.c:
Creamos el código de implementación de las funciones dónde se entregan y se quitan los permisos de lectura y escritura.

### trap.c:
Generamos mensaje de "error" al intentar escribir sobre una página que tiene admitido solo lectura.

### mprot_test.c:
Cógido de testeo de nuestras implementaciónes el cual se encarga de aplicar todos los cambios dentro de un ejemplo sencillo de protección.

### user.h:
Declaración de funciones.
```
int mprotect(void *addr, int len);
int munprotect(void *addr, int len);
```

### usys.pl y Makefile:
Llamadas al sistema
```
entry("mprotect");
entry("munprotect");
```

```
$U/_mprot_test\
```
## Dificultades
Las problemáticas principales a la hora de generar implementar las funciones fueron la creación y definición del archivo de lectura dentro del mismo archivo de prueba, sumado a esto surgieron diversos errores por problemas de escritura (intenté implementar en más de una ocasión código "antiguo" el cual no era reconocido o no estaba bien implementado por mi versión de xv6). Fuera de eso no hubo mayor problema, solo se debió cambiar la terminología y los comandos a unos más modernos.