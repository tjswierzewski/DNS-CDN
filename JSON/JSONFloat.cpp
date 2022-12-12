#include "JSONFloat.h"

JSONFloat::JSONFloat(float value) : JSONValue("Float")
{
    this->value = value;
}
float JSONFloat::getValue()
{
    return this->value;
}