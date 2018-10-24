/* Author: Nedo Skobalj
 * AES implementation for homework 1 in the course foundations to cryptography at KTH.
 * Link to specification used: http://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.197.pdf
 * This implementation pretty much just follows the above specification written by NIST.
 * Note: I have used unsigned char to represent a byte and char32_t to represent a 4-byte word.
 */
#include <iostream>

using namespace std;

// Global variables.
const int keyLength = 16; // Length of key in bytes.
const int rounds = 10; // number of encryption rounds.
const int keyLengthInWords = keyLength/4;
// S-box used in AES.
const unsigned char sBox[16][16]=   {
0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16};
// rCon values used in AES 128-bit
const char32_t rCon[11] = {
	0x8d000000 , 0x01000000, 0x02000000, 0x04000000, 0x08000000, 0x10000000, 0x20000000,
	0x40000000, 0x80000000, 0x1b000000, 0x36000000
};

// Forward declarations
void keyExpansion(unsigned char (&key)[keyLength], char32_t (&words)[(keyLength/4)*(rounds+1)], int keyLengthInWords);
char32_t toWord(unsigned char byte1, unsigned char byte2, unsigned char byte3, unsigned char byte4);
void toBytes(char32_t word, unsigned char (&bytes)[4]);
void initState(unsigned char (&state)[4][4], unsigned char firstChar);
void readKey(unsigned char (&key)[keyLength]);
char32_t subWord(char32_t word);
void rotWord(char32_t &word);
void readTestKey(unsigned char (&key)[keyLength]);
void testInitState(unsigned char (&state)[4][4]);
void addRoundKey(unsigned char (&state)[4][4], char32_t expandedKey[(keyLength/4)*(rounds+1)], int round);
void subBytes(unsigned char (&state)[4][4]);
void shiftRows(unsigned char (&state)[4][4]);
void mixColumns(unsigned char (&state)[4][4]);
unsigned char multiByTwoInGF(unsigned char value);
unsigned char multiByThreeInGF(unsigned char value);
void printState(unsigned char (&state)[4][4]);
void printKey(unsigned char (&key)[keyLength]);

int main() {
	// Read the encryption key.
	unsigned char key[keyLength];
	readKey(key);
	// Do the key expansion. Done before encryption starts since the result
	// will be the same for each round of encryption.
	char32_t expandedKey[(keyLength/4)*(rounds+1)];
	keyExpansion(key, expandedKey, keyLengthInWords);
	unsigned char state[4][4]; // "State" aka. the block that is being encrypted. see NIST spec.
	// Encryption of one 128-bit block of data, encrypt as long as input is coming in
	unsigned char testInputChar; // Use this variable to determin if input is coming in.
	while (cin >> testInputChar) {
		// Read the block that is to be encrypted and store it in the state.
		initState(state, testInputChar);
		addRoundKey(state, expandedKey, 0);
		for (int i = 1; i < rounds; ++i) {
			subBytes(state);
			shiftRows(state);
			mixColumns(state);
			addRoundKey(state, expandedKey, i);
		}
		subBytes(state);
		shiftRows(state);
		addRoundKey(state, expandedKey, rounds);
		// Encryption of block done, print result
		printState(state);
	}
	return 0;
}

/**
 * Reads the key used for encryption into the given array.
 * @param keyLength Length of key in bytes.
 * @param key       The array which the key will be read into.
 */
void readKey(unsigned char (&key)[keyLength]) {
	unsigned char byte;
	for (int i = 0; i < keyLength; ++i) {
		cin >> noskipws >> byte; // noskipws = don't skip whitespace.
		key[i] = byte;
	}
}

/**
 * Reads the block of input(from stdin) that is to be encrypted into the state matrix.
 * 4*4 = 16 which is the amount of bytes in one block in AES.
 * @param state     The state matrix
 * @param firstChar The char which will be in state[0][0].
 */
void initState(unsigned char (&state)[4][4], unsigned char firstChar) {
	unsigned char input;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			// Since we already read the first char in main() we must set
			// state[0][0] to firstChar instead of reading using cin.
			if (i == 0 && j == 0) {
				state[j][i] = firstChar;
			} else {
				cin >> input;
				state[j][i] = input;
			}
		}
	}
}

/**
 * The key expansion function in AES. Loads the expanded key into the
 * array words. The words array is made up of 32-bit words, NOT bytes.
 * @param key 				The encryption key.
 * @param words 			The result of the key expansion.
 * @param keyLengthInWords  Length of encryption key in 32-bit words.
 */
