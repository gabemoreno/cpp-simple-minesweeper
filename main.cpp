#include <algorithm>
#include <array>
#include <iostream>
#include <limits>
#include <random>

namespace {

constexpr int kBoardSize = 5;
constexpr int kMineCount = 5;
constexpr int kSafeCellCount = kBoardSize * kBoardSize - kMineCount;

struct Cell {
  bool isMine = false;
  bool isRevealed = false;
  int adjacentMines = 0;
};

using Board = std::array<std::array<Cell, kBoardSize>, kBoardSize>;

bool isInBounds(int row, int column) {
  return row >= 0 && row < kBoardSize && column >= 0 &&
         column < kBoardSize;
}

void placeMines(Board& board, std::mt19937& randomEngine) {
  std::array<int, kBoardSize * kBoardSize> positions{};

  for (int index = 0; index < static_cast<int>(positions.size()); ++index) {
    positions[index] = index;
  }

  std::shuffle(positions.begin(), positions.end(), randomEngine);

  for (int index = 0; index < kMineCount; ++index) {
    int position = positions[index];
    int row = position / kBoardSize;
    int column = position % kBoardSize;
    board[row][column].isMine = true;
  }
}

void calculateAdjacentMines(Board& board) {
  for (int row = 0; row < kBoardSize; ++row) {
    for (int column = 0; column < kBoardSize; ++column) {
      if (board[row][column].isMine) {
        continue;
      }

      int mineCount = 0;

      for (int rowOffset = -1; rowOffset <= 1; ++rowOffset) {
        for (int columnOffset = -1; columnOffset <= 1; ++columnOffset) {
          int neighborRow = row + rowOffset;
          int neighborColumn = column + columnOffset;

          if (isInBounds(neighborRow, neighborColumn) &&
              board[neighborRow][neighborColumn].isMine) {
            ++mineCount;
          }
        }
      }

      board[row][column].adjacentMines = mineCount;
    }
  }
}

void displayBoard(const Board& board, bool showMines) {
  std::cout << "\n  ";

  for (int column = 1; column <= kBoardSize; ++column) {
    std::cout << column << ' ';
  }

  std::cout << '\n';

  for (int row = 0; row < kBoardSize; ++row) {
    std::cout << row + 1 << ' ';

    for (int column = 0; column < kBoardSize; ++column) {
      const Cell& cell = board[row][column];

      if (cell.isMine && showMines) {
        std::cout << "* ";
      } else if (cell.isRevealed) {
        std::cout << cell.adjacentMines << ' ';
      } else {
        std::cout << "# ";
      }
    }

    std::cout << '\n';
  }
}

bool readSelection(const Board& board, int& row, int& column) {
  while (true) {
    std::cout << "Enter row and column (1-5): ";

    if (!(std::cin >> row >> column)) {
      if (std::cin.eof()) {
        return false;
      }

      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      std::cout << "Please enter two whole numbers.\n";
      continue;
    }

    if (row < 1 || row > kBoardSize || column < 1 || column > kBoardSize) {
      std::cout << "Coordinates must be between 1 and 5.\n";
      continue;
    }

    --row;
    --column;

    if (board[row][column].isRevealed) {
      std::cout << "That cell is already revealed.\n";
      continue;
    }

    return true;
  }
}

bool hasRevealedAllSafeCells(int revealedSafeCells) {
  return revealedSafeCells == kSafeCellCount;
}

}  // namespace

int main() {
  Board board{};
  std::random_device randomDevice;
  std::mt19937 randomEngine(randomDevice());

  placeMines(board, randomEngine);
  calculateAdjacentMines(board);

  int revealedSafeCells = 0;

  while (true) {
    displayBoard(board, false);

    int row = 0;
    int column = 0;

    if (!readSelection(board, row, column)) {
      return 0;
    }

    Cell& selectedCell = board[row][column];

    if (selectedCell.isMine) {
      displayBoard(board, true);
      std::cout << "You hit a mine. Game over.\n";
      return 0;
    }

    selectedCell.isRevealed = true;
    ++revealedSafeCells;

    if (hasRevealedAllSafeCells(revealedSafeCells)) {
      displayBoard(board, false);
      std::cout << "You revealed every safe cell. You win!\n";
      return 0;
    }
  }
}
