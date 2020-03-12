#include "query_funcs.h"

#include <iomanip>

void add_player(connection * C,
                int team_id,
                int jersey_num,
                string first_name,
                string last_name,
                int mpg,
                int ppg,
                int rpg,
                int apg,
                double spg,
                double bpg) {
  std::string row =
      "INSERT INTO "
      "PLAYER(TEAM_ID,UNIFORM_NUM,FIRST_NAME,LAST_NAME,MPG,PPG,RPG,APG,SPG,BPG) VALUES(";
  std::stringstream ss;
  work W(*C);

  ss << team_id << "," << jersey_num << "," << W.quote(first_name) << ","
     << W.quote(last_name) + "," << mpg << "," << ppg << "," << rpg << "," << apg << ","
     << spg << "," << bpg;
  row = row + ss.str() + ");";
  W.exec(row);
  W.commit();
}

void add_team(connection * C,
              string name,
              int state_id,
              int color_id,
              int wins,
              int losses) {
  work W(*C);
  std::string row = "INSERT INTO TEAM(NAME,STATE_ID,COLOR_ID,WINS,LOSSES) VALUES(" +
                    W.quote(name) + ",";
  std::stringstream ss;
  ss << state_id << "," << color_id << "," << wins << "," << losses << ");";
  row += ss.str();
  W.exec(row);
  W.commit();
}

void add_state(connection * C, string name) {
  work W(*C);
  std::string row = "INSERT INTO STATE(NAME) VALUES(" + W.quote(name) + ");";
  W.exec(row);
  W.commit();
}

void add_color(connection * C, string name) {
  work W(*C);
  std::string row = "INSERT INTO COLOR(NAME) VALUES(" + W.quote(name) + ");";
  W.exec(row);
  W.commit();
}

void print_query1(connection * C, std::string sql) {
  std::cout
      << "PLAYER_ID TEAM_ID UNIFORM_NUM FIRST_NAME LAST_NAME MPG PPG RPG APG SPG BPG\n";

  nontransaction N(*C);
  result R(N.exec(sql));
  for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
    std::cout << c[0].as<int>() << " ";
    std::cout << c[1].as<int>() << " ";
    std::cout << c[2].as<int>() << " ";
    std::cout << c[3].as<std::string>() << " ";
    std::cout << c[4].as<std::string>() << " ";
    std::cout << c[5].as<int>() << " ";
    std::cout << c[6].as<int>() << " ";
    std::cout << c[7].as<int>() << " ";
    std::cout << c[8].as<int>() << " ";
    std::cout << std::setprecision(1) << c[9].as<float>() << " ";
    std::cout << std::setprecision(1) << c[10].as<float>() << std::endl;
  }
}
void query1(connection * C,
            int use_mpg,
            int min_mpg,
            int max_mpg,
            int use_ppg,
            int min_ppg,
            int max_ppg,
            int use_rpg,
            int min_rpg,
            int max_rpg,
            int use_apg,
            int min_apg,
            int max_apg,
            int use_spg,
            double min_spg,
            double max_spg,
            int use_bpg,
            double min_bpg,
            double max_bpg) {
  std::stringstream command;
  command << "SELECT * FROM PLAYER";
  bool start = false;

  if (use_mpg) {
    if (!start) {
      command << " WHERE ";
      start = true;
    }
    command << "(MPG BETWEEN " << min_mpg << " AND " << max_mpg << ")";
  }
  if (use_ppg) {
    if (!start) {
      command << " WHERE ";
      start = true;
    }
    else {
      command << " AND ";
    }
    command << "(PPG BETWEEN " << min_ppg << " AND " << max_ppg << ")";
  }
  if (use_rpg) {
    if (!start) {
      command << " WHERE ";
      start = true;
    }
    else {
      command << "AND ";
    }
    command << "(RPG BETWEEN " << min_rpg << " AND " << max_rpg << ")";
  }
  if (use_apg) {
    if (!start) {
      command << " WHERE ";
      start = true;
    }
    else {
      command << " AND ";
    }
    command << "(APG BETWEEN " << min_apg << " AND " << max_apg << ")";
  }
  if (use_spg) {
    if (!start) {
      command << " WHERE ";
      start = true;
    }
    else {
      command << " AND ";
    }
    command << "(SPG BETWEEN " << min_spg << " AND " << max_spg << ")";
  }
  if (use_bpg) {
    if (!start) {
      command << " WHERE ";
      start = true;
    }
    else {
      command << " AND ";
    }
    command << "(BPG BETWEEN " << min_bpg << " AND " << max_bpg << ")";
  }
  command << ";";
  print_query1(C, command.str());
}

void query2(connection * C, string team_color) {
  std::stringstream command;
  command << "SELECT TEAM.NAME FROM TEAM,COLOR WHERE COLOR.COLOR_ID="
             "TEAM.COLOR_ID AND COLOR.NAME=";
  command << work(*C).quote(team_color);
  std::cout << "NAME\n";
  nontransaction N(*C);
  result R(N.exec(command.str()));
  for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
    std::cout << c[0].as<std::string>() << std::endl;
  }
}

void query3(connection * C, string team_name) {
  std::stringstream command;
  command << "SELECT FIRST_NAME,LAST_NAME FROM PLAYER, TEAM WHERE "
             "TEAM.TEAM_ID=PLAYER.TEAM_ID AND TEAM.NAME=";
  work W(*C);
  command << W.quote(team_name) << " ORDER BY PPG DESC;";
  std::cout << "FIRST_NAME LAST_NAME\n";
  W.commit();
  nontransaction N(*C);
  result R(N.exec(command.str()));
  for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
    std::cout << c[0].as<std::string>() << " ";
    std::cout << c[1].as<std::string>() << std::endl;
  }
}

void query4(connection * C, string team_state, string team_color) {
  std::stringstream command;
  command << "SELECT FIRST_NAME,LAST_NAME,UNIFORM_NUM FROM PLAYER,STATE,COLOR,TEAM WHERE "
             "STATE.STATE_ID=TEAM.STATE_ID AND COLOR.COLOR_ID=TEAM.COLOR_ID AND "
             "PLAYER.TEAM_ID=TEAM.TEAM_ID AND ";
  work W(*C);
  command << "STATE.NAME=" << W.quote(team_state) << " AND ";
  command << "COLOR.NAME=" << W.quote(team_color) << ";";
  W.commit();
  std::cout << "FIRST_NAME LAST_NAME UNIFORM_NUM\n";
  nontransaction N(*C);
  result R(N.exec(command.str()));
  for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
    std::cout << c[0].as<std::string>() << " ";
    std::cout << c[1].as<std::string>() << " ";
    std::cout << c[2].as<int>() << std::endl;
  }
}

void query5(connection * C, int num_wins) {
  std::stringstream command;
  command << "SELECT FIRST_NAME,LAST_NAME,TEAM.NAME,WINS FROM PLAYER,TEAM "
             "WHERE TEAM.TEAM_ID=PLAYER.TEAM_ID AND TEAM.WINS>";
  command << num_wins << ";";
  std::cout << "FIRST_NAME LAST_NAME NAME WINS\n";
  nontransaction N(*C);
  result R(N.exec(command.str()));
  for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
    std::cout << c[0].as<std::string>() << " ";
    std::cout << c[1].as<std::string>() << " ";
    std::cout << c[2].as<std::string>() << " ";
    std::cout << c[3].as<int>() << std::endl;
  }
}
