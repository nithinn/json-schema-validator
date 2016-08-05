/******************************************************************************
 * Copyright (c) 2016, Nithin Nellikunnu (nithin.nn@gmail.com)
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a 
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the 
 * Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *****************************************************************************/

#include <limits.h>
#include <list>
#include <iostream>
#include <regex>
#include "gtest/gtest.h"
#include "json.h"
#include "primitive_base.h"
#include "keyword_validator.h"
#include "primitive.h"
#include "validator.h"

TEST(getPrimitveType, Negative)
{
   Json::Value a;
   ASSERT_THROW(JsonPrimitive::getPrimitveType(&a), Exception);
   
   Json::Value b;
   b["id"] = 1;
   ASSERT_THROW(JsonPrimitive::getPrimitveType(&b), Exception);

   Json::Value c;
   c["type"] = "aaa";
   ASSERT_THROW(JsonPrimitive::getPrimitveType(&c), Exception);
}

TEST(getPrimitveType, Positive)
{
   Json::Value a;
   a["type"] = "integer";
   ASSERT_NO_THROW(JsonPrimitive::getPrimitveType(&a));
   ASSERT_EQ(JsonPrimitive::getPrimitveType(&a), JSON_TYPE_INTEGER);

   Json::Value b;
   b["type"] = "boolean";
   ASSERT_NO_THROW(JsonPrimitive::getPrimitveType(&b));
   ASSERT_EQ(JsonPrimitive::getPrimitveType(&b), JSON_TYPE_BOOLEAN);

   Json::Value c;
   c["type"] = "string";
   ASSERT_NO_THROW(JsonPrimitive::getPrimitveType(&c));
   ASSERT_EQ(JsonPrimitive::getPrimitveType(&c), JSON_TYPE_STRING);
}

TEST(createPrimitive, Negative)
{
   JsonPrimitive *p = NULL;

   Json::Value a;
   ASSERT_THROW(p = JsonPrimitive::createPrimitive(&a), Exception);
   delete p;
   
   Json::Value b;
   b["id"] = 1;
   ASSERT_THROW(p = JsonPrimitive::createPrimitive(&b), Exception);
   delete p;

   Json::Value c;
   c["type"] = "aaa";
   ASSERT_THROW(p = JsonPrimitive::createPrimitive(&c), Exception);
   delete p;
}

TEST(createPrimitive, Positive)
{
   Json::Value a;
   a["type"] = "integer";
   ASSERT_NO_THROW(JsonPrimitive::createPrimitive(&a));
   ASSERT_TRUE(JsonPrimitive::createPrimitive(&a) != NULL);

   Json::Value b;
   b["type"] = "boolean";
   ASSERT_NO_THROW(JsonPrimitive::createPrimitive(&b));
   ASSERT_TRUE(JsonPrimitive::createPrimitive(&b) != NULL);

   Json::Value c;
   c["type"] = "string";
   ASSERT_NO_THROW(JsonPrimitive::createPrimitive(&c));
   ASSERT_TRUE(JsonPrimitive::createPrimitive(&c) != NULL);

   Json::Value d;
   d["type"] = "null";
   ASSERT_NO_THROW(JsonPrimitive::createPrimitive(&d));
   ASSERT_TRUE(JsonPrimitive::createPrimitive(&d) != NULL);
}

TEST(IntegerPrimitive, ValidInteger)
{
   Json::Value schema;
   schema["type"] = "integer";
   JsonPrimitive *jsonInt = NULL;
   ASSERT_NO_THROW((jsonInt = JsonPrimitive::createPrimitive(&schema)));
   ASSERT_TRUE(jsonInt != NULL);

   Json::Value a = 1;
   ASSERT_EQ(jsonInt->validate(&a), JVAL_ROK);
   Json::Value b = 1000000;
   ASSERT_EQ(jsonInt->validate(&b), JVAL_ROK);
   Json::Value c = -100;
   ASSERT_EQ(jsonInt->validate(&c), JVAL_ROK);
}

