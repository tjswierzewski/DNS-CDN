#ifndef JSON_VALUE_H
#define JSON_VALUE_H

#include <string>

class JSONValue
{
private:
    std::string type;

protected:
    JSONValue(std::string type);

public:
    std::string getType();
    JSONValue();
};
#endif