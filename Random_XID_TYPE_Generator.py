
__author__ = 'Pranav'
import string,random  
def Random_32bit_XIA():
  length=8
  letters=string.ascii_lowercase+string.digits
  return '0x' + ''.join([random.choice(letters) for _ in range(length)])

for i in range(1,50000):

    f = open('/Users/Pranav/Desktop/input_xid_type_32bit.txt','a')
    value = Random_32bit_XIA()
    print >>f,value
    f.close()

