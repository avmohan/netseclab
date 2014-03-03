#!/usr/bin/env python
import sys
from itertools import islice

def bbs(s, p=335207, q=10039):
	assert p%4==3 and q%4==3
	n = p*q
	x = (s*s) % n
	while True:
		x = (x*x) % n
		yield x&1

def main():
	if len(sys.argv)==3:
		s, limit = [int(x) for x in sys.argv[1:]]
		print '\n'.join([str(i) for i in islice(bbs(s), limit)])
	else:
		print 'Incorrect usage. correct: seed, limit'

if __name__ == '__main__':
	main()
