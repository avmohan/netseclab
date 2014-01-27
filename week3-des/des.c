/*
DES Algorithm
Author: Abhijith V Mohan
*/
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <assert.h>

// #define textbook //uncomment for textbook example of avalanche effect (from Stallings p81)
void printbin(const bool[], int);
bool avalanche_testing = false; //whether doing avalanche tests
bool intcipher1[16][64], intcipher2[16][64]; //intermediate values for avalanche 
int r_no = 0; //round no for avalanche

//TABLES
//Initial Permutation
const int IP[64]= 
				{
					57, 49, 41, 33, 25, 17,  9, 1,
					59, 51, 43, 35, 27, 19, 11, 3,
					61, 53, 45, 37, 29, 21, 13, 5,
					63, 55, 47, 39, 31, 23, 15, 7,
					56, 48, 40, 32, 24, 16,  8, 0,
					58, 50, 42, 34, 26, 18, 10, 2,
					60, 52, 44, 36, 28, 20, 12, 4,
					62, 54, 46, 38, 30, 22, 14, 6 
				};

//Inverse initial permutation
const int IPi[64] = 
				{
					39, 7, 47, 15, 55, 23, 63, 31,
					38, 6, 46, 14, 54, 22, 62, 30,
					37, 5, 45, 13, 53, 21, 61, 29,
					36, 4, 44, 12, 52, 20, 60, 28,
					35, 3, 43, 11, 51, 19, 59, 27,
					34, 2, 42, 10, 50, 18, 58, 26,
					33, 1, 41,  9, 49, 17, 57, 25,
					32, 0, 40,  8, 48, 16, 56, 24
				};

//Expansion Permutation
const int EP[48] =
				{
					31,  0,  1,  2,  3,  4,
					 3,  4,  5,  6,  7,  8,
					 7,  8,  9, 10, 11, 12,
					11, 12, 13, 14, 15, 16,
					15, 16, 17, 18, 19, 20,
					19, 20, 21, 22, 23, 24,
					23, 24, 25, 26, 27, 28,
					27, 28, 29, 30, 31,  0

				};

//Permutation function
const int PF[32] = 
				{
					15,  6, 19, 20, 28, 11, 27, 16,
					 0, 14, 22, 25,  4, 17, 30,  9,
					 1,  7, 23, 13, 31, 26,  2,  8,
					18, 12, 29,  5, 21, 10,  3, 24
				};

//Permuted choice-1
const int PC1[56] =
				{
					56, 48, 40, 32, 24, 16,  8,
					 0, 57, 49, 41, 33, 25, 17,
					 9,  1, 58, 50, 42, 34, 26,
					18, 10,  2, 59, 51, 43, 35,
					62, 54, 46, 38, 30, 22, 14,
					 6, 61, 53, 45, 37, 29, 21,
					13,  5, 60, 52, 44, 36, 28,
					20, 12,  4, 27, 19, 11,  3
				};

//Permuted choice-2
const int PC2[48] =
				{
					13, 16, 10, 23,  0,  4,  2, 27,
					14,  5, 20,  9, 22, 18, 11,  3,
					25,  7, 15,  6, 26, 19, 12,  1,
					40, 51, 30, 36, 46, 54, 29, 39,
					50, 44, 32, 47, 43, 48, 38, 55,
					33, 52, 45, 41, 49, 35, 28, 31
				};

//schedule of left rotational shifts- whether to shift 1 or 2 bits
const int shifts[16] = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};

