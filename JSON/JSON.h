#ifndef JSON_H
#define JSON_H

#include <string>
#include <map>
#include "JSONValue.h"

class JSON
{
private:
    std::map<std::string, JSONValue *> data;

public:
    JSON(std::string jsonString);
    JSON();
    std::map<std::string, JSONValue *> getData();
};
#endif