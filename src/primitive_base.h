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

#ifndef __PRIMITIVE_BASE_H__
#define __PRIMITIVE_BASE_H__

#define JVAL_ROK                            0
#define JVAL_ERR_INVALID_MAXIMUM            1
#define JVAL_ERR_INVALID_MINIMUM            2
#define JVAL_ERR_NOT_A_MULTIPLE             3
#define JVAL_ERR_INVALID_MIN_LENGTH         4
#define JVAL_ERR_INVALID_MAX_LENGTH         5
#define JVAL_ERR_PATTERN_MISMATCH           6
#define JVAL_ERR_INVALID_MAX_ITEMS          7
#define JVAL_ERR_INVALID_MIN_ITEMS          8
#define JVAL_ERR_DUPLICATE_ITEMS            9
#define JVAL_ERR_INVALID_MAX_PROPERTIES     10
#define JVAL_ERR_INVALID_MIN_PROPERTIES     11
#define JVAL_ERR_REQUIRED_ITEM_MISSING      12
#define JVAL_ERR_INVALID_ARRAY_ITEM         13
#define JVAL_ERR_UNKNOWN_PROPERTY           15
#define JVAL_ERR_INVALID_SCHEMA             16
#define JVAL_ERR_NOT_AN_INTEGER             17
#define JVAL_ERR_NOT_A_NUMBER               18
#define JVAL_ERR_NOT_A_STRING               19
#define JVAL_ERR_NOT_AN_ARRAY               20
#define JVAL_ERR_NOT_AN_OBJECT              21
#define JVAL_ERR_INVALID_PROPERTY           22
#define JVAL_ERR_ADDITIONAL_ITEMS           23

typedef enum
{
   JSON_TYPE_INVALID = 0,
   JSON_TYPE_ARRAY,
   JSON_TYPE_BOOLEAN,
   JSON_TYPE_ENUM,
   JSON_TYPE_INTEGER,
   JSON_TYPE_NULL,
   JSON_TYPE_NUMBER,
   JSON_TYPE_OBJECT,
   JSON_TYPE_STRING

} JsonPrimitiveType;

class Exception
{
   public:
      Exception(std::string const& msg) {
         m_msg = msg;
      }

      ~Exception() {}

      char const* what() {
         return m_msg.c_str();
      }

   private:
      std::string m_msg;
};

class JsonPrimitive
{
   private:
      JsonPrimitiveType   m_type;

   protected:
      JsonPrimitiveType type() {return m_type;}

   public:
      JsonPrimitive(Json::Value *element) {
         m_type = getPrimitveType(element);
      }

      virtual ~JsonPrimitive() {}

      virtual int validate(const Json::Value *value) = 0;

      static JsonPrimitiveType getPrimitveType(Json::Value *value);

      // factory method for creating type specific element validator
      static JsonPrimitive *createPrimitive(Json::Value *elment);
};

#endif
