# Graphs

*Study notes for Elements of Programming Interviews in C++ (sampler), Chapter 15. See epilight_cpp_new.pdf for the full text.*

## Overview

A graph is the universal data structure for pairwise relationships. This chapter's introduction is mostly vocabulary; getting these definitions crisp pays off because interview graph problems are usually a standard algorithm hiding behind a modeling step.

**Directed graphs.** A directed graph is a pair (V, E) where V is a set of vertices and E ⊆ V x V is a set of ordered edges; for an edge (u, v), u is the *source* and v the *sink*. Graphs are frequently annotated — edge lengths/weights, vertex weights, a designated start vertex, and so on.

**Paths and reachability.** A path from u to v is a sequence of vertices beginning at u and ending at v in which every consecutive pair is an edge; a single vertex counts as a (length-0) path. The *length* of a path is its number of edges. If some path leads from u to v, then v is *reachable* from u. Many problems that never mention graphs are, at heart, reachability questions.

**DAGs and topological order.** A cycle is a nonempty path that starts and ends at the same vertex. A directed graph with no cycles is a *directed acyclic graph* (DAG). In a DAG, vertices with no incoming edges are called *sources* and vertices with no outgoing edges are called *sinks*. A *topological ordering* lines up the vertices so that every edge points from an earlier vertex to a later one; every DAG has at least one, which is why DAGs model dependency/scheduling problems (build systems, course prerequisites).

**Undirected graphs and connectivity.** An undirected graph has unordered edge pairs — drawn without arrowheads. Vertices u and v are *connected* if a path joins them; a graph is connected if every vertex pair is. A *connected component* is a maximal set of mutually connected vertices, and each vertex lies in exactly one component. Removing edges can split one component into several. For directed graphs there are three grades of connectivity: *weakly connected* (connected after forgetting edge directions), *connected* (for each pair, a directed path exists in at least one direction), and *strongly connected* (directed paths exist in both directions for every pair).

**Trees.** A *free tree* is a connected, acyclic undirected graph — equivalently, a graph in which every vertex pair is joined by exactly one path. A *rooted tree* designates a root, inducing parent/child relations; an *ordered tree* additionally orders each vertex's children. Binary trees differ from ordered trees in that a lone child still has an identity (left vs. right) — binary trees carry position as well as order. Given a graph G = (V, E), any tree (V, E') with E' ⊆ E is a *spanning tree* of G.

**Representations.** Two standard encodings:
- *Adjacency list* — each vertex stores the list of vertices it points to. Space O(|V| + |E|); the right default, especially for sparse graphs.
- *Adjacency matrix* — a |V| x |V| boolean matrix with entry 1 when the edge exists. O(1) edge queries, O(|V|^2) space.

Complexities of graph algorithms are quoted in terms of |V| and |E|.

**When to reach for graphs.** Anything spatially networked (roads between cities) is obviously a graph, but so is any binary relation: hyperlinks between pages, follows in a social network, wins between teams. The productive habit is to translate the relation into vertices and edges and then recognize a textbook problem.

## Boot camp

Given a list of match results (each a winner/loser pair of teams), decide whether a "beat-chain" leads from team A to team B — a sequence of teams starting at A and ending at B where each team beat the next.

Model it as a directed graph: one vertex per team, an edge from each winner to the loser it defeated. The question is exactly "is B reachable from A?", answerable by either DFS or BFS. A DFS version:

```cpp
struct MatchResult {
  std::string winner, loser;
};

using Graph = std::unordered_map<std::string, std::unordered_set<std::string>>;

Graph BuildBeatGraph(const std::vector<MatchResult>& results) {
  Graph g;
  for (const auto& r : results) g[r.winner].insert(r.loser);
  return g;
}

bool Reachable(const Graph& g, const std::string& from, const std::string& to,
               std::unordered_set<std::string>& visited) {
  if (from == to) return true;
  if (!visited.insert(from).second) return false;   // already explored
  auto it = g.find(from);
  if (it == g.end()) return false;                  // no outgoing edges
  for (const auto& next : it->second)
    if (Reachable(g, next, to, visited)) return true;
  return false;
}

bool CanABeatB(const std::vector<MatchResult>& results,
               const std::string& a, const std::string& b) {
  Graph g = BuildBeatGraph(results);
  std::unordered_set<std::string> visited;
  return Reachable(g, a, b, visited);
}
```

Building the graph and searching it each touch every match outcome at most a constant number of times, so **time and space are both O(E)** where E is the number of results.

### Graph search: DFS vs. BFS

Both traversals compute reachability in **O(|V| + |E|) time**. Both need **O(|V|) space** in the worst case: DFS's space lives implicitly on the call stack (worst case: the search follows one long simple path through all vertices), while BFS's lives in its queue (worst case: the start vertex fans out to every other vertex, all enqueued at once).

