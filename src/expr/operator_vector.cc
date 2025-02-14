// Copyright (c) 2023 dingodb.com, Inc. All Rights Reserved
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "operator_vector.h"

#include "codec.h"

#define NULL_PREFIX  0x00
#define NULL_INT32   (NULL_PREFIX | TYPE_INT32)
#define NULL_INT64   (NULL_PREFIX | TYPE_INT64)
#define NULL_BOOL    (NULL_PREFIX | TYPE_BOOL)
#define NULL_FLOAT   (NULL_PREFIX | TYPE_FLOAT)
#define NULL_DOUBLE  (NULL_PREFIX | TYPE_DOUBLE)
#define NULL_DECIMAL (NULL_PREFIX | TYPE_DECIMAL)
#define NULL_STRING  (NULL_PREFIX | TYPE_STRING)

#define CONST         0x10
#define CONST_INT32   (CONST | TYPE_INT32)
#define CONST_INT64   (CONST | TYPE_INT64)
#define CONST_BOOL    (CONST | TYPE_BOOL)
#define CONST_FLOAT   (CONST | TYPE_FLOAT)
#define CONST_DOUBLE  (CONST | TYPE_DOUBLE)
#define CONST_DECIMAL (CONST | TYPE_DECIMAL)
#define CONST_STRING  (CONST | TYPE_STRING)

#define CONST_N       0x20
#define CONST_N_INT32 (CONST_N | TYPE_INT32)
#define CONST_N_INT64 (CONST_N | TYPE_INT64)
#define CONST_N_BOOL  (CONST_N | TYPE_BOOL)

#define VAR_I         0x30
#define VAR_I_INT32   (VAR_I | TYPE_INT32)
#define VAR_I_INT64   (VAR_I | TYPE_INT64)
#define VAR_I_BOOL    (VAR_I | TYPE_BOOL)
#define VAR_I_FLOAT   (VAR_I | TYPE_FLOAT)
#define VAR_I_DOUBLE  (VAR_I | TYPE_DOUBLE)
#define VAR_I_DECIMAL (VAR_I | TYPE_DECIMAL)
#define VAR_I_STRING  (VAR_I | TYPE_STRING)

#define POS 0x81
#define NEG 0x82
#define ADD 0x83
#define SUB 0x84
#define MUL 0x85
#define DIV 0x86
#define MOD 0x87

#define EQ 0x91
#define GE 0x92
#define GT 0x93
#define LE 0x94
#define LT 0x95
#define NE 0x96

#define IS_NULL  0xA1
#define IS_TRUE  0xA2
#define IS_FALSE 0xA3

#define NOT 0x51
#define AND 0x52
#define OR  0x53

#define CAST 0xF0

using namespace dingodb::expr;

