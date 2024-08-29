//Name - Manas Jitendrakumar Ingle
// Student Id - 202151086
// Lab Assignment 4

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
// #include <cstdint>

#define RotRight(word, bits) (((word) >> (bits)) | ((word) << (32 - (bits))))
#define CH(x, y, z) (((x) & (y)) ^ (~(x) & (z)))
#define MAJ(x, y, z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define EP0(x) (RotRight(x, 2) ^ RotRight(x, 13) ^ RotRight(x, 22))
#define EP1(x) (RotRight(x, 6) ^ RotRight(x, 11) ^ RotRight(x, 25))
#define SIG0(x) (RotRight(x, 7) ^ RotRight(x, 18) ^ ((x) >> 3))
#define SIG1(x) (RotRight(x, 17) ^ RotRight(x, 19) ^ ((x) >> 10))


// Define the prime number p
#define P 1021

// Defining the coefficients of the elliptic curve eqn: y^2 = x^3 + ax + b
#define A 449
#define B 233


// // Calculating modular inverse of 'a' modulo 'm' using Extended Euclidean Algorithm
int modInverse(int a, int m) {
    int m0 = m, t, q;
    int x0 = 0, x1 = 1;
    if (m == 1)
        return 0;
    // Apply extended Euclid Algorithm
    while (a > 1) {
        // q is quotient
        q = a / m;
        t = m;
        // m is remainder now, process same as euclid's algo
        m = a % m, a = t;
        t = x0;
        x0 = x1 - q * x0;
        x1 = t;
    }
    // Make x1 positive
    if (x1 < 0)
        x1 += m0;
    return x1;
}

unsigned int* oper(unsigned int pt1[2], unsigned int pt2[2]) {
    int x1, y1, x2, y2;
    x1 = pt1[0];
    y1 = pt1[1];
    x2 = pt2[0];
    y2 = pt2[1];
    unsigned int* result = malloc(2 * sizeof(unsigned int));

    if (x1 == 0 && y1 == 1) {
        result[0] = x2;
        result[1] = y2;
        return result;
    }
    
    // Case when x1 != x2 
    else if (pt1[0] != pt2[0]) {
        int slope = ((y2 - y1 + P) % P) * (modInverse((x2 - x1 + P) % P, P)) % P;

        result[0] = ((slope * slope) % P - x1 - x2 + 2 * P) % P;
        result[1] = ((slope * (x1 - result[0] + P) % P) % P - y1 + P) % P;

        return result;
    }
    // Case when pt1 == pt2 
    else if (x1 == x2 && y1 == y2) {
        int slope = ((3 * x1 * x1 + A) % P) * (modInverse((2 * y1) % P, P) % P) % P;

        result[0] = ((slope * slope) % P - x1 - x2 + 2 * P) % P;
        result[1] = (slope * ((x1 - result[0] + P) % P) % P - y1 + P) % P;

        return result;
    }
    else if (x1 == x2 && y1 == P - y2) {
        result[0] = 0;
        result[1] = 1;
        return result;
    }

    return result;
}

// Scalar multiplication using double-and-add algorithm
void deffieHellmanKey(unsigned int pt[2], unsigned int n, unsigned int result[2]) {
    result[0] = pt[0];
    result[1] = pt[1];
    for (int i = 1; i < n; i++) 
    {
        oper(result, pt); 
    }
}



void selectPoint(unsigned int alpha[2]) {
    int x, y;
    for (x = 0; x < P; ++x) {
        for (y = 1; y < P; ++y) {
            if (y * y == (x * x * x + A * x + B) % P) {
                alpha[0] = x;
                alpha[1] = y;
                return;
            }
        }
    }
   
}






// SHA 256
const uint32_t K[] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
    0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
    0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
    0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
    0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
    0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
    0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
    0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
    0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

