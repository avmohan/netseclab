/*
Feistel cipher
by Abhijith V Mohan
*/
#include <assert.h>
#include <stdio.h>

//Block size: 32 bits , i.e. w=16
//Key size 16 bits

#define NO_OF_ROUNDS 16
typedef unsigned int uint;
uint key_sequence[NO_OF_ROUNDS];

inline uint left_rotate(uint x)
{
	return x<<1 | x>>31;
}

inline uint left(uint X) 
{
	//Left 16 bits of X
	return (0xFFFF0000&X)>>16;
}

inline uint right(uint X) 
{
	//Right 16 bits of X
	return 0x0000FFFF&X;
}

inline uint pack(uint L, uint R) 
{
	//Make 32bits with 16 bit L and 16 bit R
	return (L<<16)|R;
}

void keygen(uint K) 
{
	//Generates round keys from the base key
	uint i;
	for (i = 0; i < NO_OF_ROUNDS; i++) {
		key_sequence[i]=right(left_rotate(K));
	}
}

uint round_fn(uint x, uint k)
{
	return right(x^k);
}

uint feistel_round(uint X, uint round_key)
{
	//One round of the feistel function
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
			while((cnt = fread(&buf1, 1, 4, fin))==4) {
				buf2  = encrypt(buf1);
				fwrite(&buf2, 4, 1, fout);
			}
			//handle padding
			break;

		case 'd':
			while((cnt = fread(&buf1, 1, 4, fin))==4) {	
				buf2 = decrypt(buf1);
				fwrite(&buf2, 4, 1, fout);
			}
			//handle padding
			break;

		default: 
			return 0; 
	}
	fclose(fin);
	fclose(fout);
	return 0;
}
