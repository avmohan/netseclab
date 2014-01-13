#!/usr/bin/env python
"""Playfair cipher
by Abhijith V Mohan"""
from collections import OrderedDict
import sys
import frequencygraph
letters_noj = 'ABCDEFGHIKLMNOPQRSTUVWXYZ'

def cleanup(text):
    """Remove unwanted characters, replace J with I & capitalize"""
    text = text.upper()
    clean_text = []
    for c in text:
        if c=='J':
            clean_text.append('I')
        elif c in letters_noj:
            clean_text.append(c)
    return ''.join(clean_text)

def make_table(key):
    """Make playfair table from the key"""
    key = list(OrderedDict.fromkeys(cleanup(key)))
    letters_rem = [ c for c in letters_noj if c not in key]
    l = key+letters_rem
    assert len(l)==25
    table = [l[i:i+5] for i in xrange(0,25,5)]
    return table

def get_position(char, table):
    """Returns the position of char in the table"""
    for row in xrange(5):
        for column in xrange(5):
            if table[row][column]==char:
                return [row, column]

def encrypt_digram(digram, table):
    """Return digram encrypted using table"""
    assert digram[0]!=digram[1]
    apos = get_position(digram[0], table)
    bpos = get_position(digram[1], table)
    if apos[0]==bpos[0]: #same row, increase column by 1
        apos[1] = (apos[1]+1)%5
        bpos[1] = (bpos[1]+1)%5
    elif apos[1]==bpos[1]: #same column, increase row by 1
        apos[0] = (apos[0]+1)%5
        bpos[0] = (bpos[0]+1)%5
    else:
        temp = apos[1]
        apos[1] = bpos[1]
        bpos[1] = temp
    return table[apos[0]][apos[1]]+table[bpos[0]][bpos[1]]

def decrypt_digram(digram, table):
    """Return digram decrypted using table"""
    assert digram[0]!=digram[1]
    apos = get_position(digram[0], table)
    bpos = get_position(digram[1], table)
    if apos[0]==bpos[0]: #same row, decrease column by 1
        apos[1] = (apos[1]-1)%5
        bpos[1] = (bpos[1]-1)%5
    elif apos[1]==bpos[1]: #same column, decrease row by 1
        apos[0] = (apos[0]-1)%5
        bpos[0] = (bpos[0]-1)%5
    else:
        temp = apos[1]
        apos[1] = bpos[1]
        bpos[1] = temp
    return table[apos[0]][apos[1]]+table[bpos[0]][bpos[1]]

def get_digrams(text):
    """Splits plaintext into digrams"""
    text = cleanup(text)
    clean_text=[]
    l2 = False #True if char is the second char in digram
    for i in xrange(len(text)):    
        if l2 and text[i]==text[i-1]:
            if text[i-1]!= 'X':
                clean_text.append('X')
            else:
                clean_text.append('Z')
            l2 = not l2
        clean_text.append(text[i])
        l2=not l2
    if len(clean_text)%2 != 0:
        if clean_text[-1]!='X':
            clean_text.append('X')
        else:
            clean_text.append('Z')
    return [''.join(clean_text[i:i+2]) for i in xrange(0, len(clean_text), 2)]


def encrypt(plaintext, key):
    table = make_table(key)
    digrams = get_digrams(plaintext)
    digrams_encrypted = [encrypt_digram(digram, table) for digram in get_digrams(plaintext)]
    return ''.join(digrams_encrypted)
    
def decrypt(ciphertext, key):
    table = make_table(key)
    digrams= [ciphertext[i:i+2] for i in xrange(0, len(ciphertext), 2)]
    digrams_decrypted = [decrypt_digram(digram, table) for digram in digrams]
    return (''.join(digrams_decrypted)).lower()

def main():
    mode, key, inputfile, outputfile = sys.argv[1:5]
    ifile = open(inputfile, 'r')
    ofile = open(outputfile, 'w')
    if mode=='e':
        plaintext = ifile.read()
        ciphertext = encrypt(plaintext, key) 
        ofile.write(ciphertext)
        frequencygraph.freqbars_percentage('Playfair Cipher frequency graph (key={0})'.format(key), plaintext, ciphertext)
    elif mode=='d':
        ciphertext = ifile.read()
        plaintext = decrypt(ciphertext, key)
        ofile.write(plaintext)
    ifile.close()
    ofile.close()

if __name__ == '__main__':
    main()