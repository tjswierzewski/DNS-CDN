#include <sstream>
#include "DNSQuestion.h"

DNSQuestion::DNSQuestion(char *message)
{
    this->readName(message);
    this->qType = message[0 + this->nameLength] << 8 | message[1 + this->nameLength];
    this->qClass = message[2 + this->nameLength] << 8 | message[3 + this->nameLength];
}
int DNSQuestion::getNameLength()
{
    return this->nameLength;
}
std::string DNSQuestion::getName()
{
    return this->name;
}
int DNSQuestion::getQType()
{
    return this->qType;
}
int DNSQuestion::getQClass()
{
    return this->qClass;
}
int DNSQuestion::getMessageSize()
{
    return this->nameLength + 4;
}
void DNSQuestion::readName(char *message)
{
    this->nameLength = 0;
    this->name = std::string();
    int segLength;
    while ((segLength = message[this->nameLength]) > 0)
    {
        this->nameLength++;
        this->name.append(message + this->nameLength, segLength);
        this->name.append(".");
        this->nameLength += segLength;
    }
    this->name.pop_back();
    this->nameLength++;
}
std::string DNSQuestion::format()
{
    std::string output;
    std::string token;
    int pos = 0;
    int delPos;
    do
    {
        delPos = this->name.find(".", pos);
        token = this->name.substr(pos, delPos - pos);
        output.push_back((char)(delPos - pos));
        output += token;
        pos = delPos + 1;
    } while (delPos != std::string::npos);
    output.push_back((char)0);
    output.push_back((char)(this->qType >> 8));
    output.push_back((char)(this->qType & 255));
    output.push_back((char)(this->qClass >> 8));
    output.push_back((char)(this->qClass & 255));
    return output;
}