void sha256(const uint8_t *msg, uint32_t len, uint8_t *hash) {
    uint32_t H[8] = {
        0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
        0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
    };
    uint32_t W[64];
    uint32_t a, b, c, d, e, f, g, h, i, j, t1, t2;

    // Padding
    uint64_t new_len = ((len + 8) / 64 + 1) * 64;
    uint8_t *msg_padded = (uint8_t *)calloc(new_len, 1);
    memcpy(msg_padded, msg, len);
    msg_padded[len] = 0x80; // append single '1' bit
    for (i = len + 1; i < new_len - 8; i++) // pad with '0' bits
        msg_padded[i] = 0;
    uint64_t bit_len = len * 8; // length of the message in bits
    bit_len = (((bit_len >> 56) & 0xff) |
               ((bit_len >> 40) & 0xff00) |
               ((bit_len >> 24) & 0xff0000) |
               ((bit_len >> 8) & 0xff000000) |
               ((bit_len << 8) & 0xff00000000) |
               ((bit_len << 24) & 0xff0000000000) |
               ((bit_len << 40) & 0xff000000000000) |
               ((bit_len << 56) & 0xff00000000000000));
    memcpy(msg_padded + new_len - 8, &bit_len, 8); // append bit length in big-endian

    for (i = 0; i < new_len; i += 64) {
        for (j = 0; j < 16; j++) {
            W[j] = (msg_padded[i + (j * 4)] << 24) | (msg_padded[i + (j * 4) + 1] << 16) | (msg_padded[i + (j * 4) + 2] << 8) | (msg_padded[i + (j * 4) + 3]);
        }
        for (j = 16; j < 64; j++) {
            W[j] = SIG1(W[j - 2]) + W[j - 7] + SIG0(W[j - 15]) + W[j - 16];
        }
        a = H[0];
        b = H[1];
        c = H[2];
        d = H[3];
        e = H[4];
        f = H[5];
        g = H[6];
        h = H[7];
        for (j = 0; j < 64; j++) {
            t1 = h + EP1(e) + CH(e, f, g) + K[j] + W[j];
            t2 = EP0(a) + MAJ(a, b, c);
            h = g;
            g = f;
            f = e;
            e = d + t1;
            d = c;
            c = b;
            b = a;
            a = t1 + t2;
        }
        H[0] += a;
        H[1] += b;
        H[2] += c;
        H[3] += d;
        H[4] += e;
        H[5] += f;
        H[6] += g;
        H[7] += h;
    }

    // Copy hash values to the hash array
    for (i = 0; i < 8; i++) {
        hash[i * 4] = (H[i] >> 24) & 0xFF;
        hash[i * 4 + 1] = (H[i] >> 16) & 0xFF;
        hash[i * 4 + 2] = (H[i] >> 8) & 0xFF;
        hash[i * 4 + 3] = H[i] & 0xFF;
    }

    free(msg_padded);
}


// Function to concatenate two integers and dynamically allocate memory for the result string
char* concatenateIntegers(int num1, int num2) {
    // Calculate the total length of the concatenated string
    int len = snprintf(NULL, 0, "%d%d", num1, num2) + 1;

    // Dynamically allocate memory for the result string
    char *result = (char*)malloc(len * sizeof(char));
    if (result == NULL) {
        printf("Memory allocation failed!\n");
        exit(EXIT_FAILURE);
    }

    // Use snprintf to concatenate integers and convert them into a string
    snprintf(result, len, "%d%d", num1, num2);

    return result;
}

// Function to convert a hexadecimal character to its integer representation
int hexCharToInt(char hexChar) {
    if (hexChar >= '0' && hexChar <= '9') {
        return hexChar - '0';
    } else if (hexChar >= 'a' && hexChar <= 'f') {
        return 10 + hexChar - 'a';
    } else if (hexChar >= 'A' && hexChar <= 'F') {
        return 10 + hexChar - 'A';
    } else {
        return -1; // Invalid character
    }
}

// Function to convert a pair of hexadecimal characters to its byte value
int hexToByte(char hex1, char hex2) {
    return (hexCharToInt(hex1) << 4) | hexCharToInt(hex2);
}