void OperatorVector::Decode(const byte code[], size_t len)
{
    m_vector.clear();
    bool successful = true;
    const byte *b;
    for (const byte *p = code; successful && p < code + len;) {
        b = p;
        switch (*p) {
        case NULL_INT32:
            ++p;
            Add(OperatorNull<CxxTraits<TYPE_INT32>::type>());
            break;
        case NULL_INT64:
            ++p;
            Add(OperatorNull<CxxTraits<TYPE_INT64>::type>());
            break;
        case NULL_BOOL:
            ++p;
            Add(OperatorNull<CxxTraits<TYPE_BOOL>::type>());
            break;
        case NULL_FLOAT:
            ++p;
            Add(OperatorNull<CxxTraits<TYPE_FLOAT>::type>());
            break;
        case NULL_DOUBLE:
            ++p;
            Add(OperatorNull<CxxTraits<TYPE_DOUBLE>::type>());
            break;
        case CONST_INT32: {
            ++p;
            CxxTraits<TYPE_INT32>::type v;
            p = DecodeVarint(v, p);
            Add(OperatorConst<CxxTraits<TYPE_INT32>::type>(v));
            break;
        }
        case CONST_INT64: {
            ++p;
            CxxTraits<TYPE_INT64>::type v;
            p = DecodeVarint(v, p);
            Add(OperatorConst<CxxTraits<TYPE_INT64>::type>(v));
            break;
        }
        case CONST_BOOL:
            ++p;
            Add(OperatorConst<CxxTraits<TYPE_BOOL>::type>(true));
            break;
        case CONST_FLOAT: {
            ++p;
            CxxTraits<TYPE_FLOAT>::type v;
            p = DecodeFloat(v, p);
            Add(OperatorConst<CxxTraits<TYPE_FLOAT>::type>(v));
            break;
        }
        case CONST_DOUBLE: {
            ++p;
            CxxTraits<TYPE_DOUBLE>::type v;
            p = DecodeDouble(v, p);
            Add(OperatorConst<CxxTraits<TYPE_DOUBLE>::type>(v));
            break;
        }
        case CONST_DECIMAL:
            ++p;
            // TODO
            break;
        case CONST_STRING: {
            ++p;
            CxxTraits<TYPE_STRING>::type v;
            p = DecodeString(v, p);
            Add(OperatorConst<CxxTraits<TYPE_STRING>::type>(v));
            break;
        }
        case CONST_N_INT32: {
            ++p;
            CxxTraits<TYPE_INT32>::type v;
            p = DecodeVarint(v, p);
            Add(OperatorConst<CxxTraits<TYPE_INT32>::type>(-v));
            break;
        }
        case CONST_N_INT64: {
            ++p;
            CxxTraits<TYPE_INT64>::type v;
            p = DecodeVarint(v, p);
            Add(OperatorConst<CxxTraits<TYPE_INT64>::type>(-v));
            break;
        }
        case CONST_N_BOOL:
            ++p;
            Add(OperatorConst<CxxTraits<TYPE_BOOL>::type>(false));
            break;
        case VAR_I_INT32: {
            ++p;
            uint32_t v;
            p = DecodeVarint(v, p);
            Add(OperatorVarI<CxxTraits<TYPE_INT32>::type>(v));
            break;
        }
        case VAR_I_INT64: {
            ++p;
            uint32_t v;
            p = DecodeVarint(v, p);
            Add(OperatorVarI<CxxTraits<TYPE_INT64>::type>(v));
            break;
        }
        case VAR_I_BOOL: {
            ++p;
            uint32_t v;
            p = DecodeVarint(v, p);
            Add(OperatorVarI<CxxTraits<TYPE_BOOL>::type>(v));
            break;
        }
        case VAR_I_FLOAT: {
            ++p;
            uint32_t v;
            p = DecodeVarint(v, p);
            Add(OperatorVarI<CxxTraits<TYPE_FLOAT>::type>(v));
            break;
        }
        case VAR_I_DOUBLE: {
            ++p;
            uint32_t v;
            p = DecodeVarint(v, p);
            Add(OperatorVarI<CxxTraits<TYPE_DOUBLE>::type>(v));
            break;
        }
        case VAR_I_DECIMAL: {
            ++p;
            uint32_t v;
            p = DecodeVarint(v, p);
            // TODO
            // Add(OperatorVarI<CxxTraits<TYPE_DECIMAL>::type>(v));
            break;
        }
        case VAR_I_STRING: {
            ++p;
            uint32_t v;
            p = DecodeVarint(v, p);
            Add(OperatorVarI<CxxTraits<TYPE_STRING>::type>(v));
            break;
        }
        case POS:
            ++p;
            successful = AddOperatorByType<OperatorPos>(*p);
            ++p;
            break;
        case NEG:
            ++p;
            successful = AddOperatorByType<OperatorNeg>(*p);
            ++p;
            break;
        case ADD:
            ++p;
            successful = AddOperatorByType<OperatorAdd>(*p);
            ++p;
            break;
        case SUB:
            ++p;
            successful = AddOperatorByType<OperatorSub>(*p);
            ++p;
            break;
        case MUL:
            ++p;
            successful = AddOperatorByType<OperatorMul>(*p);
            ++p;
            break;
        case DIV:
            ++p;
            successful = AddOperatorByType<OperatorDiv>(*p);
            ++p;
            break;
        case MOD:
            ++p;
            successful = AddOperatorByType<OperatorMod>(*p);
            ++p;
            break;
        case EQ:
            ++p;
            successful = AddOperatorByType<OperatorEq>(*p);
            ++p;
            break;
        case GE:
            ++p;
            successful = AddOperatorByType<OperatorGe>(*p);
            ++p;
            break;
        case GT:
            ++p;
            successful = AddOperatorByType<OperatorGt>(*p);
            ++p;
            break;
        case LE:
            ++p;
            successful = AddOperatorByType<OperatorLe>(*p);
            ++p;
            break;
        case LT:
            ++p;
            successful = AddOperatorByType<OperatorLt>(*p);
            ++p;
            break;
        case NE:
            ++p;
            successful = AddOperatorByType<OperatorNe>(*p);
            ++p;
            break;
        case IS_NULL:
            ++p;
            successful = AddOperatorByType<OperatorIsNull>(*p);
            ++p;
            break;
        case IS_TRUE:
            ++p;
            successful = AddOperatorByType<OperatorIsTrue>(*p);
            ++p;
            break;
        case IS_FALSE:
            ++p;
            successful = AddOperatorByType<OperatorIsFalse>(*p);
            ++p;
            break;
        case NOT:
            Add(OperatorNot());
            ++p;
            break;
        case AND:
            Add(OperatorAnd());
            ++p;
            break;
        case OR:
            Add(OperatorOr());
            ++p;
            break;
        case CAST:
            ++p;
            successful = AddCastOperator(*p);
            ++p;
            break;
        default:
            successful = false;
            break;
        }
    }
    if (!successful) {
        throw std::runtime_error("Unknown instruction, bytes = " + ConvertBytesToHex(b, len - (b - code)));
    }
}

