# fractochales
This is very tocho.

# fuentes
de los deseos

https://arxiv.org/pdf/2101.09383.pdf
https://youtu.be/4namgpHGqtk
https://www.cs.cornell.edu/courses/cs212/1998sp/handouts/Fractals/similar.html

# instalación
en un folder, debe colocarse el main.exe, openal32.dll, sfx/, images/ y fonts/

# para compilar la versión de android
instalar android studio, gradle (incluido), android sdk, android ndk y cmake
clonar github.com/SFML/SFML.git (2.6 branch) y crear un directorio llamado build dentro de este
crear un subdirectorio con el nombre de la arquitectura a compilar [arm64-v8a ... etc] y abrir cmd ahí
ejecutar estos 3 simples comandos
    - `C:/Users/{usuario}/AppData/Local/Android/Sdk/cmake/{versión más reciente}/bin/cmake.exe -DCMAKE_SYSTEM_NAME=Android -DCMAKE_ANDROID_NDK=C:/Users/{usuario}/AppData/Local/Android/Sdk/ndk/{versión más reciente} -DCMAKE_ANDROID_ARCH_ABI={arquitectura a compilar [arm64-v8a ... etc]} -DCMAKE_ANDROID_STL_TYPE=c++_static -DCMAKE_BUILD_TYPE=Release ../.. -G "MinGW Makefiles"`
    - `mingw32-make`
    - `mingw32-make install`
abrir android studio en fractochales/android
configurar el archivo local.properties para que contenga
    `sdk.dir=C\:\\Users\\{usuario}\\AppData\\Local\\Android\\Sdk`
    `ndk.dir=C\:\\Users\\{usuario}\\AppData\\Local\\Android\\Sdk\\ndk\\{versión más reciente}`
compilar (botón play)