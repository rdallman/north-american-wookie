#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
    bzero(buffer,256);

    //TML
    char len[5];
    int length = strlen(argv[4]) + 5;
    if (length <= 0xFFFF)
      sprintf(&len[0], "%04x", length);
    strcat(buffer, len);

    //REQID TODO make this yourself (rand)
    int x = 7;
    char res[5]; // two bytes of hex = 4 characters, plus NULL terminator 
    if (x <= 0xFFFF)
        sprintf(&res[0], "%04x", x);
    strcat(buffer, res);

    //OP
    x = atoi(argv[3]);
    char op[3];
    if (x == 0xAA || x == 0x55)
        sprintf(&op[0], "%02x", x);
    else
      error("invalid op");
    strcat(buffer, op);

    //STRING
    char* str = argv[4];
    char* ascii[3];
    int i = 0;
    while(str[i]) {
       sprintf(&ascii[0],"%02x",str[i++]);
       strcat(buffer, ascii);
    }

    time_t t1, t2;
    t1 = time(0);


    n = write(sockfd,buffer,strlen(buffer));
    printf("Sent: %s\n",buffer);
    if (n < 0) 
         error("ERROR writing to socket");
    bzero(buffer,256);
    n = read(sockfd,buffer,255);

    t2 = time(0);

    if (n < 0) 
         error("ERROR reading from socket");
    printf("Received: %s\n",buffer);
    printf("Elapsed: %f\n", difftime(t2, t1));
    close(sockfd);
    return 0;
}
