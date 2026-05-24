#include "buffer.h"
#include <cstdint>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <iomanip>

std::uint32_t readU32(Buffer* bytes, size_t& offset)
{
    if (offset+4 > bytes->size)
    {
        std::cout << "Read outside buffer size! Exiting...";
        std::exit(-1);
    }

    const std::uint32_t result{static_cast<uint32_t>(bytes->data[offset]) | (static_cast<uint32_t>(bytes->data[offset+1]) << 8) | (static_cast<uint32_t>(bytes->data[offset+2]) << 16) | (static_cast<uint32_t>(bytes->data[offset+3]) << 24)};
    offset+=4;
    return result;
}

std::uint8_t readU8(Buffer* bytes, size_t& offset)
{
    if (offset+1 > bytes->size)
    {
        std::cout << "Read outside buffer size! Exiting...";
        std::exit(-1);
    }

    std::uint8_t byte{bytes->data[offset]};
    offset+=1;
    return byte;
}

void writeU32(Buffer* bytes, size_t offset, std::uint32_t byte)
{
    if (offset + 4 > bytes->size)
    {
        std::cout << "Offset out of bounds!";
        return;
    }

    bytes->data[offset] = byte & 0xFF;
    bytes->data[offset+1] = (byte >> 8) & 0xFF;
    bytes->data[offset+2] = (byte >> 16) & 0xFF;
    bytes->data[offset+3] = (byte >> 24) & 0xFF;
}

void writeU8(Buffer* bytes, size_t offset, std::uint8_t byte)
{
    if (offset + 1 > bytes->size)
    {
        std::cout << "Offset out of bounds!";
        return;
    }
    
    bytes->data[offset] = byte;
}

void writeBufferToFile(Buffer* bytes, const char* fileName)
{
    FILE* file{fopen(fileName, "wb")};

    if (file == nullptr)
    {
        std::cout << "Failed to create/open file!";
        return;
    }

    size_t written {fwrite(bytes->data, 1, bytes->size, file)};

    if (written != bytes->size)
    {
        std::cout << "Save failed, file may have been corrupted!";
    }

    fclose(file);
}

void readFileToBuffer(Buffer* bytes, const char* fileName)
{
    FILE* file {fopen(fileName, "rb")};

    if (file == nullptr)
    {
        std::cout << "Could not open file!";
        std::exit(-1);
    }

    size_t read{fread(bytes->data, 1, bytes->size, file)};

    if (read != bytes->size)
    {
        std::cout << "Could not read file, it may be corrupted!";
        std::exit(-1);
    }

    fclose(file);
}

void printFile(Buffer* bytes, size_t bytesLength)
{
    std::uint64_t offset{0};
    for(int i{0}; i < bytesLength; i++)
    {
        if (offset % 16 == 0)
        {
            std::cout << '\n';
            std::cout << std::setw(8) << std::setfill('0') << std::uppercase << std::hex << offset << ": ";
        }
        std::cout << std::setw(2) << std::setfill('0') << std::uppercase << std::hex << int(bytes->data[i]) << ' ';
        offset+=1;
    }

    std::cout << std::endl;
}

Buffer initBuffer(size_t size)
{
    Buffer bytes{(unsigned char*)malloc(size), size};

    if (bytes.data == nullptr)
    {
        std::cout << "Malloc failed, what the fuck!";
        std::exit(-1);
    }

    return bytes;
}

void freeBuffer(Buffer* bytes)
{
    free(bytes->data);
    bytes->data = nullptr;
    bytes->size = 0;
}
