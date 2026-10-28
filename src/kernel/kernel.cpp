#include <cstddef>
#include <cstdint>

class String
{
  public:
    constexpr static std::size_t strlen(const char* str)
    {
        std::size_t i{};
        for (; str[i] != '\0'; ++i);
        return i;
    }
};

struct VGA
{
    constexpr static std::uint8_t width{ 80 };
    constexpr static std::uint8_t height{ 25 };
    constexpr static std::uintptr_t memory{ 0xB8000 };

    enum class Color : std::uint8_t
    {
        Black = 0,
        Blue = 1,
        Green = 2,
        Cyan = 3,
        Red = 4,
        Magenta = 5,
        Brown = 6,
        LightGray = 7,
        DarkGray = 8,
        LightBlue = 9,
        LightGreen = 10,
        LightCyan = 11,
        LightRed = 12,
        LightMagenta = 13,
        LightBrown = 14,
        White = 15
    };

    constexpr static std::uint16_t entry(char c, Color color)
    {
        return static_cast<std::uint16_t>(c) | (static_cast<std::uint16_t>(color) << 8);
    }

    constexpr static std::uint8_t entryColor(Color fg, Color bg)
    {
        return static_cast<std::uint8_t>(fg) | (static_cast<std::uint8_t>(bg) << 4);
    }
};

class Terminal
{
  public:
    Terminal()
        : row{}, col{},
          color{ VGA::entryColor(VGA::Color::White, VGA::Color::Black) },
          buffer{ reinterpret_cast<std::uint16_t*>(VGA::memory) }
    {
        for (std::size_t y{}; y < VGA::height; ++y)
        {
            for (std::size_t x{}; x < VGA::width; ++x)
            {
                buffer[y * VGA::width + x] = VGA::entry(' ', color);
            }
        }
    }

    void setColor(VGA::Color newColor)
    {
        color = newColor;
    }

    void putEntryAt(char c, VGA::Color color, std::size_t x, std::size_t y)
    {
        buffer[y * VGA::width + x] = VGA::entry(c, color);
    }

    void putChar(char c)
    {
        putEntryAt(c, color, col, row);

        if (++col == VGA::width)
        {
            col = 0;

            if (++row == VGA::height)
            {
                row = 0;
            }
        }
    }

    void write(const char* str, std::size_t size)
    {
        for (std::size_t i{}; i < size; ++i)
        {
            putChar(str[i]);
        }
    }

    void writeString(const char* str)
    {
        write(str, String::strlen(str));
    }

  private:
    std::size_t row;
    std::size_t col;
    VGA::Color color;
    std::uint16_t* buffer;
};

extern "C" void kernel_main()
{
    Terminal terminal;
    terminal.writeString("for he knew to");
    terminal.setColor(VGA::Color::LightGreen);
    terminal.writeString(" fall ");
    terminal.setColor(VGA::Color::White);
    terminal.writeString("means to once have ");
    terminal.setColor(VGA::Color::LightRed);
    terminal.writeString("soared");
}
