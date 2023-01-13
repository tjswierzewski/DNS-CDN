#include <string>
#include "JSONString.h"

JSONString::JSONString(std::string value) : JSONValue("String")
{
    this->value = value;
}
std::string JSONString::getValue()
{
    return this->value;
}