#include "../arch/x86_64/gdt.hpp"
#include "../arch/x86_64/idt.hpp"
#include "serial.hpp"

#include <stddef.h>
#include <stdint.h>
#include <limine.h>

// Set base revision to 3
__attribute__((used, section(".limine_requests"))) static volatile LIMINE_BASE_REVISION(3);

// Place the Limine requests and prevent the compiler from optimizing
__attribute__((used, section(".limine_requests"))) static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0,
    .response = nullptr
};

// Define start and end markers for the Limine requests
__attribute__((used, section(".limine_requests_start"))) static volatile LIMINE_REQUESTS_START_MARKER;
__attribute__((used, section(".limine_requests_end"))) static volatile LIMINE_REQUESTS_END_MARKER;

// Standard C library functions for memory
extern "C" void* memcpy(void* __restrict dest, const void* __restrict src, size_t n)
{
    uint8_t* pdest = static_cast<uint8_t*>(dest);
    const uint8_t* psrc = static_cast<const uint8_t*>(src);

    for (size_t i = 0; i < n; ++i)
    {
        pdest[i] = psrc[i];
    }

    return dest;
}

extern "C" void* memset(void* s, int c, size_t n)
{
    uint8_t* p = static_cast<uint8_t*>(s);

    for (size_t i = 0; i < n; ++i)
    {
        p[i] = static_cast<uint8_t>(c);
    }

    return s;
}

extern "C" void* memmove(void* dest, const void* src, size_t n)
{
    uint8_t* pdest = static_cast<uint8_t*>(dest);
    const uint8_t* psrc = static_cast<const uint8_t*>(src);

    if (src > dest)
    {
        for (size_t i = 0; i < n; ++i)
        {
            pdest[i] = psrc[i];
        }
    }
    else if (src < dest)
    {
        for (size_t i = n; i > 0; --i)
        {
            pdest[i - 1] = psrc[i - 1];
        }
    }

    return dest;
}

extern "C" int memcmp(const void* s1, const void* s2, size_t n)
{
    const uint8_t* p1 = static_cast<const uint8_t*>(s1);
    const uint8_t* p2 = static_cast<const uint8_t*>(s2);

    for (size_t i = 0; i < n; ++i)
    {
        if (p1[i] != p2[i])
        {
            return (p1[i] < p2[i]) ? -1 : 1;
        }
    }

    return 0;
}

// C++ ABI stubs
extern "C" void __cxa_pure_virtual()
{
    for (;;)
    {
        asm volatile("hlt");
    }
}

extern "C"
{
    void* __dso_handle = 0;
}

// Global ctor array
using ctor_t = void (*)();
extern "C" ctor_t __init_array_start[], __init_array_end[];

static inline void call_global_ctors()
{
    for (ctor_t* p = __init_array_start; p != __init_array_end; ++p)
    {
        (*p)();
    }
}

// Halt and catch fire function
static inline void hcf()
{
    for (;;)
    {
        asm volatile("hlt");
    }
}

// Kernel's entry point
extern "C" void kmain()
{
    // call_global_ctors();

    if (LIMINE_BASE_REVISION_SUPPORTED == false)
    {
        hcf();
    }

    if (framebuffer_request.response == nullptr || framebuffer_request.response->framebuffer_count < 1)
    {
        hcf();
    }

    // Test "Hello World"
    serial_init();
    serial_print("\"Hello world\" - Icarus\n");

    struct limine_framebuffer* fb = framebuffer_request.response->framebuffers[0];

    for (size_t i = 0; i < 100; ++i)
    {
        volatile uint32_t* fb_ptr = static_cast<volatile uint32_t*>(fb->address);
        fb_ptr[i * (fb->pitch / 4) + i] = 0xFFFFFFu;
    }

    asm volatile("cli");
    GDT::init();
    IDT::init();

    // Test trap (prints once)
    asm volatile("int3");

    // Test fault (prints infinitely)
    asm volatile("ud2");

    hcf();
}
