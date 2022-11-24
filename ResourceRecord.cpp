// https://en.wikipedia.org/wiki/Domain_Name_System#Resource_records
// https://en.wikipedia.org/wiki/List_of_DNS_record_types

ResourceRecord::ResourceRecord(string name, int16_t type, int16_t classCode, int32_t ttl, int16_t rdlength, char *rdata)
{
    this->name = name;
    this->type = type;
    this->classCode = classCode;
    this->ttl = ttl;
    this->rdlength = rdlength;
    this->rdata = rdata;
}
ResourceRecord::string getName()
{
    return this->name;
}
ResourceRecord::int16_t getType()
{
    return this->type;
}
ResourceRecord::int16_t getClassCode()
{
    return this->classCode;
}
ResourceRecord::int32_t getTTL()
{
    return this->ttl;
}
ResourceRecord::int16_t getRDLength()
{
    return this->rdlength;
}
ResourceRecord::char *getRdata()
{
    return this->rdata;
}