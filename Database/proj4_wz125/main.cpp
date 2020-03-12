#include <fstream>
#include <iostream>
#include <pqxx/pqxx>

#include "exerciser.h"
#include "query_funcs.h"

using namespace std;
using namespace pqxx;

void buildTable(std::string filename, connection * C) {
  std::ifstream table;
  table.open(filename.c_str(), std::ifstream::in);
  std::string line;
  std::string create = "";
  while (!table.eof()) {
    getline(table, line);
    create += line;
  }
  work W(*C);
  W.exec(create);
  W.commit();
  table.close();
}

void buildplayer(std::string filename, connection * C) {
  int player_id, team_id, jersey_num, mpg, ppg, rpg, apg;
  std::string first_name, last_name;
  double spg, bpg;
  std::ifstream entries;
  entries.open(filename.c_str(), std::ifstream::in);
  std::string line;
  std::string value;
  while (getline(entries, line)) {
    stringstream ss(line);
    ss >> player_id >> team_id >> jersey_num >> first_name >> last_name >> mpg >> ppg >>
        rpg >> apg >> spg >> bpg;
    add_player(
        C, team_id, jersey_num, first_name, last_name, mpg, ppg, rpg, apg, spg, bpg);
  }
}

void buildteam(std::string filename, connection * C) {
  std::string name;
  int team_id, color_id, wins, losses, state_id;
  std::ifstream entries;
  entries.open(filename.c_str(), std::ifstream::in);
  std::string line;
  while (std::getline(entries, line)) {
    std::stringstream ss(line);
    ss >> team_id >> name >> state_id >> color_id >> wins >> losses;
    add_team(C, name, state_id, color_id, wins, losses);
  }
}
void buildstate(std::string filename, connection * C) {
  std::string name;
  int state_id;
  std::ifstream entries;
  entries.open(filename.c_str(), std::ifstream::in);
  std::string line;
  while (std::getline(entries, line)) {
    std::stringstream ss(line);
    ss >> state_id >> name;
    add_state(C, name);
  }
}
void buildcolor(std::string filename, connection * C) {
  std::string name;
  int color_id;
  std::ifstream entries;
  entries.open(filename.c_str(), std::ifstream::in);
  std::string line;
  while (std::getline(entries, line)) {
    std::stringstream ss(line);
    ss >> color_id >> name;
    add_color(C, name);
  }
}
int main(int argc, char * argv[]) {
  //Allocate & initialize a Postgres connection object
  connection * C;

  try {
    //Establish a connection to the database
    //Parameters: database name, user name, user password
    C = new connection("dbname=ACC_BBALL user=postgres password=passw0rd");
    if (C->is_open()) {
      cout << "Opened database successfully: " << C->dbname() << endl;
    }
    else {
      cout << "Can't open database" << endl;
      return 1;
    }
  }
  catch (const std::exception & e) {
    cerr << e.what() << std::endl;
    return 1;
  }

  //TODO: create PLAYER, TEAM, STATE, and COLOR tables in the ACC_BBALL database
  //      load each table with rows from the provided source txt files

  buildTable("create_table.txt", C);
  buildcolor("color.txt", C);
  buildstate("state.txt", C);
  buildteam("team.txt", C);
  buildplayer("player.txt", C);

  exercise(C);

  //Close database connection
  C->disconnect();

  return 0;
}
