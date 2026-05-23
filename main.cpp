#include <cstdlib>
#include <fstream>
#include <ios>
#include <iostream>
#include <cstdint>
#include <vector>
#include <string>
#include <cassert>
#include <iomanip>

struct Save{
    std::uint32_t version{};
    std::uint32_t health{};
    std::uint32_t coins{};
    std::uint8_t initial{};
};

void printSave(Save save)
{
    std::cout << "Version: " << int(save.version) << "\nHealth: " << int(save.health) << "\nCoins: " << int(save.coins) << "\nName initial: " << save.initial << '\n';
}

std::uint32_t readU32(const std::vector<std::uint8_t>& bytes, size_t& offset)
{
    if (offset + 4 > bytes.size())
    {
        std::cout << "Not enough bytes to read UINT32!";
        std::exit(-1);
    }

    const std::uint32_t result{static_cast<uint32_t>(bytes[offset]) | (static_cast<uint32_t>(bytes[offset+1]) << 8) | (static_cast<uint32_t>(bytes[offset+2]) << 16) | (static_cast<uint32_t>(bytes[offset+3]) << 24)};
    offset+=4;
    return result;
}

void writeU32(std::vector<std::uint8_t>& bytes, size_t offset, std::uint32_t byte)
{
    if (offset + 4 > bytes.size())
    {
        std::cout << "Not enough bytes to write UINT32!";
        std::exit(-1);
    }

    bytes[offset] = byte & 0xFF;
    bytes[offset+1] = (byte >> 8) & 0xFF;
    bytes[offset+2] = (byte >> 16) & 0xFF;
    bytes[offset+3] = (byte >> 24) & 0xFF;
}

template <typename T>
void writeBytes(std::vector<std::uint8_t>& bytes, std::uint32_t offset, T data)
{
    char* pt = reinterpret_cast<char*>(&data);
    for (size_t i{0}; i < (sizeof(data)); i++)
        bytes[i+offset] = pt[i];
}

void writeString(std::vector<std::uint8_t>& bytes, std::uint32_t offset, const std::string& data)
{
    const char* pt = data.data();
    for(size_t i{0}; i < data.size(); i++)
        bytes[i+offset] = pt[i];
}

std::vector<std::uint8_t> readFile(const std::string& fileName)
{
    std::ifstream file(fileName, std::ios::binary | std::ios::ate);

    if (!file) {
        std::cout << "Could not open file!\n";
        return {};
    }

    std::streamsize size {file.tellg()};
    file.seekg(0, std::ios::beg);

    std::vector<std::uint8_t> bytes(size);

    file.read(reinterpret_cast<char*>(bytes.data()), size);

    return bytes;
}

std::vector<std::uint8_t> convertSaveToBytes(const Save& save)
{
    std::vector<std::uint8_t> bytes(sizeof(save.version)+sizeof(save.health)+sizeof(save.coins)+sizeof(save.initial));

    size_t offset{0};

    writeBytes(bytes, offset, save.version);
    offset+=sizeof(save.version);

    writeBytes(bytes, offset, save.health);
    offset+=sizeof(save.health);

    writeBytes(bytes, offset, save.coins);
    offset+=sizeof(save.coins);

    writeBytes(bytes, offset, save.initial);
    offset+=sizeof(save.initial);

    return bytes;
}

void saveFile(const std::string& fileName, Save save)
{
    std::ofstream file(fileName, std::ios::binary);

    std::vector<std::uint8_t> bytes{'S', 'A', 'V', 'E'};

    std::vector<std::uint8_t> saveBytes {convertSaveToBytes(save)};

    bytes.insert(bytes.end(), saveBytes.begin(), saveBytes.end());

    file.write(reinterpret_cast<char*>(bytes.data()), bytes.size());
}

Save infoFile(const std::string& fileName)
{
    auto bytes {readFile(fileName)};

    if (bytes.size() != 17)
    {
        std::cout << "File size wrong!";
        std::exit(-1);
    }
    if (!(bytes[0]=='S' && bytes[1]=='A' && bytes[2]=='V' && bytes[3]=='E'))
    {
        std::cout << "File header wrong!";
        std::exit(-1);
    }

    std::size_t offset{4};
    
    uint32_t version{readU32(bytes, offset)};
    uint32_t health{readU32(bytes, offset)};
    uint32_t coins{readU32(bytes, offset)};

    Save save{version, health, coins, bytes[offset]};

    return save;
}

void modFile(const std::string& fileName, const std::string& stringOffset, const std::string& value)
{
    size_t offset{std::stoul(stringOffset, nullptr, 16)};
    std::uint32_t byte{std::stoul(value, nullptr, 10)};

    std::vector<std::uint8_t> bytes{readFile(fileName)};

    if(offset+4 > bytes.size())
    {
        std::cout << "Offset out of bounds!";
        return;
    }

    std::ofstream file(fileName,std::ios::binary);

    if (!file) {
        std::cout << "Could not open file!\n";
        return;
    }

    writeU32(bytes, offset, byte);

    file.write(reinterpret_cast<char*>(bytes.data()), bytes.size());
}

void printFile(std::vector<std::uint8_t> bytes)
{
    std::uint64_t offset{0};
    for(auto byte : bytes)
    {
        if (offset % 16 == 0)
        {
            std::cout << '\n';
            std::cout << std::setw(8) << std::setfill('0') << std::uppercase << std::hex << offset << ": ";
        }
        std::cout << std::setw(2) << std::setfill('0') << std::uppercase << std::hex << int(byte) << ' ';
        offset+=1;
    }

    std::cout << std::endl;
}

int main(int argc, char* argv[])
{
    std::vector<std::string> args;

    for(int i{1}; i < argc; i++)
    {
        args.push_back(argv[i]);
    }

    if (args.empty())
    {
        std::cout << "Commands: save, info, modify, read.\n"; 
        return -1;
    }

    if (args[0] == "save")
    {
        saveFile(args[1], Save{1, 95, 6, 'm'});
    }

    else if (args[0] == "info")
    {
        Save save {infoFile(args[1])};
        printSave(save);
    }

    else if (args[0] == "modify")
    {
        // Arg 1: file name, Arg 2: offset, Arg 3: value
        // assert(sizeof(args[3]) == 1uz);
        modFile(args[1], args[2], args[3]);
    }

    else if (args[0] == "read")
    {
        auto bytes {readFile(args[1])};
        printFile(bytes);
    }

    else 
    {
        std::cout << "Commands: save, info, modify, read.\n";
    }

    return 0;
}
