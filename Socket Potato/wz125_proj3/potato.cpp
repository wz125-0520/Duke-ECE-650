#include "potato.h"
int potato::get_num_players(){
  return players;
}
void potato::decrease_hops(){
  hops--;
}
void potato::add_path(int ID){
  path[num] = ID;
  num++;
}
int potato::get_current(){
  return num;
}
int potato::get_hops(){
  return hops;
}
