#include "ItemArray.h"
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

void readInventory(const Buffer* bytes, ItemArray* array)
{
    if (bytes->size <= SAVE_SIZE)
    {
        return;
    }

    size_t offset{INVENTORY_COUNT_OFFSET};
    std::uint32_t inventoryCount {readU32(bytes, offset)};

    reserve(array, inventoryCount);

    for(std::uint32_t i{0}; i < inventoryCount; i++)
    {
        std::uint32_t id{readU32(bytes, offset)};
        std::uint32_t amount{readU32(bytes, offset)};

        push(array, Item{id, amount});
    }
}

void writeInventory(Buffer* bytes, const ItemArray* array)
{
    for (size_t i{0}; i < array->size; i++)
    {
        Item item {array->data[i]};

        writeU32(bytes, INVENTORY_OFFSET+(i*8), item.item_id);
        writeU32(bytes, INVENTORY_OFFSET+((i*8)+4), item.item_amount);
    }
}

void convertSaveToBuffer(Buffer* bytes, const Save save)
{
    std::memcpy(bytes->data, "SAVE", 4);

    writeU32(bytes, VERSION_OFFSET, save.version);
    writeU32(bytes, HEALTH_OFFSET, save.health);
    writeU32(bytes, COINS_OFFSET, save.coins);
    writeName(bytes, save.name);
    writeU32(bytes, INVENTORY_COUNT_OFFSET, save.item_count);
    writeInventory(bytes, &save.items);
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
    Buffer bytes{initBuffer(SAVE_SIZE+(save.item_count*8))};

    convertSaveToBuffer(&bytes, save);

    writeBufferToFile(&bytes, fileName);

    freeBuffer(&bytes);
}

void printSave(Save save)
{
    std::cout << "Version: " << save.version << "\nHealth: " << save.health << "\nCoins: " << save.coins << "\nName: " << save.name << "\nInventory Count: " << save.item_count << "\n\n";

    std::cout << "Inventory:\n";

    for (size_t i{0}; i < save.item_count; i++)
    {
        std::cout << "   ID: " << save.items.data[i].item_id << ", Amount: " << save.items.data[i].item_amount << '\n';
    }
}

Save infoFile(const char* fileName)
{
    Save save{};
    save.items = initArray(0);

    Buffer bytes {readFileToBuffer(fileName)};

    size_t offset{4};

    save.version = readU32(&bytes, offset);
    save.health = readU32(&bytes, offset);
    save.coins = readU32(&bytes, offset);
    readName(&bytes, save.name);

    offset+=16;
    
    save.item_count = readU32(&bytes, offset);
    readInventory(&bytes, &save.items);

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

void pushItem(const char* fileName, const char* id, const char* amount)
{
    Save save {infoFile(fileName)};

    Item item{strtoul(id, nullptr, 10), strtoul(amount, nullptr, 10)};

    push(&save.items, item);

    save.item_count = save.items.size;

    Buffer bytes{initBuffer(SAVE_SIZE+(save.item_count*8))};

    convertSaveToBuffer(&bytes, save);

    writeBufferToFile(&bytes, fileName);

    freeBuffer(&bytes);
    freeArray(&save.items);
}

void popItem(const char* fileName)
{
    Save save {infoFile(fileName)};

    if (save.item_count==0)
    {
        std::cout << "Inventory empty!";
        freeArray(&save.items);
        std::exit(0);
    }

    pop(&save.items);

    save.item_count = save.items.size;

    Buffer bytes{initBuffer(SAVE_SIZE+(save.item_count*8))};

    convertSaveToBuffer(&bytes, save);

    writeBufferToFile(&bytes, fileName);

    freeBuffer(&bytes);
    freeArray(&save.items);
}

void backupFile(const char* fileName, Buffer* bytes)
{
    char backupFileName[256]{};
    std::strcpy(backupFileName, fileName);
    std::strcat(backupFileName, ".bak");
    writeBufferToFile(bytes, backupFileName);
}
