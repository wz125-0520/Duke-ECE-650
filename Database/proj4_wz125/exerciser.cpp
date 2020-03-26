#include "exerciser.h"

void exercise(connection * C) {
  std::cout << "TEST QUERY1:\n\n";
  query1(C, 1, 15, 20, 0, 0, 0, 0, 10, 20, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  std::cout << "\n";
  query1(C, 0, 10, 40, 0, 0, 0, 2, 10, 20, 1, 0, 5, 0, 0, 0, 0, 0, 0);

  std::cout << "TEST QUERY2:\n\n";
  query2(C, "LightBlue");
  std::cout << "\n";
  std::cout << "[TEST] Non-exist\n";
  query2(C, "Pink");
  std::cout << "TEST QUERY3:\n\n";
  query3(C, "Duke");
  std::cout << "\n";
  query3(C, "VirginiaTech");
  std::cout << "\n";
  std::cout << "[TEST] Non-exist\n";
  query3(C, "Chinese");
  std::cout << "TEST QUERY4:\n\n";
  query4(C, "NC", "Red");
  std::cout << "\n";
  query4(C, "NC", "DarkBlue");
  std::cout << "\n";
  std::cout << "[TEST] Non-exist\n";
  query4(C, "BB", "Orange");
  std::cout << "TEST QUERY5:\n\n";
  query5(C, 100);
  std::cout << "\n";
  query5(C, 11);
  add_player(C, 1, 1, "Jerome", "Robinson", 34, 19, 4, 3, 1.7, 0.4);
  add_player(C, 2, 1, "Jer", "J'Robinson", 10, 20, 4, 3, 1.7, 0.4);
  add_team(C, "A'AA", 3, 2, 3, 5);
  add_state(C, "S'tateAAA");
  add_color(C, "C'olor");
}
