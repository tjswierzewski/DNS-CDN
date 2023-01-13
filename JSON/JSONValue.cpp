#include "JSONValue.h"

JSONValue::JSONValue(std::string type)
{
    this->type = type;
}
JSONValue::JSONValue()
{
}

std::string JSONValue::getType()
{
    return this->type;
}