void keyExpansion(unsigned char (&key)[keyLength], char32_t (&words)[(keyLength/4)*(rounds+1)], int keyLengthInWords) {
	char32_t temp;
	int i = 0;
	// Fill the first four words of the expanded key with the encryption key.
	while (i < keyLengthInWords) {
		words[i] =  toWord(key[4*i], key[4*i+1], key[4*i+2], key[4*i+3]);
		++i;
	}

	i = keyLengthInWords;
	while (i < (keyLength/4)*(rounds+1)) {
		temp = words[i-1];
		if (i % keyLengthInWords == 0) {
			rotWord(temp);
			temp = subWord(temp) ^ rCon[i/keyLengthInWords];
		// Not really needed in this case since the key is always 128-bits but I added it anyway...
		} else if (keyLengthInWords > 6 && i % keyLengthInWords == 4) {
			temp = subWord(temp);
		}
		words[i] = words[i-keyLengthInWords]^temp;
		++i;
	}
}

/**
 * Performs transformation on the state using the expanded key as specified in the AES specification.
 * Transformation is just XOR-ing the columns in the state with the expanded keys.
 * The "round key" is a subset of the expanded key whichs is added to the state.
 * @param state 		The AES encryption block.
 * @param expandedKey	The expanded key.
 * @param round 		The current round of encryption
 */
void addRoundKey(unsigned char (&state)[4][4], char32_t expandedKey[(keyLength/4)*(rounds+1)], int round) {
	char32_t roundKey, stateColumn;
	unsigned char bytes[4];
	for (int c = 0; c < 4; ++c) {
		roundKey = expandedKey[round*4+c];
		// We must convert the entire state column to a word because we have to XOR the whole thing
		// with the round key.
		stateColumn = toWord(state[0][c], state[1][c], state[2][c], state[3][c]);
		stateColumn = stateColumn^roundKey;
		// Loads the bytes from stateColumn into array bytes.
		toBytes(stateColumn, bytes);
		// Have to "rotate" the bytes into the right place in the state
		for (int j = 0; j < 4; ++j) {
			state[j][c] = bytes[j];
		}
	}
}

/**
 * Performs substition using the AES S-Box on each byte in the state.
 * @param state The state/block being encrypted
 */
void subBytes(unsigned char (&state)[4][4]) {
	unsigned char leftIndex, rightIndex;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			// split byte into 4-bit parts to use for indexing in S-Box
			leftIndex = state[i][j] >> 4;
			rightIndex = state[i][j] & 0x0f;
			state[i][j] = sBox[leftIndex][rightIndex];
		}
	}
}

/**
 * Cyclicly shifts the last three rows in the state as specified in the AES specification.
 * @param state 	The state/block being encrypted
 */
void shiftRows(unsigned char (&state)[4][4]) {
	unsigned char temp[4];
	for (int i = 1; i < 4; ++i) {
		for (int k = 0; k < 4; ++k) {
			temp[k] = state[i][k];
		}
		for (int j = 0; j < 4; ++j) {
			state[i][j] = temp[(i+j)%4];
		}
	}
}

/**
 * Performs specified operations on the State column by column
 * treating each column as a four-term polynomial. See AES spec for
 * more in-depth description.
 * @param state The state/block being encrypted
 */
void mixColumns(unsigned char (&state)[4][4]) {
	// Copy the state because we are going to need all the old values
	unsigned char stateCopy[4][4];
	for (int x = 0; x < 4; ++x) {
		for (int y = 0; y < 4; ++y) {
			stateCopy[x][y] = state[x][y];
		}
	}
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			// Different calculation depending on which column we are working on.
			if (j == 0) {
				state[j][i] = (multiByTwoInGF(stateCopy[0][i]))^(multiByThreeInGF(stateCopy[1][i]))^(stateCopy[2][i])^(stateCopy[3][i]);
			} else if (j == 1) {
				state[j][i] = (stateCopy[0][i])^(multiByTwoInGF(stateCopy[1][i]))^(multiByThreeInGF(stateCopy[2][i]))^(stateCopy[3][i]);
			} else if (j == 2) {
				state[j][i] = (stateCopy[0][i])^(stateCopy[1][i])^(multiByTwoInGF(stateCopy[2][i]))^(multiByThreeInGF(stateCopy[3][i]));
			} else if (j == 3) {
				state[j][i] = (multiByThreeInGF(stateCopy[0][i]))^(stateCopy[1][i])^(stateCopy[2][i])^(multiByTwoInGF(stateCopy[3][i]));
			}
		}
	}
}

/**
 * Performs multiplication by 2 on the given value in GF(2^8)
 * @param  value The value that is multipltied by two.
 * @return       The result of the multiplication
 */
unsigned char multiByTwoInGF(unsigned char value) {
	// If the highest bit is set in the value we are working on we have to XOR the
	// multiplication (bit shift left by 1) with a constant to prevent overflow.
	const unsigned char overFlowConstant = 0x1b;
	unsigned char result = (value >= 0x80 ? (value << 1)^overFlowConstant : value << 1);
	return result;
}

