/**
 * Authors: Nedo Skobalj
 * SHA-256 for homework 2 in the course Introduction to cryptography at KTH.
 * NIST Spcification used: http://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.180-4.pdf
 * Helpful hexadecimal to SHA-256 calculator: http://www.fileformat.info/tool/hash.htm?hex=e5
 */

#include <iostream>
#include <cstdint>
#include <cstdio>
#include <vector>
#include <string>
#include <bitset>
#include <iomanip>
#include "sha256.h"


using namespace std;

// Constants used in SHA-256
uint32_t K[64] = {
	0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
   	0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
   	0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
   	0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
   	0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
   	0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
   	0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
   	0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

int main(int argc, char const *argv[]) {
	while (!cin.eof()) {
		vector<unsigned char> data = readMessageAsHex(); // Read the message and interpert it as hex values
		// If we have reached end of file(no more test cases) then stop
		if (cin.eof()) {
			return 0;
		}
		padding(data); // Perform padding on the message
		uint32_t hashArray[8]; // Array of hash values
		getInitHash(hashArray); // Initialize hashArray
		uint32_t messageWords[16]; // 512 bits of the padded message in the form of 16 32-bit words. Considered a block
		int messageBlocks = data.size()/64; // How many 512-bit blocks our message is divided into.
		for (int i = 0; i < messageBlocks; ++i) {
			readMessageBlock(messageWords, data, i); // Reads the block from data into messageWords
			uint32_t W[64]; // Message Schedule, denoted W in the NIST specification
			prepMessageSchedule(messageWords, W); // Calculate the values for the message schedule
			// Initialize the eight working variables. Spec uses a-h as working variable names so
			// same notation used here.
			uint32_t a = hashArray[0];
			uint32_t b = hashArray[1];
			uint32_t c = hashArray[2];
			uint32_t d = hashArray[3];
			uint32_t e = hashArray[4];
			uint32_t f = hashArray[5];
			uint32_t g = hashArray[6];
			uint32_t h = hashArray[7];
			// Calculate hash as specified in NIST specification
			uint32_t T1, T2;
			for (int i = 0; i < 64; ++i) {
				T1 = h + bigSigma1(e) + ch(e, f, g) + K[i] + W[i];
				T2 = bigSigma0(a) + maj(a, b, c);
				h = g;
				g = f;
				f = e;
				e = d + T1;
				d = c;
				c = b;
				b = a;
				a = T1 + T2;
			}
			// Update hashvalues
			hashArray[0] += a;
			hashArray[1] += b;
			hashArray[2] += c;
			hashArray[3] += d;
			hashArray[4] += e;
			hashArray[5] += f;
			hashArray[6] += g;
			hashArray[7] += h;
		}
		printHashArray(hashArray); // Print the answer
	}
	return 0;
}
// Logical function defined in NIST spec. for SHA-256
uint32_t ch(uint32_t x, uint32_t y, uint32_t z) {
	return (x & y)^((~x) & z);
}
// Logical function defined in NIST spec. for SHA-256
uint32_t maj(uint32_t x, uint32_t y, uint32_t z) {
	return (x & y)^(x & z)^(y & z);
}
/**
 * Returns the value one gets by bitwise rotating the word right.
 * @param  word word to be rotated
 * @param  n    number of bits to rotate
 * @return      result of rotatation
 */
uint32_t rotr(uint32_t word, int n) {
	return (word >> n) | (word << (32-n));
}

// Logical function defined in NIST spec. for SHA-256
uint32_t bigSigma0(uint32_t x) {
	return rotr(x, 2)^rotr(x, 13)^rotr(x, 22);
}
// Logical function defined in NIST spec. for SHA-256
uint32_t bigSigma1(uint32_t x) {
	return rotr(x, 6)^rotr(x, 11)^rotr(x, 25);
}
// Logical function defined in NIST spec. for SHA-256
uint32_t littleSigma0(uint32_t x) {
	return rotr(x, 7)^rotr(x, 18)^(x >> 3);
}
// Logical function defined in NIST spec. for SHA-256
uint32_t littleSigma1(uint32_t x) {
	return rotr(x, 17)^rotr(x, 19)^(x >> 10);
}
/**
 * Reads the string input from stdin and converts it into a vector
 * of bytes containing the hex-values specified *by* the string.
 * @return vector The bytes specified in the input string as vector of unsigned chars.
 */
std::vector<unsigned char> readMessageAsHex() {
	string input;
	getline(cin, input);
	vector<unsigned char> hexInput(input.length()/2); // 2 chars in string represent one hexvalue(byte) to hash
	char *inputPos = &input[0];
	unsigned char *hexPos = &hexInput[0];
	for (int i = 0; i < (input.length())/2; ++i) {
		sscanf(inputPos, "%2hhx", hexPos); // Interperate the string as hex values
		inputPos = inputPos+2;
		hexPos++;
	}
	return hexInput;
}
/**
 * Performs the padding scheme specified in the NIST SHA-256 specification.
 * In short a bit "1" is appended to the message, k zero bits after that and the 64-bit
 * representation of the message length lastly.
 * k is calculated as k = 512i + 447 - mLengthBits, where mLengthBits is the message length
 * in bits, and is the integer that gives the value 0<=k<512.
 * @param data The vector containing the message
 */
void padding(std::vector<unsigned char> &data) {
	long long mLengthBits = data.size()*8; // Max size input for SHA 256 is 2^64-1 bits
	unsigned int k = (512 + 448 - (mLengthBits % 512 + 1)) % 512; // Number of 0's to pad to the message
	// Create bitset to hold all padding, sets all bits to 0 by default
	bitset<64> lengthBits(mLengthBits); // Length of message in bit format
	string lengthPaddingString = lengthBits.to_string(); // Get message length as bit string
	bitset<512> paddingK0; // Contains the first 1 and k 0's in the padding. Can't be bigger than 512 by design
	paddingK0[511] = 1; // Appending a "1" to the message
	string paddingString = paddingK0.to_string(); // Get padding as string
	int i = 0;
	unsigned char toAdd = 0x00;
	k += 1; // Account for the "1" appended to the message.
	// Start adding the padding to the data vector containing the message
	while (k >= 8) { // There are 8 or more bits left to pad to the message with
	 	bitset<8> bitsetToAdd(paddingString.substr(i*8, 8)); // Extract the next 8 bits from the padding string
	 	toAdd = (unsigned char)bitsetToAdd.to_ulong(); // Convert from string of bits to value
	 	data.push_back(toAdd); // Add to data vector
	 	i++;
	 	k -= 8;
	}
	int takenFromLength = 0; // number of bits padded to message from the 64-bit message length block
	// This is to make sure all correct zeros are added even if k is not a multiple of 8
	if (k != 0) {
		bitset<8> bitsetToAdd(paddingString.substr(i*8, k)); // Get the rest of the 0's
		toAdd = (unsigned char)bitsetToAdd.to_ulong(); // Convert from string of bits to value
		bitset<8> morebitsetToAdd(lengthPaddingString.substr(0, (8-k))); // Get remaining bits to fill a byte from length padding string
		takenFromLength += (8-k);
		unsigned char moreToAdd = (unsigned char)morebitsetToAdd.to_ulong();
		toAdd = toAdd << (8-k);
		toAdd = toAdd|moreToAdd;
		data.push_back(toAdd);
	}
	// Now we add the 64-bit(or what is left of it) block containing the message length in bit representation
	while (takenFromLength != 64) {
		bitset<8> bitsetToAdd(lengthPaddingString.substr(takenFromLength, 8));
		toAdd = (unsigned char)bitsetToAdd.to_ulong(); // Convert from string of bits to value
		data.push_back(toAdd);
		takenFromLength += 8;
	}
}
/**
 * Fills the given array with the initial hash values specified for SHA-256
 */
void getInitHash(uint32_t (&hashArray)[8]) {
	hashArray[0] = 0x6a09e667;
	hashArray[1] = 0xbb67ae85;
	hashArray[2] = 0x3c6ef372;
	hashArray[3] = 0xa54ff53a;
	hashArray[4] = 0x510e527f;
	hashArray[5] = 0x9b05688c;
	hashArray[6] = 0x1f83d9ab;
	hashArray[7] = 0x5be0cd19;
}
/**
 * Reads a 512-bit block of the *padded* message and puts it into messageWords in the form of 16 32-bit words.
 * @param &messageWords, The array which will contain the 32-bit words.
 * @param &data, The data vector containing the padded message.
 * @param i, index of where to start reading bits from. i = 0 gives bits 1 to 512 and so on.
 */
void readMessageBlock(uint32_t (&messageWords)[16], std::vector<unsigned char> &data, int i) {
	int byteToStartFrom = i*64;
	if (byteToStartFrom >= data.size()) {
		cerr << "Can't read any more bytes" << endl;
	} else {
		for (int j = 0; j < 16; ++j) {
			uint32_t toAdd = 0x00;
			toAdd = (toAdd | data[byteToStartFrom]) << 8;
			toAdd = (toAdd | data[byteToStartFrom+1]) << 8;
			toAdd = (toAdd | data[byteToStartFrom+2]) << 8;
			toAdd = (toAdd | data[byteToStartFrom+3]);
			messageWords[j] = toAdd;
			byteToStartFrom += 4;
		}
	}
}
/**
 * Calculate the values in the message schedule as specified in the NIST SHA-256 specification.
 * @param &messageWords The current block we are working on as 32-bit word array
 * @param W 			Array which will contain the values of the message schedule
 */
void prepMessageSchedule(uint32_t (&messageWords)[16], uint32_t (&W)[64]) {
	// The first 16 words are just copied over.
	for (int i = 0; i < 16; ++i) {
		W[i] = messageWords[i];
	}
	// Calculated according to specification
	for (int i = 16; i < 64; ++i) {
		uint32_t a = littleSigma1(W[i-2]);
		uint32_t b = W[i-7];
		uint32_t c = littleSigma0(W[i-15]);
		uint32_t d = W[i-16];
		W[i] = a+b+c+d;
	}
}
/**
 * Prints out the hashArray in the correct order and with no spaced.
 * @param &hashArray Array containing the hash values
 */
void printHashArray(uint32_t (&hashArray)[8]) {
	for (int i = 0; i < 8; ++i) {
		// set width to 8 and set fill to 0 to make sure it writes all 256-bits without skipping 0's
		cout << hex << setw(8) << setfill('0') << hashArray[i];
	}
	cout << endl;
}
// FOLLOWING METHODS ARE USED ONLY FOR TESTING
void printData(std::vector<unsigned char> &data) {
	cout << "-----------------------------------------" << endl;
	cout << "DATA: " << endl;
	long long mLengthBits = data.size()*8;
	cout << dec << "Bits: " << mLengthBits << endl;
	for (int i = 0; i < data.size(); ++i) {
		cout << hex << (unsigned int)data[i] << " ";
	}
	cout << endl;
	cout << "-----------------------------------------" << endl;
}

void printMessageWords(uint32_t (&messageWords)[16]) {
	cout << "-----------------------------------------" << endl;
	cout << "MESSAGEWORDS: " << endl;
	for (int i = 0; i < 16; ++i) {
		cout << "messageWords[" << dec << i << "]: " << hex << messageWords[i] << endl;
	}
	cout << "-----------------------------------------" << endl;
}

void printMessageSchedule(uint32_t (&W)[64]) {
	cout << "MESSAGE Schedule: " << endl;
	cout << "-----------------------------------------" << endl;
	for (int i = 0; i < 64; ++i) {
		cout << "W[" << dec << i << "]: " << hex << W[i] << endl;
	}
	cout << "-----------------------------------------" << endl;
}
