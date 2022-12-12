#ifndef JSON_LIST_H
#define JSON_LIST_H

#include <list>
#include "JSONValue.h"
#include "JSON.h"

class JSONList : public JSONValue
{
private:
    std::list<JSON> list;

public:
    JSONList(std::string list);
    std::list<JSON> getList();
};
#endif