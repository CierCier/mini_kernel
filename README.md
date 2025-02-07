# Project Compilation Guide

## Prerequisites
- Linux operating system 
- GCC cross compiler for x86_64-elf
- NASM assembler (to compile the boot code)
- Make utility 
- QEMU emulator (to run the kernel)
- GRUB bootloader (to create the iso image and serve as the bootloader)
  - GRUB2 requires xorriso to create the iso image

## Project Status

- [x] Basic Kernel Setup
- [x] GDT Implementation
- [x] VGA Text Mode
- [x] Custom printf Implementation
- [x] Keyboard Driver
- [ ] Memory Management
- [ ] File System
- [ ] FAT32 File System
- [ ] Process Management
- [ ] System Calls
- [ ] LIBC Implementation
- [ ] Shell
- [ ] User Space Programs



## Steps to Compile

1. **Clone the repository:**
    ```bash
    git clone https://github/com/CierCier/mini_kernel.git
    cd mini_kernel
    ```
2. **Compile the kernel:**
    ```bash
    make
    ```
3. **Run the kernel in qemu:**
    ```bash
    make run
    ```



