#include <algorithm>
#include <iostream>
#include <vector>

void print_grid(const std::vector<std::vector<int>>& board) {
  std::cout << " ---------------------------" << std::endl;
  for (int i = 0; i < 9; ++i) {
    for (int j = 0; j < 9; ++j) {
      if (j == 1 || j == 2 || j == 4 || j == 5 || j == 7 || j == 8) {
        std::cout << ' ';
      }
      else {
        std::cout << '|';
      }
      if (board.at(i).at(j) == -1) {
        std::cout << "  ";
      }
      else {
        std::cout << ' ' << board.at(i).at(j);
      }
      if (j != 0 && j % 8 == 0) {
        std::cout << '|';
      }
    }
    std::cout << std::endl;
    if (i == 2 || i == 5 || i == 8) {
      std::cout << " ---------------------------" << std::endl;
    }
    else {
      std::cout << "  -  -  -  -  -  -  -  -  -  " << std::endl;
    }
  }
}

  int main() {
    std::vector<int> temp(9, 0);
    std::vector<std::vector<int>> board(9, temp);
    for (int i = 0; i < 9; ++i) {
      for (int j = 0; j < 9; ++j) {
        board.at(i).at(j) = i % 3 * 3 + 1 + j % 3;
      }
    }
    print_grid(board);
    system("pause");
  }
