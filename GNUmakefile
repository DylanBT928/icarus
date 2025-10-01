.PHONY: all kernel iso run clean

all: iso

kernel:
	@./scripts/build_kernel.sh

iso: kernel
	@./scripts/mkiso_limine.sh

run: iso
	@./scripts/run_qemu.sh bin/icarus.iso

clean:
	rm -rf bin/iso_root bin/icarus.iso
	-$(MAKE) -C kernel clean
