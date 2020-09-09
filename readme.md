## This is the frist practice project of my GTK GUI with C programming.

### Requirements

1. GTK installed in your Linux system
2. GCC installed
3. To compile in windows system using MSYS2, type the command frist

   > pkg-config --cflags gtk+-3.0

   > pkg-config --libs gtk+-3.0

   > gcc -Wl,-export-all-symbols `pkg-config --cflags gtk+-3.0` -o main main.c `pkg-config --libs gtk+-3.0`
