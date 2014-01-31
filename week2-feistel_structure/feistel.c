/*
Feistel cipher
by Abhijith V Mohan

Implementation of a simple cipher with feistel structure
and using it to encrypt a file in EBC mode
TODO: 
add single block encryption in main
add CBC mode
*/
#include <assert.h>
#include <stdio.h>

//Block size: 32 bits , i.e. w=16
//Key size 16 bits

#define NO_OF_ROUNDS 16
typedef unsigned int uint; //32 bits
uint key_sequence[NO_OF_ROUNDS]; //subkey sequence

//left rotate(only the rightmost 16 bits)
inline uint left_rotate(uint x)
{
	return (x<<1)&0x0000FFFF | (1<<15)&x;
}

//Left 16 bits of X
inline uint left(uint X) 
{
	return (0xFFFF0000&X)>>16;
}

//Right 16 bits of X
inline uint right(uint X) 
{
	return 0x0000FFFF&X;
}

//Make 32bits with 16 bit L and 16 bit R
inline uint pack(uint L, uint R) 
{
	return (L<<16)|R;
}

//Generates round keys from the base key
void keygen(uint K) 
{
	K &= 0x0000FFFF;
	uint i;
	key_sequence[0] = K;
	for (i = 1; i < NO_OF_ROUNDS; i++) {
		key_sequence[i]=left_rotate(key_sequence[i-1]);
	}
}

//The feistel round function
uint round_fn(uint x, uint k)
{
	return right(x^k);
}

//One round of the feistel function
uint feistel_round(uint X, uint round_key)
{
	return pack(right(X), left(X)^round_fn(right(X), round_key));
}

uint encrypt(uint X)
{
	uint i;
	for(i = 0; i < NO_OF_ROUNDS; i++) {
		X = feistel_round(X, key_sequence[i]);
	}
	return pack(right(X), left(X));
}

uint decrypt(uint X)
{
	//Go through key sequence in reverse order as encryption
	uint i;
	for(i = NO_OF_ROUNDS; i != 0; i--) {
		X = feistel_round(X, key_sequence[i-1]);
	}
	return pack(right(X), left(X));
}

int main(int argc, char *argv[])
{
	uint buf1, buf2;
	int mode = argv[1][0];	
	uint key = right((uint)atoi(argv[2]));
	char *infile = argv[3];
	char *outfile = argv[4];
	keygen(key);
	// printf("%c %u %s %s\n", mode, key, infile, outfile);
	FILE *fin, *fout;
	fin = fopen(infile, "r");
	fout = fopen(outfile, "w");
	int cnt;
	switch(mode) {
		case 'e':
			buf1=buf2=0;
			while((cnt = fread(&buf1, 1, 4, fin))>0) {
				buf2  = encrypt(buf1);
				fwrite(&buf2, 4, 1, fout);
				buf1=buf2=0;
			}
			break;

		case 'd':
			buf1=buf2=0;
			while((cnt = fread(&buf1, 1, 4, fin))>0) {	
				buf2 = decrypt(buf1);
				fwrite(&buf2, 4, 1, fout);
				buf1=buf2=0;
			}
			break;

		default: 
			return 0; 
	}
	fclose(fin);
	fclose(fout);
	return 0;
}
