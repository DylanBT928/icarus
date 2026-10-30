<div align="center">

# icarus ☀️

### custom 32-bit x86 kernel

</div>

_for he knew to fall means to once have soared_

## build and run

you will need `cmake` and a cross-compiler for `i686-elf` ([more info](https://wiki.osdev.org/Bare_Bones#Building_a_Cross-Compiler)).

```sh
cmake -B build
./scripts/build.sh
```

run using qemu
```sh
./scripts/run.sh
```

### nix users

```sh
nix build
nix run
```

## development

### nix users

```sh
nix develop
```

optionally, with `direnv` and `nix-direnv` installed:
```sh
echo "use flake" > .envrc && direnv allow
```
