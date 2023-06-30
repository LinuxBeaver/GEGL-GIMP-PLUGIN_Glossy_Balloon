# GEGL Glossy Balloon for text styling in Gimp
![image](https://github.com/LinuxBeaver/GEGL-glossy-balloon-text-styling/assets/78667207/b4ea9d43-cb5d-42fd-86ec-69a2792ac57a)

![image](https://github.com/LinuxBeaver/GEGL-glossy-balloon-text-styling/assets/78667207/e2f30429-05be-49bd-b057-a44a4834b4eb)

![image](https://github.com/LinuxBeaver/GEGL-glossy-balloon-text-styling/assets/78667207/b23303be-edeb-4b4d-9015-0209dab12689)


A classic GEGL Plugin of mine released in late May 2022 where GEGL makes a gloss coated bump text effect.


## OS specific location to put GEGL Filter binaries 

Windows
C:\Users\USERNAME\AppData\Local\gegl-0.4\plug-ins
 
 Linux 
 /home/(USERNAME)/.local/share/gegl-0.4/plug-ins
 
 Linux (Flatpak)
 /home/(USERNAME)/.var/app/org.gimp.GIMP/data/gegl-0.4/plug-ins


## Compiling and Installing
Linux

To compile and install you will need the GEGL header files (libgegl-dev on Debian based distributions or gegl on Arch Linux) and meson (meson on most distributions).

meson setup --buildtype=release build
ninja -C build


If you have an older version of gegl you may need to copy to ~/.local/share/gegl-0.3/plug-ins instead (on Ubuntu 18.04 for example).
Windows

The easiest way to compile this project on Windows is by using msys2. Download and install it from here: https://www.msys2.org/

Open a msys2 terminal with C:\msys64\mingw64.exe. Run the following to install required build dependencies:

pacman --noconfirm -S base-devel mingw-w64-x86_64-toolchain mingw-w64-x86_64-meson mingw-w64-x86_64-gegl

Then build the same way you would on Linux:

meson setup --buildtype=release build
ninja -C build

## Enjoy

![image](https://github.com/LinuxBeaver/GEGL-glossy-balloon-text-styling/assets/78667207/0ab62e35-33bb-4a88-a420-37a3d721e56f)