/**
 * Performs multiplication by 3 on the given value in GF(2^8).
 * Multiplying by 3 in GF(2^8) can be defined as:
 * (2*value) XOR value, where * is multiplication in GF(2^8).
 * @param  value The value that is multipltied by three.
 * @return       The result of the multiplication
 */
unsigned char multiByThreeInGF(unsigned char value) {
	// If the highest bit is set in the value we are working on we have to XOR the
	// multiplication (bit shift left by 1) with a constant to prevent overflow.
	const unsigned char overFlowConstant = 0x1b;
	unsigned char result = (value >= 0x80 ? (value << 1)^overFlowConstant : value << 1)^value;
	return result;
}

/**
 * Prints the state to stdout in the correct order.
 */
void printState(unsigned char (&state)[4][4]) {
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			cout << state[j][i];
		}
	}
}

/**
 * Takes 4 bytes in the form of unsigned char and converts them to one 32-bit word in the form of a char32_t.
 * Helper function for keyExpansion(). OBS: Does not consider different endiens...sloppy
 * @param  byte1 first byte, left most bits in the resulting word
 * @param  byte2 second byte, second left most bits in the resulting word
 * @param  byte3 third byte, third left most bits in the resulting word
 * @param  byte4 fourth byte, right most bits in the resulting word
 * @return       Result in form of char32_t.
 */
char32_t toWord(unsigned char byte1, unsigned char byte2, unsigned char byte3, unsigned char byte4) {
	char32_t resultWord = 0x00;
	resultWord = (resultWord | byte1) << 8;
	resultWord = (resultWord | byte2) << 8;
	resultWord = (resultWord | byte3) << 8;
	resultWord = resultWord | byte4;
	return resultWord;
}

/**
 * Takes the given 4byte word and stores it in the array bytes as bytes.
 * @param word  The word being coverted to bytes
 * @param bytes Bytes of the word where the lowest indexed byte contains the most significant bits of the word.
 */
void toBytes(char32_t word, unsigned char (&bytes)[4]) {
	for (int i = 0; i < 4; ++i) {
		bytes[i] = (word >> 24-(8*i)) & 0x000000ff;
	}
}

/**
 * Applies the AES S-box to each byte in the word.
 * @param word word to apply S-box on
 * @return     The result of the substituion in the form of a 32-bit word
 */
char32_t subWord(char32_t word) {
	unsigned char bytes[4];
	// Get the bytes in the word and store them in the array.
	toBytes(word, bytes);
	// perform substitution
	unsigned char leftIndex, rightIndex;
	for (int i = 0; i < 4; ++i) {
		// split byte into 4-bit parts
		leftIndex = bytes[i] >> 4;
		rightIndex = bytes[i] & 0x0f;
		bytes[i] = sBox[leftIndex][rightIndex];
	}
	// turn the bytes into a word and then return.
	return toWord(bytes[0], bytes[1], bytes[2], bytes[3]);

}

/**
 * Performs a cyclic permutation on the input word [a0,a1,a2,a3](bytes in  input word)
 * with the result being [a1,a2,a3,a0](bytes in output word)
 * @param word The word to be permutaded.
 */
void rotWord(char32_t &word) {
	char32_t temp = word;
	temp = temp >> 24; // now temp = a0
	word = word << 8;
	word = word | temp;
}

// Used to read in a test key found in the AES spec. ignore this function
void readTestKey(unsigned char (&key)[keyLength]) {
	key[0] = 0x2b;
	key[1] = 0x7e;
	key[2] = 0x15;
	key[3] = 0x16;
	key[4] = 0x28;
	key[5] = 0xae;
	key[6] = 0xd2;
	key[7] = 0xa6;
	key[8] = 0xab;
	key[9] = 0xf7;
	key[10] = 0x15;
	key[11] = 0x88;
	key[12] = 0x09;
	key[13] = 0xcf;
	key[14] = 0x4f;
	key[15] = 0x3c;
}
// Used for testing, ignore this function
void testInitState(unsigned char (&state)[4][4]) {
	state[0][0] = 0x32;
	state[1][0] = 0x43;
	state[2][0] = 0xf6;
	state[3][0] = 0xa8;
	state[0][1] = 0x88;
	state[1][1] = 0x5a;
	state[2][1] = 0x30;
	state[3][1] = 0x8d;
	state[0][2] = 0x31;
	state[1][2] = 0x31;
	state[2][2] = 0x98;
	state[3][2] = 0xa2;
	state[0][3] = 0xe0;
	state[1][3] = 0x37;
	state[2][3] = 0x07;
	state[3][3] = 0x34;

}

// Used for testing, ignore this function
void printKey(unsigned char (&key)[keyLength]) {
	for (int i = 0; i < keyLength; ++i) {
		cout << hex << (unsigned int)key[i] << " ";
	}
	cout << endl;
}
