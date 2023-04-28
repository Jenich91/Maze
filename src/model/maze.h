#ifndef MAZE_H
#define MAZE_H

#include <cstddef>
#include <fstream>
#include <queue>
#include <random>
#include <vector>

namespace sfleta {
class Maze {
 public:
  struct Cell {
    bool up;
    bool left;
    bool right;
    bool down;
  };

  using vector_bool = std::vector<std::vector<bool>>;
  using vector_cell = std::vector<std::vector<Cell>>;

  explicit Maze(std::string);
  explicit Maze(size_t, size_t);
  ~Maze() {}

  vector_cell& GetBuffer() { return maze_m_; }
  size_t GetCols() { return cols_; }
  size_t GetRows() { return rows_; }

  void GenerateMaze();
  std::vector<std::pair<size_t, size_t>> FindShortestPath(
      std::pair<size_t, size_t>, std::pair<size_t, size_t>);

 private:
  size_t rows_;
  size_t cols_;

  vector_cell maze_m_;
  std::vector<int> line_;
  int counter_ = 1;

  void InitMaze();
  void AssignLeftAndUpWall();

  bool GetRandomBoolOrFalse();
  void FillEmptyValuesInLine();
  void AssignUniqueSetInLine();
  void MergeSet(size_t, int);
  int CountSet(int);

  void SetRightWall(size_t);
  void SetFloor(size_t);

  int CountFloors(size_t, int);
  void CheckFloors(size_t);

  void PrepareLineForNewRow(size_t);

  void CheckLastRow();
  void ProcessLastRow();
};
}  // namespace sfleta
#endif  // MAZE_H
