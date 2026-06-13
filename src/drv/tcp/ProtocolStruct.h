#ifndef PROTOCOL_STRUCT_H
#define PROTOCOL_STRUCT_H

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

/// Parsed struct-field definition loaded from a CSV (Excel export).
class ProtocolStruct
{
public:
    enum class Type
    {
        Uint8,
        Uint16,
        Uint32,
        Uint64,
        Int8,
        Int16,
        Int32,
        Int64,
        Float,
        Double
    };

    struct Field
    {
        std::string name;
        Type        type;
        size_t      size;   // byte width
        size_t      offset; // auto-computed byte-offset in the binary layout
    };

    ProtocolStruct() = default;

    /// Load struct layout from a CSV file.
    /// Expected columns: name, type, size  (first row is a header, skipped).
    bool loadCsv(const std::string &path);

    const std::vector<Field> &fields()    const { return m_fields; }
    size_t                    totalSize() const { return m_totalSize; }
    bool                      isEmpty()   const { return m_fields.empty(); }

    /// Serialize with auto-generated test values (incrementing bytes
    /// 0x01, 0x02, … written in big-endian per field).
    std::vector<uint8_t> serializeTest() const;

    /// Map a type enum to its canonical byte width (for validation / info).
    static size_t typeByteSize(Type t);

    /// Convert type enum to human-readable string (e.g. "uint32").
    static const char *typeName(Type t);

private:
    static Type parseType(const std::string &s);

    std::vector<Field> m_fields;
    size_t             m_totalSize = 0;
};

#endif // PROTOCOL_STRUCT_H
