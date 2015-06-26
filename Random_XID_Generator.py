__author__ = 'Pranav'
import string,random  
def Random_4bytes_XIA():
  length=4
  letters=string.ascii_lowercase+string.digits
  return ''.join([random.choice(letters) for _ in range(length)])

for i in range(1,50000):

    f = open('/Users/Pranav/Desktop/input_xia.txt','a')
    value = Random_4bytes_XIA()
    print >>f,value
    f.close()