TEST(IntegerPrimitive, InvalidInteger)
{
   Json::Value schema;
   schema["type"] = "integer";
   JsonPrimitive *jsonInt = NULL;
   ASSERT_NO_THROW((jsonInt = JsonPrimitive::createPrimitive(&schema)));
   ASSERT_TRUE(jsonInt != NULL);

   Json::Value a = "a";
   ASSERT_EQ(jsonInt->validate(&a), JVAL_ERR_NOT_AN_INTEGER);
   Json::Value b[0] = 1000000;
   ASSERT_EQ(jsonInt->validate(b), JVAL_ERR_NOT_AN_INTEGER);
   Json::Value c = true;
   ASSERT_EQ(jsonInt->validate(&c), JVAL_ERR_NOT_AN_INTEGER);
   Json::Value d = 1.123;
   ASSERT_EQ(jsonInt->validate(&d), JVAL_ERR_NOT_AN_INTEGER);
}

TEST(IntegerPrimitive, ValidMinimum)
{
   Json::Value schema;
   schema["type"] = "integer";
   schema["minimum"] = 100;
   JsonPrimitive *jsonInt = NULL;
   ASSERT_NO_THROW((jsonInt = JsonPrimitive::createPrimitive(&schema)));
   ASSERT_TRUE(jsonInt != NULL);

   Json::Value a = 100;
   ASSERT_EQ(jsonInt->validate(&a), JVAL_ROK);
   Json::Value b = 1000;
   ASSERT_EQ(jsonInt->validate(&b), JVAL_ROK);
}

TEST(IntegerPrimitive, InvalidMinimum)
{
   Json::Value schema;
   schema["type"] = "integer";
   schema["minimum"] = 100;
   JsonPrimitive *jsonInt = NULL;
   ASSERT_NO_THROW((jsonInt = JsonPrimitive::createPrimitive(&schema)));
   ASSERT_TRUE(jsonInt != NULL);

   Json::Value a = 99;
   ASSERT_EQ(jsonInt->validate(&a), JVAL_ERR_INVALID_MINIMUM);
   Json::Value b = -1;
   ASSERT_EQ(jsonInt->validate(&b), JVAL_ERR_INVALID_MINIMUM);
}

TEST(IntegerPrimitive, ExlcusiveMinimum)
{
   Json::Value schema;
   schema["type"] = "integer";
   schema["minimum"] = 100;
   schema["exclusiveMinimum"] = true;
   JsonPrimitive *jsonInt = NULL;
   ASSERT_NO_THROW((jsonInt = JsonPrimitive::createPrimitive(&schema)));
   ASSERT_TRUE(jsonInt != NULL);

   Json::Value a = 99;
   ASSERT_EQ(jsonInt->validate(&a), JVAL_ERR_INVALID_MINIMUM);
   Json::Value b = 100;
   ASSERT_EQ(jsonInt->validate(&b), JVAL_ERR_INVALID_MINIMUM);
}

TEST(IntegerPrimitive, ValidMaximum)
{
   Json::Value schema;
   schema["type"] = "integer";
   schema["maximum"] = 100;
   JsonPrimitive *jsonInt = NULL;
   ASSERT_NO_THROW((jsonInt = JsonPrimitive::createPrimitive(&schema)));
   ASSERT_TRUE(jsonInt != NULL);

   Json::Value a = 100;
   ASSERT_EQ(jsonInt->validate(&a), JVAL_ROK);
   Json::Value b = 0;
   ASSERT_EQ(jsonInt->validate(&b), JVAL_ROK);
}

TEST(IntegerPrimitive, InvalidMaximum)
{
   Json::Value schema;
   schema["type"] = "integer";
   schema["maximum"] = 100;
   JsonPrimitive *jsonInt = NULL;
   ASSERT_NO_THROW((jsonInt = JsonPrimitive::createPrimitive(&schema)));
   ASSERT_TRUE(jsonInt != NULL);

   Json::Value a = 101;
   ASSERT_EQ(jsonInt->validate(&a), JVAL_ERR_INVALID_MAXIMUM);
   Json::Value b = 10000;
   ASSERT_EQ(jsonInt->validate(&b), JVAL_ERR_INVALID_MAXIMUM);
}

TEST(IntegerPrimitive, ExlcusiveMaximum)
{
   Json::Value schema;
   schema["type"] = "integer";
   schema["maximum"] = 100;
   schema["exclusiveMaximum"] = true;
   JsonPrimitive *jsonInt = NULL;
   ASSERT_NO_THROW((jsonInt = JsonPrimitive::createPrimitive(&schema)));
   ASSERT_TRUE(jsonInt != NULL);

   Json::Value a = 101;
   ASSERT_EQ(jsonInt->validate(&a), JVAL_ERR_INVALID_MAXIMUM);
   Json::Value b = 100;
   ASSERT_EQ(jsonInt->validate(&b), JVAL_ERR_INVALID_MAXIMUM);
}