//S-boxes
const int Sbox[8][4][16] =
				{
					//S1 - Sbox[0]
					{
						{14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7},
						{ 0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8},
						{ 4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0},
						{15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13}
					},

					//S2 - Sbox[1]
					{
						{15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10},
						{ 3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5},
						{ 0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15},
						{13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9}
					},

					//S3 - Sbox[2]
					{
						{10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8},
						{13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1},
						{13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7},
						{ 1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12}
					},

					//S4 - Sbox[3]
					{
						{ 7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15},
						{13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9},
						{10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4},
						{ 3, 15,  0,  6,  10, 1,  13, 8,  9,  4,  5, 11, 12,  7,  2, 14}
					},

					//S5 - Sbox[4]
					{
						{ 2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9},
						{14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6},
						{ 4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14},
						{11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3}
					},

					//S6 - Sbox[5]
					{
						{12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11},
						{10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8},
						{ 9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6},
						{ 4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13}
					},

					//S7 - Sbox[6]
					{
						{ 4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1},
						{13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6},
						{ 1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2},
						{ 6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12}
					},

					//S8 - Sbox[7]
					{
						{13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7},
						{ 1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2},
						{ 7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8},
						{ 2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11}
					}
				};

/*
Generate the permutation of input[] in output[] according to table[]
*/
void permute_bits(const bool input[], int input_size, bool output[], int output_size, const int table[]) 
{
	int i;
	for(i = 0; i < output_size; i++) {
		output[i] = input[table[i]];
	}
}

/*Sbox lookup
Map 6 bit input to 4 bit output according to the Sbox: Sbox_num
*/
void sbox_lookup(const bool input[6], bool output[4], int Sbox_num)
{
	//1st and 6th bit gives the row
	int row = (int)input[0]<<1 | (int)input[5]; 
	//middle 4 bits give  the column
	int column = (int)input[1]<<3 | (int)input[2]<<2 | (int)input[3]<<1 | (int)input[4];
	int val = Sbox[Sbox_num][row][column];
	output[0] = (bool)(val>>3 & 1);
	output[1] = (bool)(val>>2 & 1);
	output[2] = (bool)(val>>1 & 1);
	output[3] = (bool)(val & 1);
}

/*
Apply all 8 S-boxes to transform 48 bits input to 32 bits output
*/
void sbox_transform(const bool input[48], bool output[32])
{
	int i;
	for(i = 0; i < 8; i++) {
		sbox_lookup(&input[6*i], &output[4*i], i);
	}
}

/*
v3 = v1 XOR v2. vsize is no of bits.
*/
void xor(const bool v1[], const bool v2[], bool v3[], int vsize) 
{
	int i;
	for(i = 0 ; i < vsize; i++) {
		v3[i] = v1[i] ^ v2[i];
	}
}

/*
left rotate v by n bits
*/
void left_rotate(bool v[], int vsize, int n) 
{
	//left rotate v by n bits
	int i, j;
	for(j = 0; j < n; j++) {
		bool temp = v[0];
		for(i = 0; i < vsize-1; i++) {
			v[i] = v[i+1];
		}
		v[vsize-1] = temp;
	}
}

/*
Rotate the 56 bit key: each half independently.
*/
void key_rotate(bool v[], int n)
{
	//v is 56 bits, The left & right 28 bits are independently rotated by n bits
	//rotate left half(28bits) by n bits
	left_rotate(v, 28, n);
	//rotate right half(28 bits) by n bits
	left_rotate(&v[28], 28, n);
}

/*
KEY GENERATION
64 bit key input to the algorithm. every 8th bit is ignored to make 56 bit (done by PC1)
Apply permutation PC1.
Treat 56 bits as 2 blocks of 28 bits C & D.
Each round, left rotate shift C & D of prev round separately by 1 or 2 depending on shifts[16]
Apply PC2 and store in key_sequence
*/
bool key_sequence[16][48];

/*
Generate the 48 bit round keys from 64 bit input_key and store them in key_sequence[16][48]
*/
void generate_keys(const bool input_key[64])
{
	bool temp[56];
	//Apply permutation PC1 to generate 56 bit output from 64 bit input key
	permute_bits(input_key, 64, temp, 56, PC1);
	int i;
	//generating round keys
	for(i = 0; i < 16; i++) {
		//rotate each half by the no of bits given in the table: shifts
		key_rotate(temp, shifts[i]);
		//apply permutation PC2 to get 48 bit round key and store in key_sequence[i]
		permute_bits(temp, 56, key_sequence[i], 48, PC2);
	}
}

