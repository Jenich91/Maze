#ifndef CAVES_H
#define CAVES_H

#include <cstddef>
#include <fstream>
#include <string>
#include <vector>

namespace sfleta {
class Caves {
 public:
  using caves_t = std::vector<std::vector<bool>>;

  explicit Caves(std::string);
  explicit Caves(size_t, size_t, short);
  ~Caves() { delete buffer_; }

  bool Metamorphosis(short, short);

  const caves_t& GetBuffer() { return *buffer_; }
  size_t GetCols() { return cols_; }
  size_t GetRows() { return rows_; }

 private:
  caves_t* buffer_;

  size_t rows_;
  size_t cols_;

  short CheckArea(short, short);
};
}  // namespace sfleta
#endif  // CAVES_H