TEST(IntegerPrimitive, ValidMultipleOf)
{
   Json::Value schema;
   schema["type"] = "integer";
   schema["multipleOf"] = 10;
   JsonPrimitive *jsonInt = NULL;
   ASSERT_NO_THROW((jsonInt = JsonPrimitive::createPrimitive(&schema)));
   ASSERT_TRUE(jsonInt != NULL);

   Json::Value a = 100;
   ASSERT_EQ(jsonInt->validate(&a), JVAL_ROK);
   Json::Value b = 200;
   ASSERT_EQ(jsonInt->validate(&b), JVAL_ROK);
   Json::Value c = 0;
   ASSERT_EQ(jsonInt->validate(&c), JVAL_ROK);
}

TEST(IntegerPrimitive, InvalidMultipleOf)
{
   Json::Value schema;
   schema["type"] = "integer";
   schema["multipleOf"] = 5;
   JsonPrimitive *jsonInt = NULL;
   ASSERT_NO_THROW((jsonInt = JsonPrimitive::createPrimitive(&schema)));
   ASSERT_TRUE(jsonInt != NULL);

   Json::Value a = 12;
   ASSERT_EQ(jsonInt->validate(&a), JVAL_ERR_NOT_A_MULTIPLE);
   Json::Value b = 21;
   ASSERT_EQ(jsonInt->validate(&b), JVAL_ERR_NOT_A_MULTIPLE);
}

TEST(NumberPrimitive, ValidNumber)
{
   Json::Value schema;
   schema["type"] = "number";
   JsonPrimitive *jsonNum = NULL;
   ASSERT_NO_THROW((jsonNum = JsonPrimitive::createPrimitive(&schema)));
   ASSERT_TRUE(jsonNum != NULL);

   Json::Value a = 11;
   ASSERT_EQ(jsonNum->validate(&a), JVAL_ROK);
   Json::Value b = 1000.11;
   ASSERT_EQ(jsonNum->validate(&b), JVAL_ROK);
   Json::Value c = -100.11;
   ASSERT_EQ(jsonNum->validate(&c), JVAL_ROK);
}

TEST(NumberPrimitive, InvalidNumber)
{
   Json::Value schema;
   schema["type"] = "number";
   JsonPrimitive *jsonNum = NULL;
   ASSERT_NO_THROW((jsonNum = JsonPrimitive::createPrimitive(&schema)));
   ASSERT_TRUE(jsonNum != NULL);

   Json::Value a = "a";
   ASSERT_EQ(jsonNum->validate(&a), JVAL_ERR_NOT_A_NUMBER);
   Json::Value b = true;
   ASSERT_EQ(jsonNum->validate(&b), JVAL_ERR_NOT_A_NUMBER);
}

TEST(NumberPrimitive, ValidMinimum)
{
   Json::Value schema;
   schema["type"] = "number";
   schema["minimum"] = 100;
   JsonPrimitive *jsonNum = NULL;
   ASSERT_NO_THROW((jsonNum = JsonPrimitive::createPrimitive(&schema)));
   ASSERT_TRUE(jsonNum != NULL);

   Json::Value a = 100;
   ASSERT_EQ(jsonNum->validate(&a), JVAL_ROK);
   Json::Value b = 1000;
   ASSERT_EQ(jsonNum->validate(&b), JVAL_ROK);
}

TEST(NumberPrimitive, InvalidMinimum)
{
   Json::Value schema;
   schema["type"] = "number";
   schema["minimum"] = 100;
   JsonPrimitive *jsonNum = NULL;
   ASSERT_NO_THROW((jsonNum = JsonPrimitive::createPrimitive(&schema)));
   ASSERT_TRUE(jsonNum != NULL);

   Json::Value a = 99;
   ASSERT_EQ(jsonNum->validate(&a), JVAL_ERR_INVALID_MINIMUM);
   Json::Value b = -1;
   ASSERT_EQ(jsonNum->validate(&b), JVAL_ERR_INVALID_MINIMUM);
}

