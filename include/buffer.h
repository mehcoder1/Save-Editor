#ifndef BUFFER_H
#define BUFFER_H

#include <cstdint>
#include <cstddef>

struct Buffer
{
    unsigned char* data;
    size_t size;
};

std::uint32_t readU32(Buffer* bytes, size_t& offset);
std::uint8_t readU8(Buffer* bytes, size_t& offset);

void writeU32(Buffer* bytes, size_t offset, std::uint32_t byte);
void writeU8(Buffer* bytes, size_t offset, std::uint8_t byte);

void writeBufferToFile(Buffer* bytes, const char* fileName);
void readFileToBuffer(Buffer* bytes, const char* fileName);

void printFile(Buffer* bytes, size_t bytesLength);

Buffer initBuffer(size_t size);
void freeBuffer(Buffer* bytes);

#endif