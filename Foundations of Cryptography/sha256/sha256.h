// Header file for sha256.cpp

uint32_t ch(uint32_t x, uint32_t y, uint32_t z);
uint32_t maj(uint32_t x, uint32_t y, uint32_t z);
uint32_t rotr(uint32_t word, int n);
uint32_t add(uint32_t value1, uint32_t value2);
uint32_t bigSigma0(uint32_t x);
uint32_t bigSigma1(uint32_t x);
uint32_t littleSigma0(uint32_t x);
uint32_t littleSigma1(uint32_t x);
std::vector<unsigned char> readMessageAsHex();
void padding(std::vector<unsigned char> &data);
void getInitHash(uint32_t (&hashArray)[8]);
void readMessageBlock(uint32_t (&messageWords)[16], std::vector<unsigned char> &data, int i);
void prepMessageSchedule(uint32_t (&messageWords)[16], uint32_t (&W)[64]);
void printHashArray(uint32_t (&hashArray)[8]);
// For testing
void printData(std::vector<unsigned char> &data);
void printMessageWords(uint32_t (&messageWords)[16]);
void printMessageSchedule(uint32_t (&W)[64]);