TEST(NumberPrimitive, ExlcusiveMinimum)
{
   Json::Value schema;
   schema["type"] = "number";
   schema["minimum"] = 100;
   schema["exclusiveMinimum"] = true;
   JsonPrimitive *jsonNum = NULL;
   ASSERT_NO_THROW((jsonNum = JsonPrimitive::createPrimitive(&schema)));
   ASSERT_TRUE(jsonNum != NULL);

   Json::Value a = 99;
   ASSERT_EQ(jsonNum->validate(&a), JVAL_ERR_INVALID_MINIMUM);
   Json::Value b = 100;
   ASSERT_EQ(jsonNum->validate(&b), JVAL_ERR_INVALID_MINIMUM);
}

TEST(NumberPrimitive, ValidMaximum)
{
   Json::Value schema;
   schema["type"] = "number";
   schema["maximum"] = 100;
   JsonPrimitive *jsonNum = NULL;
   ASSERT_NO_THROW((jsonNum = JsonPrimitive::createPrimitive(&schema)));
   ASSERT_TRUE(jsonNum != NULL);

   Json::Value a = 100;
   ASSERT_EQ(jsonNum->validate(&a), JVAL_ROK);
   Json::Value b = 0;
   ASSERT_EQ(jsonNum->validate(&b), JVAL_ROK);
}

TEST(NumberPrimitive, InvalidMaximum)
{
   Json::Value schema;
   schema["type"] = "number";
   schema["maximum"] = 100;
   JsonPrimitive *jsonNum = NULL;
   ASSERT_NO_THROW((jsonNum = JsonPrimitive::createPrimitive(&schema)));
   ASSERT_TRUE(jsonNum != NULL);

   Json::Value a = 101;
   ASSERT_EQ(jsonNum->validate(&a), JVAL_ERR_INVALID_MAXIMUM);
   Json::Value b = 10000;
   ASSERT_EQ(jsonNum->validate(&b), JVAL_ERR_INVALID_MAXIMUM);
}

TEST(NumberPrimitive, ExlcusiveMaximum)
{
   Json::Value schema;
   schema["type"] = "number";
   schema["maximum"] = 100;
   schema["exclusiveMaximum"] = true;
   JsonPrimitive *jsonNum = NULL;
   ASSERT_NO_THROW((jsonNum = JsonPrimitive::createPrimitive(&schema)));
   ASSERT_TRUE(jsonNum != NULL);

   Json::Value a = 101;
   ASSERT_EQ(jsonNum->validate(&a), JVAL_ERR_INVALID_MAXIMUM);
   Json::Value b = 100;
   ASSERT_EQ(jsonNum->validate(&b), JVAL_ERR_INVALID_MAXIMUM);
}

TEST(NumberPrimitive, ValidMultipleOf)
{
   Json::Value schema;
   schema["type"] = "number";
   schema["multipleOf"] = 12.4;
   JsonPrimitive *jsonNum = NULL;
   ASSERT_NO_THROW((jsonNum = JsonPrimitive::createPrimitive(&schema)));
   ASSERT_TRUE(jsonNum != NULL);

   Json::Value a = 24.8;
   ASSERT_EQ(jsonNum->validate(&a), JVAL_ROK);
   Json::Value b = 124;
   ASSERT_EQ(jsonNum->validate(&b), JVAL_ROK);
}

TEST(NumberPrimitive, InvalidMultipleOf)
{
   Json::Value schema;
   schema["type"] = "number";
   schema["multipleOf"] = 5;
   JsonPrimitive *jsonNum = NULL;
   ASSERT_NO_THROW((jsonNum = JsonPrimitive::createPrimitive(&schema)));
   ASSERT_TRUE(jsonNum != NULL);

   Json::Value a = 12;
   ASSERT_EQ(jsonNum->validate(&a), JVAL_ERR_NOT_A_MULTIPLE);
   Json::Value b = 21;
   ASSERT_EQ(jsonNum->validate(&b), JVAL_ERR_NOT_A_MULTIPLE);
}

TEST(StringPrimitive, InvalidString)
{
   Json::Value schema;
   schema["type"] = "string";
   JsonPrimitive *jsonStr = NULL;
   ASSERT_NO_THROW((jsonStr = JsonPrimitive::createPrimitive(&schema)));
   ASSERT_TRUE(jsonStr != NULL);

   Json::Value a = 12;
   ASSERT_EQ(jsonStr->validate(&a), JVAL_ERR_NOT_A_STRING);
   Json::Value b = true;
   ASSERT_EQ(jsonStr->validate(&b), JVAL_ERR_NOT_A_STRING);
}

