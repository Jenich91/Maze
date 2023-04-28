#include "maze.h"

namespace sfleta {
Maze::Maze(std::string file_path) {
  try {
    std::string line;
    std::ifstream file{file_path};
    rows_ = 0;
    cols_ = 0;

    getline(file, line);
    sscanf(line.c_str(), "%zu %zu", &rows_, &cols_);

    if (rows_ < 1 || cols_ < 1) {
      throw std::invalid_argument("Invalid value of rows or cols");
    }

    vector_bool wall_m_;
    vector_bool floor_m_;

    for (size_t i = 0; i < rows_; i++) {
      getline(file, line);
      std::vector<bool> tmp;
      for (auto it = line.begin(); it < line.end(); ++it) {
        if (*it == '1' || *it == '0') {
          tmp.push_back(*it == '1');
        }
      }
      if (tmp.size() != cols_) {
        throw std::length_error("Open wrong file!");
      }
      wall_m_.push_back(tmp);
    }
    if (wall_m_.size() != rows_) {
      throw std::length_error("Open wrong file!");
    }

    getline(file, line);

    for (size_t i = 0; i < rows_; i++) {
      getline(file, line);
      std::vector<bool> tmp;
      for (auto it = line.begin(); it < line.end(); ++it) {
        if (*it == '1' || *it == '0') {
          tmp.push_back(*it == '1');
        }
      }
      if (tmp.size() != cols_) {
        throw std::length_error("Open wrong file!");
      }
      floor_m_.push_back(tmp);
    }
    if (floor_m_.size() != rows_) {
      throw std::length_error("Open wrong file!");
    }

    file.close();
    InitMaze();

    for (size_t i = 0; i < rows_; i++) {
      for (size_t j = 0; j < cols_; j++) {
        if (wall_m_.data()[i][j] == true) maze_m_[i][j].right = true;
        if (floor_m_.data()[i][j] == true) maze_m_[i][j].down = true;
      }
    }
    AssignLeftAndUpWall();

  } catch (std::exception& e) {
    rows_ = 0;
    cols_ = 0;
    return;
  }
}

Maze::Maze(size_t rows, size_t cols) : rows_{rows}, cols_{cols} {
  GenerateMaze();
}

void Maze::InitMaze() {
  maze_m_.resize(rows_);
  for (size_t i = 0; i < rows_; i++) {
    maze_m_[i].resize(cols_);
  }
}

std::vector<std::pair<size_t, size_t>> Maze::FindShortestPath(
    std::pair<size_t, size_t> start_point,
    std::pair<size_t, size_t> finish_point) {
  using node = std::vector<std::pair<size_t, size_t>>;

  size_t M = maze_m_.size();
  size_t N = maze_m_[0].size();

  vector_bool visited;
  visited.resize(M, std::vector<bool>(N));

  std::queue<node> possible_ways;

  visited[start_point.first][start_point.second] = true;

  possible_ways.push({start_point});

  while (!possible_ways.empty()) {
    node now_position = possible_ways.front();

    size_t x = now_position.back().first;
    size_t y = now_position.back().second;

    if (now_position.back() == finish_point) {
      break;
    }

    possible_ways.pop();

    auto PushNewPoint = [&](int x, int y) {
      if (!visited[x][y]) {
        node dir{now_position};
        dir.push_back(std::make_pair(x, y));
        visited[x][y] = true;
        possible_ways.push({dir});
      }
    };

    if (maze_m_[x][y].up == false) {
      PushNewPoint(x - 1, y);
    }
    if (maze_m_[x][y].left == false) {
      PushNewPoint(x, y - 1);
    }
    if (maze_m_[x][y].right == false) {
      PushNewPoint(x, y + 1);
    }
    if (maze_m_[x][y].down == false) {
      PushNewPoint(x + 1, y);
    }
  }
  if (possible_ways.empty()) possible_ways.push({start_point});

  return possible_ways.front();
}

void Maze::AssignLeftAndUpWall() {
  for (size_t i = 0; i < rows_; i++) {
    for (size_t j = 0; j < cols_; j++) {
      if (i == 0) {
        maze_m_[i][j].up = true;
      } else {
        if (maze_m_[i - 1][j].down == true) maze_m_[i][j].up = true;
      }
    }
  }

  for (size_t i = 0; i < rows_; i++) {
    for (size_t j = 0; j < cols_; j++) {
      if (j == 0) {
        maze_m_[i][j].left = true;
      } else {
        if (maze_m_[i][j - 1].right == true) maze_m_[i][j].left = true;
      }
    }
  }
}

bool Maze::GetRandomBoolOrFalse() {
  std::mt19937 rng(std::random_device{}());
  return std::uniform_int_distribution<>{0, 1}(rng);
}

void Maze::FillEmptyValuesInLine() {
  for (size_t i = 0; i < cols_; i++) {
    line_.push_back(0);
  }
}

void Maze::AssignUniqueSetInLine() {
  for (size_t col_num = 0; col_num < cols_; col_num++) {
    if (line_[col_num] == 0) {
      line_[col_num] = counter_;
      counter_++;
    }
  }
}

void Maze::SetRightWall(size_t row_num) {
  for (size_t col_num = 0; col_num < cols_ - 1; col_num++) {
    bool wall_yes = GetRandomBoolOrFalse();
    if (wall_yes == true || line_[col_num] == line_[col_num + 1]) {
      maze_m_[row_num][col_num].right = true;
    } else {
      MergeSet(col_num, line_[col_num]);
    }
  }
  maze_m_[row_num][cols_ - 1].right = true;
}

void Maze::MergeSet(size_t col_num, int current_set_num) {
  int next_set_num = line_[col_num + 1];
  for (size_t j = 0; j < cols_; j++) {
    if (line_[j] == next_set_num) {
      line_[j] = current_set_num;
    }
  }
}

void Maze::SetFloor(size_t row_num) {
  for (size_t col_num = 0; col_num < cols_; col_num++) {
    bool floor_yes = GetRandomBoolOrFalse();
    if (floor_yes == true && CountSet(line_[col_num]) != 1) {
      maze_m_[row_num][col_num].down = true;
    }
  }
}

int Maze::CountSet(int current_set_num) {
  int count_set = 0;
  for (size_t col_num = 0; col_num < cols_; col_num++) {
    if (line_[col_num] == current_set_num) {
      count_set++;
    }
  }
  return count_set;
}

void Maze::CheckFloors(size_t row_num) {
  for (size_t col_num = 0; col_num < cols_; col_num++) {
    if (CountFloors(row_num, line_[col_num]) == 0) {
      maze_m_[row_num][col_num].down = false;
    }
  }
}

int Maze::CountFloors(size_t row_num, int current_set_num) {
  int count_floor = 0;
  for (size_t col_num = 0; col_num < cols_; col_num++) {
    if (line_[col_num] == current_set_num &&
        maze_m_[row_num][col_num].down == false) {
      count_floor++;
    }
  }
  return count_floor;
}

void Maze::PrepareLineForNewRow(size_t row_num) {
  for (size_t col_num = 0; col_num < cols_; col_num++) {
    if (maze_m_[row_num][col_num].down == true) {
      line_[col_num] = 0;
    }
  }
}

void Maze::ProcessLastRow() {
  AssignUniqueSetInLine();
  SetRightWall(rows_ - 1);
  CheckLastRow();
}

void Maze::CheckLastRow() {
  for (size_t col_num = 0; col_num < cols_ - 1; col_num++) {
    if (line_[col_num] != line_[col_num + 1]) {
      maze_m_[rows_ - 1][col_num].right = false;
      MergeSet(col_num, line_[col_num]);
    }
    maze_m_[rows_ - 1][col_num].down = true;
  }
  maze_m_[rows_ - 1][cols_ - 1].down = true;
}

void Maze::GenerateMaze() {
  InitMaze();
  FillEmptyValuesInLine();

  for (size_t row_num = 0; row_num < rows_ - 1; row_num++) {
    AssignUniqueSetInLine();
    SetRightWall(row_num);
    SetFloor(row_num);
    CheckFloors(row_num);
    PrepareLineForNewRow(row_num);
  }
  ProcessLastRow();
  counter_ = 1;
  line_.clear();
  AssignLeftAndUpWall();
}
}  // namespace sfleta