std::string OperatorVector::ConvertBytesToHex(const byte *data, size_t len)
{
    char *buf = new char[len * 2 + 1];
    BytesToHex(buf, data, len);
    buf[len * 2] = '\0';
    std::string result(buf);
    delete[] buf;
    return result;
}

template <template <typename> class OP> bool OperatorVector::AddOperatorByType(byte type)
{
    switch (type) {
    case TYPE_INT32:
        Add(OP<CxxTraits<TYPE_INT32>::type>());
        return true;
    case TYPE_INT64:
        Add(OP<CxxTraits<TYPE_INT64>::type>());
        return true;
    case TYPE_BOOL:
        Add(OP<CxxTraits<TYPE_BOOL>::type>());
        return true;
    case TYPE_FLOAT:
        Add(OP<CxxTraits<TYPE_FLOAT>::type>());
        return true;
    case TYPE_DOUBLE:
        Add(OP<CxxTraits<TYPE_DOUBLE>::type>());
        return true;
    case TYPE_DECIMAL:
        Add(OP<CxxTraits<TYPE_DECIMAL>::type>());
        return true;
    case TYPE_STRING:
        Add(OP<CxxTraits<TYPE_STRING>::type>());
        return true;
    default:
        break;
        throw std::runtime_error("Unsupported types of operator , bytes = " + ConvertBytesToHex(&type, 1));
    }
    return false;
}