// AES Encryption/Decryption algo
unsigned char sbox[256] = {
//0     1    2      3     4    5     6     7      8    9     A      B    C     D     E     F
  0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,	// 0
  0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,	// 1
  0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,	// 2
  0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,	// 3
  0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,	// 4
  0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,	// 5
  0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,	// 6
  0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,	// 7
  0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,	// 8
  0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,	// 9
  0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,	// A
  0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,	// B
  0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,	// C
  0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,	// D
  0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,	// E
  0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16	// F
};

unsigned char Rcon[10] = {
  0x01, 0x02, 0x04, 0x08, 0x10,
  0x20, 0x40, 0x80, 0x1b, 0x36
};

unsigned char PolyMulti(unsigned char a, unsigned char b)
{
  unsigned char p = 0;  
  unsigned char hi_bit_set;  // Variable to store the highest bit of a during multiplication
  for (int i = 0; i < 8; i++)   // Looping through each bit of b
	{
	  if (b & 1)            // Check if the least significant bit of b is set
		p ^= a;             // If it is, XOR the result with a
	  hi_bit_set = (a & 0x80);
	  a <<= 1;
	  if (hi_bit_set)
		a ^= 0x1b;				// Applying modular reduction by x^8 + x^4 + x^3 + x + 1
	  b >>= 1;
	}
  return p;
}

unsigned sub_word (unsigned word)
{
  // Applying the S-box substitution to each byte of the input word
  return (unsigned) (sbox[(unsigned char) (word >> 24)] << 24) |
	(unsigned) (sbox[(unsigned char) (word >> 16)] << 16) |
	(unsigned) (sbox[(unsigned char) (word >> 8)] << 8) |
	(unsigned) (sbox[(unsigned char) (word)]);
}


unsigned rot_word (unsigned word)
{
  
  return word << 8 | word >> 24;    // Rotate the input word left by 8 bits
}


void key_expansion(unsigned char key[16], unsigned w[44])
{
  unsigned temp;        // Temporary variable for key expansion

    // Copy the first 4 words of the key directly into the round keys
  for (int i = 0; i < 4; i++)
	{
	  w[i] =
		(unsigned) (key[4 * i] << 24) | (unsigned) (key[4 * i + 1] << 16) |
		(unsigned) (key[4 * i + 2] << 8) | (unsigned) key[4 * i + 3];
	}
// Performing key expansion to generate the remaining round keys
  for (int i = 4; i < 44; i++)
	{
	  temp = w[i - 1];
	  if (i % 4 == 0)
		{
		    // Applying substitution and round constant to every fourth word
		  temp = sub_word (rot_word (temp)) ^ ((unsigned)Rcon[i / 4 - 1]<<24);
		}
	  w[i] = w[i - 4] ^ temp;   // Generating each round key using a combination of previous keys and temp
	}
}

unsigned char subbyteCompress(unsigned char value) {
    unsigned char polyProduct = PolyMulti(value, 221) ^ 125;// Perform polynomial multiplication and XOR operation with constant
    return sbox[polyProduct];   // Perform S-box substitution on the result and return
}

// Function to apply SubByte opern to the entire state matrix
void findSubByte(unsigned char state[4][4]) {
    // Iterating over each element of the state matrix
    for(int i=0; i<4; i++) {
        // Applying SubByte opern with compression of each element
        state[0][i] = subbyteCompress(state[0][i]);
        state[1][i] = subbyteCompress(state[1][i]);
        state[2][i] = subbyteCompress(state[2][i]);
        state[3][i] = subbyteCompress(state[3][i]);
    }
}

void shift_rows(unsigned char state[4][4]) {
    unsigned char temp;
// Shifting elements in the rows of the state matrix
    // Shift row 1
    temp = state[1][0]; // Temporary variable for swapping elements
    state[1][0] = state[1][1];
    state[1][1] = state[1][2];
    state[1][2] = state[1][3];
    state[1][3] = temp;

    // Shift row 2
    temp = state[2][0];
    state[2][0] = state[2][2];
    state[2][2] = temp;
    temp = state[2][1];
    state[2][1] = state[2][3];
    state[2][3] = temp;

    // Shift row 3
    temp = state[3][0];
    state[3][0] = state[3][3];
    state[3][3] = state[3][2];
    state[3][2] = state[3][1];
    state[3][1] = temp;
}

