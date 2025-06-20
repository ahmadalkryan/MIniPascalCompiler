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
    union {
        struct {
            int arrayStart;
            int arrayEnd;
            DataType elementType; // Type of elements in the array
        };
    };

    // Constructors
    TypeInfo() : baseType(DataType::UNKNOWN), arrayStart(0), arrayEnd(0), elementType(DataType::UNKNOWN) {}
    explicit TypeInfo(DataType type) : baseType(type), arrayStart(0), arrayEnd(0), elementType(DataType::UNKNOWN) {}
    TypeInfo(DataType type, int start, int end, DataType elemType)
        : baseType(type), arrayStart(start), arrayEnd(end), elementType(elemType) {
    }

    bool operator==(const TypeInfo& other) const {
        if (baseType != other.baseType)
            return false;

        if (baseType == DataType::ARRAY) {
            return arrayStart == other.arrayStart &&
                arrayEnd == other.arrayEnd &&
                elementType == other.elementType;
        }

        return true;
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
                TypeInfo(elementType).toString();
        default:
            return "unknown";
        }
    }
};

#endif // SEMANTIC_TYPES_H