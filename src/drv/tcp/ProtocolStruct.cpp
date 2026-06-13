#include "ProtocolStruct.h"

#include <algorithm>
#include <cstring>
#include <fstream>
#include <sstream>
#include <stdexcept>

#include "log.h"

// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------

namespace
{

std::string trim(const std::string &s)
{
    auto start = s.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) return {};
    auto end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

std::vector<std::string> splitLine(const std::string &line)
{
    std::vector<std::string> cols;
    std::istringstream       ss(line);
    std::string              cell;
    while (std::getline(ss, cell, ','))
        cols.push_back(trim(cell));
    return cols;
}

} // anonymous namespace

// ---------------------------------------------------------------------------
// Type helpers
// ---------------------------------------------------------------------------

size_t ProtocolStruct::typeByteSize(Type t)
{
    switch (t)
    {
    case Type::Uint8:  return 1;
    case Type::Int8:   return 1;
    case Type::Uint16: return 2;
    case Type::Int16:  return 2;
    case Type::Uint32: return 4;
    case Type::Int32:  return 4;
    case Type::Uint64: return 8;
    case Type::Int64:  return 8;
    case Type::Float:  return 4;
    case Type::Double: return 8;
    }
    return 0;
}

const char *ProtocolStruct::typeName(Type t)
{
    switch (t)
    {
    case Type::Uint8:  return "uint8";
    case Type::Uint16: return "uint16";
    case Type::Uint32: return "uint32";
    case Type::Uint64: return "uint64";
    case Type::Int8:   return "int8";
    case Type::Int16:  return "int16";
    case Type::Int32:  return "int32";
    case Type::Int64:  return "int64";
    case Type::Float:  return "float";
    case Type::Double: return "double";
    }
    return "???";
}

ProtocolStruct::Type ProtocolStruct::parseType(const std::string &s)
{
    if (s == "uint8")  return Type::Uint8;
    if (s == "uint16") return Type::Uint16;
    if (s == "uint32") return Type::Uint32;
    if (s == "uint64") return Type::Uint64;
    if (s == "int8")   return Type::Int8;
    if (s == "int16")  return Type::Int16;
    if (s == "int32")  return Type::Int32;
    if (s == "int64")  return Type::Int64;
    if (s == "float")  return Type::Float;
    if (s == "double") return Type::Double;

    throw std::runtime_error("Unknown data type: " + s);
}

// ---------------------------------------------------------------------------
// CSV loading
// ---------------------------------------------------------------------------

bool ProtocolStruct::loadCsv(const std::string &path)
{
    m_fields.clear();
    m_totalSize = 0;

    std::ifstream f(path);
    if (!f.is_open())
    {
        LogErr("ProtocolStruct: cannot open %s", path.c_str());
        return false;
    }

    std::string line;
    size_t      lineNo = 0;

    while (std::getline(f, line))
    {
        ++lineNo;

        // skip empty lines and the header row
        if (line.empty()) continue;
        if (lineNo == 1 && line.find("name") != std::string::npos) continue;

        auto cols = splitLine(line);
        if (cols.size() < 3)
        {
            LogErr("ProtocolStruct:%zu: expected 3 columns, got %zu", lineNo, cols.size());
            continue;
        }

        Field field;
        field.name   = cols[0];
        field.size   = static_cast<size_t>(std::stoul(cols[2]));
        field.type   = parseType(cols[1]);
        field.offset = m_totalSize;

        // Warn if the declared size doesn't match the type's canonical size
        size_t canonical = typeByteSize(field.type);
        if (field.size != canonical)
            LogInf("ProtocolStruct: field \"%s\" declared size %zu != canonical %zu",
                   field.name.c_str(), field.size, canonical);

        m_fields.push_back(field);
        m_totalSize += field.size;

        LogInf("ProtocolStruct: + %-24s %-6s %2zu B  @ offset %3zu",
               field.name.c_str(), typeName(field.type), field.size, field.offset);
    }

    LogInf("ProtocolStruct: loaded %zu fields, total %zu bytes from %s",
           m_fields.size(), m_totalSize, path.c_str());
    return true;
}

// ---------------------------------------------------------------------------
// Test serialization
// ---------------------------------------------------------------------------

// Write an unsigned integer value in big-endian byte order.
template <typename T>
static void writeBigEndian(uint8_t *dst, T val)
{
    const size_t n = sizeof(T);
    for (size_t i = 0; i < n; ++i)
    {
        const size_t shift = 8 * (n - 1 - i);
        dst[i] = static_cast<uint8_t>(static_cast<uint64_t>(val) >> shift);
    }
}

std::vector<uint8_t> ProtocolStruct::serializeTest() const
{
    std::vector<uint8_t> buf(m_totalSize, 0);

    for (const auto &f : m_fields)
    {
        uint8_t *p = buf.data() + f.offset;

        switch (f.type)
        {
        case Type::Uint8:  p[0] = 0xAB; break;
        case Type::Int8:   p[0] = 0x7F; break;
        case Type::Uint16: writeBigEndian(p, uint16_t(0xCDEF)); break;
        case Type::Int16:  writeBigEndian(p, int16_t(0x1234));  break;
        case Type::Uint32:
            writeBigEndian(p, uint32_t(0x12345678));
            break;
        case Type::Int32:
            writeBigEndian(p, int32_t(0x7FFFFFFF));
            break;
        case Type::Uint64:
            writeBigEndian(p, uint64_t(0x1122334455667788ULL));
            break;
        case Type::Int64:
            writeBigEndian(p, int64_t(0x1122334455667788LL));
            break;
        case Type::Float:
        {
            float val = 3.14159f;
            uint32_t raw;
            std::memcpy(&raw, &val, sizeof(raw));
            writeBigEndian(p, raw);
            break;
        }
        case Type::Double:
        {
            double val = 2.718281828;
            uint64_t raw;
            std::memcpy(&raw, &val, sizeof(raw));
            writeBigEndian(p, raw);
            break;
        }
        }
    }

    return buf;
}
