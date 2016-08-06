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
   JsonPrimitive *p = NULL;

   Json::Value a;
   a["type"] = "integer";
   ASSERT_NO_THROW(p = JsonPrimitive::createPrimitive(&a));
   delete p;
   ASSERT_TRUE((p = JsonPrimitive::createPrimitive(&a)) != NULL);
   delete p;

   Json::Value b;
   b["type"] = "boolean";
   ASSERT_NO_THROW(p = JsonPrimitive::createPrimitive(&b));
   delete p;
   ASSERT_TRUE((p = JsonPrimitive::createPrimitive(&b)) != NULL);
   delete p;

   Json::Value c;
   c["type"] = "string";
   ASSERT_NO_THROW(p = JsonPrimitive::createPrimitive(&c));
   delete p;
   ASSERT_TRUE((p = JsonPrimitive::createPrimitive(&c)) != NULL);
   delete p;

   Json::Value d;
   d["type"] = "null";
   ASSERT_NO_THROW(p = JsonPrimitive::createPrimitive(&d));
   delete p;
   ASSERT_TRUE((p = JsonPrimitive::createPrimitive(&d)) != NULL);
   delete p;
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
   delete jsonInt;
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
   delete jsonInt;
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
   delete jsonInt;
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
   delete jsonInt;
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
   delete jsonInt;
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
   delete jsonInt;
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
   delete jsonInt;
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
   delete jsonInt;
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
   delete jsonInt;
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
   delete jsonInt;
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
   delete jsonNum;
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
   delete jsonNum;
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
   delete jsonNum;
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
   delete jsonNum;
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
   delete jsonNum;
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
   delete jsonNum;
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
   delete jsonNum;
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
   delete jsonNum;
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
   delete jsonNum;
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
   delete jsonNum;
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
   delete jsonStr;
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
   delete jsonStr;
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
   delete jsonStr;
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
   delete jsonStr;
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
   delete jsonStr;
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
   delete jsonStr;
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
   delete jsonStr;
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
   delete jsonStr;
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
   delete jsonArr;
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
   delete jsonArr;
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
   delete jsonArr;
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
   delete jsonArr;
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
   delete jsonArr;
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
   delete jsonArr;
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
   delete jsonArr;
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
   delete jsonArr;
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
   delete jsonArr;
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
   delete jsonArr;
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
   delete jsonArr;
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
   delete jsonObj;
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
   delete jsonObj;
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
   delete jsonObj;
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
   delete jsonObj;
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
   delete jsonObj;
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
   delete jsonObj;
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
   delete jsonObj;
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
   delete jsonObj;
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
   delete jsonObj;
}