TEST(StringPrimitive, ValidString)
{
   Json::Value schema;
   schema["type"] = "string";
   JsonPrimitive *jsonStr = NULL;
   ASSERT_NO_THROW((jsonStr = JsonPrimitive::createPrimitive(&schema)));
   ASSERT_TRUE(jsonStr != NULL);

   Json::Value a = "12";
   ASSERT_EQ(jsonStr->validate(&a), JVAL_ROK);
   Json::Value b = "true";
   ASSERT_EQ(jsonStr->validate(&b), JVAL_ROK);
}

TEST(StringPrimitive, InvalidMinLength)
{
   Json::Value schema;
   schema["type"] = "string";
   schema["minLength"] = 5;
   JsonPrimitive *jsonStr = NULL;
   ASSERT_NO_THROW((jsonStr = JsonPrimitive::createPrimitive(&schema)));
   ASSERT_TRUE(jsonStr != NULL);

   Json::Value a = "1234";
   ASSERT_EQ(jsonStr->validate(&a), JVAL_ERR_INVALID_MIN_LENGTH);
   Json::Value b = "";
   ASSERT_EQ(jsonStr->validate(&b), JVAL_ERR_INVALID_MIN_LENGTH);
}

TEST(StringPrimitive, ValidMinLength)
{
   Json::Value schema;
   schema["type"] = "string";
   schema["minLength"] = 5;
   JsonPrimitive *jsonStr = NULL;
   ASSERT_NO_THROW((jsonStr = JsonPrimitive::createPrimitive(&schema)));
   ASSERT_TRUE(jsonStr != NULL);

   Json::Value a = "12345";
   ASSERT_EQ(jsonStr->validate(&a), JVAL_ROK);
   Json::Value b = "123456789";
   ASSERT_EQ(jsonStr->validate(&b), JVAL_ROK);
}

TEST(StringPrimitive, InvalidMaxLength)
{
   Json::Value schema;
   schema["type"] = "string";
   schema["maxLength"] = 5;
   JsonPrimitive *jsonStr = NULL;
   ASSERT_NO_THROW((jsonStr = JsonPrimitive::createPrimitive(&schema)));
   ASSERT_TRUE(jsonStr != NULL);

   Json::Value a = "123456";
   ASSERT_EQ(jsonStr->validate(&a), JVAL_ERR_INVALID_MAX_LENGTH);
   Json::Value b = "1234456789";
   ASSERT_EQ(jsonStr->validate(&b), JVAL_ERR_INVALID_MAX_LENGTH);
}

TEST(StringPrimitive, ValidMaxLength)
{
   Json::Value schema;
   schema["type"] = "string";
   schema["maxLength"] = 5;
   JsonPrimitive *jsonStr = NULL;
   ASSERT_NO_THROW((jsonStr = JsonPrimitive::createPrimitive(&schema)));
   ASSERT_TRUE(jsonStr != NULL);

   Json::Value a = "12345";
   ASSERT_EQ(jsonStr->validate(&a), JVAL_ROK);
   Json::Value b = "1";
   ASSERT_EQ(jsonStr->validate(&b), JVAL_ROK);
   Json::Value c = "";
   ASSERT_EQ(jsonStr->validate(&c), JVAL_ROK);
}

TEST(StringPrimitive, ValidRegex)
{
   Json::Value schema;
   schema["type"] = "string";
   schema["pattern"] = ".*";
   JsonPrimitive *jsonStr = NULL;
   ASSERT_NO_THROW((jsonStr = JsonPrimitive::createPrimitive(&schema)));
   ASSERT_TRUE(jsonStr != NULL);

   Json::Value a = "abcd";
   ASSERT_EQ(jsonStr->validate(&a), JVAL_ROK);
   Json::Value b = "1234";
   ASSERT_EQ(jsonStr->validate(&b), JVAL_ROK);
}

TEST(StringPrimitive, InvalidRegex)
{
   Json::Value schema;
   schema["type"] = "string";
   schema["pattern"] = "abcd";
   JsonPrimitive *jsonStr = NULL;
   ASSERT_NO_THROW((jsonStr = JsonPrimitive::createPrimitive(&schema)));
   ASSERT_TRUE(jsonStr != NULL);

   Json::Value a = "abcde";
   ASSERT_EQ(jsonStr->validate(&a), JVAL_ERR_PATTERN_MISMATCH);
   Json::Value b = "abc";
   ASSERT_EQ(jsonStr->validate(&b), JVAL_ERR_PATTERN_MISMATCH);
}

