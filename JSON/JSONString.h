#ifndef JSON_STRING_H
#define JSON_STRING_H

#include "JSONValue.h"

class JSONString : public JSONValue
{
private:
    std::string value;

public:
    JSONString(std::string value);
    std::string getValue();
};
#endif