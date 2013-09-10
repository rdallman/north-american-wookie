import SocketServer
import binascii
import sys
import array

class TCPHandler(SocketServer.BaseRequestHandler):


  def handle(self):
    msg = self.request.recv(1024)
    msg = binascii.hexlify(msg)
    print "\nReceived: "+msg.strip()

    length = int(msg[0:4], 16)
    rid = int(msg[4:8], 16)
    op = int(msg[8:10], 16)
    string = msg[10:].decode('hex')
    if op is 85:
      numv = '%04X' %(int(self.vowels(string)))
      length = '0006'
    elif op is 170:
      numv = binascii.hexlify(self.devowel(string))
      length = '%04X' %(len(numv) + 4)
    else:
      numv = "Error, not a valid operation"

    length = array.array('B', str(length).decode("hex"))
    rid = '%04X' %(rid)
    rid = array.array('B', str(rid).decode("hex"))
    numv = array.array('B', numv.decode("hex"))
    reply = length + rid + numv

    self.request.sendall(reply)

  def vowels(self, string):
    vowels = "aeiouAEIOU"
    return str(len([letter for letter in string if letter in vowels]))

  def devowel(self, string):
    vowels = "aeiouAEIOU"
    return ''.join(letter for letter in string if letter not in vowels)


if __name__ == "__main__":


  HOST, PORT =  '', int(sys.argv[1])

  server = SocketServer.TCPServer((HOST, PORT), TCPHandler)

  server.serve_forever()