TEST(ArrayPrimitive, ValidArray)
{
   Json::Value schema;
   schema["type"] = "array";
   JsonPrimitive *jsonArr = NULL;
   ASSERT_NO_THROW((jsonArr = JsonPrimitive::createPrimitive(&schema)));
   ASSERT_TRUE(jsonArr != NULL);

   Json::Value a;
   a[0] = 0;
   a[1] = 1;
   ASSERT_EQ(jsonArr->validate(&a), JVAL_ROK);
   Json::Value b;
   b[0] = "abc";
   b[1] = "dcd";
   ASSERT_EQ(jsonArr->validate(&b), JVAL_ROK);
}

TEST(ArrayPrimitive, ValidMinItems)
{
   Json::Value schema;
   schema["type"] = "array";
   schema["minItems"] = 2;
   JsonPrimitive *jsonArr = NULL;
   ASSERT_NO_THROW((jsonArr = JsonPrimitive::createPrimitive(&schema)));
   ASSERT_TRUE(jsonArr != NULL);

   Json::Value a;
   a[0] = 0;
   a[1] = 1;
   ASSERT_EQ(jsonArr->validate(&a), JVAL_ROK);
   Json::Value b;
   b[0] = "abc";
   b[1] = "dcd";
   b[2] = 3;
   ASSERT_EQ(jsonArr->validate(&b), JVAL_ROK);
}

TEST(ArrayPrimitive, InvalidMinItems)
{
   Json::Value schema;
   schema["type"] = "array";
   schema["minItems"] = 3;
   JsonPrimitive *jsonArr = NULL;
   ASSERT_NO_THROW((jsonArr = JsonPrimitive::createPrimitive(&schema)));
   ASSERT_TRUE(jsonArr != NULL);

   Json::Value a;
   a[0] = 0;
   ASSERT_EQ(jsonArr->validate(&a), JVAL_ERR_INVALID_MIN_ITEMS);
   Json::Value b;
   b[0] = "abc";
   b[1] = "dcd";
   ASSERT_EQ(jsonArr->validate(&b), JVAL_ERR_INVALID_MIN_ITEMS);
}

TEST(ArrayPrimitive, ValidMaxItems)
{
   Json::Value schema;
   schema["type"] = "array";
   schema["maxItems"] = 2;
   JsonPrimitive *jsonArr = NULL;
   ASSERT_NO_THROW((jsonArr = JsonPrimitive::createPrimitive(&schema)));
   ASSERT_TRUE(jsonArr != NULL);

   Json::Value a;
   a[0] = 0;
   a[1] = 1;
   ASSERT_EQ(jsonArr->validate(&a), JVAL_ROK);
   Json::Value b;
   b[0] = "abc";
   ASSERT_EQ(jsonArr->validate(&b), JVAL_ROK);
}

TEST(ArrayPrimitive, InvalidMaxItems)
{
   Json::Value schema;
   schema["type"] = "array";
   schema["maxItems"] = 2;
   JsonPrimitive *jsonArr = NULL;
   ASSERT_NO_THROW((jsonArr = JsonPrimitive::createPrimitive(&schema)));
   ASSERT_TRUE(jsonArr != NULL);

   Json::Value a;
   a[0] = 0;
   a[1] = 1;
   a[2] = 2;
   ASSERT_EQ(jsonArr->validate(&a), JVAL_ERR_INVALID_MAX_ITEMS);
   Json::Value b;
   b[0] = "abc";
   b[1] = "dcd";
   b[2] = 3;
   ASSERT_EQ(jsonArr->validate(&b), JVAL_ERR_INVALID_MAX_ITEMS);
}

TEST(ArrayPrimitive, ItemsTuple)
{
   Json::Value schema;
   schema["type"] = "array";
   schema["items"][0]["type"] = "string";
   schema["items"][1]["type"] = "string";
   schema["items"][2]["type"] = "integer";
   JsonPrimitive *jsonArr = NULL;
   ASSERT_NO_THROW((jsonArr = JsonPrimitive::createPrimitive(&schema)));
   ASSERT_TRUE(jsonArr != NULL);

   Json::Value a;
   a[0] = "a";
   a[1] = "b";
   a[2] = 1;
   ASSERT_EQ(jsonArr->validate(&a), JVAL_ROK);
}

