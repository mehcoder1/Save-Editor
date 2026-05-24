#include <iostream>
#include <cassert>
#include <cstring>

#include "buffer.h"
#include "save.h"

int main(int argc, char* argv[])
{
    if (argc == 1)
    {
        std::cout << "Commands: save, info, modify, read, verify.\n"; 
        return -1;
    }

    if (std::strcmp(argv[1], "save") == 0)
    {
        if (argc < 3)
            return -1;
        
        saveFile(argv[2], Save{1, 95, 6, 'm'});
        std::cout << "Saved file!";
    }

    else if (std::strcmp(argv[1], "info") == 0)
    {
        if (argc < 3)
            return -1;

        Save save {infoFile(argv[2])};
        printSave(save);
    }

    else if (std::strcmp(argv[1], "set") == 0)
    {
        if (argc < 5)
        {
            std::cout << "Not enough arguments. Command usage, set FILE_NAME FIELD VALUE.";
            return -1;
        }

        if (std::strcmp(argv[3], "version") == 0)
        {
            changeVersion(argv[2], strtoul(argv[4], nullptr, 10));
            std::cout << "Updated version number!";
        }
        else if (std::strcmp(argv[3], "health") == 0)
        {
            changeHealth(argv[2], strtoul(argv[4], nullptr, 10));
            std::cout << "Updated health value!";
        }
        else if (std::strcmp(argv[3], "coins") == 0)
        {
            changeCoins(argv[2], strtoul(argv[4], nullptr, 10));
            std::cout << "Updated coins amount!";
        }
        else
        {
            std::cout << "Valid fields: version, health, coins, name.";
        }
    }

    else if (std::strcmp(argv[1], "modify") == 0)
    {
        // Arg 1: file name, Arg 2: offset, Arg 3: value
        if (argc < 5)
            return -1;

        modFile(argv[2], argv[3], argv[4]);
        std::cout << "Modified file!";
    }

    else if (std::strcmp(argv[1], "read") == 0)
    {
        if (argc < 3)
            return -1;

        Buffer bytes{initBuffer(SAVE_SIZE)};

        readFileToBuffer(&bytes, argv[2]);
        printFile(&bytes, bytes.size);

        freeBuffer(&bytes);
    }

    else if (std::strcmp(argv[1], "verify") == 0)
    {
        if (argc < 3)
            return -1;

        if (verifyFile(argv[2]))
            std::cout << "The file is good!";
    }

    else 
    {
        std::cout << "Commands: save, info, modify, read, verify.\n";
    }

    return 0;
}
