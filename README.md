# JSON Schema Validator
JSON Schema [Draft 4](https://tools.ietf.org/id/draft-fge-json-schema-validation-00.txt) validation implementation in C++ using JsonCpp library.

| IMPORTANT | This work is in progress   |
|-----------|----------------------------|

## 1. Introduction
JSON (JavaScript Object Notation) is a lightweight data-interchange format. The JSON Schema Draf 4 specification defines the validation keyword
and validation process for valdating JSON data received from various sources in an application. JSON Schema Validator implements the Draf 4 specification
in C++ and JsonCpp for parsing and traversing the JSON data.


## 2. Dependencies
This software uses JsonCpp 1.7.2 for parsing the json schema file and the user provided json data, which is part of this codebase.

Tested with c++0x 

## 3. Using JSON Schema Validator in your project
Add the source code (.cpp and .h files) under 'src' and 'jsoncpp' directory into your project and compile and build as you would any other source file.
The 'src/' and 'jsoncpp/json/' should be added to your compiler include path. The headers files that must be included in your source code as follows:

...
#include <json.h>
#include <validdator.>
...

the 'sample' directory has an example code that show how to use this schema validator in your application.

## 4. Testing
Unit testing and validation is done using GTEST 1.7.0. For running the test suite do the following:

...
>> cd test/ut/
>> make
>> ./jvalut
...


## 5. License
See the LICENSE file for details. In summary, Json Schema validator is licensed under the MIT license.
