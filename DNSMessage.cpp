#include "DNSMessage.h"
#include <cstring>

DNSMessage::DNSMessage(unsigned short int identification, int qr, int opcode, int aa, int tc, int rd, int ra, int rcode, unsigned short int numberOfQuestions, unsigned short int numberOfAnswerRecords, unsigned short int numberOfAuthorityRecords, unsigned short int numberOfAdditionalRecords)
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
    this->numberOfAnswerRecords = numberOfAnswerRecords;
    this->numberOfAuthorityRecords = numberOfAuthorityRecords;
    this->numberOfAdditionalRecords = numberOfAdditionalRecords;
}
DNSMessage::DNSMessage(char *message, int size)
{
    this->identification = message[0] << 8 | message[1];
    this->flags = message[2] << 8 | message[3];
    this->numberOfQuestions = message[4] << 8 | message[5];
    this->numberOfAnswerRecords = message[6] << 8 | message[7];
    this->numberOfAuthorityRecords = message[8] << 8 | message[9];
    this->numberOfAdditionalRecords = message[10] << 8 | message[11];
    int offset = 12;
    for (size_t i = 0; i < this->numberOfQuestions; i++)
    {
        this->questions.push_back(DNSQuestion(message + offset));
        offset += this->questions.back().getMessageSize();
    }
    for (size_t i = 0; i < this->numberOfAnswerRecords; i++)
    {
        // Parse Answer Records
    }
    for (size_t i = 0; i < this->numberOfAuthorityRecords; i++)
    {
        // Parse Authority Records
    }
    for (size_t i = 0; i < this->numberOfAdditionalRecords; i++)
    {
        // Parse Authority Records
    }
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
int DNSMessage::getNumberOfAnswerRecords()
{
    return this->numberOfAnswerRecords;
}
int DNSMessage::getNumberOfAuthorityRecords()
{
    return this->numberOfAuthorityRecords;
}
int DNSMessage::getNumberOfAdditionalRecords()
{
    return this->numberOfAdditionalRecords;
}
DNSQuestion *DNSMessage::getQuestion()
{
    if (this->questions.size() <= 0)
    {
        return NULL;
    }
    return &this->questions.front();
}