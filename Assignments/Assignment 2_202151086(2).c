//Name: Manas Jitendrakumar Ingle
//Roll.no: 202151086
// Lab Assignment 2


#include <stdio.h>

// S-box lookup table
unsigned char Sbox[256] = {
      0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
     0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0,
    0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,
     0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75,
     0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84,
     0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,
     0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8,
     0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2,
     0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,
     0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB,
     0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79,
     0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,
     0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A,
     0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E,
     0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,
     0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16
};
unsigned int roundKeys[16];


// Left circular shift function
unsigned int leftRotate(unsigned int n, unsigned int a) {
    return (n << a) | (n >> (32 - a));              // Performs left circular shift
}

// Generate round keys
void generateRoundKeys(unsigned int K, unsigned int roundKeys[]) 
{
    unsigned int temp = K;              // Temporary variable to store the key
    for (int i = 1; i < 17; i++) {
        unsigned char Y0 = (temp >> 24) & 0xFF;
        unsigned char Y1 = (temp >> 16) & 0xFF; // Extracting bytes from the key
        unsigned char Y2 = (temp >> 8) & 0xFF;
        unsigned char Y3 = temp & 0xFF;

        Y0 = Sbox[Y0];      
        Y1 = Sbox[Y1];      //Performing S-box substitution
        Y2 = Sbox[Y2];
        Y3 = Sbox[Y3];
        temp = leftRotate((Y0 << 24) | (Y1 << 16) | (Y2 << 8) | Y3,i);
        roundKeys[i-1] = temp;
    }
}

// Round function
unsigned int roundFunction(unsigned int R, unsigned int K) {
    unsigned int X=R^K;             // Applying round function which involves XOR with round key
    unsigned char x0 = (X >> 24) & 0xFF;
    unsigned char x1 = (X >> 16) & 0xFF;    
    unsigned char x2 = (X >> 8) & 0xFF;
    unsigned char x3 = X & 0xFF;

    unsigned char S0 = 0;
   
    for(unsigned i=0;i<256;i++){
        if(Sbox[i]==x0){                
            S0= i;              //Finding Index of Given Elements[Inverse]
        }
    }
    
    unsigned char S1 = 0;
    for(unsigned i=0;i<256;i++){
        if(Sbox[i]==x1){
            S1= i;              //Finding Index of Given Elements[Inverse]
        }
    }
    unsigned char S2 = 0;
    for(unsigned i=0;i<256;i++){
        if(Sbox[i]==x2){
            S2= i;          //Finding Index of Given Elements[Inverse]
        }
    }
    unsigned char S3 =0;
    for(unsigned i=0;i<256;i++){
        if(Sbox[i]==x3){
            S3= i;          //Finding Index of Given Elements[Inverse]
        }
    }

    unsigned int a=(S0 << 24) | (S1 << 16) | (S2 << 8) | S3;
    return a;
}

//Feistel round operation which involves swapping and applying round function
void feistelRound(unsigned int *L, unsigned int *R, unsigned int roundKey) {
    unsigned int temp = *R;
    *R = *L ^ roundFunction(*R, roundKey);
    *L = temp;
}

// Encrypt function
unsigned long long encrypt(unsigned long long orignaltext, unsigned int key) {
    unsigned int L = orignaltext >>32;              // Splitting the original text into left and right halves
    unsigned int R = orignaltext & 0xFFFFFFFF;
    
    for (int i = 0; i < 16; ++i) {
        feistelRound(&L, &R, roundKeys[i]);         // Applying Feistel rounds using generated round keys
    }
    unsigned long long number=(unsigned long long)L << 32;
    number=number|R;                                // Combining the left and right halves to form ciphertext
    return number;
}

// Decrypt function
unsigned long long decrypt(unsigned long long ciphertext, unsigned int key) {
    
    unsigned int L = ciphertext >> 32;          // Splitting the ciphertext into left and right halves
    unsigned int R = ciphertext & 0xFFFFFFFF;

    for (int i = 15; i >= 0; --i) {
        feistelRound(&R, &L, roundKeys[i]);     // Applying Feistel rounds in reverse order using generated round keys
    }


    unsigned long long number=(unsigned long long)L << 32;
    number=number|R;                            // Combining the left and right halves to form decrypted text
    return number;
    }

int main() {
    unsigned long long orignaltext; 
    unsigned int key ;
    printf("Enter Plaintext: \n");
    scanf("%llu",&orignaltext);
    
    printf("Enter key: \n");
    scanf("%u",&key);
    generateRoundKeys(key, roundKeys);

    
    printf("Plaintext: %llu\n", orignaltext);

    printf("Key: %u\n",key);
    unsigned long long ciphertext = encrypt(orignaltext, key);
    printf("Ciphertext: %llu\n", ciphertext);

    unsigned long long decrypted = decrypt(ciphertext, key);
    printf("Decrypted text: %llu\n", decrypted);

    return 0;
}
