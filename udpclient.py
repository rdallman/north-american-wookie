import socket
import sys
import binascii

HOST, PORT = "localhost", 9999
#data = " ".join(sys.argv[1:])
data = sys.argv[1]

# TML
length = '%04X' %(len(data) + 5)
# TODO make this random
id = '%04X' %7
# OP
op = '%04X' %85
#STRING
string = binascii.hexlify(data)

hexstr = str(length)+str(id)+str(op)+string

# SOCK_DGRAM is the socket type to use for UDP sockets
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

# As you can see, there is no connect() call; UDP has no connections.
# Instead, data is directly sent to the recipient via sendto().
sock.sendto(hexstr + "\n", (HOST, PORT))
received = sock.recv(1024)

print "Sent:     {}".format(data)
print "Received: {}".format(received)
