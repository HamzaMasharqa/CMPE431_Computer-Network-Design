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

//#define ls 5863588
#define DATA_SIZE 1000
#define MAXCHAR 1000
#define DEFAULT_BUFLEN 1024
#define PORT 2428


void fileMaker(int fd){
 int length, clin;
  char clientmsg[DEFAULT_BUFLEN],clientfile[DEFAULT_BUFLEN], bmsg[DEFAULT_BUFLEN];
  int clientmsglen = DEFAULT_BUFLEN;

int clientfilelen = DEFAULT_BUFLEN;

 char data[DATA_SIZE];

    FILE * fPtr;
    
    
     clin = send(fd, "What is the File Name : \n \n \n ", strlen("What is the File Name : \n \n \n "), 0);
    clin = recv(fd, clientfile, clientfilelen, 0);
    
 clin = send(fd, "text inside : \n \n \n ", strlen("text inside: \n \n \n "), 0);
    clin = recv(fd, clientmsg, clientmsglen, 0);
    
    
    
    char fopenPath[DEFAULT_BUFLEN] = {clientmsg ,"/",clientfile};

    fPtr = fopen( fopenPath , "w");


  
    if(fPtr == NULL)
    {
       
        printf("Unable to create file.\n");
        exit(EXIT_FAILURE);
    }


   clin = send(fd, "Enter contents to store in file : \n", strlen("Enter contents to store in file : \n"), 0);
    
    clin = recv(fd, clientfile, clientmsglen, 0);
    
    fgets(data, DATA_SIZE, stdin)==clientfile;


 
    fputs(data, fPtr);


    
    fclose(fPtr);


   
    printf("File created and saved successfully. :) \n");



}

void fileReader(int fd){

  int length, clin;
  char clientmsg[DEFAULT_BUFLEN], bmsg[DEFAULT_BUFLEN];
  int clientmsglen = DEFAULT_BUFLEN;

 clin = send(fd, "File Name : \n \n \n ", strlen("File Name : \n \n \n "), 0);
    clin = recv(fd, clientmsg, clientmsglen, 0);


 FILE *fp;
 clientmsg[clin] = '\0';
 char* filename =clientmsg;
 
 
    char str[MAXCHAR];
    
    
    
 
    fp = fopen(clientmsg, "r");
    if (fp == NULL && strcmp(clientmsg,filename)==0){
    
    
    clin = send(fd, "Could not open file \n\n", strlen("Could not open file \n\n\n"), 0);
     
      
    }
    while (fgets(str, MAXCHAR, fp) != NULL)
     clin = send(fd, "File contents : \n \n \n ", strlen("File contents : \n \n \n "), 0);
      clin = send(fd, str, strlen(str), 0);
        printf("%s", str);
    fclose(fp);
   



}

void readDir(int fd) {

  int length, clin;
  char clientmsg[DEFAULT_BUFLEN], bmsg[DEFAULT_BUFLEN];
  int clientmsglen = DEFAULT_BUFLEN;

  DIR * pDIR;

  struct dirent * pDirEnt;

  pDIR = opendir("HI");

  if (pDIR == NULL) {
    fprintf(stderr, "%s %d: opendir() failed (%s)\n",
      __FILE__, __LINE__, strerror(errno));
    exit(-1);
  }

  clin = send(fd, "Those are the files in your directory: \n \n ", strlen("Those are the files in your directory: \n \n "), 0);

  pDirEnt = readdir(pDIR);
  while (pDirEnt != NULL) {

    clin = send(fd, pDirEnt -> d_name, strlen(pDirEnt -> d_name), 0);
    clin = send(fd, "\n", strlen("\n"), 0);

    //   printf( "%s\n", pDirEnt->d_name ,'\n','\n','\n');
    pDirEnt = readdir(pDIR);

  }
  clin = send(fd, "\n\n\n", strlen("\n\n\n"), 0);
  closedir(pDIR);

}

void do_job(int fd) {
  int length, clin;
  char clientmsg[DEFAULT_BUFLEN], bmsg[DEFAULT_BUFLEN];
  int clientmsglen = DEFAULT_BUFLEN;

  char choise[DEFAULT_BUFLEN];

  char Welcomsge[100] = "Welcome to the Hamza's server  \n";
  
  //   char userName[100] = "Please Enter your username : \n";

  //   char passWord[100] = "Please enter your password :\n";

  clin = send(fd, Welcomsge, strlen(Welcomsge), 0);
  //clin = send(fd, userName, strlen(userName), 0);
  //  clin = send(fd, passWord, strlen(passWord), 0);

  do {
    clin = send(fd, "please choose one of the commands : \n \n \n ", strlen("please choose one of the commands : \n \n \n "), 0);
    clin = recv(fd, clientmsg, clientmsglen, 0);

    clientmsg[clin] = '\0';

    if (strcmp(clientmsg, "list\n") == 0) {

      readDir(fd);
    } else if (strcmp(clientmsg, "get\n") == 0){
    
    fileReader(fd);
     
    }else if (strcmp(clientmsg, "put\n") == 0){
    
      fileMaker(fd);

     
    }else{
     printf("something wrong");
    }

  } while (clin > 0);
  //////////////////////////////////////////////////////////////////////////////////////////

}

int main() {
  int server, client;
  struct sockaddr_in local_addr;
  struct sockaddr_in remote_addr;
  int length, fd, clin, optval;
  pid_t pid;

  if ((server = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("Can't create socket!");
    return (1);
  }

  memset( & local_addr, 0, sizeof(local_addr));
  memset( & remote_addr, 0, sizeof(remote_addr));

  local_addr.sin_family = AF_INET;
  local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  local_addr.sin_port = htons(PORT);

  optval = 1;
  setsockopt(server, SOL_SOCKET, SO_REUSEADDR, & optval, sizeof optval);

  if (bind(server, (struct sockaddr * ) & local_addr, sizeof(local_addr)) < 0) {

    perror("Error");
    return (1);
  }

  if (listen(server, SOMAXCONN) < 0) {
    perror("listen");
    exit(1);
  }

  printf("Socket server now starting on port %d\n", PORT);
  printf("Wait for connection\n");

  while (1) {
    length = sizeof remote_addr;
    if ((fd = accept(server, (struct sockaddr * ) & remote_addr, &
        length)) == -1) {
      perror("Accept Problem!");
      continue;
    }
    printf("Server: got connection from %s\n", \
      inet_ntoa(remote_addr.sin_addr));

    if ((pid = fork()) == 0) {
      close(server);
      do_job(fd);

      printf("User FInished !\n");
      close(fd);
      exit(0);
    }

  }

  close(server);

}
