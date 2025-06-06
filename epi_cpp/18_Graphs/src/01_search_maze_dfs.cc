#include <istream>
#include <string>
#include <vector>

#include "test_framework/generic_test.h"
#include "test_framework/serialization_traits.h"
#include "test_framework/test_failure.h"
#include "test_framework/timed_executor.h"

using std::vector;

enum class Color { kWhite, kBlack };

struct Coordinate;
bool SearchMazeHelper(const Coordinate&, const Coordinate&,
                      vector<vector<Color>>*, vector<Coordinate>*);
bool IsFeasible(const Coordinate&, const vector<vector<Color>>&);

struct Coordinate {
  bool operator==(const Coordinate& that) const {
    return x == that.x && y == that.y;
  }
  void print() {
    std::cout << x << " " << y << std::endl;
  }

  int x, y;
};

// Check cur is within maze and is a white pixel
bool IsFeasible(const Coordinate& cur, const vector<vector<Color>>& maze) {
  return cur.x >= 0 && cur.x < maze.size() && cur.y >= 0 && cur.y < maze[cur.x].size() &&
         maze[cur.x][cur.y] == Color::kWhite;
}

vector<Coordinate> SearchMaze(vector<vector<Color>> maze, const Coordinate& s,
                              const Coordinate& e) {
  vector<Coordinate> path;
  SearchMazeHelper(s, e, &maze, &path);
  // for(int i=0; i<path.size(); ++i) {
  //   path[i].print();
  // }
  return path;
}

// Time O(|V| + |E|)
// Perform DFS to find a feasible path.
bool SearchMazeHelper(const Coordinate& cur, const Coordinate& e,
                      vector<vector<Color>>* maze_ptr,
                      vector<Coordinate>* path_ptr) {
  auto& maze = *maze_ptr;
  // Checks cur is within maze and is a white pixel.
  if (!IsFeasible(cur, maze)) {
    return false;
  }
  auto& path = *path_ptr;
  path.emplace_back(cur);
  maze[cur.x][cur.y] = Color::kBlack;
  if (cur == e) {
    return true;
  }

  for (const Coordinate& next_move : vector<Coordinate>{{cur.x, cur.y + 1},
                                                        {cur.x, cur.y - 1},
                                                        {cur.x + 1, cur.y},
                                                        {cur.x - 1, cur.y}}) {
    if (SearchMazeHelper(next_move, e, maze_ptr, path_ptr)) {
      return true;
    }

  }
  path.pop_back();  // Cannot find a path, remove the entry added in path.emplace_back(cur).

  return false;
}

namespace test_framework {
template <>
struct SerializationTrait<Color> : SerializationTrait<int> {
  using serialization_type = Color;

  static serialization_type Parse(const json& json_object) {
    return static_cast<serialization_type>(
        SerializationTrait<int>::Parse(json_object));
  }
};
}  // namespace test_framework

namespace test_framework {
template <>
struct SerializationTrait<Coordinate> : UserSerTrait<Coordinate, int, int> {
  static std::vector<std::string> GetMetricNames(const std::string& arg_name) {
    return {};
  }

  static std::vector<int> GetMetrics(const Coordinate& x) { return {}; }
};
}  // namespace test_framework

bool PathElementIsFeasible(const vector<vector<Color>>& maze,
                           const Coordinate& prev, const Coordinate& cur) {
  if (!(0 <= cur.x && cur.x < maze.size() && 0 <= cur.y &&
        cur.y < maze[cur.x].size() && maze[cur.x][cur.y] == Color::kWhite)) {
    return false;
  }
  return cur == Coordinate{prev.x + 1, prev.y} ||
         cur == Coordinate{prev.x - 1, prev.y} ||
         cur == Coordinate{prev.x, prev.y + 1} ||
         cur == Coordinate{prev.x, prev.y - 1};
}

bool SearchMazeWrapper(TimedExecutor& executor,
                       const vector<vector<Color>>& maze, const Coordinate& s,
                       const Coordinate& e) {
  vector<vector<Color>> copy = maze;

  auto path = executor.Run([&] { return SearchMaze(copy, s, e); });

  if (path.empty()) {
    return s == e;
  }

  if (!(path.front() == s) || !(path.back() == e)) {
    throw TestFailure("Path doesn't lay between start and end points");
  }

  for (size_t i = 1; i < path.size(); i++) {
    if (!PathElementIsFeasible(maze, path[i - 1], path[i])) {
      throw TestFailure("Path contains invalid segments");
    }
  }

  return true;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"executor", "maze", "s", "e"};
  return GenericTestMain(args, "search_maze.cc", "search_maze.tsv",
                         &SearchMazeWrapper, DefaultComparator{}, param_names);
}