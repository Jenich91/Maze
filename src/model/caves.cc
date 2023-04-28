#include "caves.h"

namespace sfleta {
Caves::Caves(std::string path) : buffer_{new caves_t}, rows_{0}, cols_{0} {
  std::string line;
  std::ifstream file{path};

  getline(file, line);
  sscanf(line.c_str(), "%zu %zu", &rows_, &cols_);

  while (getline(file, line)) {
    std::vector<bool> tmp;

    for (auto it = line.begin(); it < line.end(); ++it) {
      if (*it == '1' || *it == '0') {
        tmp.push_back(*it == '1');
      }
    }

    if (tmp.size() != cols_) {
      rows_ = 0;
      cols_ = 0;
      return;
    }
    buffer_->push_back(tmp);
  }
  if (buffer_->size() != rows_) {
    rows_ = 0;
    cols_ = 0;
    return;
  }
  file.close();
}

Caves::Caves(size_t rows, size_t cols, short chance)
    : buffer_{new caves_t}, rows_{rows}, cols_{cols} {
  for (size_t i = 0; i < rows; ++i) {
    std::vector<bool> tmp;
    for (size_t j = 0; j < cols; ++j) {
      tmp.push_back((rand() % 100) < chance);
    }
    buffer_->push_back(tmp);
  }
}

bool Caves::Metamorphosis(short birth_limit, short death_limit) {
  caves_t tmp_buffer_(rows_, std::vector<bool>(cols_));
  bool res = true;

  for (size_t i = 0; i < rows_; ++i) {
    for (size_t j = 0; j < cols_; ++j) {
      if (buffer_->data()[i][j])
        tmp_buffer_.data()[i][j] = !(CheckArea(i, j) < death_limit);
      else
        tmp_buffer_.data()[i][j] = CheckArea(i, j) > birth_limit;

      if (res) res = (buffer_->data()[i][j] == tmp_buffer_.data()[i][j]);
    }
  }
  *buffer_ = tmp_buffer_;
  return res;
}

short Caves::CheckArea(short i, short j) {
  short count = 8;

  for (int k = i - 1; k < i + 2; ++k) {
    for (int n = j - 1; n < j + 2; ++n) {
      if (k < 0 || k == (int)rows_ || n < 0 || n == (int)cols_ ||
          (k == i && n == j))
        continue;
      if (!buffer_->data()[k][n]) --count;
    }
  }
  return count;
}
}  // namespace sfleta
