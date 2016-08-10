#usage: g++ -Wall -pedantic main.cpp && time python benchmark.py tests/*

import sys
import os

for filename in sys.argv[1:]:
  os.system("./a.out < %s" % (filename))
  

