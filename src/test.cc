#include "gtest/gtest.h"
#include "model/caves.h"
#include "model/maze.h"

bool IsEqual(sfleta::Caves *a, sfleta::Caves *b) {
  bool res = a->GetCols() == b->GetCols() && a->GetRows() == b->GetRows();
  auto caves_test = a->GetBuffer();
  auto caves_origin = b->GetBuffer();

  for (size_t i = 0; i < caves_origin.size(); ++i) {
    for (size_t j = 0; j < caves_origin[0].size(); ++j) {
      if (res) res = caves_test[i][j] == caves_origin[i][j];
    }
  }
  return res;
}

TEST(caves, generation_1) {
  size_t rows = 10;
  size_t cols = 10;
  sfleta::Caves c(rows, cols, rand() % 100);
  ASSERT_EQ(c.GetRows(), rows);
  ASSERT_EQ(c.GetCols(), cols);
}

TEST(caves, generation_2) {
  size_t rows = 2;
  size_t cols = 50;
  sfleta::Caves c(rows, cols, rand() % 100);
  ASSERT_EQ(c.GetRows(), rows);
  ASSERT_EQ(c.GetCols(), cols);
}

TEST(caves, generation_3) {
  size_t rows = 1;
  size_t cols = 1;
  sfleta::Caves c(rows, cols, rand() % 100);
  ASSERT_EQ(c.GetRows(), rows);
  ASSERT_EQ(c.GetCols(), cols);
}

TEST(caves, generation_4) {
  size_t rows = 50;
  size_t cols = 1;
  sfleta::Caves c(rows, cols, rand() % 100);
  ASSERT_EQ(c.GetRows(), rows);
  ASSERT_EQ(c.GetCols(), cols);
}

TEST(caves, generation_5) {
  size_t rows = 25;
  size_t cols = 17;
  sfleta::Caves c(rows, cols, rand() % 100);
  ASSERT_EQ(c.GetRows(), rows);
  ASSERT_EQ(c.GetCols(), cols);
}

TEST(caves, generation_6) {
  size_t rows = 50;
  size_t cols = 50;
  sfleta::Caves c(rows, cols, rand() % 100);
  ASSERT_EQ(c.GetRows(), rows);
  ASSERT_EQ(c.GetCols(), cols);
}

TEST(caves, open_file_1) {
  sfleta::Caves c("./example/example_cave_1.txt");
  ASSERT_EQ(c.GetRows(), 10);
  ASSERT_EQ(c.GetCols(), 10);
}

TEST(caves, open_file_2) {
  sfleta::Caves c("./example/example_cave_2.txt");
  ASSERT_EQ(c.GetRows(), 20);
  ASSERT_EQ(c.GetCols(), 20);
}

TEST(caves, open_file_3) {
  sfleta::Caves c("./example/example_cave_3.txt");
  ASSERT_EQ(c.GetRows(), 50);
  ASSERT_EQ(c.GetCols(), 50);
}

TEST(caves, metamorf) {
  sfleta::Caves test("./example/example_cave_1.txt");
  sfleta::Caves orig("./example/example_cave_1_result.txt");

  while (!test.Metamorphosis(4, 3)) {
  }
  ASSERT_TRUE(IsEqual(&test, &orig));
}

TEST(maze, generation_1) {
  sfleta::Maze m(6, 6);
  std::vector<std::pair<size_t, size_t>> result;
  for (size_t i = 0; i < m.GetRows(); i++) {
    for (size_t j = 0; j < m.GetCols(); j++) {
      result = m.FindShortestPath(std::make_pair(0, 0), std::make_pair(i, j));
      ASSERT_EQ(i, result.back().first);
      ASSERT_EQ(j, result.back().second);
    }
  }
}

TEST(maze, generation_2) {
  sfleta::Maze m(10, 10);
  std::vector<std::pair<size_t, size_t>> result;
  for (size_t i = 0; i < m.GetRows(); i++) {
    for (size_t j = 0; j < m.GetCols(); j++) {
      result = m.FindShortestPath(std::make_pair(0, 0), std::make_pair(i, j));
      ASSERT_EQ(i, result.back().first);
      ASSERT_EQ(j, result.back().second);
    }
  }
}

TEST(maze, open_1) {
  sfleta::Maze m("./example/maze2.txt");
  std::vector<std::pair<size_t, size_t>> result;
  for (size_t i = 0; i < m.GetRows(); i++) {
    for (size_t j = 0; j < m.GetCols(); j++) {
      result = m.FindShortestPath(std::make_pair(0, 0), std::make_pair(i, j));
      ASSERT_EQ(i, result.back().first);
      ASSERT_EQ(j, result.back().second);
    }
  }
}

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
