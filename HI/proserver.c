#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Socket API headers */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

/* Definations */
#define DEFAULT_BUFLEN 1024
#define PORT 2428







///void seCurity()

void read_dir(){

 DIR *pDIR;
 
    struct dirent *pDirEnt;

    /* Open the current directory */

    pDIR = opendir(".");

    if ( pDIR == NULL ) {
        fprintf( stderr, "%s %d: opendir() failed (%s)\n",
                __FILE__, __LINE__, strerror( errno ));
        exit( -1 );
    }

    /* Get each directory entry from pDIR and print its name */

    pDirEnt = readdir( pDIR );
    while ( pDirEnt != NULL ) {
        printf( "%s\n", pDirEnt->d_name );
        pDirEnt = readdir( pDIR );
    }

    /* Release the open directory */

    closedir( pDIR );

    return 0;




}




void do_job(int fd) {
int length,rcnt;
char recvbuf[DEFAULT_BUFLEN],bmsg[DEFAULT_BUFLEN];
int  recvbuflen = DEFAULT_BUFLEN;

    char Welcomsge[100] = "Welcome to the Hamza's server";

   rcnt = send(fd, Welcomsge, strlen(Welcomsge), 0);
   
   read_dir();
   
   
   
    do {
        rcnt = recv(fd, recvbuf, recvbuflen, 0);
        if (rcnt > 0) {
           

       
        rcnt = send( fd, recvbuf, rcnt, 0 );
            if (rcnt < 0) {
                printf("Send failed:\n");
                close(fd);
                break;
            }
        

        }
        else if (rcnt == 0)
            printf("Connection closing...\n");
        else  {
            printf("Receive failed:\n");
            close(fd);
            break;
        }
    } while (rcnt > 0);
}



int main()
{
int server, client;
struct sockaddr_in local_addr;
struct sockaddr_in remote_addr;
int length,fd,rcnt,optval;
pid_t pid;


if ((server = socket( AF_INET, SOCK_STREAM, 0)) < 0 ) { 
    perror("Can't create socket!");
    return(1);
}



memset( &local_addr, 0, sizeof(local_addr) );
memset( &remote_addr, 0, sizeof(remote_addr) );

local_addr.sin_family = AF_INET;
local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
local_addr.sin_port = htons(PORT);


optval = 1;
setsockopt(server, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);

if ( bind( server, (struct sockaddr *)&local_addr, sizeof(local_addr) ) < 0 )
{
   
    perror("Bind error");
    return(1);
}

if ( listen( server, SOMAXCONN ) < 0 ) {
        perror("listen");
        exit(1);
}

printf("Concurrent  socket server now starting on port %d\n",PORT);
printf("Wait for connection\n");

while(1) {  
    length = sizeof remote_addr;
    if ((fd = accept(server, (struct sockaddr *)&remote_addr, \
          &length)) == -1) {
          perror("Accept Problem!");
          continue;
    }
    printf("Server: got connection from %s\n", \
            inet_ntoa(remote_addr.sin_addr));

   
    if ((pid=fork()) == 0) {
        close(server);
        do_job(fd);
        printf("Child finished their job!\n");
        close(fd);
        exit(0);
    }

}


close(server);

}
