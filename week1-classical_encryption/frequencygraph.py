"""Frequency graph generator"""
import os
import pygal
from collections import Counter
from pygal.style import LightStyle
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