/*
ROUND FUNCTION F(R, K)
*/
void round_fn(const bool R[32], const bool K[48], bool output[32])
{
	bool temp1[48], temp2[48], temp3[32];
	//Apply EP on R to get 48 bits.
	permute_bits(R, 32, temp1, 48, EP);
	//XOR it with key K
	xor(temp1, K, temp2, 48);
	//Apply S-BOX to get 32 bits
	sbox_transform(temp2, temp3);
	//Apply Permutatation PF
	permute_bits(temp3, 32, output, 32, PF);
}


/*FEISTEL ROUND - A single round
L_i = R_(i-1)
R_I = L_(i-1) XOR F(R_(i-1), K_i)
*/
void feistel_round(const bool input[64], bool output[64], const bool round_key[48])
{
	bool temp1[32];
	//L_i = R_(i-1)
	memcpy(output, &input[32], 32);
	//R_i = L_(i-1) XOR F(R_i-1), K_i
	round_fn(&input[32], round_key, temp1);
	xor(input, temp1, &output[32], 32);
	
	//Below part only for avalanche testing; not part of des
	if(avalanche_testing) {	
		memcpy(intcipher2[r_no], output, 64);
		r_no++;
	}
}


/*ENCRYPTION - Encrypt a single 64 bit block
*/
void encrypt(const bool input[64], bool output[64])
{
	r_no = 0;
	int i;
	bool temp1[64], temp2[64];
	permute_bits(input, 64, temp1, 64, IP);
	for(i = 0; i < 16; i++) {
		feistel_round(temp1, temp2, key_sequence[i]);
		memcpy(temp1, temp2, 64);
	}
	memcpy(temp2, &temp1[32], 32);
	memcpy(&temp2[32], temp1, 32);
	permute_bits(temp2, 64, output, 64, IPi);
}


/*DECRYPTION - Decrypt a single 64 bit block
*/
void decrypt(const bool input[64], bool output[64])
{
	r_no = 0;
	int i;
	bool temp1[64], temp2[64];
	permute_bits(input, 64, temp1, 64, IP);
	for(i = 15; i >= 0; i--) {
		feistel_round(temp1, temp2, key_sequence[i]);
		memcpy(temp1, temp2, 64);
	}
	memcpy(temp2, &temp1[32], 32);
	memcpy(&temp2[32], temp1, 32);
	permute_bits(temp2, 64, output, 64, IPi);
}

/**Random utility functions follow */
/*
Generate 64 bit bool array from an integer.
*/
void dectobin(unsigned long int dec, bool bin[64]) 
{
	int i;
	for(i = 0; i < 64; i++) {
		bin[i] = (dec & (1ul<<(63-i)));
	}
}

/*
Generate integer from a 64 bit bool array
*/
unsigned long bintodec(const bool bin[64])
{
	unsigned long dec = 0;
	int i;
	for(i = 0; i < 64; i++) {
		dec = dec<<1;
		dec = dec|(int)(bin[i]);
	}
	return dec;
}

/*
Generate 64 bit bool array from bit-string str[100]
*/
void strtobin(const char str[100], bool bin[64])
{
	memset(bin, 0, 64);
	int c = 0, i;
	for(i = 0; c < 64 && str[i]!='\0' && str[i]!='\n'; i++) {
		assert(str[i]=='1' || str[i]=='0' || str[i]==' ');
		if(str[i]=='0' || str[i]=='1') {
			bin[c] = (bool)(str[i] - '0');
			c++;
		}
	}
}

/*
Print out the n bit bool array, grouped by bytes.
*/
void printbin(const bool bitstring[], int n) 
{
	int i;
	for (i = 0; i < n; i++) {
		printf(bitstring[i]? "1" : "0");
		if((i+1)%8==0)
			printf(" ");
	}
	printf("\n");
}

