#include "potato.h"

int main(int argc, char* argv[]){
  srand((unsigned int)time(NULL));
  if(argc != 4){
    std::cout << "The input arguments is not enough! Format:./ringmaster <port_num> <num_players> <num_hops>\n";
    return EXIT_FAILURE;
  }
  int temp_players;
  sscanf(argv[2], "%d", &temp_players);
  if(temp_players < 2){
    std::cout << "Error: num_players needs greater than 1\n";
    return EXIT_FAILURE;
  }
  
  int temp_hops;
  sscanf(argv[3],"%d", &temp_hops);
  if(temp_hops < 0 || temp_hops > 512){
    std::cout << "Error: num_hops must be greater than or equal to zero and less than or equal to 512\n";
    return EXIT_FAILURE;
  }
  
  ringmaster* master = new ringmaster(argv[2], argv[3]); 
  master->print_init();
  master->connect_center(NULL, argv[1]);
  master->send_neigh_msg();
  master->send_potato();
  master->recv_potato();
  master->over_fds();
  delete(master);
  return EXIT_SUCCESS;
}

void ringmaster::over_fds(){
  freeaddr();
  for(int i = 0; i < num_players; i++){
    close(player_sockfd[i]);
  }
}

int ringmaster::find_max(){
  int nfds = player_sockfd[0];
  for(int i = 0; i < player_sockfd.size(); i++){
    if(nfds < player_sockfd[i]){
      nfds = player_sockfd[i];
    }
  }
  return nfds + 1;
}

void ringmaster::recv_potato(){
  potato final_potato;
  fd_set recv_fd;
    FD_ZERO(&recv_fd);
    int nfds = find_max();
    for(int i = 0; i < num_players; i++){
      FD_SET(player_sockfd[i], &recv_fd);
    }
    int retval = select(nfds, &recv_fd, NULL, NULL, NULL);
    if (retval == -1){
      perror("select()");
    }
    for(int i = 0; i < player_sockfd.size(); i++){
      if(FD_ISSET(player_sockfd[i], &recv_fd)){
	if(recv(player_sockfd[i], &final_potato, sizeof(final_potato), MSG_WAITALL) == sizeof(potato)){
	  int node;
	  std::cout << "Trace of potato:\n";
	  for(int i = 0; i < num_hops + 1; i++){
	    node = final_potato.get_path(i);
	    if(i == num_hops - 1){
	      std::cout << node <<std::endl;
    	      for(int i = 0; i < num_players; i++){
		send(player_sockfd[i], &final_potato, sizeof(final_potato), 0); 
	      }
	      return;
	    }
	    else{
	      std::cout << node << ", ";
	    }
	  }
      }
    }
  }
}

void ringmaster::send_potato(){
  potato send_potato(num_hops, num_players);
  if(num_hops == 0){
    for(int i = 0; i < num_players; i++){
      send(player_sockfd[i], &send_potato, sizeof(send_potato), 0); 
      std::cout << "Trace of potato:\n";
    }
    return;
  }
  int random_start_ID = rand() % num_players;
  std::cout << "Ready to start the game, sending potato to player " << random_start_ID << std::endl;
  send(player_sockfd[random_start_ID], &send_potato, sizeof(send_potato), 0);
}

void ringmaster::connect_center(char* hostname,const char* port){
  init_server(hostname, port);
  int master_fd = create();
  ready_to_listen(master_fd);
  connections(master_fd);
}
void ringmaster::print_init(){
  std::cout << "Potato Ringmaster\n";
  std::cout << "Players = " << this->num_players << std::endl;
  std::cout << "Hops = " << this->num_hops << std::endl;
}

void ringmaster::send_neigh_msg(){
  for(int i = 0; i < num_players; i++){
    int neighbor_right = player_neigh_port[(i + num_players - 1) % num_players];//right
    int neighbor_left = player_neigh_port[(i + 1) % num_players];//left
    std::string str = gethostnameip((i + num_players - 1) % num_players);
    info addr_info;
    strcpy(addr_info.ip_addr, str.c_str());
    addr_info.port_neigh_right = neighbor_right;
    addr_info.port_neigh_left = neighbor_left;
    send(player_sockfd[i], &addr_info, sizeof(addr_info), 0);   
  }
}

void ringmaster::connections(int current_fd){
  int times = 0;
  int temp_fd;
  while(times < num_players){  
    temp_fd = accept_connect(current_fd, times);
    // std::cout<<"All the socket_fd: " << temp_fd<<std::endl;
    player_sockfd.push_back(temp_fd);    
    send_initial_msg(temp_fd, num_players, times);
    times++;
  }
}


void ringmaster::send_initial_msg(int client_connection_fd, int num_players, int times ){
    send(client_connection_fd, &num_players, sizeof(num_players), 0);
    send(client_connection_fd, &times, sizeof(times), 0);
    int receive_id;
    recv(client_connection_fd, &receive_id, sizeof(receive_id), 0);
    std::cout << "Player " << receive_id << " is ready to play\n";
    int neigh_port;
    recv(client_connection_fd, &neigh_port, sizeof(neigh_port), MSG_WAITALL );
    player_neigh_port[receive_id] = neigh_port;
    //std::cout<< "Neighbour socket_fd for working as a server: " << neigh_port << std::endl;
}

