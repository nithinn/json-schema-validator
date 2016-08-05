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

#include <iostream>
#include <string>
#include <cmath>
#include <limits>
#include <list>
#include <vector>
#include <map>
#include <algorithm>
#include <regex>
#include <json.h>
#include <primitive_base.h>
#include <keyword_validator.h>

inline bool almostEqual(double a, double b, double errorFactor = 2.0)
{
   return (a == b) || std::abs(a - b) < std::abs(std::min(a, b)) * \
             std::numeric_limits<double>::epsilon() * errorFactor;
}

int IntValid::validate(const Json::Value *value)
{
   if (!value->isInt()) {
      return JVAL_ERR_NOT_AN_INTEGER;
   }

   return JVAL_ROK;
}

IntMaximum::IntMaximum(int maximum, bool exclusiveMaximum = false)
{
   m_maximum = maximum;
   m_exclusiveMaximum = exclusiveMaximum;
}

int IntMaximum::validate(const Json::Value *value)
{
   Json::Int val = value->asInt();   
   if (val > m_maximum) {
      return JVAL_ERR_INVALID_MAXIMUM;
   }

   if (val == m_maximum && m_exclusiveMaximum == true) {
      return JVAL_ERR_INVALID_MAXIMUM;
   }

   return JVAL_ROK;
}

IntMinimum::IntMinimum(int minimum, bool exclusiveMinimum = false)
{
   m_minimum = minimum;
   m_exclusiveMinimum = exclusiveMinimum;
}

int IntMinimum::validate(const Json::Value *value)
{
   Json::Int val = value->asInt();   
   if (val < m_minimum) {
      return JVAL_ERR_INVALID_MINIMUM;
   }

   if (val == m_minimum && m_exclusiveMinimum == true) {
      return JVAL_ERR_INVALID_MINIMUM;
   }

   return JVAL_ROK;
}

int NumberValid::validate(const Json::Value *value)
{
   if (!value->isNumeric()) {
      return JVAL_ERR_NOT_A_NUMBER;
   }
 
   return JVAL_ROK;
}

NumberMaximum::NumberMaximum(double maximum, bool exclusiveMaximum = false)
{
   m_maximum = maximum;
   m_exclusiveMaximum = exclusiveMaximum;
}

int NumberMaximum::validate(const Json::Value *value)
{
   double val = value->asDouble();   
   if (val > m_maximum) {
      return JVAL_ERR_INVALID_MAXIMUM;
   }

   if (val == m_maximum && m_exclusiveMaximum == true) {
      return JVAL_ERR_INVALID_MAXIMUM;
   }

   return JVAL_ROK;
}

NumberMinimum::NumberMinimum(double minimum, bool exclusiveMinimum = false)
{
   m_minimum = minimum;
   m_exclusiveMinimum = exclusiveMinimum;
}

int NumberMinimum::validate(const Json::Value *value)
{
   double val = value->asDouble();   
   if (val < m_minimum) {
      return JVAL_ERR_INVALID_MINIMUM;
   }

   if (val == m_minimum && m_exclusiveMinimum == true) {
      return JVAL_ERR_INVALID_MINIMUM;
   }

   return JVAL_ROK;
}

IntMultipleOf::IntMultipleOf(int multipleOf = 1) 
{
   m_multipleOf = multipleOf;
}

int IntMultipleOf::validate(const Json::Value *value)
{
   int val = value->asInt();   
   if ((val % m_multipleOf) != 0) {
      return JVAL_ERR_NOT_A_MULTIPLE;
   }

   return JVAL_ROK;
}

NumberMultipleOf::NumberMultipleOf(double multipleOf) 
{
   m_multipleOf = multipleOf;
}

int NumberMultipleOf::validate(const Json::Value *value)
{
   double val = value->asDouble();
   double remainder = fmod(val, m_multipleOf);

   if (almostEqual(remainder, 0.0) || almostEqual(remainder, m_multipleOf)) {
      return JVAL_ROK;
   }

   return JVAL_ERR_NOT_A_MULTIPLE;
}

int StringValid::validate(const Json::Value *value)
{
   if (!value->isString()) {
      return JVAL_ERR_NOT_A_STRING;
   }

   return JVAL_ROK;
}

