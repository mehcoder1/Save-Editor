#include "buffer.h"
#include "save.h"

#include <cstring>
#include <iostream>

void convertSaveToBuffer(Buffer* bytes, Save save)
{
    std::memcpy(bytes->data, "SAVE", 4);

    writeU32(bytes, size_t(4), save.version);
    writeU32(bytes, size_t(8), save.health);
    writeU32(bytes, size_t(12), save.coins);
    writeU8(bytes, size_t(16), save.initial);
}

bool verifyFile(char* fileName)
{
    Buffer bytes{initBuffer(SAVE_SIZE)};

    readFileToBuffer(&bytes, fileName);

    if (bytes.data[0] != 'S' || bytes.data[1] != 'A' || bytes.data[2] != 'V' || bytes.data[3] != 'E')
    {
        std::cout << "File format is wrong or corrupted!";
        freeBuffer(&bytes);
        return false;
    }

    freeBuffer(&bytes);
    return true;
}

void modFile(char* fileName, char* stringOffset, char* value)
{
    size_t offset {std::strtoul(stringOffset, nullptr, 16)};
    std::uint32_t byte{std::strtoul(value, nullptr, 10)};

    Buffer bytes{initBuffer(SAVE_SIZE)};

    readFileToBuffer(&bytes, fileName);

    if (offset != 16)
        writeU32(&bytes, offset, byte);
    else
        writeU8(&bytes, offset, (std::uint8_t)byte);

    writeBufferToFile(&bytes, fileName);

    freeBuffer(&bytes);
}

void saveFile(const char* fileName, Save save)
{
    Buffer bytes{initBuffer(SAVE_SIZE)};

    convertSaveToBuffer(&bytes, save);

    writeBufferToFile(&bytes, fileName);

    freeBuffer(&bytes);
}

void printSave(Save save)
{
    std::cout << "Version: " << save.version << "\nHealth: " << save.health << "\nCoins: " << save.coins << "\nInitial: " << save.initial;
}

Save infoFile(char* fileName)
{
    Buffer bytes{initBuffer(SAVE_SIZE)};
    Save save{};

    readFileToBuffer(&bytes, fileName);

    size_t offset{4};

    save.version = readU32(&bytes, offset);
    save.health = readU32(&bytes, offset);
    save.coins = readU32(&bytes, offset);
    save.initial = readU8(&bytes, offset);

    freeBuffer(&bytes);

    return save;
}

void changeVersion(const char* fileName, std::uint32_t value)
{
    Buffer bytes{initBuffer(SAVE_SIZE)};

    readFileToBuffer(&bytes, fileName);

    writeU32(&bytes, VERSION_OFFSET, value);

    writeBufferToFile(&bytes, fileName);

    freeBuffer(&bytes);
}

void changeHealth(const char* fileName, std::uint32_t value)
{
    Buffer bytes{initBuffer(SAVE_SIZE)};

    readFileToBuffer(&bytes, fileName);

    writeU32(&bytes, HEALTH_OFFSET, value);

    writeBufferToFile(&bytes, fileName);

    freeBuffer(&bytes);
}

void changeCoins(const char* fileName, std::uint32_t value)
{
    Buffer bytes{initBuffer(SAVE_SIZE)};

    readFileToBuffer(&bytes, fileName);

    writeU32(&bytes, COINS_OFFSET, value);

    writeBufferToFile(&bytes, fileName);

    freeBuffer(&bytes);
}
