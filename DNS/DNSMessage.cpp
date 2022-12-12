#include <netinet/in.h>
#include <string>
#include <sstream>
#include <cstring>
#include "DNSMessage.h"

/**
 * Create DNS Message from attributes
 */
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
/**
 * Create DNS Message from char buffer
 */
DNSMessage::DNSMessage(char *message, int size)
{
    this->identification = ntohs(*(uint16_t *)message);
    this->flags = (message[2] << 8) + message[3];
    this->numberOfQuestions = (message[4] << 8) + message[5];
    this->numberOfAnswerRecords = (message[6] << 8) + message[7];
    this->numberOfAuthorityRecords = (message[8] << 8) + message[9];
    this->numberOfAdditionalRecords = (message[10] << 8) + message[11];
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
/**
 * Get Identification number
 */
int DNSMessage::getIdentification()
{
    return this->identification;
}
/**
 * QR flag
 */
int DNSMessage::getQR()
{
    return this->flags >> 15;
}
/**
 * OPCODE flag
 */
int DNSMessage::getOPCODE()
{
    return this->flags >> 11 & 0x1111;
}
/**
 * AA flag
 */
int DNSMessage::getAA()
{
    return this->flags >> 10 & 0x1;
}
/**
 * TC flag
 */
int DNSMessage::getTC()
{
    return this->flags >> 9 & 0x1;
}
/**
 * RD flag
 */
int DNSMessage::getRD()
{
    return this->flags >> 8 & 0x1;
}
/**
 * RA flag
 */
int DNSMessage::getRA()
{
    return this->flags >> 7 & 0x1;
}
/**
 * RCODE flag
 */
int DNSMessage::getRCODE()
{
    return this->flags & 0x1111;
}
/**
 * Get number of questions in message
 */
int DNSMessage::getNumberOfQuestions()
{
    return this->numberOfQuestions;
}
/**
 * Get number of answers in message
 */
int DNSMessage::getNumberOfAnswerRecords()
{
    return this->numberOfAnswerRecords;
}
/**
 * Get number of authority record in message
 */
int DNSMessage::getNumberOfAuthorityRecords()
{
    return this->numberOfAuthorityRecords;
}
/**
 * Get number of additional records in message
 */
int DNSMessage::getNumberOfAdditionalRecords()
{
    return this->numberOfAdditionalRecords;
}
/**
 * Get first question
 */
DNSQuestion *DNSMessage::getQuestion()
{
    if (this->questions.size() <= 0)
    {
        return NULL;
    }
    return &this->questions.front();
}
/**
 * Add question to message
 */
void DNSMessage::addQuestion(DNSQuestion question)
{
    this->questions.push_back(question);
    this->numberOfQuestions++;
}
/**
 * Add answer to message
 */
void DNSMessage::addAnswer(DNSResponse answer)
{
    this->answers.push_back(answer);
    this->numberOfAnswerRecords++;
}
/**
 * format DNS Message for network
 */
std::string DNSMessage::format()
{
    std::string output;
    output.push_back((char)(this->identification >> 8));
    output.push_back((char)(this->identification & 255));
    output.push_back((char)(this->flags >> 8));
    output.push_back((char)(this->flags & 255));
    output.push_back((char)(this->numberOfQuestions >> 8));
    output.push_back((char)(this->numberOfQuestions & 255));
    output.push_back((char)(this->numberOfAnswerRecords >> 8));
    output.push_back((char)(this->numberOfAnswerRecords & 255));
    output.push_back((char)(this->numberOfAuthorityRecords >> 8));
    output.push_back((char)(this->numberOfAuthorityRecords & 255));
    output.push_back((char)(this->numberOfAdditionalRecords >> 8));
    output.push_back((char)(this->numberOfAdditionalRecords & 255));
    for (size_t i = 0; i < this->numberOfQuestions; i++)
    {
        output += this->questions[i].format();
    }
    for (size_t i = 0; i < this->numberOfAnswerRecords; i++)
    {
        output += this->answers[i].format();
    }
    for (size_t i = 0; i < this->numberOfAuthorityRecords; i++)
    {
        // Write Authority Records
    }
    for (size_t i = 0; i < this->numberOfAdditionalRecords; i++)
    {
        // Write Authority Records
    }
    return output;
}
