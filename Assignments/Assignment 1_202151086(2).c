/*Name :- Ingle Manas Jitendrakumar     Student ID :- 202151086*/
#include<stdio.h>
#include<string.h>

#define SIZE 30

void adjustPlaintext(char plaintext[], char adjusted_plaintext[]) {
    int i, j, length, new_length = 0;

    length = strlen(plaintext);

    // Adjust the length of the plaintext and handle repetition of letters
    // according to the rules of the Playfair cipher
    for (i = 0; i < length; i++) {
        if ((plaintext[i] == plaintext[i + 1]) && i%2==0) {
            adjusted_plaintext[new_length++] = plaintext[i];
            adjusted_plaintext[new_length++] = (plaintext[i] == 'X' ? 'Z' : 'X'); // Inserting 'X' or 'Z'
        } else {
            adjusted_plaintext[new_length++] = plaintext[i];
        }
    }

    // Check if the adjusted plaintext has odd length and ends with 'X'
    if (new_length % 2 != 0 && adjusted_plaintext[new_length - 1] == 'X') {
        adjusted_plaintext[new_length++] = 'Z'; // Add 'Z' at the end to avoid double character issue
    }

    // Check if the adjusted plaintext has odd length and ends with 'Z'
    if (new_length % 2 != 0 && adjusted_plaintext[new_length - 1] == 'Z') {
        adjusted_plaintext[new_length++] = 'X'; // Add 'X' at the end to make the size even
    }
    
    if (new_length % 2 != 0) {
        adjusted_plaintext[new_length++] = 'X'; // Add 'X' at the end to make the size even
    }
    
}

// Function to check if a character is present in the table
int check(char table[6][5], char k) {
    int i, j;
    for (i = 0; i < 6; ++i) {
        for (j = 0; j < 5; ++j) {
            if (table[i][j] == k)
                return 0;
        }
    }
    return 1;
}

// Function to generate the key table matrix
void generateKeyTable(char key[], int ks, char keyT[6][5]) {
    int i, j, k = 0;
    char alphabet[30] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ,.?;"; // Including symbols

    // Inserting the key into the table
    for (i = 0; i < 6; ++i) {
        for (j = 0; j < 5; ++j) {
            if (k < ks ) {
                if(check(keyT, key[k])){
                   keyT[i][j] = key[k++]; 
                }else{
                    k++;
                    j--;
                }
                
            } else {
                // Fill in the remaining alphabets and symbols
                while (!check(keyT, alphabet[k - ks]))
                    k++;
                keyT[i][j] = alphabet[k - ks];
                k++;
            }
        }
    }
}

// Function to encrypt using the Playfair cipher
void EncryptPlayfair(char plaintext[],char ciphertext[], char keyT[6][5]) {
    int i, j;

    // Encrypt each digraph in the adjusted plaintext using the key table
    // and store the ciphertext
    for (i = 0, j = 0; i < strlen(plaintext); i += 2) {
        char c1 = plaintext[i];
        char c2 = plaintext[i + 1];
        int r1, r2, c11, c22;

        // Find the positions of the characters in the key table
        for (int row = 0; row < 6; row++) {
            for (int col = 0; col < 5; col++) {
                if (keyT[row][col] == c1) {
                    r1 = row;
                    c11 = col;
                }
                if (keyT[row][col] == c2) {
                    r2 = row;
                    c22 = col;
                }
            }
        }

        // Encrypt the digraph based on the positions in the key table
        if (r1 == r2) {
            // Same row
            ciphertext[j++] = keyT[r1][(c11 + 1) % 5];
            ciphertext[j++] = keyT[r2][(c22 + 1) % 5];
        } else if (c11 == c22) {
            // Same column
            ciphertext[j++] = keyT[(r1 + 1) % 6][c11];
            ciphertext[j++] = keyT[(r2 + 1) % 6][c22];
        } else {
            // Forming rectangle
            ciphertext[j++] = keyT[r1][c22];
            ciphertext[j++] = keyT[r2][c11];
        }
    }

    // Print the ciphertext
    printf("\nCiphertext after Playfair Encryption (C1): ");
    for (i = 0; i < j; i++) {
        printf("%c", ciphertext[i]);
    }
    printf("\n");
}


int getIndex(char c) {
    char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ,.?;";
    for (int i = 0; i < 30; i++) {
        if (c == alphabet[i]) {
            return i;
        }
    }
    return -1; // Character not found
}

