<div align="center">

# icarus ☀️

### Custom 32-bit x86 Kernel

</div>

Icarus is a hobby kernel developed from scratch. We started this project for fun and curiosity to learn more about what happens under the hood.

## Prerequisites

- [i686-elf cross-compiler](https://wiki.osdev.org/GCC_Cross-Compiler)
- CMake
- Ninja
- xorriso
- GRUB
- QEMU

## Build and Run

### Non-Nix Users

```sh
cmake -B build
./scripts/build.sh
./scripts/run.sh
```

### Nix Users

```sh
nix build
nix run
```

#### Develop on Nix

Enter the Nix shell with all toolchains and dependencies:

```sh
nix develop
```

Optionally, automate loading the environment with `direnv` and `nix-direnv`:

```sh
echo "use flake" > .envrc
direnv allow
```
