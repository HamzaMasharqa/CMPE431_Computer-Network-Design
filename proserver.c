#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#define DEFAULT_BUFLEN 1024
#define PORT 2428

sturct user{
char username[20];
char password[20];
}


int login(&username,&password)
{
   

    char username[50];
    char password[50];

    FILE *infile;
    struct user person;


   

    infile = fopen ("s.txt", "r");
    if (infile == NULL)
    {
        fprintf(stderr, "\nError opening file\n");
        exit (1);
    }

   
    while(fread(&person, sizeof(struct user), 1, infile)){
        if(strcmp(username,person.username) == 0 && \
           strcmp(password, person.password) ==0)
        {
            hrmenu();
            break;
        }    
        else
        {
            printf("Wrong Credentials, Please try again!\n");
            login();    
        }
    }

    fclose(infile);
    return 0;    
}



void read_dir(int fd){
int length,rcnt;
char recvbuf[DEFAULT_BUFLEN],bmsg[DEFAULT_BUFLEN];
int  recvbuflen = DEFAULT_BUFLEN;


 DIR *pDIR;
 
    struct dirent *pDirEnt;


    pDIR = opendir("HI");

    if ( pDIR == NULL ) {
        fprintf( stderr, "%s %d: opendir() failed (%s)\n",
                __FILE__, __LINE__, strerror( errno ));
        exit( -1 );
    }

   

    pDirEnt = readdir( pDIR );
    while ( pDirEnt != NULL ) {
        printf( "%s\n", pDirEnt->d_name ,'\n','\n','\n');
        pDirEnt = readdir( pDIR );
       
    }


    closedir( pDIR );

    return 0;
    
}
void do_job(int fd) {
int length,rcnt;
char recvbuf[DEFAULT_BUFLEN],bmsg[DEFAULT_BUFLEN];
int  recvbuflen = DEFAULT_BUFLEN;
int userN,passW;

    char Welcomsge[100] = "Welcome to the Hamza's server  \n";
     char userName[100] = "Please Enter your username : \n";
     scanf(userN);
      char passWord[100] = "Please enter your password :\n";
     scanf(passW);
   rcnt = send(fd, Welcomsge, strlen(Welcomsge), 0);
    rcnt = send(fd, userName, strlen(userName), 0);
    rcnt = send(fd, passWord, strlen(passWord), 0);
     
    read_dir(fd);
 
 
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
