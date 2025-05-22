#include <istream>
#include <string>
#include <vector>
#include <unordered_map>
#include <stack>
#include <algorithm>

#include "test_framework/generic_test.h"
#include "test_framework/serialization_traits.h"
#include "test_framework/test_failure.h"
#include "test_framework/timed_executor.h"

using std::vector;

enum class Color { kWhite, kBlack };

struct Coordinate;
bool SearchMazeHelper(const Coordinate&, const Coordinate&,
                      vector<vector<Color>>*, vector<Coordinate>*);
bool SearchMazeBFSHelper(const Coordinate, const Coordinate&,
                      vector<vector<Color>>*, vector<Coordinate>*);
bool IsFeasible(const Coordinate&, const vector<vector<Color>>&);

struct Coordinate {
  bool operator==(const Coordinate& that) const {
    return x == that.x && y == that.y;
  }
  bool operator!=(const Coordinate& that) const {
    return !(x == that.x && y == that.y);
  }
  void print() {
    std::cout << x << " " << y << std::endl;
  }

  int x, y;
};

namespace std {
    template <>
    struct hash<Coordinate> {
        size_t operator()(const Coordinate& k) const {
            // A simple way to combine hashes of individual members.
            // You can use boost::hash_combine for more robust combining.
            // For simple int types, XORing them with some shifts can work.
            // The magic number 0x9e3779b9 is a common constant in hash combining.
            size_t h1 = std::hash<int>{}(k.x);
            size_t h2 = std::hash<int>{}(k.y);
            return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
        }
    };
}

vector<Coordinate> SearchMaze(vector<vector<Color>> maze, const Coordinate& s,
                              const Coordinate& e) {
  vector<Coordinate> path;
  //SearchMazeHelper(s, e, &maze, &path);
  bool result = SearchMazeBFSHelper(s, e, &maze, &path);
  // for(int i=0; i<path.size(); ++i) {
  //   path[i].print();
  // }
  return path;
}

void pathTo(const Coordinate src, const Coordinate& dst, vector<Coordinate>* path_ptr,
            std::unordered_map<Coordinate, Coordinate>* edgeTo_ptr){
  auto& path = *path_ptr;
  for(Coordinate x = dst; x != src; x = (*edgeTo_ptr)[x]) {
    path.push_back(x);
  }
  path.push_back(src);
  std::reverse(path.begin(), path.end());
}

bool SearchMazeBFSHelper(const Coordinate src, const Coordinate& dst,  // cur is not reference
                      vector<vector<Color>>* maze_ptr,
                      vector<Coordinate>* path_ptr) {
  auto& maze = *maze_ptr;  // Use a reference instead of a pointer
  auto& path = *path_ptr;

  std::unordered_map<Coordinate, Coordinate> edgeTo;
  std::vector<Coordinate> queue;  /// Use for the queue
  queue.push_back(src);
  maze[src.x][src.y] = Color::kBlack;

  if (src == dst) {
    return true;
  }
  while(queue.size()>0) {
    Coordinate cur = queue.back();
    queue.pop_back();
    for (const Coordinate& next_move : vector<Coordinate>{{cur.x, cur.y + 1},
                                                          {cur.x, cur.y - 1},
                                                          {cur.x + 1, cur.y},
                                                          {cur.x - 1, cur.y}}) {
      if (IsFeasible(next_move, maze)) {
        maze[next_move.x][next_move.y] = Color::kBlack;  // Mark it as a visisted node
        queue.push_back(next_move);
        edgeTo[next_move] = cur;
        if (next_move == dst) {  // Found
          // Make pathTo from cur to e
          pathTo(src, dst, &path, &edgeTo);
          return true;
        }
      }
    }
  }
  return false;
}

// Check cur is within maze and is a white pixel
bool IsFeasible(const Coordinate& cur, const vector<vector<Color>>& maze) {
  return cur.x >= 0 && cur.x < maze.size() && cur.y >= 0 && cur.y < maze[cur.x].size() &&
         maze[cur.x][cur.y] == Color::kWhite;
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
}

// namespace test_framework
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
  // return false for wrong movement like (+1, +1), this is a result
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
