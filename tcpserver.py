import SocketServer

class TCPHandler(SocketServer.BaseRequestHandler):


  def handle(self):
    self.data = self.request.recv(1024).strip()
    print "{} wrote:".format(self.client_address[0])
    print self.data
    ary = list(self.data)
    length = ary[0]
    rid = ary[1]
    op = ary[2]
    string = ary[4:]
    numv = self.devowel(string)
    self.request.sendall(numv)

  def vowels(self, string):
    vowels = "aeiouAEIOU"
    return str(len([letter for letter in string if letter in vowels]))

  def devowel(self, string):
    vowels = "aeiouAEIOU"
    return ''.join(letter for letter in string if letter not in vowels)


if __name__ == "__main__":
  HOST, PORT = "localhost", 9999

  server = SocketServer.TCPServer((HOST, PORT), TCPHandler)

  server.serve_forever()


