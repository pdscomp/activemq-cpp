/**
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "PrimitiveValueConverter.h"

#include <decaf/lang/Boolean.h>
#include <decaf/lang/Byte.h>
#include <decaf/lang/Character.h>
#include <decaf/lang/Short.h>
#include <decaf/lang/Integer.h>
#include <decaf/lang/Long.h>
#include <decaf/lang/Float.h>
#include <decaf/lang/Double.h>

namespace activemq{
namespace util{

////////////////////////////////////////////////////////////////////////////////
template<>
bool PrimitiveValueConverter::convert( const PrimitiveValueNode& value ) const
    throw( decaf::lang::exceptions::UnsupportedOperationException ) {

    switch( value.getType() ) {
        case PrimitiveValueNode::BOOLEAN_TYPE:
            return value.getBool();
        case PrimitiveValueNode::STRING_TYPE:
        case PrimitiveValueNode::BIG_STRING_TYPE:
        {
            try{
                return decaf::lang::Boolean::parseBoolean( value.getString() );
            } catch( decaf::lang::Exception& ex ) {}
        }
        default:
            throw decaf::lang::exceptions::UnsupportedOperationException(
                 __FILE__, __LINE__, "Unsupported Type Conversion" );
    }
}

////////////////////////////////////////////////////////////////////////////////
template<>
unsigned char PrimitiveValueConverter::convert( const PrimitiveValueNode& value ) const
    throw( decaf::lang::exceptions::UnsupportedOperationException ) {

    switch( value.getType() ) {
        case PrimitiveValueNode::BYTE_TYPE:
            return value.getByte();
        case PrimitiveValueNode::STRING_TYPE:
        case PrimitiveValueNode::BIG_STRING_TYPE:
        {
            try{
                return decaf::lang::Byte::parseByte( value.getString() );
            } catch( decaf::lang::Exception& ex ) {}
        }
        default:
            throw decaf::lang::exceptions::UnsupportedOperationException(
                 __FILE__, __LINE__, "Unsupported Type Conversion" );
    }
}

////////////////////////////////////////////////////////////////////////////////
template<>
char PrimitiveValueConverter::convert( const PrimitiveValueNode& value ) const
    throw( decaf::lang::exceptions::UnsupportedOperationException ) {

    switch( value.getType() ) {
        case PrimitiveValueNode::CHAR_TYPE:
            return value.getChar();
        case PrimitiveValueNode::STRING_TYPE:
        case PrimitiveValueNode::BIG_STRING_TYPE:
        {
            try{
                return (char)decaf::lang::Byte::parseByte( value.getString() );
            } catch( decaf::lang::Exception& ex ) {}
        }
        default:
            throw decaf::lang::exceptions::UnsupportedOperationException(
                 __FILE__, __LINE__, "Unsupported Type Conversion" );
    }
}

////////////////////////////////////////////////////////////////////////////////
template<>
short PrimitiveValueConverter::convert( const PrimitiveValueNode& value ) const
    throw( decaf::lang::exceptions::UnsupportedOperationException ) {

    switch( value.getType() ) {
        case PrimitiveValueNode::BYTE_TYPE:
            return (short)value.getByte();
        case PrimitiveValueNode::SHORT_TYPE:
            return value.getShort();
        case PrimitiveValueNode::STRING_TYPE:
        case PrimitiveValueNode::BIG_STRING_TYPE:
        {
            try{
                return decaf::lang::Short::parseShort( value.getString() );
            } catch( decaf::lang::Exception& ex ) {}
        }
        default:
            throw decaf::lang::exceptions::UnsupportedOperationException(
                 __FILE__, __LINE__, "Unsupported Type Conversion" );
    }
}

////////////////////////////////////////////////////////////////////////////////
template<>
int PrimitiveValueConverter::convert( const PrimitiveValueNode& value ) const
    throw( decaf::lang::exceptions::UnsupportedOperationException ) {

    switch( value.getType() ) {
        case PrimitiveValueNode::BYTE_TYPE:
            return (int)value.getByte();
        case PrimitiveValueNode::SHORT_TYPE:
            return (int)value.getShort();
        case PrimitiveValueNode::INTEGER_TYPE:
            return value.getInt();
        case PrimitiveValueNode::STRING_TYPE:
        case PrimitiveValueNode::BIG_STRING_TYPE:
        {
            try{
                return decaf::lang::Integer::parseInt( value.getString() );
            } catch( decaf::lang::Exception& ex ) {}
        }
        default:
            throw decaf::lang::exceptions::UnsupportedOperationException(
                __FILE__, __LINE__, "Unsupported Type Conversion" );
    }
}

////////////////////////////////////////////////////////////////////////////////
template<>
long long PrimitiveValueConverter::convert( const PrimitiveValueNode& value ) const
    throw( decaf::lang::exceptions::UnsupportedOperationException ) {

    switch( value.getType() ) {
        case PrimitiveValueNode::BYTE_TYPE:
            return (long long)value.getByte();
        case PrimitiveValueNode::SHORT_TYPE:
            return (long long)value.getShort();
        case PrimitiveValueNode::INTEGER_TYPE:
            return (long long)value.getInt();
        case PrimitiveValueNode::LONG_TYPE:
            return (long long)value.getLong();
        case PrimitiveValueNode::STRING_TYPE:
        case PrimitiveValueNode::BIG_STRING_TYPE:
        {
            try{
                return decaf::lang::Long::parseLong( value.getString() );
            } catch( decaf::lang::Exception& ex ) {}
        }
        default:
            throw decaf::lang::exceptions::UnsupportedOperationException(
                __FILE__, __LINE__, "Unsupported Type Conversion" );
    }
}

////////////////////////////////////////////////////////////////////////////////
template<>
float PrimitiveValueConverter::convert( const PrimitiveValueNode& value ) const
    throw( decaf::lang::exceptions::UnsupportedOperationException ) {

    switch( value.getType() ) {
        case PrimitiveValueNode::FLOAT_TYPE:
            return value.getFloat();
        case PrimitiveValueNode::STRING_TYPE:
        case PrimitiveValueNode::BIG_STRING_TYPE:
        {
            try{
                return decaf::lang::Float::parseFloat( value.getString() );
            } catch( decaf::lang::Exception& ex ) {}
        }
        default:
            throw decaf::lang::exceptions::UnsupportedOperationException(
                 __FILE__, __LINE__, "Unsupported Type Conversion" );
    }
}

////////////////////////////////////////////////////////////////////////////////
template<>
double PrimitiveValueConverter::convert( const PrimitiveValueNode& value ) const
    throw( decaf::lang::exceptions::UnsupportedOperationException ) {

    switch( value.getType() ) {
        case PrimitiveValueNode::FLOAT_TYPE:
            return (double)value.getFloat();
        case PrimitiveValueNode::DOUBLE_TYPE:
            return value.getDouble();
        case PrimitiveValueNode::STRING_TYPE:
        case PrimitiveValueNode::BIG_STRING_TYPE:
        {
            try{
                return decaf::lang::Double::parseDouble( value.getString() );
            } catch( decaf::lang::Exception& ex ) {}
        }
        default:
            throw decaf::lang::exceptions::UnsupportedOperationException(
                 __FILE__, __LINE__, "Unsupported Type Conversion" );
    }
}

////////////////////////////////////////////////////////////////////////////////
template<>
std::string PrimitiveValueConverter::convert( const PrimitiveValueNode& value ) const
    throw( decaf::lang::exceptions::UnsupportedOperationException ) {

    switch( value.getType() ) {
        case PrimitiveValueNode::BOOLEAN_TYPE:
            return decaf::lang::Boolean::toString( value.getBool() );
        case PrimitiveValueNode::BYTE_TYPE:
            return decaf::lang::Byte::toString( value.getByte() );
        case PrimitiveValueNode::CHAR_TYPE:
            return decaf::lang::Character::valueOf( value.getChar() ).toString();
        case PrimitiveValueNode::SHORT_TYPE:
            return decaf::lang::Short::toString( value.getShort() );
        case PrimitiveValueNode::INTEGER_TYPE:
            return decaf::lang::Integer::toString( value.getInt() );
        case PrimitiveValueNode::LONG_TYPE:
            return decaf::lang::Long::toString( value.getLong() );
        case PrimitiveValueNode::FLOAT_TYPE:
            return decaf::lang::Float::toString( value.getFloat() );
        case PrimitiveValueNode::DOUBLE_TYPE:
            return decaf::lang::Double::toString( value.getDouble() );
        case PrimitiveValueNode::STRING_TYPE:
        case PrimitiveValueNode::BIG_STRING_TYPE:
            return value.getString();
        default:
            throw decaf::lang::exceptions::UnsupportedOperationException(
                __FILE__, __LINE__, "Unsupported Type Conversion" );
    }
}

}}