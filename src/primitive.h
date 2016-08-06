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

#ifndef __PRIMITIVE_H__
#define __PRIMITIVE_H__

class JsonInteger : public JsonPrimitive
{
   public:
      JsonInteger(Json::Value *schema);
      ~JsonInteger();
      int validate(const Json::Value *value);

   private:
      std::list<KeywordValidator*> m_validators; 
};

class JsonNumber : public JsonPrimitive
{
   public:
      JsonNumber(Json::Value *schema);
      ~JsonNumber();
      int validate(const Json::Value *value);

   private:
      std::list<KeywordValidator*> m_validators; 
};

class JsonString : public JsonPrimitive
{
   public:
      JsonString(Json::Value *schema);
      ~JsonString();
      int validate(const Json::Value *value);

   private:
      std::list<KeywordValidator*> m_validators; 
};

class JsonObject : public JsonPrimitive
{
   public:
      JsonObject(Json::Value *element);
      ~JsonObject();
      int validate(const Json::Value *value);
   
   private:
      std::list<KeywordValidator*> m_validators; 
      void validateMembers(const Json::Value *value);
};

class JsonEnum : public JsonPrimitive
{
   public:
      JsonEnum(Json::Value *element);
      ~JsonEnum() {}
      int validate(const Json::Value *value);

   private:
      void getOptions(std::vector<std::string> &options);
};

class JsonBoolean : public JsonPrimitive
{
   public:
      JsonBoolean(Json::Value *element);
      ~JsonBoolean() {}
      int validate(const Json::Value *value);
};

class JsonNull : public JsonPrimitive
{
   public:
      JsonNull(Json::Value *element);
      ~JsonNull() {}
      int validate(const Json::Value *value);
};

class JsonArray : public JsonPrimitive
{
   public:
      JsonArray(Json::Value *schema);
      ~JsonArray();
      int validate(const Json::Value *value);

   private:
      std::list<KeywordValidator*> m_validators; 

};

#endif