// Performing the MixColumns operation on the state matrix
void mix_columns(unsigned char state[4][4]) {
    // Defining the mix matrix for the MixColumns operation as given in Assignment
    unsigned char mix_matrix[4][4] = {
        {1, 4, 4, 5},
        {5, 1, 4, 4},
        {4, 5, 1, 4},
        {4, 4, 5, 1}
    };
    unsigned char result[4][4];// Initializing a result matrix to store the result of the MixColumns operation

    // Iterating through each element of the state matrix
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            result[i][j] = 0;
            for(int k = 0; k < 4; k++) {
                result[i][j] = result[i][j] ^ PolyMulti(mix_matrix[i][k], state[k][j]);
            }
        }
    }
        // Updating the state matrix with the result of the MixColumns operation
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            state[i][j] = result[i][j];
        }
    }
}
// Adding round keys to the state matrix
void add_round_keys(unsigned char state[4][4], unsigned w[44], int roundNumber) {
    for(int i=0; i<4; i++) {
        // XORing each element of the state matrix with corresponding round keys
        state[0][i] = state[0][i] ^ (w[4*roundNumber + i]>>24);
        state[1][i] = state[1][i] ^ (w[4*roundNumber + i]>>16);
        state[2][i] = state[2][i] ^ (w[4*roundNumber + i]>>8);
        state[3][i] = state[3][i] ^ (w[4*roundNumber + i]);
    }
}

// Performing AES encryption on the state matrix
void aes_encryption(unsigned char plainText[16],unsigned char key[16],unsigned char cipherText[16]) {

    unsigned char state[4][4];

    for(int i=0;i<4;i++) {
        for(int j=0;j<4;j++) {
            state[j][i] = plainText[i*4 + j];
        }
    }

    unsigned int word[44];
    
    key_expansion(key,word);

    add_round_keys(state,word,0);
    for(int i=1; i<=9; i++) {
        // Applying SubBytes, ShiftRows, MixColumns, and AddRoundKeys operations
        findSubByte(state);
        shift_rows(state);
        mix_columns(state);
        add_round_keys(state,word,i);
    }
    // Applying SubBytes, ShiftRows, and AddRoundKeys operations for the final round
    findSubByte(state);
    shift_rows(state);
    add_round_keys(state,word,10);

    for(int i=0;i<4;i++) {
        for(int j=0;j<4;j++) {
            cipherText[i*4 + j] = state[j][i];
        }
    }
}

void inv_shift_rows(unsigned char state[4][4]) {
    unsigned char inverse;

    // Shift row 1
    inverse = state[1][3];
    state[1][3] = state[1][2];
    state[1][2] = state[1][1];
    state[1][1] = state[1][0];
    state[1][0] = inverse;

    // Shift row 2
    inverse = state[2][0];
    state[2][0] = state[2][2];
    state[2][2] = inverse;
    inverse = state[2][1];
    state[2][1] = state[2][3];
    state[2][3] = inverse;

    // Shift row 3
    inverse = state[3][0];
    state[3][0] = state[3][1];
    state[3][1] = state[3][2];
    state[3][2] = state[3][3];
    state[3][3] = inverse;
}

void inv_mix_columns(unsigned char state[4][4]) {
    
    unsigned char mix_matrix[4][4] = {
        {165, 7, 26, 115},
        {115, 165, 7, 26},
        {26, 115, 165, 7},
        {7, 26, 115, 165}
    };
    unsigned char result[4][4];

    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            result[i][j] = 0;
            for(int k = 0; k < 4; k++) {
                result[i][j] = result[i][j] ^ PolyMulti(mix_matrix[i][k], state[k][j]);
            }
        }
    }

    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            state[i][j] = result[i][j];
        }
    }
}

unsigned char inverseLookup(unsigned char ms) {
    for(int i=0; i<256; i++) {
        if(sbox[i] == ms) {
            return i;
        }
    }
    return 0;
}

unsigned char inverse_subByte(unsigned char ms) {
    unsigned char lookUp = inverseLookup(ms);
    return PolyMulti(lookUp^125,248);
}

