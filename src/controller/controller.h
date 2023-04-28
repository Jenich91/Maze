#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "../model/caves.h"
#include "../model/maze.h"
#include "QtCore/qdir.h"
#include "QtCore/qpoint.h"

namespace sfleta {
class Controller {
 public:
  explicit Controller() : caves_{nullptr}, maze_{nullptr} {}
  ~Controller() {
    delete caves_;
    delete maze_;
  };

  void InitCaves(QString filepath) {
    caves_ = new Caves(filepath.toStdString());
  }
  void InitCaves(size_t rows, size_t cols, short chance) {
    caves_ = new Caves(rows, cols, chance);
  }

  const Caves::caves_t& GetBufferCaves() { return caves_->GetBuffer(); }
  bool MetamorphosisCaves(short birth_lim, short death_lim) {
    return caves_->Metamorphosis(birth_lim, death_lim);
  }

  size_t GetColsCaves() const { return caves_->GetCols(); }
  size_t GetRowsCaves() const { return caves_->GetRows(); }
  bool GetElemCaves(size_t rows, size_t cols) {
    return caves_->GetBuffer()[rows][cols];
  }

  bool IsEmptyCaves() { return caves_ == nullptr; }

  void InitMaze(QString filepath) { maze_ = new Maze(filepath.toStdString()); }
  void InitMaze(size_t rows, size_t cols) { maze_ = new Maze(rows, cols); }
  auto& GetBufferMaze() { return maze_->GetBuffer(); }
  auto FindPathMaze(QPointF from, QPointF to) {
    return maze_->FindShortestPath(std::make_pair(from.x(), from.y()),
                                   std::make_pair(to.x(), to.y()));
  }
  size_t GetColsMaze() const { return maze_->GetCols(); }
  size_t GetRowsMaze() const { return maze_->GetRows(); }
  bool IsEmptyMaze() { return maze_ == nullptr; }

 private:
  Caves* caves_;
  Maze* maze_;
};
}  // namespace sfleta
#endif  // CONTROLLER_H
