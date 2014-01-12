#!/usr/bin/env python
"""Caeser cipher"""
import sys
import frequencygraph
letters='ABCDEFGHIJKLMNOPQRSTUVWXYZ'

def encrypt(plaintext, key):
    ciphertext=[]
    for character in plaintext.upper():
        if character in letters:
            ciphertext.append(letters[(letters.index(character)+key)%26])
        else:
            ciphertext.append(character)
    return ''.join(ciphertext)

def decrypt(ciphertext, key):
    plaintext=[]
    for character in ciphertext.upper():
        if character in letters:
            plaintext.append(letters[(letters.index(character)-key)%26])
        else:
            plaintext.append(character)
    return ''.join(plaintext).lower()

def main():
    mode, key, inputfile, outputfile = sys.argv[1:5]
    key = int(key)
    ifile = open(inputfile, 'r')
    ofile = open(outputfile, 'w')
    if mode=='e':
        plaintext = ifile.read()
        ciphertext = encrypt(plaintext, key) 
        ofile.write(ciphertext)
        frequencygraph.freqbars_percentage('Caeser Cipher frequency graph (key={0})'.format(key), plaintext, ciphertext)
    elif mode=='d':
        ciphertext = ifile.read()
        plaintext = decrypt(ciphertext, key)
        ofile.write(plaintext)
    ifile.close()
    ofile.close()

if __name__ == '__main__':
    main()