void inverse_SubByte_State(unsigned char state[4][4]) {
    for(int i=0; i<4; i++) {
        state[0][i] = inverse_subByte(state[0][i]);
        state[1][i] = inverse_subByte(state[1][i]);
        state[2][i] = inverse_subByte(state[2][i]);
        state[3][i] = inverse_subByte(state[3][i]);
    }
}

void aes_decryption(unsigned char Text[16],unsigned char key[16],unsigned char decryptedText[16]) {

    unsigned char state[4][4];
    for(int i=0;i<4;i++) {
        for(int j=0;j<4;j++) {
            state[j][i] = Text[i*4 + j];
        }
    }

    unsigned int word[44];
    key_expansion(key,word);
    add_round_keys(state,word,10);
    inv_shift_rows(state);
    inverse_SubByte_State(state);
    for(int i=9; i>=1; i--) {
        
        add_round_keys(state,word,i);
        inv_mix_columns(state);
        inv_shift_rows(state);
        inverse_SubByte_State(state);
        
    }
    add_round_keys(state,word,0);

    for(int i=0;i<4;i++) {
        for(int j=0;j<4;j++) {
            decryptedText[i*4 + j]=state[j][i];
        }
    }
}

// print Bytes
// void printBytes(unsigned char *bytes, int length) {
//     for (int i = 0; i < length; i++) {
//         printf("%02x ", bytes[i]);
//     }
//     printf("\n");
// }