void encryptAffine(char plaintext[], char ciphertext[], int a, int b) {
    int i;
    int length = strlen(plaintext);

    for (i = 0; i < length; i++) {
        // Get the index of the current character in the alphabet set
        int index = getIndex(plaintext[i]);
        
        // Apply the encryption formula: Ci = (a * Pi + b) mod 30
        int Ci = (a * index + b) % 30;

        // Adjust Ci if it's negative
        if (Ci < 0) {
            Ci += 30;
        }

        // Convert the result back to the character representation
        ciphertext[i] = (Ci == 26) ? ',' : (Ci == 27) ? '.' : (Ci == 28) ? '?' : (Ci == 29) ? ';' : 'A' + Ci;
    }

    // Null-terminate the ciphertext string
    ciphertext[length] = '\0';
}


void encryptShift(char ciphertext[], char encrypted_text[], int shift_key) {
    int i;
    int length = strlen(ciphertext);
    char sample_space[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ,.?;";

    for (i = 0; i < length; i++) {
        char current_char = ciphertext[i];
        int index = strchr(sample_space, current_char) - sample_space; // Get the index of the current character in the sample space

        // Shift the index by the shift key amount
        int shifted_index = (index + shift_key) % strlen(sample_space);

        // Assign the encrypted character based on the shifted index
        encrypted_text[i] = sample_space[shifted_index];
    }

    // Null-terminate the encrypted text
    encrypted_text[length] = '\0';
}

// Function to decrypt the ciphertext using the Shift cipher
void decryptShift(char encrypted_text[], char decrypted_text[], int shift_key) {
    int i;
    int length = strlen(encrypted_text);
    char sample_space[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ,.?;";

    for (i = 0; i < length; i++) {
        char current_char = encrypted_text[i];
        int index = strchr(sample_space, current_char) - sample_space; // Get the index of the current character in the sample space

        // Reverse the shift operation by subtracting the shift key amount
        int shifted_index = (index - shift_key + strlen(sample_space)) % strlen(sample_space);

        // Assign the decrypted character based on the shifted index
        decrypted_text[i] = sample_space[shifted_index];
    }

    // Null-terminate the decrypted text
    decrypted_text[length] = '\0';
}


int modInverse(int a, int m) {
    a = a % m;
    for (int x = 1; x < m; x++) {
        if ((a * x) % m == 1) {
            return x;
        }
    }
    return -1; // No modular inverse exists
}
void decryptAffine(char ciphertext[], char plaintext[], int a, int b) {
    int i;
    int length = strlen(ciphertext);

    // Get the modular multiplicative inverse of 'a' modulo 30
    int a_inv = modInverse(a, 30);

    for (i = 0; i < length; i++) {
        // Get the index of the current character in the ciphertext alphabet set
        int index = getIndex(ciphertext[i]);
        
        // Apply the decryption formula: Pi = (a_inv * (Ci - b)) mod 30
        int Pi = (a_inv * (index - b)) % 30;

        // Adjust Pi if it's negative
        if (Pi < 0) {
            Pi += 30;
        }

        // Convert the result back to the character representation
        plaintext[i] = (Pi == 26) ? ',' : (Pi == 27) ? '.' : (Pi == 28) ? '?' : (Pi == 29) ? ';' : 'A' + Pi;
    }

    // Null-terminate the plaintext string
    plaintext[length] = '\0';
}

void DecryptPlayfair(char ciphertext[], char decryptedtext[], char keyT[6][5]) {
    int i, j;

    // Decrypt each digraph in the ciphertext using the key table
    // and store the decrypted text
    for (i = 0, j = 0; i < strlen(ciphertext); i += 2) {
        char c1 = ciphertext[i];
        char c2 = ciphertext[i + 1];
        int r1, r2, c11, c22;

        // Find the positions of the characters in the key table
        for (int row = 0; row < 6; row++) {
            for (int col = 0; col < 5; col++) {
                if (keyT[row][col] == c1) {
                    r1 = row;
                    c11 = col;
                }
                if (keyT[row][col] == c2) {
                    r2 = row;
                    c22 = col;
                }
            }
        }

        // Decrypt the digraph based on the positions in the key table
        if (r1 == r2) {
            // Same row
            decryptedtext[j++] = keyT[r1][(c11 + 4) % 5]; // Decryption is the same as encryption but using subtraction instead
            decryptedtext[j++] = keyT[r2][(c22 + 4) % 5]; // and handling wrap-around
        } else if (c11 == c22) {
            // Same column
            decryptedtext[j++] = keyT[(r1 + 5) % 6][c11];
            decryptedtext[j++] = keyT[(r2 + 5) % 6][c22];
        } else {
            // Forming rectangle
            decryptedtext[j++] = keyT[r1][c22];
            decryptedtext[j++] = keyT[r2][c11];
        }
    }

    // Null-terminate the decrypted text
    decryptedtext[j] = '\0';
}



int main() {
    char key[SIZE];
    char keyT[6][5];
    char plaintext[2 * SIZE]; // Doubled size to accommodate possible diagraphs
    char Adj_plaintext[2 * SIZE];
    char Playfair_encrypted[2 * SIZE];
    char Affine_encrypted[2 * SIZE];
    char Shift_encrypted[2 * SIZE];
    char Shift_decrypted[2 * SIZE];
    char Affine_decrypted[2 * SIZE];
    char Playfair_decrypted[2 * SIZE];

    // Input plaintext
    printf("\nEnter the Plain Text: ");
    scanf("%s", plaintext);
    
    printf("\nRules used for Adjustment of plaintext: \n");
    printf("If any repeating consucutive letters are present[only in case they form diagraphs] we insert X \n");
    printf("In the case no of characters are odd [As they can't form diagraphs] we insert X or Z(in case X is last element to avoid XX diagraph)\n");
    printf("And in the case when there is repeated X(Consecutivelly) or single X at end we insert Z instead of X to avoid XX diagraph(Example'REX'makes'REXX') \n");
    printf("According to Playfair Cipher correct Adjustment of APPLE should be = APPLEX as there is no conflict of diagraphs due to P's as AP is 1st PL is 2nd diagraph \n");
    printf("But in HELLO Adjustment should be HELXLO as now LL forms an diagraph conflict.\n");
    printf("Also for example 'NIXXON' instead of adding X that will create diagraph conflict we use Z \n");
    printf("############################################################################################################################################");
    adjustPlaintext(plaintext, Adj_plaintext);
    printf("\n\nAdjusted Plain Text (∆) : %s\n",Adj_plaintext);
    // Input key
    printf("Enter the Key for Playfair Matrix: ");
    scanf("%s", key);

    // Generate the key table matrix
    generateKeyTable(key, strlen(key), keyT);

    // Display the key table matrix
    printf("\nThe Key Table Matrix is as follows:\n");
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 5; j++) {
            printf("%c ", keyT[i][j]);
        }
        printf("\n");
    }
    // Encrypt the adjusted plaintext using the Playfair cipher
    EncryptPlayfair(Adj_plaintext,Playfair_encrypted, keyT);
    
    int a = 11; // Multiplicative key
    int b = 15; // Additive key

    // Encrypt the adjusted plaintext using the Affine cipher
    encryptAffine(Playfair_encrypted, Affine_encrypted, a, b);

    // Print the ciphertext
    printf("Ciphertext after Affine Encryption(C2): %s\n",Affine_encrypted);
    
    int shift_key; // Shift key K3
    printf("\nEnter the Shift Key: ");
    scanf("%d",&shift_key);

    // Encrypt the ciphertext using the Shift cipher
    encryptShift(Affine_encrypted, Shift_encrypted, shift_key);

    // Print the encrypted text (C3)
    printf("\nCiphertext after Shift Encryption (C3): %s\n", Shift_encrypted);
    
    // Decrypt the encrypted text using the Shift cipher
    decryptShift(Shift_encrypted, Shift_decrypted, shift_key);
    
    // Print the decrypted text
    printf("Decrypted Ciphertext after Shift Decryption D1 : %s\n", Shift_decrypted);
    
    // Decrypt the ciphertext using the Affine cipher
    decryptAffine(Shift_decrypted, Affine_decrypted, a, b);

    // Print the decrypted plaintext
    printf("Decrypted Ciphertext after Affine Decryption D2 : %s\n",Affine_decrypted);
    
    DecryptPlayfair(Affine_decrypted,Playfair_decrypted,keyT);
    
    // Print the decrypted plaintext
    printf("Decrypted Ciphertext after Playfair Decryption D3 : %s\n",Playfair_decrypted);


    return 0;
}