#!/usr/bin/env python
"""Vigenere cipher"""
import sys
import frequencygraph
letters = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'

def encrypt(plaintext, key):
    key = ''.join([c for c in key.upper() if c in letters])
    ciphertext = []
    inx = 0
    for character in plaintext.upper():
        if character in letters:
            ciphertext.append(letters[(letters.index(character) + letters.index(key[inx]))%26])
            inx = (inx+1) % len(key)
        else:
            ciphertext.append(character)
    return ''.join(ciphertext)

def decrypt(ciphertext, key):
    key = ''.join([c for c in key.upper() if c in letters])
    plaintext = []
    inx = 0
    for character in ciphertext.upper():
        if character in letters:
            plaintext.append(letters[(letters.index(character) - letters.index(key[inx]))%26])
            inx = (inx+1) % len(key)
        else:
            plaintext.append(character)
    return ''.join(plaintext).lower()

def main():
    mode, key, inputfile, outputfile = sys.argv[1:5]
    key = ''.join([c for c in key.upper() if c in letters])
    ifile = open(inputfile, 'r')
    ofile = open(outputfile, 'w')
    if mode=='e':
        plaintext = ifile.read()
        ciphertext = encrypt(plaintext, key) 
        ofile.write(ciphertext)
        frequencygraph.freqbars_percentage('Vigenere Cipher frequency graph (key={0})'.format(key), plaintext, ciphertext)
    elif mode=='d':
        ciphertext = ifile.read()
        plaintext = decrypt(ciphertext, key)
        ofile.write(plaintext)
    ifile.close()
    ofile.close()

if __name__ == '__main__':
    main()
