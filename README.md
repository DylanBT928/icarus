<div align="center">

# icarus ☀️

</div>

Icarus is a minimal 32-bit x86 (i386) kernel implemented in C++ with a GAS boot stub, built with an i686-elf cross-compiler. It is loaded by GRUB via Multiboot v1 and run in QEMU. I built this as a hobby project to learn more about operating systems and kernel development.

## Prerequisites

### Required Tools

- **i686-elf cross-compiler** (GCC/G++/AS) with libgcc
- **GRUB tools** (for ISO creation): `grub-file`, `grub-mkrescue`
- **QEMU** (for testing the kernel)

> Note: You can build the cross-compiler by following the [OSDev Cross-Compiler Guide](https://wiki.osdev.org/GCC_Cross-Compiler).

## Building

### Build the Kernel

```bash
# Create build directory
mkdir -p build
cd build

# Assemble boot.s
i686-elf-as ../kernel/arch/i386/boot.s -o boot.o

# Compile kernel.cpp
i686-elf-g++ -c ../kernel/src/kernel.cpp -o kernel.o \
  -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti

# Link the kernel
i686-elf-g++ -T ../kernel/linker.ld -o icarus.bin \
  -ffreestanding -O2 -nostdlib boot.o kernel.o -lgcc
```

### Creating Bootable ISO

```bash
# Create ISO directory structure
mkdir -p isodir/boot/grub

# Copy kernel and GRUB config
cp icarus.bin isodir/boot/
cp ../grub/grub.cfg isodir/boot/grub/

# Create bootable ISO
i686-elf-grub-mkrescue -o icarus.iso isodir
```

## Running

### Using QEMU

```bash
# Run the kernel directly
qemu-system-i386 -kernel icarus.bin

# Run from ISO
qemu-system-i386 -cdrom icarus.iso
```

### Expected Output

When the kernel boots successfully, you should see:

```
"Hello world" - Icarus
```

## Resources

- [OSDev Wiki](https://wiki.osdev.org/) - Comprehensive OS development resource

## License

This project is licensed under the Apache License 2.0. See the [LICENSE](./LICENSE) file for details.
