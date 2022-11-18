#include "DNSMessage.h"

DNSMessage::DNSMessage(uint16_t identification, int qr, int opcode, int aa, int tc, int rd, int ra, int rcode, int16_t numberOfQuestions, int16_t numberOfResourceRecords, int16_t numberOfAdditionalRecords)
{
    this->identification = identification;
    this->flags = qr << 15;
    this->flags += opcode << 11;
    this->flags += aa << 10;
    this->flags += tc << 9;
    this->flags += rd << 8;
    this->flags += ra << 7;
    this->flags += rcode;
    this->numberOfQuestions = numberOfQuestions;
    this->numberOfResourceRecords = numberOfResourceRecords;
    this->numberOfAdditionalRecords = numberOfAdditionalRecords;
}
int DNSMessage::getIdentification()
{
    return this->identification;
}
int DNSMessage::getQR()
{
    return this->flags >> 15;
}
int DNSMessage::getOPCODE()
{
    return this->flags >> 11 && 0x1111;
}
int DNSMessage::getAA()
{
    return this->flags >> 10 && 0x1;
}
int DNSMessage::getTC()
{
    return this->flags >> 9 && 0x1;
}
int DNSMessage::getRD()
{
    return this->flags >> 8 && 0x1;
}
int DNSMessage::getRA()
{
    return this->flags >> 7 && 0x1;
}
int DNSMessage::getRCODE()
{
    return this->flags && 0x1111;
}
int DNSMessage::getNumberOfQuestions()
{
    return this->numberOfQuestions;
}
int DNSMessage::getNumberOfResoureRecords()
{
    return this->numberOfResourceRecords;
}
int DNSMessage::getNumberOfAdditionalRecords()
{
    return this->numberOfAdditionalRecords
}