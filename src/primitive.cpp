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

JsonBoolean::JsonBoolean(Json::Value *element) : JsonPrimitive(element)
{
}

int JsonBoolean::validate(const Json::Value *value)
{
   (void)value;
   return JVAL_ROK;
}

JsonNull::JsonNull(Json::Value *element) : JsonPrimitive(element)
{
}

int JsonNull::validate(const Json::Value *value)
{
   (void)value;
   return JVAL_ROK;
}

JsonInteger::JsonInteger(Json::Value *schema) : JsonPrimitive(schema)
{
   // validator for integer type
   m_validators.push_back(new IntValid);

   // validator for minimum check
   if (schema->isMember("minimum")) {
      Json::Value min = schema->get("minimum", min);

      bool exclusiveMin = false;
      if (schema->isMember("exclusiveMinimum")) {
         Json::Value eMin = schema->get("exclusiveMinimum", eMin);
         exclusiveMin = eMin.asBool();
      }

      IntMinimum *minimum = new IntMinimum(min.asInt(), exclusiveMin);
      m_validators.push_back(minimum);
   }

   // validator for maximum check
   if (schema->isMember("maximum")) {
      Json::Value max = schema->get("maximum", max);

      bool exclusiveMax = false;
      if (schema->isMember("exclusiveMaximum")) {
         Json::Value eMax = schema->get("exclusiveMaximum", eMax);
         exclusiveMax = eMax.asBool();
      }

      NumberMaximum *maximum = new NumberMaximum(max.asInt(), exclusiveMax);
      m_validators.push_back(maximum);
   }

   // validator for multipleOf
   if (schema->isMember("multipleOf")) {
      Json::Value multipleOf = schema->get("multipleOf", multipleOf);
      m_validators.push_back(new IntMultipleOf(multipleOf.asInt()));
   }
}

int JsonInteger::validate(const Json::Value *value)
{
   for (std::list<KeywordValidator*>::iterator b = m_validators.begin();
         b != m_validators.end();
         b++) {

      int ret = (*b)->validate(value);
      if (JVAL_ROK != ret) {
         return ret;
      }
   }

   return JVAL_ROK;
}

JsonNumber::JsonNumber(Json::Value *schema) : JsonPrimitive(schema)
{
   // validator for integer type
   m_validators.push_back(new NumberValid);

   // validator for minimum check
   if (schema->isMember("minimum")) {
      Json::Value min = schema->get("minimum", min);

      bool exclusiveMin = false;
      if (schema->isMember("exclusiveMinimum")) {
         Json::Value eMin = schema->get("exclusiveMinimum", eMin);
         exclusiveMin = eMin.asBool();
      }

      NumberMinimum *minimum = new NumberMinimum(min.asInt(), exclusiveMin);
      m_validators.push_back(minimum);
   }

   // validator for maximum check
   if (schema->isMember("maximum")) {
      Json::Value max = schema->get("maximum", max);

      bool exclusiveMax = false;
      if (schema->isMember("exclusiveMaximum")) {
         Json::Value eMax = schema->get("exclusiveMaximum", eMax);
         exclusiveMax = eMax.asBool();
      }

      IntMaximum *maximum = new IntMaximum(max.asInt(), exclusiveMax);
      m_validators.push_back(maximum);
   }

   // validator for multipleOf
   if (schema->isMember("multipleOf")) {
      Json::Value multipleOf = schema->get("multipleOf", multipleOf);
      m_validators.push_back(new NumberMultipleOf(multipleOf.asDouble()));
   }
}

int JsonNumber::validate(const Json::Value *value)
{
   for (std::list<KeywordValidator*>::iterator b = m_validators.begin();
         b != m_validators.end();
         b++) {

      int ret = (*b)->validate(value);
      if (JVAL_ROK != ret) {
         return ret;
      }
   }

   return JVAL_ROK;
}

