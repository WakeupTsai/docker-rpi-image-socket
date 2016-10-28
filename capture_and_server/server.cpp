#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<fstream>
#include<string>
#include<iostream>

using namespace std;

#define SIZE 100
char line[SIZE];


int send_image(int socket){
  FILE *picture;
  int size, read_size, stat, packet_index;
  char send_buffer[10240], read_buffer[256];
  packet_index = 1;

  fstream fin;
  fin.open("LastPicture.txt",ios::in);
  fin.getline(line,sizeof(line),'\n');
   
  if( line[0]=='\0'){
    cout << "No picture" << endl;
    return 0;
  }
  
  string filename = "picture/"+(string)line;  
  cout << "Send Image: " << (string)line << endl;  

  picture = fopen(filename.c_str(), "r");

  fin.close();

  fseek(picture, 0, SEEK_END);
  size = ftell(picture);
  fseek(picture, 0, SEEK_SET);

  //Send Picture Name
  char name[128];
  bzero(name,128);
  strcpy(name, line);
  write(socket, name, sizeof(name));

  //Send Picture Size
  write(socket, (void *)&size, sizeof(int));

  //Send Picture as Byte Array

  do { //Read while we get errors that are due to signals.
    stat=read(socket, &read_buffer , 255);
  } while (stat < 0);


  while(!feof(picture)) {
    //while(packet_index = 1){
    //Read from the file into our send buffer
    read_size = fread(send_buffer, 1, sizeof(send_buffer)-1, picture);

    //Send data through our socket
    do{
    stat = write(socket, send_buffer, read_size);
    }while (stat < 0);

    packet_index++;

    //Zero out our send buffer
    bzero(send_buffer, sizeof(send_buffer));
  }

}

int main(int argc , char *argv[])
{
  int socket_desc , new_socket , c, read_size,buffer = 0;
  struct sockaddr_in server , client;
  char *readin;

  //Create socket
  socket_desc = socket(AF_INET , SOCK_STREAM , 0);

  //Prepare the sockaddr_in structure
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons( 8889 );

  //Bind
  bind(socket_desc,(struct sockaddr *)&server , sizeof(server));
  //Listen
  listen(socket_desc , 3);
  cout << "Listen" << endl << endl;
  //Accept and incoming connection
  c = sizeof(struct sockaddr_in);

  while((new_socket = accept(socket_desc, (struct sockaddr *)&client,(socklen_t*)&c))){
    cout << "Coonnect" << endl ;
    if ( send_image(new_socket) == 0 ){
      close(new_socket);
    }
  }
  close(socket_desc);
  return 0;

}

