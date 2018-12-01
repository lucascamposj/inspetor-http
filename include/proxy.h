int create_socket();

void config_address(int, struct sockaddr_in *);

int proxy_connect(int, struct sockaddr_in *);

// recebe dado do servidor
int proxy_receive(int, char *);

int proxy_bind(int, struct sockaddr_in *);

int proxy_accept(int socket);

int proxy_send(int ,char *);
