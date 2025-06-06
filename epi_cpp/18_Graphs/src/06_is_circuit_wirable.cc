#include <algorithm>
#include <queue>
#include <stdexcept>
#include <vector>

#include "test_framework/generic_test.h"
#include "test_framework/serialization_traits.h"
#include "test_framework/timed_executor.h"

using std::all_of;
using std::queue;
using std::vector;

struct GraphVertex;
bool Bfs(GraphVertex* s);

struct GraphVertex {
  int d = -1;
  vector<GraphVertex*> edges;
};


// Bipartite (Two-color) problem
bool IsAnyPlacementFeasible(vector<GraphVertex>* graph) {
  return all_of(begin(*graph), end(*graph),
                [](GraphVertex& v) { return v.d != -1 || Bfs(&v); });
}

bool Bfs(GraphVertex* s) {
  s->d = 0;
  queue<GraphVertex*> q;
  q.emplace(s);

  while (!empty(q)) {
    for (GraphVertex*& t : q.front()->edges) {
      if (t->d == -1) {  // Unvisited vertex.
        t->d = q.front()->d + 1;
        q.emplace(t);
      } else if (t->d == q.front()->d) {  // Same distance implies the same color
        return false;
      }
    }
    q.pop();
  }
  return true;
}

struct Edge {
  int from;
  int to;
};

namespace test_framework {
template <>
struct SerializationTrait<Edge> : UserSerTrait<Edge, int, int> {};
}  // namespace test_framework

bool IsAnyPlacementFeasibleWrapper(TimedExecutor& executor, int k,
                                   const vector<Edge>& edges) {
  vector<GraphVertex> graph;
  if (k <= 0) {
    throw std::runtime_error("Invalid k value");
  }
  graph.reserve(k);

  for (int i = 0; i < k; i++) {
    graph.push_back(GraphVertex{});
  }

  for (auto& e : edges) {
    if (e.from < 0 || e.from >= k || e.to < 0 || e.to >= k) {
      throw std::runtime_error("Invalid vertex index");
    }
    graph[e.from].edges.push_back(&graph[e.to]);
  }

  return executor.Run([&] { return IsAnyPlacementFeasible(&graph); });
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"executor", "k", "edges"};
  return GenericTestMain(
      args, "is_circuit_wirable.cc", "is_circuit_wirable.tsv",
      &IsAnyPlacementFeasibleWrapper, DefaultComparator{}, param_names);
}
