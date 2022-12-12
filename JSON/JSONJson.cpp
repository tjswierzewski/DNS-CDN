#include "JSONJson.h"
#include "JSON.h"

JSONJson::JSONJson(JSON value) : JSONValue("JSON")
{
    this->value = value;
}
JSON JSONJson::getValue()
{
    return this->value;
}