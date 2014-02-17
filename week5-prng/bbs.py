#!/usr/bin/env python
import sys
from itertools import islice

def bbs(s, p, q):
	assert p%4==3 and q%4==3
	n = p*q
	x = (s*s) % n
	while True:
		x = (x*x) % n
		yield x&1

def main():
	if len(sys.argv)==5:
		s, p, q, limit = [int(x) for x in sys.argv[1:]]
		print '\n'.join([str(i) for i in islice(bbs(s, p, q), limit)])
	else:
		print 'Incorrect usage'

if __name__ == '__main__':
	main()