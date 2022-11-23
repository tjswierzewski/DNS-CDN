#include <vector>
#include "DNSQuestion.h"

class DNSMessage
{
private:
    unsigned short int identification;
    unsigned short int flags;
    unsigned short int numberOfQuestions;
    unsigned short int numberOfAnswerRecords;
    unsigned short int numberOfAuthorityRecords;
    unsigned short int numberOfAdditionalRecords;
    std::vector<DNSQuestion> questions;

public:
    DNSMessage(unsigned short int identification, int qr, int opcode, int aa, int tc, int rd, int ra, int rcode, unsigned short int numberOfQuestions, unsigned short int numberOfAnswerRecords, unsigned short int numberOfAuthorityRecords, unsigned short int numberOfAdditionalRecords);
    DNSMessage(char *message, int size);
    int getIdentification();
    int getQR();
    int getOPCODE();
    int getAA();
    int getTC();
    int getRD();
    int getRA();
    int getRCODE();
    int getNumberOfQuestions();
    int getNumberOfAnswerRecords();
    int getNumberOfAuthorityRecords();
    int getNumberOfAdditionalRecords();
    DNSQuestion *getQuestion();
};