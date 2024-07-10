#!/usr/bin/python

# Usage: addfwhdr.py <input_file> <output_file>

import os
import os.path
import stat
import sys
import struct

if 3 != len(sys.argv):
    print 'Usage: addfwhdr.py <input_file> <output_file>'
    sys.exit(1)

if not os.path.isfile(sys.argv[1]):
    print 'Error:', sys.argv[1], 'is not a file!'
    sys.exit(2)

if os.path.isdir(sys.argv[2]):
    print 'Error:', sys.argv[2], 'is a directory!'
    sys.exit(2)

fileSize = os.path.getsize(sys.argv[1])
fsBytes = struct.pack('<L', fileSize)
hInFile = open(sys.argv[1], 'rb')
hOutFile = open(sys.argv[2], 'wb')
hOutFile.write(fsBytes)
fsBytes = hInFile.read()
hOutFile.write(fsBytes)
hInFile.close()
hOutFile.close()
