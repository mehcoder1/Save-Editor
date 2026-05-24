#ifndef SAVE_H
#define SAVE_H

#include <cstdint>
#include "buffer.h"

constexpr size_t SAVE_SIZE{17};

constexpr size_t VERSION_OFFSET{4};
constexpr size_t HEALTH_OFFSET{8};
constexpr size_t COINS_OFFSET{12};
constexpr size_t NAME_OFFSET{16};
constexpr size_t INVENTORY_OFFSET{32};

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

void changeVersion(const char* fileName, std::uint32_t value);
void changeHealth(const char* fileName, std::uint32_t value);
void changeCoins(const char* fileName, std::uint32_t value);

#endif