#define _CRT_SECURE_NO_WARNINGS

#include <algorithm>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <unistd.h>

int static convert_screencap_c(const char *cmd, const char *filename2, int width, int height, int max_color_value)
{
    FILE *f = popen(cmd, "r");
    if (!f)
    {
        std::cerr << "Error opening pipe" << std::endl;
        return 1;
    }
    FILE *f2 = fopen(filename2, "wb");

    if (!f2)
    {
        std::cerr << "Error opening file" << std::endl;
        fclose(f);
        return 1;
    }
    size_t size_my_buffer = (size_t)width * height * 4 + 17;
    fprintf(f2, "P6\n%d %d\n%d\n", width, height, max_color_value);
    char *buffer{new char[size_my_buffer]};
    if (!buffer)
    {
        std::cerr << "Error allocating buffer" << std::endl;
        fclose(f);
        fclose(f2);
        return 1;
    }
    fread(buffer, size_my_buffer, 1, f);
    for (size_t j{}; j < size_my_buffer - 1; j++)
    {
        if (((j + 1) % 4 == 0) || (j < 16))
        {
            continue;
        }
        fputc(buffer[j], f2);
    }
    pclose(f);
    fclose(f2);
    delete[] buffer;
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc < 4)
    {
        std::cout << "Usage: " << argv[0] << " <filename> <width> <height> " << std::endl;
        return 1;
    }
    return convert_screencap_c("screencap", argv[1], std::atoi(argv[2]), std::atoi(argv[3]), 255);
}
