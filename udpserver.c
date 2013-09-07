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
      printf("%s",mesg);

      //length
      char length[5];
      strncpy(length, mesg, 4);

      //rid
      char rid[5];
      strncpy(rid, mesg+4, 4);

      //op
      char op[3];
      strncpy(op, mesg+8, 2);
      int iop = (int)strtol(op, NULL, 16);

      //message
      char string[strlen(mesg)];
      strncpy(string, mesg+10, strlen(mesg));

      char tml[5];
      char reply[strlen(mesg)];
      bzero(&reply, strlen(mesg));

      // make reply
      if (iop == 85) {
        //num vowels
        strcat(reply, "0006");
        strcat(reply, rid);
        sprintf(&tml[0], "%04x", vowels(&string));
        strcat(reply, tml);
      } else if (iop == 170) {
        //devowel
        devowel(&string);
        sprintf(&tml[0], "%04x", strlen(string)+4);
        strcat(reply, tml);
        strcat(reply, rid);
        strcat(reply, string);
      }

      printf("-------------------------------------------------------\n");
      sendto(sockfd,reply,strlen(reply),0,(struct sockaddr *)&cliaddr,sizeof(cliaddr));
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
  while(str[i+2]) {
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