/**
	Test for des taken from http://people.csail.mit.edu/rivest/pubs/Riv85.txt by Rivest(1985)
	Quote from the abstract:
	"Use the recurrence relation:
	X0      =       9474B8E8C73BCA7D (hexadecimal)
	X(i+1)  =       IF  (i is even)  THEN  E(Xi,Xi)  ELSE  D(Xi,Xi)
	to compute a sequence of 64-bit values:  X0, X1, X2, ..., X16.  Here
	E(X,K)  denotes the DES encryption of  X  using key  K, and  D(X,K)  denotes
	the DES decryption of  X  using key  K.  If you obtain
		X16     =       1B1A2DDB4C642438
	your implementation does not have any of the 36,568 possible single-fault 
	errors described herein."
*/
bool test_DES()
{
	bool X[17][64], Y[64];
	dectobin(0x9474B8E8C73BCA7D, X[0]);
	int i;
	for(i = 0; i < 16; i++) {
		generate_keys(X[i]);
		if(i%2 == 0) {
			encrypt(X[i], X[i+1]);
		}
		else {
			decrypt(X[i], X[i+1]);
		}
	}
	dectobin(0x1B1A2DDB4C642438, Y);
	// printbin(X[16], 64);
	// printbin(Y, 64);
	bool passed = true;
	for(i = 0; i< 64; i++) {
		if(X[16][i]!=Y[i])
			passed = false;
	}
	return passed;
}

/*Calculate hamming distance b/w A & B*/
int ham_distance(const bool A[64], const bool B[64])
{
	int d=0, i;
	for(i = 0; i < 64; i++) {
		d+=(A[i]!=B[i]);
	}
	return d;
}

/*Fill X with random bits*/
void random_bitfill(bool X[64])
{
	srand(time(NULL));
	unsigned long int x = (long int)rand()<<32|rand();
	dectobin(x, X);
}

/*
Encrypting 2 near-identical plaintexts with same key.
*/
void avalanche_test1()
{
	avalanche_testing = true;
	printf("Test 1: Two plaintexts differ by 1 bit, encrypted with same key\n");
	bool P1[64], P2[64], K[64], C[64];
	
	#ifndef textbook
	//random bits
	random_bitfill(K);
	random_bitfill(P1);
	memcpy(P2, P1, 64);
	assert(ham_distance(P1, P2)==0);
	int k = rand()%64;
	P2[k] = !P2[k];
	#endif
	
	#ifdef textbook
	// textbook example
	dectobin(0x029648c438303864, K);
	dectobin(0x0000000000000000, P1);
	dectobin(0x8000000000000000, P2);
	#endif
	
	assert(ham_distance(P1, P2) == 1);
	printf("P1: ");
	printbin(P1, 64);
	printf("P2: ");
	printbin(P2, 64);
	printf("K : ");
	printbin(K, 64);
	
	generate_keys(K);
	encrypt(P1, C);
	int i;
	for(i = 0; i < 16; i++) {
		memcpy(intcipher1[i], intcipher2[i], 64);
	}
	encrypt(P2, C);
	printf("Hamming distance with plaintext after each round\n");
	printf("round %2d: %2d\n", 0, 1);
	for(i = 0; i < 16; i++) {
		int d = ham_distance(intcipher1[i], intcipher2[i]);
		printf("round %2d: %2d\n", i+1, d);	
	}
	avalanche_testing = false;
}

