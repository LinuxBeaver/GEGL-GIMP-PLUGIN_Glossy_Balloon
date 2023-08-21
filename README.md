Download binaries here. (FOLLOW INSTRUCTIONS)
https://github.com/LinuxBeaver/GEGL-glossy-balloon-text-styling/releases

# GEGL Glossy Balloon for text styling in Gimp
![image](https://github.com/LinuxBeaver/GEGL-glossy-balloon-text-styling/assets/78667207/b4ea9d43-cb5d-42fd-86ec-69a2792ac57a)

![image](https://github.com/LinuxBeaver/GEGL-glossy-balloon-text-styling/assets/78667207/e2f30429-05be-49bd-b057-a44a4834b4eb)

A classic Gimp Plugin of mine released in late May 2022 where GEGL makes a gloss coated bump text effect.
This plugin ships with a bonus filter "threshold alpha 2" that behaves like Gimp's Curves on Alpha Channel. The
bonus operation is highly recommended but not needed for Glossy Balloon to work. If you download the binaries it 
will ship with it.

This plugin looks great with fonts like "DynaPuff" that you can download here.
https://fonts.google.com/specimen/DynaPuff
but it will apply on any font and obviously you don't need DynaPuff for it to work.

## OS specific location to put GEGL Filter binaries 

**Windows**
C:\Users\USERNAME\AppData\Local\gegl-0.4\plug-ins
 
** Linux **
 /home/(USERNAME)/.local/share/gegl-0.4/plug-ins
 
 **Linux (Flatpak)**
 /home/(USERNAME)/.var/app/org.gimp.GIMP/data/gegl-0.4/plug-ins

![image](https://github.com/LinuxBeaver/GEGL-glossy-balloon-text-styling/assets/78667207/f15fb5eb-c8d7-4c08-bbac-97048864e657)


## Compiling and Installing
**Linux**

To compile and install you will need the GEGL header files (libgegl-dev on Debian based distributions or gegl on Arch Linux) and meson (meson on most distributions).

meson setup --buildtype=release build
ninja -C build


If you have an older version of gegl you may need to copy to ~/.local/share/gegl-0.3/plug-ins instead (on Ubuntu 18.04 for example).

**Windows**

The easiest way to compile this project on Windows is by using msys2. Download and install it from here: https://www.msys2.org/

Open a msys2 terminal with C:\msys64\mingw64.exe. Run the following to install required build dependencies:

pacman --noconfirm -S base-devel mingw-w64-x86_64-toolchain mingw-w64-x86_64-meson mingw-w64-x86_64-gegl

Then build the same way you would on Linux:

meson setup --buildtype=release build
ninja -C build

## Extra preview of this based Gimp plugin

![image](https://github.com/LinuxBeaver/GEGL-glossy-balloon-text-styling/assets/78667207/b23303be-edeb-4b4d-9015-0209dab12689)

![image](https://github.com/LinuxBeaver/GEGL-glossy-balloon-text-styling/assets/78667207/22904bb0-b2ac-4b1c-b727-bd51c182fa58)

![image](https://github.com/LinuxBeaver/GEGL-glossy-balloon-text-styling/assets/78667207/b3e0f258-09fd-4bc1-8644-2976b939de0c)

![image](https://github.com/LinuxBeaver/GEGL-glossy-balloon-text-styling/assets/78667207/059e40b0-0d72-4731-b901-1d7b0ce17ac8)




## Enjoy

![image](https://github.com/LinuxBeaver/GEGL-glossy-balloon-text-styling/assets/78667207/0ab62e35-33bb-4a88-a420-37a3d721e56f)

