# include	<stdio.h>
# include	<string.h>
# include	<unistd.h>

# define	ASCII		128

// Name: encode
//
// Function: encode a string using the Vigenère cipher algorithm. Shift a letter 
// (byte) forward by the ordinal number of corresponding letter in the key. 
//
// Inputs: encryption key (key) and cleartext (cleartext)
//
// Outputs: enciphered text (cihpertext)

void encode(char *key, char *cleartext, char *ciphertext)
{
	int i, lk = strlen(key), lc = strlen(cleartext);
	for (i = 0; i < lc; i+= 1) 
	{
		ciphertext[i] = (unsigned char) (cleartext[i] + key[i % lk]) % ASCII;
	}
	ciphertext[lc] = '\0';
	return;
}

// Name: decode
//
// Function: decode a string using the Vigenère cipher algorithm. Shift a letter 
// (byte) backward by the ordinal number of corresponding letter in the key. 
//
// Inputs: decryption key (key) and ciphertext (ciphertext)
//
// Outputs: deciphered text (cleartext)

void decode(char *key, char *ciphertext, char *cleartext)
{
	int i, lk = strlen(key), lc = strlen(ciphertext);
	for (i = 0; i < lc; i+= 1) 
	{
		cleartext[i] = (unsigned char) (ciphertext[i] - key[i % lk]) % ASCII;
	}
	cleartext[lc] = '\0';
	return;
}

int main(void)
{
	char *cleartext = "hello";
	char key[] = "keyyy";
	char ciphertext[100];
	printf("The given key is: %s\n",key);
	printf("The given text is: %s\n",cleartext);
	encode(key, cleartext, ciphertext);
	printf("The encrypted text is: %s\n", ciphertext);
	decode(key, ciphertext, ciphertext);
	printf("The decrypted text is: %s\n", ciphertext);

}