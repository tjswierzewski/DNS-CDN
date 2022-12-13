#include <string>
#include "JSON.h"
#include "JSONString.h"
#include "JSONJson.h"
#include "JSONFloat.h"
#include "JSONInt.h"
#include "JSONList.h"
#include <iostream>

JSON::JSON(std::string jsonString)
{
    int colon = 0;
    int key = 0;
    int comma = 0;
    do
    {
        key = jsonString.find('"', comma);
        comma = jsonString.find(',', key);
        colon = jsonString.find(":", key);
        std::string keyString = jsonString.substr(key + 1, colon - key - 2);
        switch (jsonString[colon + 1])
        {
        case '"':
        {
            JSONString *stringValue = new JSONString(jsonString.substr(colon + 2, comma - colon - 3));
            this->data.insert({keyString, (JSONValue *)stringValue});
            break;
        }
        case '{':
        {
            int openBracket = colon + 1;
            int closeBracket = jsonString.find('}', openBracket);
            while (openBracket = jsonString.find('{', openBracket + 1), openBracket != -1 && openBracket < closeBracket)
            {
                closeBracket = jsonString.find('}', openBracket);
            }

            JSONJson *jsonValue = new JSONJson(JSON(jsonString.substr(colon + 1, closeBracket - colon)));
            this->data.insert({keyString, (JSONValue *)jsonValue});
            comma = jsonString.find(',', closeBracket);
            break;
        }
        case '[':
        {
            int endBrace = jsonString.find(']', colon + 1);
            JSONList *listValue = new JSONList(jsonString.substr(colon + 1, endBrace - colon));
            this->data.insert({keyString, (JSONValue *)listValue});
            comma = jsonString.find(',', endBrace);
            break;
        }
        default:
        {
            int period = jsonString.find('.', colon);
            if (period != -1 && period < comma)
            {
                JSONFloat *floatValue = new JSONFloat(stof(jsonString.substr(colon + 1, comma - colon - 1)));
                this->data.insert({keyString, (JSONValue *)floatValue});

                break;
            }
            else
            {
                JSONInt *intValue = new JSONInt(stoi(jsonString.substr(colon + 1, comma - colon - 1)));
                this->data.insert({keyString, (JSONValue *)intValue});

                break;
            }
        }
        }
    } while (comma != -1);
}
JSON::JSON()
{
}
std::map<std::string, JSONValue *> JSON::getData()
{
    return this->data;
}