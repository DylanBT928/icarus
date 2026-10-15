{
  description = "Icarus development shell";

  inputs = {
    flake-parts.url = "github:hercules-ci/flake-parts";
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
  };

  outputs =
    inputs@{ flake-parts, ... }:
    flake-parts.lib.mkFlake { inherit inputs; } {
      imports = [ ];
      systems = [
        "x86_64-linux"
        "aarch64-linux"
      ];
      perSystem =
        {
          config,
          self',
          inputs',
          pkgs,
          system,
          ...
        }:
        {
          devShells.default = pkgs.mkShell {
            packages = with pkgs; [
              gdb
              gnumake
              mtools
              grub2
              pkgsCross.i686-embedded.buildPackages.binutils
              pkgsCross.i686-embedded.buildPackages.gcc
              qemu
              xorriso
            ];
          };
        };
      flake = { };
    };
}
