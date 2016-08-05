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

      ~JsonPrimitive() {}

      virtual int validate(const Json::Value *value) = 0;

      static JsonPrimitiveType getPrimitveType(Json::Value *value);

      // factory method for creating type specific element validator
      static JsonPrimitive *createPrimitive(Json::Value *elment);
};

#endif
