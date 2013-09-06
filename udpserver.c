/* Sample UDP server */

#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>

int main(int argc, char**argv)
{
   int sockfd,n;
   struct sockaddr_in servaddr,cliaddr;
   socklen_t len;
   char mesg[1000];

   sockfd=socket(AF_INET,SOCK_DGRAM,0);

   bzero(&servaddr,sizeof(servaddr));
   servaddr.sin_family = AF_INET;
   servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
   servaddr.sin_port=htons(9999);
   bind(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));

   for (;;)
   {
      len = sizeof(cliaddr);
      n = recvfrom(sockfd,mesg,1000,0,(struct sockaddr *)&cliaddr,&len);
      printf("-------------------------------------------------------\n");
      mesg[n] = 0;
      printf("Received the following:\n");
      printf("%s",mesg);

      //return string -- TODO make me bigger
      char ret[256];

      //length
      char length[5];
      strncpy(length, mesg, 4);

      //rid
      char rid[5];
      strncpy(rid, mesg+4, 4);

      //op
      char op[3];
      strncpy(op, mesg+8, 2);

      char string[strlen(mesg)];
      strncpy(string, mesg+10, strlen(mesg));

      printf("length: %s", rid);

      printf("-------------------------------------------------------\n");
      sendto(sockfd,"hello",n,0,(struct sockaddr *)&cliaddr,sizeof(cliaddr));
   }
}