TEST(ArrayPrimitive, AdditionalItemsOk)
{
   Json::Value schema;
   schema["type"] = "array";
   schema["items"][0]["type"] = "string";
   schema["items"][1]["type"] = "string";
   schema["items"][2]["type"] = "integer";
   schema["additionalItems"] = true;
   JsonPrimitive *jsonArr = NULL;
   ASSERT_NO_THROW((jsonArr = JsonPrimitive::createPrimitive(&schema)));
   ASSERT_TRUE(jsonArr != NULL);

   Json::Value a;
   a[0] = "a";
   a[1] = "b";
   a[2] = 1;
   a[3] = 4;
   ASSERT_EQ(jsonArr->validate(&a), JVAL_ROK);
}

TEST(ArrayPrimitive, AdditionalItemsNotOk)
{
   Json::Value schema;
   schema["type"] = "array";
   schema["items"][0]["type"] = "string";
   schema["items"][1]["type"] = "string";
   schema["items"][2]["type"] = "integer";
   schema["additionalItems"] = false;
   JsonPrimitive *jsonArr = NULL;
   ASSERT_NO_THROW((jsonArr = JsonPrimitive::createPrimitive(&schema)));
   ASSERT_TRUE(jsonArr != NULL);

   Json::Value a;
   a[0] = "a";
   a[1] = "b";
   a[2] = 1;
   a[3] = 4;
   ASSERT_EQ(jsonArr->validate(&a), JVAL_ERR_ADDITIONAL_ITEMS);
}

TEST(ArrayPrimitive, ItemsList)
{
   Json::Value schema;
   schema["type"] = "array";
   schema["items"]["type"] = "integer";
   JsonPrimitive *jsonArr = NULL;
   ASSERT_NO_THROW((jsonArr = JsonPrimitive::createPrimitive(&schema)));
   ASSERT_TRUE(jsonArr != NULL);

   Json::Value a;
   a[0] = 1;
   a[1] = 2;
   a[2] = 100;
   ASSERT_EQ(jsonArr->validate(&a), JVAL_ROK);
}

TEST(ArrayPrimitive, ValidUniqueItems)
{
   Json::Value schema;
   schema["type"] = "array";
   schema["items"]["type"] = "integer";
   schema["uniqueItems"] = true;
   JsonPrimitive *jsonArr = NULL;
   ASSERT_NO_THROW((jsonArr = JsonPrimitive::createPrimitive(&schema)));
   ASSERT_TRUE(jsonArr != NULL);

   Json::Value a;
   a[0] = 1;
   a[1] = 2;
   a[2] = 100;
   ASSERT_EQ(jsonArr->validate(&a), JVAL_ROK);
}

TEST(ArrayPrimitive, InvalidUniqueItems)
{
   Json::Value schema;
   schema["type"] = "array";
   schema["items"]["type"] = "integer";
   schema["uniqueItems"] = true;
   JsonPrimitive *jsonArr = NULL;
   ASSERT_NO_THROW((jsonArr = JsonPrimitive::createPrimitive(&schema)));
   ASSERT_TRUE(jsonArr != NULL);

   Json::Value a;
   a[0] = 1;
   a[1] = 2;
   a[2] = 2;
   ASSERT_EQ(jsonArr->validate(&a), JVAL_ERR_DUPLICATE_ITEMS);
}

TEST(ObjectPrimitive, ValidObject)
{
   Json::Value schema;
   schema["type"] = "object";
   JsonPrimitive *jsonObj = NULL;
   ASSERT_NO_THROW((jsonObj = JsonPrimitive::createPrimitive(&schema)));
   ASSERT_TRUE(jsonObj != NULL);

   Json::Value a;
   a["id"] = 0;
   ASSERT_EQ(jsonObj->validate(&a), JVAL_ROK);
}

TEST(ObjectPrimitive, InvalidObject)
{
   Json::Value schema;
   schema["type"] = "object";
   JsonPrimitive *jsonObj = NULL;
   ASSERT_NO_THROW((jsonObj = JsonPrimitive::createPrimitive(&schema)));
   ASSERT_TRUE(jsonObj != NULL);

   Json::Value a;
   a[0] = 0;
   ASSERT_EQ(jsonObj->validate(&a), JVAL_ERR_NOT_AN_OBJECT);
   Json::Value b = 0;
   ASSERT_EQ(jsonObj->validate(&b), JVAL_ERR_NOT_AN_OBJECT);
}

