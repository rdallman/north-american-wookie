/* Sample UDP server */

#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>

void devowel(char* str);

int main(int argc, char**argv)
{
   int sockfd,n;
   struct sockaddr_in servaddr,cliaddr;
   socklen_t len;
   char mesg[1000];
   bzero(mesg, 1000);
   int port = atoi(argv[1]);

   sockfd=socket(AF_INET,SOCK_DGRAM,0);

   bzero(&servaddr,sizeof(servaddr));
   servaddr.sin_family = AF_INET;
   servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
   servaddr.sin_port=htons(port);
   bind(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));

   for (;;)
   {
      len = sizeof(cliaddr);
      n = recvfrom(sockfd,mesg,1000,0,(struct sockaddr *)&cliaddr,&len);
      printf("-------------------------------------------------------\n");
      mesg[n] = 0;
      printf("Received the following:\n");

      int i = 0;
      char hex[10];
      char hexmesg[1000];
      bzero(hexmesg, 1000);
      while(i < n) {
        sprintf(&hex[0], "%02X", 0xFF & mesg[i]);
        strcat(hexmesg, hex);
        i++;
      }
      printf("%s", hexmesg);

      //length
      char length[5];
      strncpy(length, hexmesg, 4);

      //rid
      char rid[5];
      strncpy(rid, hexmesg+4, 4);

      //op
      char op[3];
      strncpy(op, hexmesg+8, 2);
      int iop = (int)strtol(op, NULL, 16);

      //message
      char string[strlen(hexmesg)];
      strncpy(string, hexmesg+10, strlen(hexmesg));

      char tml[5];

      // make reply
      if (iop == 85) {
        //num vowels
        char reply[6];
        reply[0] = 0x00;
        reply[1] = 0x06;
        int hex = (int)strtol(rid, NULL, 16);
        reply[2] = ((hex >> 8) & 0xFF);
        reply[3] = hex & 0xFF;
        int numv = vowels(&string);
        reply[4] = ((numv >> 8) & 0xFF);
        reply[5] = numv & 0xFF;
        printf("-------------------------------------------------------\n");
        sendto(sockfd,reply,sizeof(reply),0,(struct sockaddr *)&cliaddr,sizeof(cliaddr));
      } else if (iop == 255 || iop == 170) {
        //devowel
        char reply[strlen(string)+4];
        bzero(reply, strlen(string)+4);
        devowel(&string);
        int tml = strlen(string)+4;
        reply[0] = ((tml >> 8) & 0xFF);
        reply[1] = tml & 0xFF;
        int hex = (int)strtol(rid, NULL, 16);
        reply[2] = ((hex >> 8) & 0xFF);
        reply[3] = hex & 0xFF;
        char letter[3];
        for (i=0;i<strlen(string);i+=2) {
          strncpy(letter, string+i, 2);
          reply[i+4] = (int)strtol(letter, NULL, 16);
        }

        printf("-------------------------------------------------------\n");
        sendto(sockfd,reply,sizeof(reply),0,(struct sockaddr *)&cliaddr,sizeof(cliaddr));
      }

   }
}

int vowels(char* str) {
  int i = 0;
  int vowels = 0;
  char letter[3];
  while(str[i]) {
    strncpy(letter, str+i, 2);
    int ascii = (int)strtol(letter, NULL, 16);
    if (isvowel(ascii))
      vowels++;
    i++;
    i++;
  }

  return vowels;
}

void devowel(char* str) {
  char *vstr[strlen(str)];
  bzero(&vstr, strlen(str));
  int i = 0;
  char letter[3];
  while(str[i]) {
    strncpy(letter, str+i, 2);
    int ascii = (int)strtol(letter, NULL, 16);
    if (!isvowel(ascii)) {
      sprintf(&letter[0], "%02x", ascii);
      strcat(vstr, letter);
    }
    i++;
    i++;
  }
  strcpy(str, vstr);
}

int isvowel(int hex) {
  int vowels = 0;
  switch(hex) {
    case 65:
      vowels++;
      break;
    case 97:
      vowels++;
      break;
    case 69:
      vowels++;
      break;
    case 101:
      vowels++;
      break;
    case 73:
      vowels++;
      break;
    case 105:
      vowels++;
      break;
    case 79:
      vowels++;
      break;
    case 111:
      vowels++;
      break;
    case 85:
      vowels++;
      break;
    case 117:
      vowels++;
      break;
  }
  return vowels;
}
