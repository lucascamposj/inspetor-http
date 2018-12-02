/*
Propriedade de:
Andre Garrido Damaceno.- mat. 15/0117531
Lucas Campos Jorge - mat. 15/0154135
*/

#ifndef _Primary_libraries
  #define _Primary_libraries
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <ctype.h>
#endif

#ifndef _Socket_libraries
  #define _Socket_libraries
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <netdb.h>
		#include <arpa/inet.h>
    #include <unistd.h>
#endif

int create_socket()
{
  return socket(AF_INET,SOCK_STREAM,0);
}

void config_address(int port, struct sockaddr_in * address)
{
  bzero((char *) address, sizeof(*address));
  address->sin_family = AF_INET;
  address->sin_port = htons(port);
  address->sin_addr.s_addr = INADDR_ANY;
}

int get_ip(char hostname[], char *ip){
	struct hostent *he;
	struct in_addr **addr_list;
	int i;

	if ( (he = gethostbyname( hostname ) ) == NULL)
	{
		return -1;
	}

	//Cast the h_addr_list to in_addr , since h_addr_list also has the ip address in long format only
	addr_list = (struct in_addr **) he->h_addr_list;

	for(i = 0; addr_list[i] != NULL; i++)
	{
		//Return the first one;
		strcpy(ip , inet_ntoa(*addr_list[i]) );
	}

	return 0;
}

int proxy_connect(int socket,struct sockaddr_in * address){
  return connect(socket, (struct sockaddr *) address, sizeof(*address));
}

// recebe dado do servidor
int proxy_receive(int socket, char * message){
  return read(socket, message, sizeof(message));
}

int proxy_bind(int socket, struct sockaddr_in *address){
  return bind(socket, (struct sockaddr *) address, sizeof(*address));
}

int proxy_accept(int socket){
  return accept(socket, NULL, NULL);
}

int proxy_send(int socket, char * message){
  return send(socket, message, sizeof(message), 0);
}
