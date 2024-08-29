
//Name: Manas Jitendrakumar Ingle
//ID :202151086



#include <time.h>
#include <stdio.h>
#include <stdlib.h>

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
    unsigned char polyProduct = PolyMulti(value, 201) ^ 39;// Perform polynomial multiplication and XOR operation with constant
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
void aes_encryption(unsigned char state[4][4], unsigned word[44]) {
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
    return PolyMulti(lookUp^39,39);
}

void inverse_SubByte_State(unsigned char state[4][4]) {
    for(int i=0; i<4; i++) {
        state[0][i] = inverse_subByte(state[0][i]);
        state[1][i] = inverse_subByte(state[1][i]);
        state[2][i] = inverse_subByte(state[2][i]);
        state[3][i] = inverse_subByte(state[3][i]);
    }
}

void aes_decryption(unsigned char state[4][4],unsigned word[44]) {
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
}

int main ()
{
  printf("\n++++##### Question 1  and Question 2 ####++++\n");
 //We sacn m1 and m2 sequentially where m1=plaintext m2=key
  
  //Scanning m2
  unsigned char key[16];
  printf("Enter key in 16-bit hex format: ");
  for(int i=0; i<16; i++) {
      scanf("%hhx", &key[i]);
  }
  unsigned w[44];
  key_expansion(key,w);
  
  unsigned char state[4][4];
  printf("\nEnter plaintext in 16-bit hex format: ");
  for(int i=0; i<4; i++) {
      for(int j=0; j<4; j++) {
          scanf("%hhx", &state[j][i]);
      }
  }
  aes_encryption(state,w);
  unsigned char encryption[4][4];
  for(int i=0; i<4; i++) {
      for(int j=0; j<4; j++) {
          encryption[j][i] = state[j][i];
      }
  }
  printf("\nOutput of Compression Function (Encryption) : ");
  for(int i=0; i<4; i++) {
      for(int j=0; j<4; j++) {
          printf("%02x ", encryption[j][i]);
      }
  }
  
   
   printf("\n++++##### Question 3  and Question 4 ####++++\n");
 //We sacn m1 and m2 sequentially where m1=plaintext m2=key
  
  //Scanning m2 which means key for problem 3
 unsigned char key2[16];
  printf("Enter (m2) for Problem 3 in 16-bit hex format: ");
  for(int i=0; i<16; i++) {
      scanf("%hhx", &key2[i]);
  }
  unsigned w2[44];
  key_expansion(key2,w2);
  // Scanning Plaintext for Problem 3
  unsigned char state2[4][4];
  printf("\nEnter (m1) for Problem 3 in 16-bit hex format: ");
  for(int i=0; i<4; i++) {
      for(int j=0; j<4; j++) {
          scanf("%hhx", &state2[j][i]);
      }
  } 

aes_encryption(state2,w2);
  unsigned char encryption2[4][4];
  for(int i=0; i<4; i++) {
      for(int j=0; j<4; j++) {
          encryption2[j][i] = state2[j][i];
      }
  }
  printf("\nValues after Compressing (m1 and m2) : ");
  for(int i=0; i<4; i++) {
      for(int j=0; j<4; j++) {
          printf("%02x ", encryption2[j][i]);
      }
  }
  
   unsigned char rand_key[16];
  srand(time(0));
    // Generating random values for the key array
    for (int i = 0; i < 16; i++) {
        rand_key[i] = rand() % 256; // Generating random numbers between 0 and 255
    }
  unsigned w3[44];
  key_expansion(rand_key,w3);
  // ================================================================
  printf("\n\n m2': ");
  for (int i = 0; i < 16; i++) {
        printf("%02x ",rand_key[i]); // Generating random numbers between 0 and 255
    }
  
  
  
  aes_decryption(state2,w3);
  unsigned char decryption[4][4];
  for(int i=0; i<4; i++) {
      for(int j=0; j<4; j++) {
          decryption[j][i] = state2[j][i];
      }
  }
  
  printf("\n\n m1': ");
  for(int i=0; i<4; i++) {
      for(int j=0; j<4; j++) {
          printf("%02x ", decryption[j][i]);
      }
  }
  
  aes_encryption(state2,w3);
  unsigned char encryption3[4][4];
  for(int i=0; i<4; i++) {
      for(int j=0; j<4; j++) {
          encryption3[j][i] = state2[j][i];
      }
  }
  printf("\n\nOutput of Compression Function of (m1'and m2') : ");
  for(int i=0; i<4; i++) {
      for(int j=0; j<4; j++) {
          printf("%02x ", encryption3[j][i]);
      }
  }
  
  
  
  
  
  return 0;
}

// 0 1 2 3 4 5 6 7 8 9 a b c d e f
