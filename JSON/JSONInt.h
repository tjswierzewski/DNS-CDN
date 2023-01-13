#ifndef JSON_INT_H
#define JSON_INT_H

#include "JSONValue.h"

class JSONInt : public JSONValue
{
private:
    int value;

public:
    JSONInt(int value);
    int getValue();
};

#endif