
import string,random  
def Random_4bytes_XIA():
  length=4
  letters=string.ascii_lowercase+string.digits
  return ''.join([random.choice(letters) for _ in range(length)])

for i in range(1,320):
	print Random_4bytes_XIA()
