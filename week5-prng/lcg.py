#!/usr/bin/env python
"""Author: Abhijith V Mohan"""
import sys
from itertools import islice

def lcg(x0, m=2147483647, a=16807, c=0):
	while True:
		x0 = (a*x0 + c) % m
		yield x0

def main():
	if len(sys.argv) == 3:
		x0, limit = [int(x) for x in sys.argv[1:]]
		print '\n'.join([str(i) for i in islice(lcg(x0), limit)])

	elif len(sys.argv) == 6:
		x0, m, a, c, limit = [int(x) for x in sys.argv[1:]]
		print '\n'.join([str(i) for i in islice(lcg(x0, m, a, c), limit)])

	else:
		print 'Incorrect usage. Correct(seed, limit)'
if __name__ == '__main__':
	main()