TEST(ObjectPrimitive, ValidMaxProperties)
{
   Json::Value schema;
   schema["type"] = "object";
   schema["maxProperties"] = 2;
   JsonPrimitive *jsonObj = NULL;
   ASSERT_NO_THROW((jsonObj = JsonPrimitive::createPrimitive(&schema)));
   ASSERT_TRUE(jsonObj != NULL);

   Json::Value a;
   a["id"] = 0;
   ASSERT_EQ(jsonObj->validate(&a), JVAL_ROK);
}

TEST(ObjectPrimitive, InvalidMaxProperties)
{
   Json::Value schema;
   schema["type"] = "object";
   schema["maxProperties"] = 1;
   JsonPrimitive *jsonObj = NULL;
   ASSERT_NO_THROW((jsonObj = JsonPrimitive::createPrimitive(&schema)));
   ASSERT_TRUE(jsonObj != NULL);

   Json::Value a;
   a["id1"] = 0;
   a["id2"] = 0;
   ASSERT_EQ(jsonObj->validate(&a), JVAL_ERR_INVALID_MAX_PROPERTIES);
}

TEST(ObjectPrimitive, ValidMinProperties)
{
   Json::Value schema;
   schema["type"] = "object";
   schema["minProperties"] = 2;
   JsonPrimitive *jsonObj = NULL;
   ASSERT_NO_THROW((jsonObj = JsonPrimitive::createPrimitive(&schema)));
   ASSERT_TRUE(jsonObj != NULL);

   Json::Value a;
   a["id1"] = 0;
   a["id2"] = 0;
   ASSERT_EQ(jsonObj->validate(&a), JVAL_ROK);
}

TEST(ObjectPrimitive, InvalidMinProperties)
{
   Json::Value schema;
   schema["type"] = "object";
   schema["minProperties"] = 2;
   JsonPrimitive *jsonObj = NULL;
   ASSERT_NO_THROW((jsonObj = JsonPrimitive::createPrimitive(&schema)));
   ASSERT_TRUE(jsonObj != NULL);

   Json::Value a;
   a["id1"] = 0;
   ASSERT_EQ(jsonObj->validate(&a), JVAL_ERR_INVALID_MIN_PROPERTIES);
}

TEST(ObjectPrimitive, InvalidRequiredProperties)
{
   Json::Value schema;
   schema["type"] = "object";
   schema["required"][0] = "id1";
   schema["required"][1] = "id2";
   JsonPrimitive *jsonObj = NULL;
   ASSERT_NO_THROW((jsonObj = JsonPrimitive::createPrimitive(&schema)));
   ASSERT_TRUE(jsonObj != NULL);

   Json::Value a;
   a["id1"] = 0;
   ASSERT_EQ(jsonObj->validate(&a), JVAL_ERR_REQUIRED_ITEM_MISSING);
}

TEST(ObjectPrimitive, ValidRequiredProperties)
{
   Json::Value schema;
   schema["type"] = "object";
   schema["required"][0] = "id1";
   schema["required"][1] = "id2";
   JsonPrimitive *jsonObj = NULL;
   ASSERT_NO_THROW((jsonObj = JsonPrimitive::createPrimitive(&schema)));
   ASSERT_TRUE(jsonObj != NULL);

   Json::Value a;
   a["id1"] = 0;
   a["id2"] = 0;
   ASSERT_EQ(jsonObj->validate(&a), JVAL_ROK);

   Json::Value b;
   b["id1"] = 0;
   b["id2"] = 0;
   b["id3"] = 0;
   ASSERT_EQ(jsonObj->validate(&b), JVAL_ROK);
}

TEST(ObjectPrimitive, ValidProperties)
{
   Json::Value schema;
   schema["type"] = "object";
   schema["properties"]["id"]["type"] = "integer";
   schema["properties"]["name"]["type"] = "string";
   schema["properties"]["present"]["type"] = "boolean";
   JsonPrimitive *jsonObj = NULL;
   ASSERT_NO_THROW((jsonObj = JsonPrimitive::createPrimitive(&schema)));
   ASSERT_TRUE(jsonObj != NULL);

   Json::Value a;
   a["id"] = 1;
   a["name"] = "abcd";
   a["present"] = true;
   ASSERT_EQ(jsonObj->validate(&a), JVAL_ROK);
}
