# Protección de Memoria en xv6

## Resumen
Se modificó el programador de xv6 para incluir un sistema de protección de archivos. Esta protección se encarga de dar acceso y quitar acceso a la escritura y lectura de archivos, habilitando y deshabilitando permisos.

## Archivos modificados/creados:
    ||kernel||
    - file.h
    - fs.h
    - syscall.c / syscall.h
    - sysfile.c
---
    ||mkfs||
    - mkfs.c
---

    ||user||
    - Prueba.c (Creado)
    - user.h
    - usys.pl

## Cambios realizados:
### memlayout.h:
Agregamos la variable permissions dentro de struct inode, lo que permite definir los permisos del archivo (Modificación realizada para archivos abiertos en el sistema).
```
23|  int permissions; // Bits de permisos: 0, 1, 2, 3 o 5
```

### fs.h:
Nuevamente agregamos la variable de permissions, esta vez asociado a los metadatos del archivo.
```
39|  int permissions; 
```

### syscall.c y syscall.h:
Agregamos la llamada a sistema de chmod. Además se le asocia el número de llamada.
||syscall.c||
```
104|  extern uint64 sys_chmod(void);
...
131|  [SYS_chmod]    sys_chmod,
```
||syscall.h||
```
23|  #define SYS_chmod 22
```

### sysfile.c:
Se agregó la verificación de permisos para el archivo dentro de sys_open.
```
337|    if(ip->permissions == 5) {
338|      if(omode != O_RDONLY) {
339|        iunlockput(ip);
340|        end_op();
341|        return -1;
342|      }
343|    } else {
344|      if((omode & O_WRONLY) && !(ip->permissions & 2)) {
345|        iunlockput(ip);
346|        end_op();
347|        return -1;
348|      }
349|
350|      if((omode & O_RDONLY) && !(ip->permissions & 1)) {
351|        iunlockput(ip);
352|        end_op();
353|        return -1;
354|      }
355|    }
```

Lo que está ocurriendo aquí es que se verifica inicialmente el permiso especial de inmutabilidad valor: ```5```. Si el archivo cuenta con este permiso, implica que es inmutable, por lo que:
1) Solo se puede abrir en modo lectura.
2) Si se intenta abrir en cualquier otro modo se bloquea la operación, se finaliza y devuelve el código de error ```-1```.

Si no es inmutable, es decir no tiene el valor ```5``` en el campo de permisos, se intenta abrir el archivo en modo solo lectura y solo escritura por separado, valores ```2``` y ```1``` respectivamente.

Si no se cumple alguna de las dos condiciones, se devuelve el código de error```-1```.

### mkfs.c:
Debido a errores durante la programación (específicamente relacionados con el BSIZE), se agregó una línea de código a mkfs.c para comprobar los tamaños del ```BSIZE``` y de ```dinode```.
```
249|  printf("BSIZE: %d, sizeof(dinode): %ld\n", BSIZE, sizeof(struct dinode));
```

### Prueba.c:
Cógido de testeo de nuestras implementaciónes el cual se encarga de aplicar todos los cambios a un archivo, pasando por todos los permisos y verificando el correcto funcionamiento de estos.

### user.h:
Declaración de funciones.
```
25|  int chmod(const char*, int);
```

### usys.pl y Makefile:
Llamadas al sistema.
||usys.pl||
```
39|  entry("chmod");
```
||Makefile||
```
142|  $U/_prueba\
```
## Dificultades
Las problemáticas principales a la hora de modificar xv6 fueron principalmente producto del ```BSIZE```, como se mencionó anteriormente a la hora de ejecutar qemu devolvía los siguientes errores:
```
mkfs: mkfs/mkfs.c:272: iappend: Assertion fbn < MAXFILE' failed.
make: *** [Makefile:145: fs.img] Aborted (core dumped)
make: *** Deleting file 'fs.img'
```
```
mkfs: mkfs/mkfs.c:88: main: Assertion (BSIZE % sizeof(struct dinode)) == 0' failed.
make: *** [Makefile:145: fs.img] Aborted (core dumped)
al intentar make fs.img
```
En un caso inclusó logro inicial qemu pero no me dejaba escribir y la consola se veía tal que así:
```
xv6 kernel is booting

hart 1 starting
hart 2 starting
panic: virtio_disk_intr status
```
Estos errores se ligaban directamente con los valores de ```BSIZE```, ```dinode``` y ```MAXFILE```, como tal no descubrí que fue lo que produjo inicialmente que ocurrieran esto, tengo la sospecha de que en algún momento de realizar las modificaciones a xv6 llegué a modificar valores en fs.h que produjeron los errores, lo pude arreglar copiando una instancia anterior de dicho archivo, previo a las modificaciones.

Aparte de esto, no hubieron mayores inconvenientes.