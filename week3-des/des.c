/*
DES Algorithm
by Abhijith V Mohan
*/
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

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
					15,  6, 19, 20, 28, 11, 27,  16,
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
const int Sbox[8][4][16];

/*
KEY GENERATION
64 bit key input to the algorithm. every 8th bit is ignored to make 56 bit
Apply permutation PC1.
Treat 56 bits as 2 blocks of 28 bits C0 & D0.
Each round, left rotate shift C & D of prev round separately by 1 or 2 depending on shifts[16]
Apply PC2 and store in key array
*/



/*
ROUND FUNCTION F(R, K)
Apply EP on R to get 48 bits.
XOR it with key K
Use Sbox to get 32 bits
Apply Permutatation PF to get 32 bits
*/



/*FEISTEL ROUND - A single round
L_i = R_(i-1)
R_I = L_(i-1) XOR F(R_(i-1), K_i)
*/


/*ENCRYPTION - Encrypt a single 64 bit block
*/


/*DECRYPTION - Decrypt a single 64 bit block
*/


/*MAIN - Read from file 64 bits into bool array
Encypt & write to file
*/

int main(int argc, char const *argv[])
{
	/* code */
	return 0;
}