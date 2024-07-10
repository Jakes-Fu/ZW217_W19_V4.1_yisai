import sys, os, struct, csv, re, shutil

def write_byte(out, word):
	byte = int(word, 16)
	buf = struct.pack('B', byte)
	out.write(buf)

fd = open(r'd:\tmp.txt', 'r')
lines = fd.readlines()
fd.close()

out = open(r'd:\tmp.bin', 'wb')
for line in lines:
	words = line.split(' ')
	for word in words:
		write_byte(out, word)

out.close()


