#ifndef JSON_FLOAT_H
#define JSON_FLOAT_H

#include "JSONValue.h"

class JSONFloat : public JSONValue
{
private:
    float value;

public:
    JSONFloat(float value);
    float getValue();
};
#endif