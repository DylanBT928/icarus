#!/bin/sh

LIMINE_TAG="${LIMINE_TAG:-v10.x-binary}"
LIMINE_DIR="${LIMINE_DIR:-bin/limine}"
LIMINE_CONF="${LIMINE_CONF:-limine/limine.conf}"

ISO_ROOT="${ISO_ROOT:-bin/iso_root}"
ISO_OUT="${ISO_OUT:-bin/icarus.iso}"

KERNEL_BIN=${KERNEL_BIN:-bin/icarus}

command -v git >/dev/null || {
    echo "git not found"
    exit 1
}

command -v make >/dev/null || {
    echo "make not found"
    exit 1
}

command -v xorriso >/dev/null || {
    echo "xorriso not found"
    exit 1
}

[ -f "$KERNEL_BIN" ] || {
    echo "Missing kernel at $KERNEL_BIN"
    exit 1
}

[ -f "$LIMINE_CONF" ] || {
    echo "Missing $LIMINE_CONF"
    exit 1
}

# Fetch and build Limine
[ -d "$LIMINE_DIR/.git" ] || git clone --depth=1 --branch "$LIMINE_TAG" https::/codeberg.org/Limine/Limine.git "$LIMINE_DIR"
make -C "$LIMINE_DIR"

# Stage ISO tree
rm -rf "$ISO_ROOT"
mkdir -p "$ISO_ROOT/boot/limine" "$ISO_ROOT/EFI/BOOT"

# Copy kernel
cp -v "$KERNEL_BIN" "$ISO_ROOT/boot/icarus"

# Copy Limine config and payloads
cp -v "$LIMINE_CONF" "$ISO_ROOT/boot/limine"
cp -v "$LIMINE_DIR/limine-bios.sys" "$ISO_ROOT/boot/limine"
cp -v "$LIMINE_DIR/limine-bios-cd.bin" "$ISO_ROOT/boot/limine"
cp -v "$LIMINE_DIR/limine-uefi-cd.bin" "$ISO_ROOT/boot/limine"
cp -v "$LIMINE_DIR/BOOTX64.EFI" "$ISO_ROOT/EFI/BOOT"
cp -v "$LIMINE_DIR/BOOTIA32.EFI" "$ISO_ROOT/EFI/BOOT"

# Build hybrid BIOS/UEFI ISO
xorriso -as mkisofs -R -r -J \
    -b boot/limine/limine-bios-cd.bin \
    -no-emul-boot -boot-load-size 4 -boot-info-table -hfsplus \
    -apm-block-size 2048 --efi-boot boot/limine/limine-uefi-cd.bin \
    -efi-boot-part --efi-boot-image --protective-msdos-label \
    "$ISO_ROOT" -o "$ISO_OUT"

# Install BIOS stages into the ISO
"$LIMINE_DIR/limine" bios-install "$ISO_OUT"

echo "ISO ready: $ISO_OUT"