MinLength::MinLength(int minLength = 0)
{
   m_minLength = minLength;
}

int MinLength::validate(const Json::Value *value)
{
   if (value->asString().size() < m_minLength) {
      return JVAL_ERR_INVALID_MIN_LENGTH;
   }

   return JVAL_ROK;
}

MaxLength::MaxLength(int maxLength = 0) 
{
   m_maxLength = maxLength;
}

int MaxLength::validate(const Json::Value *value)
{
   if (value->asString().size() > m_maxLength) {
      return JVAL_ERR_INVALID_MAX_LENGTH;
   }

   return JVAL_ROK;
}

Pattern::Pattern(JSONCPP_STRING pattern = ".*") : m_pattern(pattern)
{
}

int Pattern::validate(const Json::Value *value)
{
   if (!std::regex_match(value->asString(), m_pattern)) {
      return JVAL_ERR_PATTERN_MISMATCH;
   }

   return JVAL_ROK;
}

int ArrayValid::validate(const Json::Value *value)
{
   if (!value->isArray()) {
      return JVAL_ERR_NOT_AN_ARRAY;
   }

   return JVAL_ROK;
}

/**
 * @brief Array validation keyword. Constructor
 */
MinItems::MinItems(unsigned int minItems = 0) 
{
   m_minItems = minItems;
}

/**
 * @brief Array validation keyword, validates Minimum number of items present
 * in the list
 *
 * @param schema
 * @param value
 *
 * @return 
 */
int MinItems::validate(const Json::Value *value)
{
   if (value->size() < m_minItems) {
      return JVAL_ERR_INVALID_MIN_ITEMS;
   }

   return JVAL_ROK;
}

/**
 * @brief Array validation keyword. Constructor
 */
MaxItems::MaxItems(unsigned int maxItems = 0) 
{
   m_maxItems = maxItems;
}

/**
 * @brief Array validation keyword, validates maximum number of items present
 * in the list
 *
 * @param schema
 * @param value
 *
 * @return 
 */
int MaxItems::validate(const Json::Value *value)
{
   if (value->size() > m_maxItems) {
      return JVAL_ERR_INVALID_MAX_ITEMS;
   }

   return JVAL_ROK;
}

ItemsTuple::ItemsTuple(Json::Value items)
{
   m_items = items;

   for (unsigned int i = 0; i < items.size(); i++) {
      m_primitives.push_back(JsonPrimitive::createPrimitive(&items[i]));
   }
}

int ItemsTuple::validate(const Json::Value *value)
{
   for (std::size_t i = 0; i < value->size() && i < m_primitives.size(); i++) {
      int ret = m_primitives[i]->validate(&((*value)[i]));
      if (JVAL_ROK != ret) {
         return JVAL_ERR_INVALID_ARRAY_ITEM;
      }
   }

   return JVAL_ROK;
}

ItemsList::ItemsList(Json::Value items)
{
   m_items = items;
   m_primitive = JsonPrimitive::createPrimitive(&items);
}

int ItemsList::validate(const Json::Value *value)
{
   for (std::size_t i = 0; i < value->size(); i++) {
      int ret = m_primitive->validate(&((*value)[i]));
      if (JVAL_ROK != ret) {
         return JVAL_ERR_INVALID_ARRAY_ITEM;
      }
   }

   return JVAL_ROK;
}

/**
 * @brief Array Validation keyword. Constructor
 */
UniqueItems::UniqueItems(bool uniqueItems = false)
{
   m_uniqueItems = uniqueItems;
}

/**
 * @brief Array Validation keyword.
 *    Validates uniquieness in the list items
 *
 * @param schema
 * @param value
 *
 * @return 
 */
int UniqueItems::validate(const Json::Value *value)
{
   std::set<Json::Value> valueSet;

   if (m_uniqueItems) {
      for (std::size_t i = 0; i < value->size(); i++) {
        std::pair<std::set<Json::Value>::iterator, bool> ret = \
            valueSet.insert((*value)[i]);
        if (false == ret.second) {
            return JVAL_ERR_DUPLICATE_ITEMS;
        }
      }
   }

   return JVAL_ROK;
}

/**
 * @brief Array Validation keyword. Constructor
 */
