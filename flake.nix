{
  description = "Icarus development shell";

  inputs = {
    flake-parts.url = "github:hercules-ci/flake-parts";
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    nixpkgs-stable.url = "github:NixOS/nixpkgs/nixos-26.05";
  };

  outputs =
    inputs@{ flake-parts, ... }:
    flake-parts.lib.mkFlake { inherit inputs; } {
      imports = [ ];
      systems = [
        "x86_64-linux"
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
        let
          stable = inputs'.nixpkgs-stable.legacyPackages;
        in
        {
          devShells.default = pkgs.mkShell {
            packages = with pkgs; [
              cmake
              gdb
              gnumake
              stable.grub2_efi
              mtools
              ninja
              pkgsCross.x86_64-embedded.buildPackages.binutils
              pkgsCross.x86_64-embedded.buildPackages.gcc
              qemu
              xorriso
            ];
          };

          packages.default = pkgs.stdenvNoCC.mkDerivation {
            name = "icarus";
            version = "0.1";

            src = inputs.self;

            nativeBuildInputs = with pkgs; [
              cmake
              stable.grub2_efi
              mtools
              ninja
              pkgsCross.x86_64-embedded.buildPackages.binutils
              pkgsCross.x86_64-embedded.buildPackages.gcc
              xorriso
            ];

            cmakeBuildDir = "build";
            cmakeBuildType = "Release";
            cmakeFlags = [ "-GNinja" ];

            buildPhase = ''
              runHook preBuild
              cmake --build . --target iso
              runHook postBuild
            '';

            installPhase = ''
              runHook preInstall
              mkdir -p "$out/share/icarus"
              cp icarus.iso "$out/share/icarus/icarus.iso"
              cp src/kernel/icarus "$out/share/icarus/icarus"
              runHook postInstall
            '';

          };

          # creates a shell script as a binary in the nix store
          # that runs qemu on the built iso in the nix store
          packages.runner = pkgs.writeShellScriptBin "icarus-runner" ''
            set -eu

            state="$HOME/.local/state/icarus"
            mkdir -p "$state"

            if [ ! -e "$state/OVMF_VARS.fd" ]; then
              cp "${pkgs.OVMF.fd}/FV/OVMF_VARS.fd" "$state/OVMF_VARS.fd"
              chmod u+w "$state/OVMF_VARS.fd"
            fi

            exec ${pkgs.qemu}/bin/qemu-system-x86_64 \
              -drive if=pflash,format=raw,unit=0,readonly=on,file="${pkgs.OVMF.fd}/FV/OVMF_CODE.fd" \
              -drive if=pflash,format=raw,unit=1,file="$state/OVMF_VARS.fd" \
              -cdrom "${self'.packages.default}/share/icarus/icarus.iso" \
              -boot order=d,menu=on \
              -no-reboot \
              -no-shutdown \
              -serial stdio \
              "$@"
          '';

          apps.default = {
            type = "app";
            program = "${self'.packages.runner}/bin/icarus-runner";
          };

          # a debug package seperate from the standard release package
          # build with `nix build .#debug`
          # and run with `nix run .#debug`
          packages.debug = self'.packages.default.overrideAttrs (_: {
            name = "icarus-debug";
            cmakeBuildType = "Debug";
            dontStrip = true; # don't automatically remove debug symbols
          });

          # new flags expose qemu to gdb locally over TCP
          # and pauses CPU before booting until gdb unpauses
          # `gdb result/share/icarus/icarus`
          # and then
          # `(gdb) remote target 127.0.0.1:1234`
          packages.debug-runner = pkgs.writeShellScriptBin "icarus-debug" ''
            set -eu

            state="$HOME/.local/state/icarus"
            mkdir -p "$state"

            if [ ! -e "$state/OVMF_VARS.fd" ]; then
              cp "${pkgs.OVMF.fd}/FV/OVMF_VARS.fd" "$state/OVMF_VARS.fd"
              chmod u+w "$state/OVMF_VARS.fd"
            fi

            exec ${pkgs.qemu}/bin/qemu-system-x86_64 \
              -drive if=pflash,format=raw,unit=0,readonly=on,file="${pkgs.OVMF.fd}/FV/OVMF_CODE.fd" \
              -drive if=pflash,format=raw,unit=1,file="$state/OVMF_VARS.fd" \
              -cdrom "${self'.packages.debug}/share/icarus/icarus.iso" \
              -boot order=d,menu=on \
              -no-reboot \
              -no-shutdown \
              -gdb tcp:127.0.0.1:1234 \
              -S \
              -serial stdio \
              "$@"
          '';

          apps.debug = {
            type = "app";
            program = "${self'.packages.debug-runner}/bin/icarus-debug";
          };

        };

    };
}
