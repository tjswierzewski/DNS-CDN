#include "DNSQuestion.h"

DNSQuestion::DNSQuestion(char *message)
{
    this->nameLength = message[0];
    this->name = std::string(message + 1, this->nameLength);
    this->qType = message[1 + this->nameLength] << 8 | message[2 + this->nameLength];
    this->qClass = message[3 + this->nameLength] << 8 | message[4 + this->nameLength];
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
    return this->nameLength + 5;
}