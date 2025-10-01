# Nuke built-in rules
.SUFFIXES:

# Paths / layout
ROOT := ..
BINDIR := $(ROOT)/bin
OBJDIR := $(BINDIR)/obj

# Final executable name
override OUTPUT := icarus

# Toolchain selection
TOOLCHAIN :=
TOOLCHAIN_PREFIX :=
ifneq ($(TOOLCHAIN),)
	ifeq ($(TOOLCHAIN_PREFIX),)
		TOOLCHAIN_PREFIX := $(TOOLCHAIN)-
	endif
endif

# Compiler
ifneq ($(TOOLCHAIN_PREFIX),)
	CC := $(TOOLCHAIN_PREFIX)gcc
else
	CC := cc
endif

# Linker
LD := $(TOOLCHAIN_PREFIX)ld

ifeq ($(TOOLCHAIN),llvm)
	CC := clang
	LD := ld.lld
endif

# Flags
CFLAGS := -g -O2 -pipe
CPPFLAGS :=
NASMFLAGS := -g
LDFLAGS :=

# Detect clang
override CC_IS_CLANG := $(sheel ! $(CC) --version 2>/dev/null | grep -q '^Target: '; echo $$?)
ifeq ($(CC_IS_CLANG),1)
	override CC += -target x86_64-unknown-none-elf
endif

# Internal compile/link flags
override CFLAGS += \
	-Wall -Wextra \
	-std=gnu11 \
	-ffreestanding \
	-fno-stack-protector \
	-fno-stack-check \
	-fno-lto \
	-fno-PIC \
	-ffunction-sections -fdata-sections \
	-m64 -march=x86-64 -mabi=sysv \
	-mno-80387 -mno-mmx -mno-sse -mno-sse2 \
	-mno-red-zone \
	-mcmodel=kernel

LIMINE_DIR := $(ROOT)/bin/limine
LIMINE_HDR := $(LIMINE_DIR)/limine.h

override CPPFLAGS := \
	-I src \
	-I core \
	-I $(LIMINE_DIR) \
	$(CPPFLAGS) \
	-DLIMINE_API_REVISION=3 \
	-MMD -MP

override NASMFLAGS := \
	-f elf64 \
	$(patsubst -g,-g -F dwarf,$(NASMFLAGS)) \
	-Wall

override LDFLAGS += \
	-m elf_x86_64 \
	-nostdlib \
	-static \
	-z max-page-size=0x1000 \
	--gc-sections \
	-T arch/x86_64/linker.ld

# Sources / objects
SRC_DIRS := core

override SRCFILES := $(shell find -L $(SRC_DIRS) -type f \( -name '*.c' -o -name '*.S' -o -name '*.asm' \) 2>/dev/null | LC_ALL=C sort)
override CFILES := $(filter %.c,$(SRCFILES))
override ASFILES := $(filter %.S,$(SRCFILES))
override NASMFILES := $(filter %.asm,$(SRCFILES))

override OBJ := $(addprefix $(OBJDIR)/,$(CFILES:.c=.c.o) \
	$(ASFILES:.S=.S.o) \
	$(NASMFILES:.asm=.asm.o))

override HEADER_DEPS := $(addprefix $(OBJDIR)/,$(CFILES:.c=.c.d) \
	$(ASFILES:.S=.S.d))

# Default target
.PHONY: all
all: $(BINDIR)/$(OUTPUT)

-include $(HEADER_DEPS)

# Limine header (ensure limine.h exists)
$(LIMINE_HDR): 
	@[ -d "$(LIMINE_DIR)/.git" ] || git clone --depth=1 --branch v10.x-binary https://codeberg.org/Limine/Limine.git "$(LIMINE_DIR)"

# Link final kernel
$(BINDIR)/$(OUTPUT): GNUmakefile arch/x86_64/linker.ld $(OBJ)
	@mkdir -p "$(dir $@)"
	$(LD) $(LDFLAGS) $(OBJ) -o $@

# Compiler rules
# C
$(OBJDIR)/%.c.o: %.c GNUmakefile $(LIMINE_HDR)
	@mkdir -p "$(dir $@)"
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

# GAS
$(OBJDIR)/%.S.o: %.S GNUmakefile $(LIMINE_HDR)
	@mkdir -p "$(dir $@)"
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

# NASM
$(OBJDIR)/%.asm.o: %.asm GNUmakefile $(LIMINE_HDR)
	@mkdir -p "$(dir $@)"
	nasm $(NASMFLAGS) $< -o $@

# Clean
.PHONY: clean
clean: rm -rf "$(BINDIR)/$(OUTPUT)" "$(OBJDIR)"
