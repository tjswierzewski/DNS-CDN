#ifndef JSON_JSON_H
#define JSON_JSON_H

#include "JSONValue.h"
#include "JSON.h"

class JSONJson : public JSONValue
{
private:
    JSON value;

public:
    JSONJson(JSON value);
    JSON getValue();
};
#endif