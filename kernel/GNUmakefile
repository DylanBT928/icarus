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
	CXX := $(TOOLCHAIN_PREFIX)g++
else
	CXX := c++
endif

# Linker
LDXX := $(CXX)

ifeq ($(TOOLCHAIN),llvm)
	LDXX := ld.lld
endif

# Detect clang
CXX_IS_CLANG := $(shell $(CXX) --version 2>/dev/null | head -n1 | grep -qi clang && echo 1 || echo 0)
ifeq ($(CXX_IS_CLANG),1)
	override CXX += -target x86_64-unknown-none-elf
endif

# Limine header
LIMINE_DIR := $(ROOT)/bin/limine
LIMINE_HDR := $(LIMINE_DIR)/limine.h

# Flags
CXXFLAGS := -g -O2 -pipe

# Common kernel flags
COMMON_FLAGS := \
	-ffreestanding \
	-fno-exceptions \
	-fno-rtti \
	-fno-unwind-tables \
	-fno-asynchronous-unwind-tables \
	-fno-stack-protector \
	-fno-stack-check \
	-fno-lto \
	-fno-PIC \
	-ffunction-sections \
	-fdata-sections \
	-nostdinc++ \
	-m64 \
	-march=x86-64 \
	-mabi=sysv \
	-mno-80387 \
	-mno-mmx \
	-mno-sse \
	-mno-sse2 \
	-mno-red-zone \
	-mcmodel=kernel

override CXXFLAGS += \
	-Wall \
	-Wextra \
	-std=gnu++20 \
	$(COMMON_FLAGS)

# Preprocessor
CPPFLAGS := \
	-I core \
	-I $(LIMINE_DIR) \
	-DLIMINE_API_REVISION=3 \
	-MMD \
	-MP

# NASM
NASMFLAGS := \
	-g \
	-F \
	dwarf \
	-f \
	elf64 \
	-Wall

# Linker flags
LDFLAGS := \
	-nostdlib \
	-static \
	-Wl,-z,max-page-size=0x1000 \
	-Wl,--gc-sections \
	-Wl,-T,arch/x86_64/linker.ld

# Sources
SRC_DIRS := core

SRCFILES := $(shell find -L $(SRC_DIRS) -type f \( -name '*.cpp' -o -name '*.S' -o -name '*.asm' \) 2>/dev/null | LC_ALL=C sort)
CPPFILES := $(filter %.cpp,$(SRCFILES))
ASFILES := $(filter %.S,$(SRCFILES))
NASMFILES := $(filter %.asm,$(SRCFILES))

OBJ := \
	$(addprefix $(OBJDIR)/,$(CPPFILES:.cpp=.cpp.o)) \
	$(addprefix $(OBJDIR)/,$(ASFILES:.S=.S.o)) \
	$(addprefix $(OBJDIR)/,$(NASMFILES:.asm=.asm.o))

HEADER_DEPS := \
	$(addprefix $(OBJDIR)/,$(CPPFILES:.cpp=.cpp.d)) \
	$(addprefix $(OBJDIR)/,$(ASFILES:.S=.S.d))

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
	$(LDXX) $(LDFLAGS) $(OBJ) -o $@

# Compile rules
# C++ sources
$(OBJDIR)/%.cpp.o: %.cpp GNUmakefile $(LIMINE_HDR)
	@mkdir -p "$(dir $@)"
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

# GAS assembly with preprocessor
$(OBJDIR)/%.S.o: %.S GNUmakefile $(LIMINE_HDR)
	@mkdir -p "$(dir $@)"
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

# NASM assembly
$(OBJDIR)/%.asm.o: %.asm GNUmakefile $(LIMINE_HDR)
	@mkdir -p "$(dir $@)"
	nasm $(NASMFLAGS) $< -o $@

# Clean
.PHONY: clean
clean: 
	rm -rf "$(BINDIR)/$(OUTPUT)" "$(OBJDIR)"
