#include "potato.h"

void server::init_server( char* hostname,const  char* port){
  //for user typed in hostname and port number
  int status;
  memset(&host_info, 0, sizeof(host_info));
  host_info.ai_family   = AF_UNSPEC;
  host_info.ai_socktype = SOCK_STREAM;
  host_info.ai_flags    = AI_PASSIVE;
    
  status = getaddrinfo(hostname, port, &host_info, &host_info_list);
  if (status != 0) {
    std::cerr << "Error: cannot get address info for host" << std::endl;
    std::cerr << "  (" << hostname << "," << port << ")" << std::endl;
    exit(EXIT_FAILURE);
  }
}

int server::create(){
  int socket_fd;
  int status;
  //create a socket for this
  socket_fd = socket(host_info_list->ai_family, 
		     host_info_list->ai_socktype, 
		     host_info_list->ai_protocol);
  if (socket_fd == -1) {
    std::cerr << "Error: cannot create socket" << std:: endl;
    exit(EXIT_FAILURE);
  }
  return socket_fd;
}

void server::ready_to_listen(int current_fd){
  int status;
  int yes = 1;
  status = setsockopt(current_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
  status = bind(current_fd, host_info_list->ai_addr, host_info_list->ai_addrlen);//bind the socket with host_info_list->ai_addr
  if (status == -1) {
    std::cerr << "Error: cannot bind socket" << std::endl;
    exit(EXIT_FAILURE);
  } 
  
  status = listen(current_fd, 100);
  if (status == -1) {
    std::cerr << "Error: cannot listen on socket" << std::endl; 
    exit(EXIT_FAILURE);
  }
}

int server::accept_connect(int socket_fd, int times){//for master
    struct sockaddr_storage socket_addr;
    socklen_t socket_addr_len = sizeof(socket_addr);
    int client_connection_fd;
    client_connection_fd = accept(socket_fd, (struct sockaddr *)&socket_addr, &socket_addr_len);//return the client's IP addr
    if (client_connection_fd == -1) {
      std::cerr << "Error: cannot accept connection on socket" << std::endl;
      exit(EXIT_FAILURE);
    }
    struct sockaddr_in *temp = (struct sockaddr_in *)&socket_addr;
    hostname_ip.push_back( inet_ntoa(temp->sin_addr));
    return client_connection_fd;
}


int server::accept_connect(int socket_fd){//for player
    struct sockaddr_storage socket_addr;
    socklen_t socket_addr_len = sizeof(socket_addr);
    int client_connection_fd;
    client_connection_fd = accept(socket_fd, (struct sockaddr *)&socket_addr, &socket_addr_len);//return the client's IP addr
    if (client_connection_fd == -1) {
      std::cerr << "Error: cannot accept connection on socket" << std::endl;
      exit(EXIT_FAILURE);
    }
    return client_connection_fd;
}
