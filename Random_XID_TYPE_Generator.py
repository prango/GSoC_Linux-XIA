
__author__ = 'Pranav'
import string,random  
def Random_4bytes_XIA():
  length=2
  letters=string.ascii_lowercase+string.digits
  return '0x' + ''.join([random.choice(letters) for _ in range(length)])

for i in range(1,500):

    f = open('/Users/Pranav/Desktop/input_xid_type.txt','a')
    value = Random_4bytes_XIA()
    print >>f,value
    f.close()

