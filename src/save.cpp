#include "buffer.h"
#include "save.h"

#include <cstring>
#include <iostream>
#include <cstdlib>

void readName(const Buffer* bytes, char name[17])
{
    name[16] = '\0';

    size_t offset {NAME_OFFSET};

    for (size_t i{0}; i < 16; i++)
    {
        name[i] = readU8(bytes, offset);
    }
}

void writeName(Buffer* bytes, const char* name)
{
    for (size_t i{0}; i < 16; i++)
    {
        if (i < strlen(name))
            writeU8(bytes, NAME_OFFSET+i, name[i]);
        else
            writeU8(bytes, NAME_OFFSET+i, 0);
    }
}

void convertSaveToBuffer(Buffer* bytes, const Save save)
{
    std::memcpy(bytes->data, "SAVE", 4);

    writeU32(bytes, size_t(4), save.version);
    writeU32(bytes, size_t(8), save.health);
    writeU32(bytes, size_t(12), save.coins);
    writeName(bytes, save.name);
}

bool verifyFile(char* fileName)
{
    Buffer bytes {readFileToBuffer(fileName)};

    if (!(bytes.size >= SAVE_SIZE))
    {   
        std::cout << "File is corrupted!";
        freeBuffer(&bytes);
        return false;
    }

    if (bytes.data[0] != 'S' || bytes.data[1] != 'A' || bytes.data[2] != 'V' || bytes.data[3] != 'E')
    {
        std::cout << "File format is wrong or corrupted!";
        freeBuffer(&bytes);
        return false;
    }

    freeBuffer(&bytes);
    return true;
}

void modFile(const char* fileName, char* stringOffset, char* value)
{
    size_t offset {std::strtoul(stringOffset, nullptr, 16)};
    std::uint32_t byte{std::strtoul(value, nullptr, 10)};

    Buffer bytes {readFileToBuffer(fileName)};

    backupFile(fileName, &bytes);

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
    std::cout << "Version: " << save.version << "\nHealth: " << save.health << "\nCoins: " << save.coins << "\nName: " << save.name;
}

Save infoFile(char* fileName)
{
    Save save{};

    Buffer bytes {readFileToBuffer(fileName)};

    size_t offset{4};

    save.version = readU32(&bytes, offset);
    save.health = readU32(&bytes, offset);
    save.coins = readU32(&bytes, offset);
    readName(&bytes, save.name);

    freeBuffer(&bytes);

    return save;
}

void changeVersion(const char* fileName, std::uint32_t value)
{
    Buffer bytes {readFileToBuffer(fileName)};

    backupFile(fileName, &bytes);

    writeU32(&bytes, VERSION_OFFSET, value);

    writeBufferToFile(&bytes, fileName);

    freeBuffer(&bytes);
}

void changeHealth(const char* fileName, std::uint32_t value)
{
    Buffer bytes {readFileToBuffer(fileName)};

    backupFile(fileName, &bytes);

    writeU32(&bytes, HEALTH_OFFSET, value);

    writeBufferToFile(&bytes, fileName);

    freeBuffer(&bytes);
}

void changeCoins(const char* fileName, std::uint32_t value)
{
    Buffer bytes {readFileToBuffer(fileName)};

    backupFile(fileName, &bytes);

    writeU32(&bytes, COINS_OFFSET, value);

    writeBufferToFile(&bytes, fileName);

    freeBuffer(&bytes);
}

void changeName(const char* fileName, const char* name)
{
    Buffer bytes {readFileToBuffer(fileName)};

    backupFile(fileName, &bytes);

    writeName(&bytes, name);

    writeBufferToFile(&bytes, fileName);

    freeBuffer(&bytes);
}

void backupFile(const char* fileName, Buffer* bytes)
{
    char backupFileName[256]{};
    std::strcpy(backupFileName, fileName);
    std::strcat(backupFileName, ".bak");
    writeBufferToFile(bytes, backupFileName);
}