bool OperatorVector::AddCastOperator(byte b)
{
    switch (b) {
    case (TYPE_INT32 << 4) | TYPE_INT64:
        Add(OperatorCast<CxxTraits<TYPE_INT32>::type, CxxTraits<TYPE_INT64>::type>());
        return true;
    case (TYPE_INT32 << 4) | TYPE_BOOL:
        Add(OperatorCast<CxxTraits<TYPE_INT32>::type, CxxTraits<TYPE_BOOL>::type>());
        return true;
    case (TYPE_INT32 << 4) | TYPE_FLOAT:
        Add(OperatorCast<CxxTraits<TYPE_INT32>::type, CxxTraits<TYPE_FLOAT>::type>());
        return true;
    case (TYPE_INT32 << 4) | TYPE_DOUBLE:
        Add(OperatorCast<CxxTraits<TYPE_INT32>::type, CxxTraits<TYPE_DOUBLE>::type>());
        return true;
    case (TYPE_INT64 << 4) | TYPE_INT32:
        Add(OperatorCast<CxxTraits<TYPE_INT64>::type, CxxTraits<TYPE_INT32>::type>());
        return true;
    case (TYPE_INT64 << 4) | TYPE_BOOL:
        Add(OperatorCast<CxxTraits<TYPE_INT64>::type, CxxTraits<TYPE_BOOL>::type>());
        return true;
    case (TYPE_INT64 << 4) | TYPE_FLOAT:
        Add(OperatorCast<CxxTraits<TYPE_INT64>::type, CxxTraits<TYPE_FLOAT>::type>());
        return true;
    case (TYPE_INT64 << 4) | TYPE_DOUBLE:
        Add(OperatorCast<CxxTraits<TYPE_INT64>::type, CxxTraits<TYPE_DOUBLE>::type>());
        return true;
    case (TYPE_BOOL << 4) | TYPE_INT32:
        Add(OperatorCast<CxxTraits<TYPE_BOOL>::type, CxxTraits<TYPE_INT32>::type>());
        return true;
    case (TYPE_BOOL << 4) | TYPE_INT64:
        Add(OperatorCast<CxxTraits<TYPE_BOOL>::type, CxxTraits<TYPE_INT64>::type>());
        return true;
    case (TYPE_BOOL << 4) | TYPE_FLOAT:
        Add(OperatorCast<CxxTraits<TYPE_BOOL>::type, CxxTraits<TYPE_FLOAT>::type>());
        return true;
    case (TYPE_BOOL << 4) | TYPE_DOUBLE:
        Add(OperatorCast<CxxTraits<TYPE_BOOL>::type, CxxTraits<TYPE_DOUBLE>::type>());
        return true;
    case (TYPE_FLOAT << 4) | TYPE_INT32:
        Add(OperatorCast<CxxTraits<TYPE_FLOAT>::type, CxxTraits<TYPE_INT32>::type>());
        return true;
    case (TYPE_FLOAT << 4) | TYPE_INT64:
        Add(OperatorCast<CxxTraits<TYPE_FLOAT>::type, CxxTraits<TYPE_INT64>::type>());
        return true;
    case (TYPE_FLOAT << 4) | TYPE_BOOL:
        Add(OperatorCast<CxxTraits<TYPE_FLOAT>::type, CxxTraits<TYPE_BOOL>::type>());
        return true;
    case (TYPE_FLOAT << 4) | TYPE_DOUBLE:
        Add(OperatorCast<CxxTraits<TYPE_FLOAT>::type, CxxTraits<TYPE_DOUBLE>::type>());
        return true;
    case (TYPE_DOUBLE << 4) | TYPE_INT32:
        Add(OperatorCast<CxxTraits<TYPE_DOUBLE>::type, CxxTraits<TYPE_INT32>::type>());
        return true;
    case (TYPE_DOUBLE << 4) | TYPE_INT64:
        Add(OperatorCast<CxxTraits<TYPE_DOUBLE>::type, CxxTraits<TYPE_INT64>::type>());
        return true;
    case (TYPE_DOUBLE << 4) | TYPE_BOOL:
        Add(OperatorCast<CxxTraits<TYPE_DOUBLE>::type, CxxTraits<TYPE_BOOL>::type>());
        return true;
    case (TYPE_DOUBLE << 4) | TYPE_FLOAT:
        Add(OperatorCast<CxxTraits<TYPE_DOUBLE>::type, CxxTraits<TYPE_FLOAT>::type>());
        return true;
    case (TYPE_INT32 << 4) | TYPE_INT32:
    case (TYPE_INT64 << 4) | TYPE_INT64:
    case (TYPE_BOOL << 4) | TYPE_BOOL:
    case (TYPE_FLOAT << 4) | TYPE_FLOAT:
    case (TYPE_DOUBLE << 4) | TYPE_DOUBLE:
    case (TYPE_DECIMAL << 4) | TYPE_DECIMAL:
    case (TYPE_STRING << 4) | TYPE_STRING:
        return true;
    default:
        break;
        throw std::runtime_error("Unsupported types of cast operator, bytes = " + ConvertBytesToHex(&b, 1));
    }
    return false;
}
