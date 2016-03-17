#include "common/IIO.h"

using namespace trinity;

const IIO::SemanticMapper& IIO::semanticMapper() {
    static SemanticMapper mapper;
    if (mapper.empty()) {
        mapper.insert(Semantic::Scalar, "Scalar");
        mapper.insert(Semantic::Vector, "Vector");
        mapper.insert(Semantic::Color, "Color");
    }
    return mapper;
}

const IIO::ValueTypeMapper& IIO::valueTypeMapper() {
    static ValueTypeMapper mapper;
    if (mapper.empty()) {
        mapper.insert(ValueType::T_FLOAT, "T_FLOAT");
        mapper.insert(ValueType::T_DOUBLE, "T_DOUBLE");
        mapper.insert(ValueType::T_UINT8, "T_UINT8");
        mapper.insert(ValueType::T_UINT16, "T_UINT16");
        mapper.insert(ValueType::T_UINT32, "T_UINT32");
        mapper.insert(ValueType::T_UINT64, "T_UINT64");
        mapper.insert(ValueType::T_INT8, "T_INT8");
        mapper.insert(ValueType::T_INT16, "T_INT16");
        mapper.insert(ValueType::T_INT32, "T_INT32");
        mapper.insert(ValueType::T_INT64, "T_INT64");
    }
    return mapper;
}