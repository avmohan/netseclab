#!/usr/bin/env python
"""Frequency graph generator"""
import sys
import os
import pygal
from collections import Counter
from pygal.style import LightStyle
import caesar
import vigenere
import playfair
import hill
letters = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'
pygal.util.is_major = lambda x: False
reload(pygal.graph.graph)
reload(pygal.graph.radar)

class CustomConfig(pygal.Config):
    explicit_size = True
    width = 800
    height = 500
    human_readable = True
    include_x_axis = True
    x_title = 'Letters'
    y_title = 'Frequency'
    pretty_print = True
    dirname = 'file://' + os.path.dirname(os.path.abspath(__file__)) + '/'
    js = [dirname + 'svg.jquery.js', dirname + 'pygal-tooltips.js']

class CustomConfig2(pygal.Config):
    explicit_size = True
    width = 800
    height = 500
    include_x_axis = True
    pretty_print = True
    dirname = 'file://' + os.path.dirname(os.path.abspath(__file__)) + '/'
    js = [dirname + 'svg.jquery.js', dirname + 'pygal-tooltips.js']    

def freqbars(graphtitle, plaintext, ciphertext):
    plaintext = plaintext.upper()
    ciphertext = ciphertext.upper()
    freq_plain = Counter(plaintext)
    freq_cipher = Counter(ciphertext)
    bar_chart = pygal.Bar(config=CustomConfig(), style=LightStyle)
    bar_chart.title = graphtitle[:-1] + ', total no of letters =' + str(sum((freq_plain[c] for c in letters))) + ')'
    bar_chart.x_labels = list(letters)
    bar_chart.add('Plaintext', [ freq_plain[c] for c in letters ])
    bar_chart.add('Ciphertext', [ freq_cipher[c] for c in letters ])
    bar_chart.render_in_browser()


def freqbars_percentage(graphtitle, plaintext, ciphertext):
    plaintext = plaintext.upper()
    ciphertext = ciphertext.upper()
    freq_plain = Counter(plaintext)
    freq_cipher = Counter(ciphertext)
    total_letters = float(sum((freq_plain[c] for c in letters)))
    values_plain = [ freq_plain[c] * 100 / total_letters for c in letters ]
    values_cipher = [ freq_cipher[c] * 100 / total_letters for c in letters ]
    bar_chart = pygal.Bar(config=CustomConfig(), style=LightStyle, y_title='Percentage frequency')
    bar_chart.title = graphtitle
    bar_chart.x_labels = list(letters)
    bar_chart.add('Plaintext', values_plain)
    bar_chart.add('Ciphertext', values_cipher)
    bar_chart.render_in_browser()

def main():
    """Plot relative frequency graph for Caesar, Vigenere, Hill & Playfair ciphers"""
    #Values
    inputfile = sys.argv[1]
    caesar_key = 3
    vigenere_key = 'Vigenere'
    hill_key = hill.npmatrix([[6,24,1],[13,16,10],[20,17,15]])
    playfair_key = 'Playfair key'

    #Plaintext
    plaintext = open(inputfile, 'r').read().upper()
    freq_plain = Counter(plaintext)
    e_count = freq_plain['E']
    freq_plain = sorted([freq_plain[c] for c in letters], reverse=True)
    freq_plain = [i/float(e_count) for i in freq_plain]

    #Caesar
    caesar_text = caesar.encrypt(plaintext, caesar_key)
    freq_caesar = Counter(caesar_text)
    freq_caesar = sorted([freq_caesar[c] for c in letters], reverse=True)
    freq_caesar = [i/float(e_count) for i in freq_caesar]
    
    #Vigenere
    vigenere_text = vigenere.encrypt(plaintext, vigenere_key)
    freq_vigenere = Counter(vigenere_text)
    freq_vigenere = sorted([freq_vigenere[c] for c in letters], reverse=True)
    freq_vigenere = [i/float(e_count) for i in freq_vigenere]

    #Hill
    hill_text = hill.encrypt(plaintext, hill_key)
    freq_hill = Counter(hill_text)
    freq_hill = sorted([freq_hill[c] for c in letters], reverse=True)
    freq_hill = [i/float(e_count) for i in freq_hill]

    #Playfair
    playfair_text = playfair.encrypt(plaintext, playfair_key)
    freq_playfair = Counter(playfair_text)
    freq_playfair = sorted([freq_playfair[c] for c in letters], reverse=True)
    freq_playfair = [i/float(e_count) for i in freq_playfair]

    #Plotting linegraphs
    line_chart = pygal.Line(config=CustomConfig2, style=LightStyle, y_title='Relative frequency')
    line_chart.title = 'Relative frequency for various ciphers'
    line_chart.add('Plaintext', freq_plain)
    line_chart.add('Caesar', freq_caesar)
    line_chart.add('Vigenere', freq_vigenere)
    line_chart.add('Hill', freq_hill)
    line_chart.add('Playfair', freq_playfair)
    line_chart.render_in_browser()

    print Counter(plaintext)

if __name__ == '__main__':
    main()