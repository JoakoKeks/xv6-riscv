Pasos seguidos para la instalación de Xv6
1.- Comenzamos la instalación desde el Windows Powershell utilizando el siguiente comando
- wsl --install
- *Aquí se instala todo lo necesario para la utilización de ubuntu en nuestro dispositivo*
- Luego de la instalación se nos pedirá crear una cuenta para ubuntu y seguimos las instalaciónes por Visual Studio

2.- Desde Visual Studio instalamos las dependencias necesarias
- Posteriormente debemos instalar el toolchain que vamos a utilizar, yo utilicé el siguiente: "https://bernardnongpoh.github.io/posts/riscv"
- Seguimos los pasos encontrados en la página

3.- Actualizamos qemu en caso de que de problemas
- En mi caso particular qemu no quería instalarse debido a la versión utilizada así que debí actualizar siguiendo estos pasos:
    - git clone https://gitlab.com/qemu-project/qemu.git
    - cd qemu
    - git checkout v7.2.0
    - ./configure --target-list=riscv64-softmmu,riscv64-linux-user
    - make -j$(nproc)
    - sudo make install
    - qemu-system-riscv64 --version
- Luego de completar estos pasos nos dirijimos nuevamente a nuestra carpeta de xv6 y corremos el comando "make qemu" y ya debería funcionar todo

