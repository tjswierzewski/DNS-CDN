class DNSMessage
{
private:
    uint16_t identification;
    uint16_t flags;
    uint16_t numberOfQuestions;
    uint16_t numberOfResourceRecords;
    uint16_t numberOfAdditionalRecords;

public:
    DNSMessage(uint16_t identification, int qr, int opcode, int aa, int tc, int rd, int ra, int rcode, int16_t numberOfQuestions, int16_t numberOfResourceRecords, int16_t numberOfAdditionalRecords);
    int getIdentification();
    int getQR();
    int getOPCODE();
    int getAA();
    int getTC();
    int getRD();
    int getRA();
    int getRCODE();
    int getNumberOfQuestion();
    int getNumberOfResoureRecords();
    int getNumberOfAdditionalRecords();
};