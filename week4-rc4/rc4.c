/*
RC4 stream cipher
Author: Abhijith V Mohan
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

typedef unsigned char byte;

/*Structure to store the state of the PRNG*/
typedef struct
{
	byte i, j;
	byte S[256];
} Rc4State;

void swap(byte *a, byte *b)
{
	byte temp = *a;
	*a = *b;
	*b = temp;
}

void die(const char *message)
{
	if(errno) {
		perror(message);
	} 
	else {
		printf("ERROR: %s\n", message);
	}
	exit(1);
}

/*Initialization & initial permutation
also initialize i&j counters in state for stream generation*/
void rc4InitState( Rc4State *state, const byte K[256], size_t keylen)
{
	byte T[256];
	if(!(keylen>=1 && keylen<=256)) die("Invalid keylen\n");
	int i;
	for(i = 0; i < 256; i++) {
		state-> S[i] = i;
		T[i] = K[i % keylen];
	}

	//Initial permutation of S
	byte *S = state->S;
	int j = 0;
	for(i = 0; i < 256; i++) {
		j = (j + S[i] + T[i]) % 256;
		swap(&S[i], &S[j]);
	}

	//Initialize counters in state
	state->i = state->j = 0;
}

/*Encrypt/Decrypt text by XORing with next byte of keystream*/
byte rc4CryptByte(Rc4State *state, byte text)
{
	byte t, k;
	byte *i = &(state->i), *j = &(state->j);
	byte *S = state->S;
	*i = (*i+1) % 256;
	*j = (*j+S[*i]) % 256;
	swap(&S[*i], &S[*j]);
	t = (S[*i] + S[*j]) % 256;
	k = S[t];

	return text ^ k;
}

/*Encrypt/decrypt text[] of len bytes in place*/
void rc4Crypt(Rc4State *state, byte text[], size_t len)
{
	size_t i;
	for(i = 0; i < len; i++) {
		text[i] = rc4CryptByte(state, text[i]);
	}
}

/*Print usage note*/
void usage()
{
	printf("Usage: ./rc4 <keystring> <inputfile> <outputfile>\n");
	exit(1);
}

/*
Usage: ./rc4 <keystring> <inputfile> <outputfile>
*/
int main(int argc, char const *argv[])
{
	Rc4State rc4_state;
	if(argc<4)
		usage();
	int keylen = strlen(argv[1]);
	rc4InitState(&rc4_state, (const byte*)argv[1], keylen);
	FILE *fin = fopen(argv[2], "r");
	if(!fin)
		die("File can't be opened for reading\n");
	FILE *fout = fopen(argv[3], "w");
	if(!fout)
		die("File can't be opened for writing\n");
	byte buf[100];
	int ctr;
	while((ctr = fread(&buf, 1, 100, fin))>0) {
		rc4Crypt(&rc4_state, buf, ctr);
		fwrite(&buf, 1, ctr, fout);
	}
	return 0;
}
