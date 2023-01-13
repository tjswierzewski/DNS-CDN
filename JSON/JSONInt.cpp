#include "JSONInt.h"

JSONInt::JSONInt(int value) : JSONValue("Int")
{
    this->value = value;
}
int JSONInt::getValue()
{
    return this->value;
}