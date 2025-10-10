#!/bin/sh

ISO="${1:-bin/icarus.iso}"
qemu-system-x86_64 -serial stdio -cdrom "$ISO"
