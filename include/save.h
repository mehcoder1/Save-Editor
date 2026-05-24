#ifndef SAVE_H
#define SAVE_H

#include <cstdint>
#include "buffer.h"

constexpr size_t SAVE_SIZE{17};

struct Save{
    std::uint32_t version{};
    std::uint32_t health{};
    std::uint32_t coins{};
    std::uint8_t initial{};
};

void convertSaveToBuffer(Buffer* bytes, Save save);

Save infoFile(char* fileName);

void saveFile(const char* fileName, Save save);

void modFile(char* fileName, char* stringOffset, char* value);

bool verifyFile(char* fileName);

void printSave(Save save);

#endif