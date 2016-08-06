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

#include <string>
#include <fstream>
#include <streambuf>
#include <iostream>
#include <sstream>
#include <vector>
#include <list>
#include <stdexcept>
#include <algorithm>
#include <regex>
#include <json.h>
#include <primitive_base.h>
#include <keyword_validator.h>
#include <primitive.h>
#include <validator.h>

JsonValidator::JsonValidator()
{
   m_primitive = NULL;
}

JsonValidator::JsonValidator(std::string &schema)
{
   m_primitive = NULL;
   parseSchema(schema);
}

JsonValidator::JsonValidator(Json::Value *schema)
{
   m_primitive = NULL;
   m_primitive = JsonPrimitive::createPrimitive(schema);
}

JsonValidator::~JsonValidator()
{
   delete m_primitive;
}

void JsonValidator::readSchema(const char *schema_file)
{
   std::ifstream t(schema_file);

   // reserver memory for entire schema instead of string class automatic
   // reallocation
   std::string str;
   t.seekg(0, std::ios::end);   
   str.reserve(t.tellg());
   t.seekg(0, std::ios::beg);

   str.assign((std::istreambuf_iterator<char>(t)),
         std::istreambuf_iterator<char>());

   parseSchema(str);
}

void JsonValidator::parseSchema(std::string &str)
{
   Json::Reader   reader;
   Json::Value    schema;
   bool parsingSuccessful = reader.parse(str.c_str(), schema);
   if (!parsingSuccessful) {
      throw Exception(reader.getFormattedErrorMessages());
   }

   m_primitive = JsonPrimitive::createPrimitive(&schema);
}

int JsonValidator::validate(const Json::Value *value)
{
   int ret = JVAL_ROK;

   try {
      ret = m_primitive->validate(value);
   } catch (Exception &e) {
      std::cout << e.what() << std::endl;
   }

   return ret;
}

/**
 * @brief Creates the primitive type based on the schema
 *
 * @param schema contains schema of the form
 *    {
 *       "type" : "integer"
 *       ...
 *       ...
 *    }
 *
 * @return 
 */
JsonPrimitive *JsonPrimitive::createPrimitive(Json::Value *schema)
{
   JsonPrimitiveType type = JsonPrimitive::getPrimitveType(schema);

   switch (type) {
      case JSON_TYPE_INTEGER:
         return new JsonInteger(schema);
      case JSON_TYPE_NUMBER:
         return new JsonNumber(schema);
      case JSON_TYPE_STRING:
         return new JsonString(schema);
      case JSON_TYPE_OBJECT:
         return new JsonObject(schema);
      case JSON_TYPE_ARRAY:
         return new JsonArray(schema);
      case JSON_TYPE_BOOLEAN:
         return new JsonBoolean(schema);
      case JSON_TYPE_NULL:
         return new JsonNull(schema);
      default:
         throw Exception("Invalid Schema");
   }
}

/**
 * @brief Converts type-specific json schema keywords into enum values
 *
 * @param schema contains schema of the form
 *    {
 *       "type" : "integer"
 *       ...
 *       ...
 *    }
 *
 * @return 
 */
JsonPrimitiveType JsonPrimitive::getPrimitveType(Json::Value *schema)
{
   if (schema->isMember("type")) {
      Json::Value typeValue = schema->get("type", typeValue);

      if (typeValue.asString() == "integer") {
         return JSON_TYPE_INTEGER;
      } else if (typeValue.asString() == "number") {
         return JSON_TYPE_NUMBER;
      } else if (typeValue.asString() == "string") {
         return JSON_TYPE_STRING;
      } else if (typeValue.asString() == "object") {
         return JSON_TYPE_OBJECT;
      } else if (typeValue.asString() == "array") {
         return JSON_TYPE_ARRAY;
      } else if (typeValue.asString() == "boolean") {
         return JSON_TYPE_BOOLEAN;
      } else if (typeValue.asString() == "null") {
         return JSON_TYPE_NULL;
      } else {
         std::string err = "Unknown type keyword \"" + \
                            typeValue.asString() + "\"";
         throw Exception(err);
      }
   } else {
      throw Exception("\"type\" keyword is undefined");
   }
}


