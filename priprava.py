import sys
from StringIO import StringIO
#nacte sudoku z stdin v libovolnem formatu a napise ho na stdout v spravnem formatu

def pripravaSTR(string):
  stream=StringIO(string)
  return priprava(stream)
  
def priprava(stream=sys.stdin):
  n=0
  znaky=[]
  while n<9*9:
    znak = stream.read(1)
    if znak in ('.','0'):
      znaky.append('.')
      n+=1
    elif znak.isdigit():
      znaky.append(znak)
      n+=1

  vystup = []
  for i in range(9):
    for j in range(9):
      vystup.append(znaky[i*9+j])
      if j!=8:
        vystup.append(' ')
    vystup.append('\n')
  
  return ''.join(vystup)
    
  
if __name__ == '__main__':
  print priprava()