/*
Encrypting same plaintext with near-identical keys
*/
void avalanche_test2()
{
	avalanche_testing = true;
	printf("Test 2: Same plaintext encrypted with two keys differing by 1 bit\n");
	bool P[64], K1[64], K2[64], C[64];
	
	#ifndef textbook
	//random bits
	random_bitfill(P);
	random_bitfill(K1);
	memcpy(K2, K1, 64);
	int k;
	//choose a random bit to flip (don't choose the ignored 8th bits)
	do {
		k = rand()%64;
	}while((k+1)%8 ==0);
	K2[k] = !K2[k];
	#endif
	
	#ifdef textbook
	//textbook example
	dectobin(0x68852f7a1376eba4, P);
	dectobin(0xe4f6de303a0862dcL, K1);
	dectobin(0x64f6de303a0862dc, K2);
	#endif
	
	assert(ham_distance(K1, K2) == 1);
	printf("P : ");
	printbin(P, 64);
	printf("K1: ");
	printbin(K1, 64);
	printf("K2: ");
	printbin(K2, 64);

	generate_keys(K1);
	encrypt(P, C);
	int i;
	for(i = 0; i < 16; i++) {
		memcpy(intcipher1[i], intcipher2[i], 64);
	}
	generate_keys(K2);
	encrypt(P, C);
	printf("Hamming distance with plaintext after each round\n");
	printf("round %2d: %2d\n", 0, 0);
	for(i = 0; i < 16; i++) {
		int d = ham_distance(intcipher1[i], intcipher2[i]);
		printf("round %2d: %2d\n", i+1, d);	
	}
	avalanche_testing = false;
}

int main(int argc, char* argv[])
{
	srand(time(NULL));
	if (argc<2)
		goto usage;

	int mode = argv[1][0];
	if(mode == 't') {
		bool passed =test_DES();
		printf(passed? "PASSED\n" : "FAILED\n");
	}
	if (mode == 'a') {
		avalanche_testing = true;
		printf("Avalanche tests\n");
		avalanche_test1();
		printf("------------------------\n");
		avalanche_test2();	
	}
	else if (mode == 'i') {
		char ch, input[100], key[100];
		bool P[64], K[64], C[64];
		do {
			printf("encrypt or decrypt?(e/d)");
			scanf("%c", &ch);
		}while(!(ch == 'e' || ch == 'd'));
		getc(stdin); //remove '\n'
		if(ch=='e') {	
			printf("Plaintext (64bit bitstring)\n");
			fgets(input, 100, stdin);
			printf("Key (64bit bitstring)\n");
			fgets(key, 100, stdin);
			strtobin(input, P);
			strtobin(key, K);
			generate_keys(K);
			encrypt(P, C);
			printf("P: ");
			printbin(P, 64);
			printf("K: ");
			printbin(K, 64);
			printf("C: ");
			printbin(C, 64);
		}
		else {
			printf("Ciphertext (64bit bitstring)\n");
			fgets(input, 100, stdin);
			printf("Key (64bit bitstring)\n");
			fgets(key, 100, stdin);
			strtobin(input, C);
			strtobin(key, K);
			generate_keys(K);
			decrypt(C, P);
			printf("C: ");
			printbin(C, 64);
			printf("K: ");
			printbin(K, 64);
			printf("P: ");
			printbin(P, 64);
		}
	}
	else if(mode == 'e' || mode == 'd') {
		if(argc<5)
			goto usage;
		char *infile, *outfile;
		unsigned long keyint, buf; 
		FILE *fin, *fout;
		bool I[64], K[64], O[64];
		keyint = strtoul(argv[2], NULL, 10);
		dectobin(keyint, K);
		generate_keys(K);
		infile = argv[3];
		outfile = argv[4];
		fin = fopen(infile, "r");
		fout = fopen(outfile, "w");
		buf = 0;
		int cnt = 0;
		while((cnt = fread(&buf, 1, 8, fin))>0) {
			dectobin(buf, I);
			if(mode == 'e') {
				encrypt(I, O);
			}
			else if(mode == 'd') {
				decrypt(I, O);
			}
			buf = bintodec(O);
			fwrite(&buf, 8, 1, fout);
			buf = 0;
		}
		fclose(fin);
		fclose(fout);
	}
	return 0;
	usage:
		printf("Usage: ./des t|a|i|[e|d <key> <infile> <outfile>]]\n");
		return 1;
}