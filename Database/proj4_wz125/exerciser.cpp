#include "exerciser.h"

void exercise(connection * C) {
  std::cout << "TEST QUERY1:\n\n";
  query1(C, 1, 15, 20, 0, 0, 0, 0, 10, 20, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  std::cout << "\n";
  query1(C, 1, 15, 20, 0, 0, 0, 1, 5, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  std::cout << "\n";
  query1(C, 0, 10, 40, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0);
  std::cout << "\n";
  query1(C, 0, 10, 40, 0, 0, 0, 2, 10, 20, 1, 0, 5, 0, 0, 0, 0, 0, 0);

  std::cout << "TEST QUERY2:\n\n";
  query2(C, "LightBlue");
  std::cout << "\n";
  query2(C, "Pink");
  std::cout << "TEST QUERY3:\n\n";
  query3(C, "Duke");
  std::cout << "\n";
  query3(C, "VirginiaTech");
  std::cout << "\n";
  query3(C, "Chinese");
  std::cout << "TEST QUERY4:\n\n";
  query4(C, "NC", "Red");
  std::cout << "\n";
  query4(C, "NC", "DarkBlue");
  std::cout << "\n";
  query4(C, "BB", "Orange");
  std::cout << "TEST QUERY5:\n\n";
  query5(C, 100);
  std::cout << "\n";
  query5(C, 11);
}
