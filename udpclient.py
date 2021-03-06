import socket
import sys
import binascii
import time
import array

HOST, PORT = "", 9999

HOST= sys.argv[1]
PORT = int(sys.argv[2])
op = int(sys.argv[3])  & 0x000000FF
intop = op
data = sys.argv[4]

# TML
length = '%04X' %(len(data) + 5)
# TODO make this random
id = '%04X' %7
# OP
op = '%02X' %op
#STRING
string = binascii.hexlify(data)

hexstr = str(length)+str(id)+str(op)+str(string)

byteary = array.array('B', hexstr.decode("hex"))

# SOCK_DGRAM is the socket type to use for UDP sockets
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

sock.sendto(byteary, (HOST, PORT))
t1 = time.time()

received = sock.recv(1024)
hexstr = binascii.hexlify(received)

print "Sent:     "+data

if intop is 85:
  print "Received: "+str(int(hexstr[8:], 16))
else:
  print "Recieved: "+binascii.unhexlify(hexstr[8:])

print "Total Time: "+str((time.time() - t1) * 1000)
