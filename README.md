<div align="center">

# icarus ☀️

**Experimental 64-bit operating system built with C++ and Limine**

![GitHub Release](https://img.shields.io/github/v/release/DylanBT928/icarus)
![GitHub code size in bytes](https://img.shields.io/github/languages/code-size/DylanBT928/icarus)
![GitHub License](https://img.shields.io/github/license/DylanBT928/icarus)

</div>

Icarus is an experimental x86-64 kernel implemented in C++, booted via the Limine bootloader. It runs in long mode on QEMU and serves as a hobby project to learn more about operating systems and kernel development.

## Roadmap

- [x] Limine barebones (bootloader → kernel entry)
- [x] GDT / IDT / ISRs / IRQs (basic exception + interrupt handling)
- [ ] PMM (physical memory manager)
- [ ] Paging (basic page tables)
- [ ] VMM (virtual memory manager + allocators)
- [ ] Basic drivers (keyboard, framebuffer/terminal output)
- [ ] ACPI tables (parse for multiprocessor + timers)
- [ ] APIC → IOAPIC → LAPIC → HPET → APIC timer
- [ ] SMP bring-up (secondary cores online)
- [ ] Multitasking / scheduling
- [ ] Syscalls (define ABI)
- [ ] ELF loader (for user programs)
- [ ] Userspace (init process, libc stubs, etc.)
- [ ] SIMD / advanced CPU features (SSE, AVX, FPU context switching)

## Prerequisites

- **x86_64-elf cross-compiler**
- **QEMU** (testing the kernel)
- **GNU Make** (build automation)

> Note: You can build the cross-compiler by following the [OSDev Cross-Compiler Guide](https://wiki.osdev.org/GCC_Cross-Compiler).

## Building

Everything is automated through the GNUmakefile.
To build and run Icarus, simply execute:

```bash
make run
```

This will:

1. Build the kernel (`kernel/`)
2. Package it into a Limine-bootable ISO (`bin/icarus.iso`)
3. Launch it automatically in QEMU

### Available Make Targets

```bash
make all     # Default target (build ISO)
make kernel  # Build only the kernel
make iso     # Package ISO with Limine
make run     # Build and run the kernel in QEMU
make clean   # Remove build artifacts
```

### Running Manually

If you prefer to run QEMU manually:

```bash
qemu-system-x86_64 -serial stdio -cdrom bin/icarus.iso
```

## Resources

- [OSDev Wiki](https://wiki.osdev.org/) - Comprehensive OS development resource

## License

This project is licensed under the Apache License 2.0. See the [LICENSE](./LICENSE) file for details.