They differ in the by-products:
- **BFS** visits vertices in increasing distance from the start, so it yields shortest path lengths in edge count for free.
- **DFS** naturally exposes structure — each vertex gets a *discovery time* (first reached) and a *finishing time* (all descendants done), and comparing these times underlies cycle detection, topological sorting, and component analysis.

## Top tips

- If the problem involves physically connected objects — road segments, wires, pipes — a graph model is the obvious first move.
- More broadly, any binary relationship between objects (linked pages, social follows, tournament results) can be encoded as a graph, and doing so usually reduces the task to a well-studied graph problem.
- Problems about *structure* — does a cycle exist, what are the components, is there an ordering — tend to fall to DFS.
- Problems about *optimization* — fewest hops, cheapest route, cheapest connecting network — call for BFS, Dijkstra's algorithm, or minimum spanning tree algorithms.

## 15.1 Paint a Boolean matrix

An n x m boolean array encodes a two-color image, one entry per pixel. Entries are *adjacent* if they differ by one step vertically or horizontally (so each entry has at most four neighbors, and adjacency is symmetric). The *region* of entry (x, y) is the set of all entries reachable from (x, y) via paths of adjacent, same-colored entries. Implement a routine that, given A and a coordinate (x, y), inverts the color of the entire region containing (x, y) — the flood-fill of paint programs.

*Hint: settle the conceptual algorithm first; implementation tricks come second.*

> Note: the sampler PDF cuts off immediately after the solution heading for this problem, so the development below is my own completion of the standard approach rather than a summary of the book's text.

**Approach 1 — the graph view.** The matrix induces an implicit undirected graph: vertices are entries, edges join 4-adjacent entries of equal color. The region of (x, y) is precisely the connected component containing (x, y), so any component-discovery traversal works. No explicit graph needs to be built — neighbors are computed on the fly from coordinates, which is the key implementation observation.

**Approach 2 — BFS.** Record the region's original color, then breadth-first traverse from (x, y), flipping each entry as it is enqueued. Flipping doubles as the "visited" mark: a flipped entry no longer matches the original color, so it can never be enqueued twice, and no separate visited structure is needed.

```cpp
void FlipRegionBFS(std::vector<std::vector<bool>>& A, int x, int y) {
  if (A.empty() || A[0].empty()) return;
  const bool original = A[x][y];
  std::queue<std::pair<int, int>> frontier;
  frontier.emplace(x, y);
  A[x][y] = !original;                       // flip on enqueue == mark visited
  const int dr[] = {1, -1, 0, 0}, dc[] = {0, 0, 1, -1};
  while (!frontier.empty()) {
    auto [r, c] = frontier.front();
    frontier.pop();
    for (int k = 0; k < 4; ++k) {
      int nr = r + dr[k], nc = c + dc[k];
      if (nr >= 0 && nr < (int)A.size() && nc >= 0 && nc < (int)A[0].size() &&
          A[nr][nc] == original) {
        A[nr][nc] = !original;
        frontier.emplace(nr, nc);
      }
    }
  }
}
```

**Approach 3 — DFS.** The recursive twin: flip the current entry, then recurse into each same-colored neighbor. Shorter to write; the traversal state lives on the call stack, which can overflow for huge single-colored images (an explicit stack avoids that).

```cpp
void FlipRegionDFS(std::vector<std::vector<bool>>& A, int r, int c, bool original) {
  if (r < 0 || r >= (int)A.size() || c < 0 || c >= (int)A[0].size() ||
      A[r][c] != original)
    return;
  A[r][c] = !original;
  FlipRegionDFS(A, r + 1, c, original);
  FlipRegionDFS(A, r - 1, c, original);
  FlipRegionDFS(A, r, c + 1, original);
  FlipRegionDFS(A, r, c - 1, original);
}
```

**Complexity (both):** every entry enters the traversal at most once and does O(1) work per neighbor, so **O(nm) time**; auxiliary space is **O(nm)** in the worst case (queue or stack when the whole image is one region), and O(1) beyond the traversal container since the flip serves as the visited flag.

**Worked example.** Take the 3 x 4 image below (1 = dark) and flip at (1, 1):

```
1 1 0 0        0 0 0 0
1 1 1 0   ->   0 0 0 0
0 1 0 0        0 0 0 0   (only the 6-cell dark region changed;
                          the (2,2) zero was never part of it)
```

The traversal starting at (1,1) reaches the five other dark cells through 4-adjacency and flips all six to 0; the light cells, including the one diagonally touching at (2, 2), are untouched because diagonal contact is not adjacency.

**Variants:** none appear in the sampler — the chapter text is truncated at this point.
