#include <errno.h>       
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>

#define PORTNUM     50000

char message[]="Hello World\n";

int main(void)
{
    int s, t;
    struct sockaddr_in sa;

    memset(&sa, 0, sizeof(struct sockaddr_in)); 
    
    sa.sin_family = AF_INET;
    sa.sin_port = htons(PORTNUM);

    s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    if(s < 0)
    {
        printf("Server: Socket could not be opened\n");
        return -1;
    }
    else
    {
        printf("Server: Socket was opened successfully\n");
    }

    if (connect(s, (struct sockaddr *) &sa,sizeof(struct sockaddr_in)) < 0) {
        printf("connect() failed\n");   
    }
    else
    {
        printf("Connection to server was successful\n");
        send(s,message,strlen(message),0);
    }

    return 0;
}