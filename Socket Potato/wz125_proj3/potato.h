#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <map>
#include <vector>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <cstring>

class server{
private:
  std::vector<std::string> hostname_ip; 
public:
  struct addrinfo host_info;
  struct addrinfo* host_info_list;
  void init_server(char* hostname, const char* port);
  int create();
  void ready_to_listen(int current_fd);
  int accept_connect(int socket_fd, int times);
  int accept_connect(int socket_fd);
  std::string gethostnameip(int id){
    return hostname_ip[id];
  }
  void freeaddr(){
    freeaddrinfo(host_info_list);
  }
};

class ringmaster: public server{
private:
  std::map<int, int> player_neigh_port;
  std::vector<int> player_sockfd;
  //int neighbor_right;
  //int neighbor_left;
  int num_players;
  int num_hops;
public:
  ringmaster(char* players, char* hops){
    num_players = atoi(players);
    num_hops = atoi(hops);
  };
  void print_init();
  void connect_center(char* hostname, const char* port);
  void connections(int current_fd);
  void send_initial_msg(int client_connection_fd, int numplayers, int times);
  void send_neigh_msg();
  void send_potato();
  void recv_potato();
  int find_max();
  void over_fds();
};

class player: public server {
private:
  std::vector<int> access_fd;
  int player_id;
  int player_num;
  int connect_master_fd;
  int master_fd;//to master
  int client_fd;//to player as client
  int commu_fd;//to player as server
  int player_server_fd;
  struct addrinfo client_info;
  struct addrinfo* client_info_list;
public:
  void connect_master(char* hostname,const char* port){
    create_player(hostname, port);
    master_fd = connect_master_fd;
    get_init_msg();
  }
  void create_player(const char* hostname,const char* port);
  void init_client(int argc, char* argv[]);
  void get_init_msg();
  int as_server_socket();
  void receive_potato();
  int max_fds(int fd1, int fd2, int fd3);
  void over();
};

class info{
public:  
  char ip_addr[100];
  int port_neigh_left;
  int port_neigh_right;
  info(){memset(ip_addr, 0, sizeof(ip_addr));}
};

class potato{
private:
  int hops;
  int players;
  int path[512];
  int num;
public:
 potato():hops(0),players(0), num(0){
    memset(path, 0, sizeof(path));
 }
 potato(int hops, int players):hops(hops), players(players), num(0){
    memset(path, 0, sizeof(path));
  }
  int get_path(int i){
    return path[i];
  }
  void decrease_hops();
  void add_path(int ID);
  int get_current();
  int get_num_players();
  void handle_potato(potato* recv_potato, int player_id);
  int get_hops();
};
