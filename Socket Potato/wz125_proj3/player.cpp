#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <sstream>
#include "potato.h"


int main(int argc, char *argv[])
{
  player* players = new player();;
  
  players->init_client(argc, argv);
  players->receive_potato();
  players->freeaddr();
  players->over();
  delete(players);
  return EXIT_SUCCESS;
}
void player::over(){
  close(master_fd);
  close(commu_fd);
  close(client_fd);
}
void player::init_client(int argc, char* argv[]){
  if (argc != 3) {
    std::cout << "Syntax: client <machine name> <port_num>\n";
    exit(EXIT_FAILURE);
  }
  char* hostname = argv[1];
  char* port = argv[2];  
  connect_master(hostname, port);
}

void player::create_player(const char* hostname,const char* port){
  int status;
  memset(&client_info, 0, sizeof(client_info));
  client_info.ai_family   = AF_UNSPEC;
  client_info.ai_socktype = SOCK_STREAM;

  status = getaddrinfo(hostname, port, &client_info, &client_info_list);
  //host_info stored the destination server IP 
  if (status != 0) {
    std::cerr << "Error: cannot get address info for host\n";
    std::cerr << "  (" << hostname << "," << port << ")\n";
    exit(EXIT_FAILURE);
  } //if
  //create the socket_fd for current node to work as a client.
  //client will use this socket_fd to communicate with others
  connect_master_fd = socket(client_info_list->ai_family, 
		     client_info_list->ai_socktype, 
		     client_info_list->ai_protocol);
  if (connect_master_fd == -1) {
    std::cerr << "Error: cannot create socket\n";
    std::cerr << "  (" << hostname << "," << port << ")\n";
    exit(EXIT_FAILURE);
  } //if
    
  status = connect(connect_master_fd, client_info_list->ai_addr, client_info_list->ai_addrlen);//host_info_list->ai_addr server IP, got from getaddrinfo
  if (status == -1) {
    std::cerr << "Error: cannot connect to socket\n";
    std::cerr << "  (" << hostname << "," << port << ")\n";
    exit(EXIT_FAILURE);
  } //if
  freeaddrinfo(client_info_list);
  //  freeaddr();
}

void player::get_init_msg(){
  //get player_id, num_player and resend ACK
  recv(connect_master_fd, &player_num, sizeof(player_num), 0);
  recv(connect_master_fd, &player_id, sizeof(player_id), 0);
  std::cout << "Connected as player " << player_id << " out of " << player_num << " total players\n";  
  send(connect_master_fd, &player_id, sizeof(player_id), 0);

  int port_as_server = as_server_socket();
  
  send(connect_master_fd, &port_as_server, sizeof(port_as_server),MSG_WAITALL);
  //get neighbor's socket_fd
  info socket;
  recv(connect_master_fd, &socket, sizeof(socket), MSG_WAITALL);
  std::stringstream sstr;
  sstr << socket.port_neigh_right;
  std::string str_temp = sstr.str();
  char port_id[10];
  strcpy(port_id, str_temp.c_str());
  create_player(socket.ip_addr, port_id);
  client_fd = connect_master_fd;
  commu_fd = accept_connect(player_server_fd);//this socket works for node as server to communicate
  access_fd.push_back(client_fd);
  access_fd.push_back(commu_fd);
}

int player::max_fds(int fd1, int fd2, int fd3){
  int nfds;
  nfds = fd1 > fd2 ? fd1 : fd2;
  nfds = nfds > fd3 ? nfds : fd3;
  return nfds + 1;
}
void player::receive_potato(){
  fd_set socketfds;
  int fds[] = {master_fd, client_fd, commu_fd};
  potato recv_potato;
  srand((unsigned int)time(NULL) + player_id);
  while(1){
    FD_ZERO(&socketfds);
    FD_SET(master_fd, &socketfds);
    FD_SET(client_fd, &socketfds);
    FD_SET(commu_fd, &socketfds);
    int nfds = max_fds(master_fd, client_fd, commu_fd);
    int retval = select(nfds, &socketfds, NULL, NULL,NULL);
    if (retval == -1){
      perror("select()");
    }
    for(int i=0; i< 3; i++){
      if(FD_ISSET(fds[i], &socketfds)){
	if(recv(fds[i], &recv_potato, sizeof(recv_potato), MSG_WAITALL) == sizeof(potato)){	 
	    if(recv_potato.get_hops() == 0){
	      return;
	    }
	    recv_potato.decrease_hops();
	    recv_potato.add_path(player_id);
       	    if(recv_potato.get_hops() == 0){
	      std::cout << "I'm it\n";
	      if(send(master_fd, &recv_potato, sizeof(recv_potato), 0) == sizeof(potato)){
		return;
	      }
	      else{
		std::cout<<"Send to master error size\n";
	      }
	    }

	    int left_right = rand() % 2;
    
	    int total_players = recv_potato.get_num_players();
	    int next_id;
	    if(left_right == 0){
	      next_id = (player_id + total_players - 1 ) % total_players;
	    }
	    else{
	      next_id = (player_id + 1) % total_players;
	    }
	    std::cout << "Sending potato to " << next_id << std::endl;
	    send(access_fd[left_right], &recv_potato, sizeof(recv_potato), 0);
	  }
	} 
      }
    }
}

int player::as_server_socket(){
  init_server(NULL, "");
  struct sockaddr_in* temp_addr = (struct sockaddr_in*) host_info_list->ai_addr;
  temp_addr->sin_port = 0;
  player_server_fd = create();
  ready_to_listen(player_server_fd);
  struct sockaddr_in temp;
  socklen_t len = sizeof(temp);
  int status = getsockname(player_server_fd,(struct sockaddr*)&temp, &len);
  if(status == -1){
    std::cerr << "getsockname() error\n";
  }
  return ntohs(temp.sin_port);
}