AdditionalItems::AdditionalItems(unsigned int itemsSize)
{
   m_itemsSize = itemsSize;
}

/**
 * @brief Array Validation keyword. Validates additional items in a tuple.
 *
 * @param schema
 * @param value
 *
 * @return 
 */
int AdditionalItems::validate(const Json::Value *value)
{
   // An empty array is always valid
   if (0 == value->size()) {
      return JVAL_ROK;
   }

   if (value->size() > m_itemsSize) {
      return JVAL_ERR_ADDITIONAL_ITEMS;
   }

   return JVAL_ROK;
}

/**
 * @brief Object validation keyword. Constructor
 */
MaxProperties::MaxProperties(unsigned int maxProperties)
{
   m_maxProperties = maxProperties;
}

int ObjectValid::validate(const Json::Value *value)
{
   if (!value->isObject()) {
      return JVAL_ERR_NOT_AN_OBJECT;
   }

   return JVAL_ROK;
}

/**
 * @brief Object validation keyword. Validates maximum number of properties of
 * an object
 *
 * @param schema
 * @param value
 *
 * @return 
 */
int MaxProperties::validate(const Json::Value *value)
{
   if (value->size() > m_maxProperties) {
      return JVAL_ERR_INVALID_MAX_PROPERTIES;
   }

   return JVAL_ROK;
}

/**
 * @brief Object validation keyword. Constructor
 */
MinProperties::MinProperties(unsigned int minProperties = 0)
{
   m_minProperties = minProperties;
}

/**
 * @brief Object validation keyword. Validates minimum number of properties of
 * an object
 *
 * @param schema
 * @param value
 *
 * @return 
 */
int MinProperties::validate(const Json::Value *value)
{
   if (value->size() < m_minProperties) {
      return JVAL_ERR_INVALID_MIN_PROPERTIES;
   }

   return JVAL_ROK;
}

Required::Required(Json::Value required)
{
   for (unsigned int i = 0; i < required.size(); i++) {
      m_required.push_back(required[i].asString());
   }
}

/**
 * @brief Object validation keyword. Validates mandatory properties of an
 * object
 *
 * @param schema
 * @param value
 *
 * @return
 */
int Required::validate(const Json::Value *value)
{
   for (size_t i = 0; i < m_required.size(); i++) {
      if (!value->isMember(m_required[i])) {
         return JVAL_ERR_REQUIRED_ITEM_MISSING;
      }
   }

   return JVAL_ROK;
}

Properties::Properties(Json::Value properties, bool additionalProperties = true)
{
   m_properties = properties;
   m_additionalProperties = additionalProperties;

   for (Json::ValueIterator itr = properties.begin();\
         itr != properties.end();\
         itr++) {

      Json::Value property = properties.get(itr.name(), property);
      JsonPrimitive *primitive = JsonPrimitive::createPrimitive(&property);
      m_primitives[itr.name()] = primitive;
   }
}

/**
 * @brief Object validation keyword. Validates properties of an object
 *
 * @param schema
 * @param value
 *
 * @return 
 */
int Properties::validate(const Json::Value *value)
{
   if (m_additionalProperties) {
      return JVAL_ROK;
   }

   for (Json::ValueConstIterator itr = value->begin();\
         itr != value->end();\
         itr++) {

      JsonPrimitive *primitive = m_primitives[itr.name()];
      Json::Value property = *itr;
      if (JVAL_ROK != primitive->validate(&property)) {
         return JVAL_ERR_INVALID_PROPERTY;
      }
   }

   return JVAL_ROK;
}

AdditionalProperties::AdditionalProperties()
{
    m_additionalProperties = true;
}

int AdditionalProperties::validate(const Json::Value *value)
{
   (void)value;
#if 0
    Json::Value additionalProps = schema->get("additionalProperties",
            additionalProps);
    if (additionalProps.isBool()) {
        m_additionalProperties = additionalProps.asBool();
        if (true == m_additionalProperties) {
            return JVAL_ROK;
        }
    } else if (additionalProps.isObject()) {
        return JVAL_ROK;
    } else {
        return JVAL_ERR_INVALID_SCHEMA;
    }
#endif

    return JVAL_ROK;
}
