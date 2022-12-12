#include <list>
#include "JSONList.h"

JSONList::JSONList(std::string list) : JSONValue("List")
{

    int openBracket = 1;
    int nextOpenBracket = openBracket;
    while (nextOpenBracket != -1)
    {
        int nextOpenBracket = openBracket;
        int closeBracket = list.find('}', nextOpenBracket);
        nextOpenBracket = list.find('{', nextOpenBracket + 1);
        while (nextOpenBracket != -1 && nextOpenBracket < closeBracket)
        {
            closeBracket = list.find('}', closeBracket + 1);
            nextOpenBracket = list.find('{', nextOpenBracket + 1);
        }
        this->list.emplace_back(JSON(list.substr(openBracket, closeBracket - openBracket)));
        if (nextOpenBracket == -1)
        {
            break;
        }

        openBracket = nextOpenBracket + 2;
    }
}
std::list<JSON> JSONList::getList()
{
    return this->list;
}