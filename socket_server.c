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
#define BUF_SIZE    100
#define MSG_LEN     12

static void print_msg(char *buffer, int length);
static int read_from_socket(int socket, char *buffer, int length);

int main(void)
{
    int s, t;
    struct sockaddr_in sa;
    static char buf[BUF_SIZE];

    memset(&sa, 0, sizeof(struct sockaddr_in)); 
    
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = htonl(INADDR_ANY);
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

    if (bind(s,(struct sockaddr *) &sa,sizeof(struct sockaddr_in)) < 0) {
        printf("Server: Socket bind failed\n");
        close(s);
        return(-1);                               /* bind address to socket */
    }
    else
    {
        printf("Server: Socket bind was successful\n");
    }

    if(listen(s, 1) < 0)
    {
        printf("Server: Listen error on port %d\n",PORTNUM);
        close(s);
        return(-1);
    }
    else
    {
        printf("Server: Listening on port %d\n",PORTNUM);   
    }

    while(1)
    {
        t = accept(s,NULL,NULL);

        if( t < 0)
        {
            printf("Connection accept failed\n");
            close(s);
            return(-1);
        }
        else
        {
            printf("New connection accepted from client\n");
            if(read_from_socket(t,buf, MSG_LEN) < 0)
            {
                printf("Socket read failed\n");
            }
            else
            {
                printf("Socket read successful\n");
                printf("Message received from client: ");
                print_msg(buf, MSG_LEN);
            }
        }
    }

    return 0;
}

static int read_from_socket(int socket, char *buffer, int length)
{
    int bcount; /* counts bytes read */
    int br;     /* bytes read this pass */

    bcount= 0;
    br= 0;
    while (bcount < length) 
    {   /* loop until full buffer */
        if ((br= read(socket,buffer,length-bcount)) > 0) {
            bcount += br;                   /* increment byte counter */
            buffer += br;                   /* move buffer ptr for next read */
        }
        else if (br < 0)                    /* signal an error to the caller */
            return(-1);
    }
    return(bcount);
}

static void print_msg(char *buffer, int length)
{
    int i = 0;

    while(i < length)
    {
        putchar(buffer[i]);
        i++;
    }

    return;
}