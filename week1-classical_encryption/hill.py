#!/usr/bin/env python
import sys
import frequencygraph
from numpy import linalg
from numpy import matrix as npmatrix
letters = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'

def sign(x):
    """Returns sign of x"""
    if x==0:
        return 0
    return x/abs(x)

def egcd(a, b):
    """Extended Euclid's algo"""
    if a == 0:
        return (b, 0, 1)
    else:
        g, y, x = egcd(b % a, a)
        return (g, x - (b / a) * y, y)

def modinv(a, m):
    """Returns modular inverse of a modulo m"""
    g, x, y = egcd(a, m)
    assert g==1  # else modular inverse does not exist
    return x % m

def matrix_modinv(matrix, m):
    """Return inverse of the matrix modulo m"""
    matrix_det = int(round(linalg.det(matrix)))
    return modinv(abs(matrix_det), m)*linalg.inv(matrix)*matrix_det*sign(matrix_det)

def getmatrix(block):
    """Get 1-by-n matrix corresponding to block"""
    matrix = npmatrix([[letters.index(c)] for c in block])
    return matrix

def getblock(matrix):
    """Get block of text corresponding to 1-by-n matrix"""
    block = ''.join(letters[int(round(i))%26] for i in matrix.getA1())
    return block

def cleanup(text, blocksize):
    """Remove unwanted chars and makes text length a multiple of block size"""
    text = text.upper()
    clean_text = [c for c in text if c in letters]
    while (len(clean_text) % blocksize)!=0:
        clean_text.append('Z')
    return clean_text

def encrypt(plaintext, key):
    blocksize = len(key)
    clean_text = cleanup(plaintext, blocksize)
    blocks_plain = [getmatrix(clean_text[i:i+blocksize]) for i in xrange(0, len(clean_text), blocksize)]
    blocks_cipher = [getblock(key*block) for block in blocks_plain]
    ciphertext = ''.join(blocks_cipher)
    return ciphertext

def decrypt(ciphertext, key):
    blocksize = len(key)
    blocks_cipher = [getmatrix(ciphertext[i:i+blocksize]) for i in xrange(0, len(ciphertext), blocksize)]
    inv_key = matrix_modinv(key, 26)
    blocks_plain = [getblock(inv_key*block) for block in blocks_cipher]
    plaintext = (''.join(blocks_plain)).lower()
    return plaintext

key = npmatrix([[6,24,1],[13,16,10],[20,17,15]]) #Key matrix

def main():
    mode, inputfile, outputfile = sys.argv[1:4]
    ifile = open(inputfile, 'r')
    ofile = open(outputfile, 'w')
    if mode=='e':
        plaintext = ifile.read()
        ciphertext = encrypt(plaintext, key) 
        ofile.write(ciphertext)
        frequencygraph.freqbars_percentage('Hill Cipher frequency graph)'.format(key), plaintext, ciphertext)
    elif mode=='d':
        ciphertext = ifile.read()
        plaintext = decrypt(ciphertext, key)
        ofile.write(plaintext)
    ifile.close()
    ofile.close()


if __name__ == '__main__':
    main()