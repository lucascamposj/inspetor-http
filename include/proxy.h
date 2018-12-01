/*
Propriedade de:
Andre Garrido Damaceno.- mat. 15/0117531
Lucas Campos Jorge - mat. 15/0154135
*/

int create_socket();
void config_address(int, struct sockaddr_in *);
int proxy_connect(int, struct sockaddr_in *);
int proxy_receive(int, char *); // recebe dado do servidor
int proxy_bind(int, struct sockaddr_in *);
int proxy_accept(int socket);
int proxy_send(int ,char *);
