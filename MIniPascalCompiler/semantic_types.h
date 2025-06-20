#ifndef SEMANTIC_TYPES_H
#define SEMANTIC_TYPES_H

#include <string>

enum class DataType {
    INTEGER,
    REAL,
    BOOLEAN,
    ARRAY,
    UNKNOWN
};

struct TypeInfo {
    DataType baseType;
    int arrayStart;
    int arrayEnd;

    TypeInfo() : baseType(DataType::UNKNOWN), arrayStart(0), arrayEnd(0) {}
    TypeInfo(DataType type) : baseType(type), arrayStart(0), arrayEnd(0) {}
    TypeInfo(DataType type, int start, int end) : baseType(type), arrayStart(start), arrayEnd(end) {}

    bool operator==(const TypeInfo& other) const {
        return baseType == other.baseType &&
            arrayStart == other.arrayStart &&
            arrayEnd == other.arrayEnd;
    }

    std::string toString() const {
        switch (baseType) {
        case DataType::INTEGER:
            return "integer";
        case DataType::REAL:
            return "real";
        case DataType::BOOLEAN:
            return "boolean";
        case DataType::ARRAY:
            return "array[" + std::to_string(arrayStart) + ".." + std::to_string(arrayEnd) + "] of " +
                TypeInfo(static_cast<DataType>(baseType)).toString(); //  ⁄œÌ· Â‰« ·≈ŸÂ«— ‰Ê⁄ «·⁄‰«’— ›Ì «·„’›Ê›…
        default:
            return "unknown";
        }
    }
};

#endif // SEMANTIC_TYPES_H