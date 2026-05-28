#ifndef BUFFER_H
#define BUFFER_H

#include <cstdint>
#include <cstddef>

struct Buffer
{
    unsigned char* data;
    size_t size;
};

std::uint32_t readU32(const Buffer* bytes, size_t& offset);
std::uint8_t readU8(const Buffer* bytes, size_t& offset);

void writeU32(Buffer* bytes, size_t offset, std::uint32_t byte);
void writeU8(Buffer* bytes, size_t offset, std::uint8_t byte);

void writeBufferToFile(Buffer* bytes, const char* fileName);
Buffer readFileToBuffer(const char* fileName);

void diffFiles(const char* fileName1, const char* fileName2);

void printFile(Buffer* bytes);

Buffer initBuffer(size_t size);
void freeBuffer(Buffer* bytes);

#endif