int main() {

   unsigned int alpha[2];
   selectPoint(alpha);

    printf("Point alpha: (%d, %d)\n", alpha[0], alpha[1]);

    // Step 6: Perform Diffie-Hellman key exchange using Alice's and Bob's private keys
    int nA, nB;
    printf("Enter Alice's private key nA (1 to 330): ");
    scanf("%d", &nA);
    printf("Enter Bob's private key nB (1 to 330): ");
    scanf("%d", &nB);


    unsigned int secretKeyAlice[2];
    unsigned int secretKeyBob[2];

    deffieHellmanKey(alpha, nA, secretKeyAlice);
    deffieHellmanKey(alpha, nB, secretKeyBob);


    deffieHellmanKey(secretKeyAlice, nB, secretKeyAlice);
    deffieHellmanKey(secretKeyBob, nA, secretKeyBob);


    // // Print the shared secret keys
    printf("Shared secret key for Alice Ka: (%d, %d)\n", secretKeyAlice[0], secretKeyAlice[1]);
    printf("Shared secret key for Bob Kb: (%d, %d)\n", secretKeyBob[0], secretKeyBob[1]);

    unsigned int finalKey[2];
    finalKey[0] = secretKeyAlice[0] & secretKeyBob[0];
    finalKey[1] = secretKeyAlice[1] & secretKeyBob[1];

    
    //SHA 256 on alice
    unsigned char hash[32];
    const unsigned char *msg = concatenateIntegers(finalKey[0],finalKey[1]);
    printf("Message: %s\n", msg);
    sha256((const uint8_t *)msg, strlen(msg), hash);

    
    unsigned char *Ka = hash;
    unsigned char *Kb = hash;
    // both Ka and Kb are same so we know that hash for both is going to be the same
    unsigned char k1[16];
    unsigned char k2[16];

    memcpy(k1, Ka, 16);
    memcpy(k2, Ka + 16, 16);
    //memcpy(k1, Ka + 8, 16);

    printf("Ka: ");
    //printBytes(Ka, 32);
    for (int i = 0; i < 32; i++) {
        printf("%02x ", Ka[i]);
    }
    printf("\n");
    printf("\n");
    
    printf("Kb: ");
    //printBytes(Kb, 32);
    for (int i = 0; i < 32; i++) {
        printf("%02x ", Kb[i]);
    }
    printf("\n");
    printf("\n");
   



    unsigned char Ma[16]; // Array to store 16 bytes input

    printf("input Alice's 128-bit message Ma : ");
    for(int i=0;i<16;i++){
        scanf("%hhx",&Ma[i]);
    }

    // triple AES enc

    unsigned char ct1[16];
    unsigned char ct2[16];
    unsigned char ct3[16];
    
    aes_encryption(Ma, k1,ct1);

    aes_decryption(ct1, k2,ct2);

    aes_encryption(ct2, k1,ct3);

    //Output encrypted message
    printf("Encrypted message Ca: ");
    for (int i = 0; i < 16; i++) {
        printf("%02x ", ct3[i]);
    }
    printf("\n");
    //printBytes(ct3, 16);
    //printf("\n");


    // find MACa
    
    unsigned char Kax[32];
    for (int i = 0; i < 31; i++) {
        Kax[i] = Ka[i];
    }
    Kax[31] = Ka[31]^215;

    

    // Create a new array to store the concatenated values
    unsigned char Cnct1[48];

    // Copy Ma to the beginning of concatenated
    memcpy(Cnct1, Kax, sizeof(Kax));

    // Copy Kax to the end of concatenated
    memcpy(Cnct1 + sizeof(Kax), Ma, sizeof(Ma));

    


    unsigned char hash2[32];

    sha256((const uint8_t *)Cnct1, strlen(Cnct1), hash2);

    printf("\n");
   
    unsigned char Kaz[32];
    for (int i = 0; i < 31; i++) {
        Kaz[i] = Ka[i];
    }
    Kaz[31] = Ka[31]^125;

   

    // Create a new array to store the concatenated values
    unsigned char Cnct2[64];

    // Copy Ma to the beginning of concatenated
    memcpy(Cnct2, Kaz, sizeof(Kaz));

    // Copy Kax to the end of concatenated
    memcpy(Cnct2 + sizeof(Kaz), Cnct2, sizeof(Cnct2));

    

    uint8_t hash3[32];

    sha256((const uint8_t *)Cnct2, strlen(Cnct2), hash3);

    //printf("\n");
    // Print the hash as hexadecimal
    printf("SHA-256 Hash MACa is: ");
    for (int i = 0; i < 32; i++)
        printf("%02x ", hash3[i]);
    printf("\n");

    // triple AES dec. (since Ka and Kb are same, k1 and k2 will also be the same)
    
    unsigned char De[16];
    unsigned char De1[16];
    unsigned char De2[16];
    
     aes_decryption(ct3, k1,De);

     aes_encryption(De, k2,De1);

     aes_decryption(De1, k1,De2);

    // now find MACb
    
    unsigned char Kaxx[32];
    for (int i = 0; i < 31; i++) {
        Kaxx[i] = Kb[i];
    }
    Kaxx[31] = Kb[31]^215;

    


    // Create a new array to store the concatenated values
    unsigned char Cnct1x[48];

    // Copy Ma to the beginning of concatenated
    memcpy(Cnct1x, Kaxx, sizeof(Kaxx));

    


    unsigned char hash22[32];

    sha256((uint8_t *)Cnct1x, strlen(Cnct1x), hash22);

    printf("\n");
   

    unsigned char Kaz2[32];
    for (int i = 0; i < 31; i++) {
        Kaz2[i] = Kb[i];
    }
    Kaz2[31] = Kb[31]^125;

   
    unsigned char Cnct2x[64];

    // Copying Ma to the beginning of concatenated
    memcpy(Cnct2x, Kaz2, sizeof(Kaz2));

    // Copying Kax to the end of concatenated
    memcpy(Cnct2x + sizeof(Kaz2), Cnct2x, sizeof(Cnct2x));



    unsigned char hash32[32];

    sha256((uint8_t *)Cnct2x, strlen(Cnct2x), hash32);

    // Print the hash as hexadecimal
    printf("SHA-256 Hash MACb is: ");
    for (int i = 0; i < 32; i++)
        printf("%02x ", hash32[i]);
    printf("\n");

    printf("\n");
    //Output encrypted message
    printf("Decrypted message Mb: ");
    for (int i = 0; i < 16; i++) {
        printf("%02x ", De2[i]);
    }
    printf("\n");
    printf("\n");

    return 0;
}