JsonString::JsonString(Json::Value *schema) : JsonPrimitive(schema)
{
   m_validators.push_back(new StringValid);

   // checking length constraints
   if (schema->isMember("minLength")) {
      Json::Value min = schema->get("minLength", min);
      size_t length = min.asUInt();
      m_validators.push_back(new MinLength(length));
   }

   if (schema->isMember("maxLength")) {
      Json::Value max = schema->get("maxLength", max);
      size_t length = max.asUInt();
      m_validators.push_back(new MaxLength(length));
   }

   // string pattern
   if (schema->isMember("pattern")) {
      Json::Value pattern = schema->get("pattern", pattern);
      m_validators.push_back(new Pattern(pattern.asString()));
   }
}

int JsonString::validate(const Json::Value *value)
{
   for (std::list<KeywordValidator*>::iterator b = m_validators.begin();
         b != m_validators.end();
         b++) {

      int ret = (*b)->validate(value);
      if (JVAL_ROK != ret) {
         return ret;
      }
   }

   return JVAL_ROK;
}

JsonArray::JsonArray(Json::Value *schema) : JsonPrimitive(schema)
{
   m_validators.push_back(new ArrayValid);

   // checking minimum number of items constraint
   if (schema->isMember("minItems")) {
      Json::Value min = schema->get("minItems", min);
      m_validators.push_back(new MinItems(min.asUInt()));
   }

   // checking maximum number of items constraint
   if (schema->isMember("maxItems")) {
      Json::Value max = schema->get("maxItems", max);
      m_validators.push_back(new MaxItems(max.asUInt()));
   }

   // checking maximum number of items constraint
   if (schema->isMember("uniqueItems")) {
      Json::Value unique = schema->get("uniqueItems", unique);
      m_validators.push_back(new UniqueItems(unique.asBool()));
   }

   // items
   if (schema->isMember("items")) {
      Json::Value items = schema->get("items", items);
      if (items.isArray())
      {
         // if items in the json array is of tuple type then additionalitems
         // keyword will be used for the validation
         if (schema->isMember("additionalItems")) {
            Json::Value ai = schema->get("additionalItems", ai);

            // if additinalItems can be boolean or object. If it is an object
            // the validation of tuple always succeeds
            // the Only time additionalitems is useful when the array is tuple
            // and additionalitems == false
            if (ai.type() == Json::booleanValue && \
                  ai.asBool() == false) {
               m_validators.push_back(new AdditionalItems(items.size()));
            }
         }

         m_validators.push_back(new ItemsTuple(items));
      }
      else if (items.isObject())
      {
         // ignore the additionalitems keyword even if present in the schema
         m_validators.push_back(new ItemsList(items));
      }
   }
}

int JsonArray::validate(const Json::Value *value)
{
   for (std::list<KeywordValidator*>::iterator b = m_validators.begin();
         b != m_validators.end();
         b++) {

      int ret = (*b)->validate(value);
      if (JVAL_ROK != ret) {
         return ret;
      }
   }

   return JVAL_ROK;
}

JsonObject::JsonObject(Json::Value *schema) : JsonPrimitive(schema)
{
   m_validators.push_back(new ObjectValid);

   // checking minimum number of properties constraint
   if (schema->isMember("minProperties")) {
      Json::Value min = schema->get("minProperties", min);
      m_validators.push_back(new MinProperties(min.asUInt()));
   }

   // checking maximum number of properties constraint
   if (schema->isMember("maxProperties")) {
      Json::Value max = schema->get("maxProperties", max);
      m_validators.push_back(new MaxProperties(max.asUInt()));
   }

   // required properties
   if (schema->isMember("required")) {
      Json::Value req = schema->get("required", req);
      m_validators.push_back(new Required(req));
   }

   // properties
   if (schema->isMember("properties")) {
      Json::Value properties = schema->get("properties", properties);

      bool additionalProperties = false;
      if (schema->isMember("additionalProperties")) {
         Json::Value v = schema->get("additionalProperties", v);
         additionalProperties = v.asBool(); 
      }

      m_validators.push_back(new Properties(properties, additionalProperties));
   }
}

int JsonObject::validate(const Json::Value *value)
{
   for (std::list<KeywordValidator*>::iterator b = m_validators.begin();
         b != m_validators.end();
         b++) {

      int ret = (*b)->validate(value);
      if (JVAL_ROK != ret) {
         return ret;
      }
   }

   return JVAL_ROK;
}


