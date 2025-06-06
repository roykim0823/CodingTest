// You are given a m x n 2D grid initialized with these three possible values.

// -1 - A wall or an obstacle. 0 - A gate. INF - Infinity means an empty room. We use the value 2^31 - 1 = 2147483647 to represent INF as you may assume that the distance to a gate is less than 2147483647.

// Fill each empty room with the distance to its nearest gate. If it is impossible to reach a gate, it should be filled with INF.

// For example, given the 2D grid:

// INF  -1  0  INF
// INF INF INF  -1
// INF  -1 INF  -1
//   0  -1 INF INF
  
// After running your function, the 2D grid should be:
//   3  -1   0   1
//   2   2   1  -1
//   1  -1   2  -1
//   0  -1   3   4

#include <vector>
#include <queue>
#include <algorithm>
#include <limits>

using namespace std;


// BFS
// Time:  O(m * n)
// Space: O(g)

class Solution {
public:
    void wallsAndGates(vector<vector<int>>& rooms) {
        const int INF = numeric_limits<int>::max();
        queue<pair<int, int>> q;
        for (int i = 0; i < rooms.size(); ++i) {
            for (int j = 0; j < rooms[0].size(); ++j) {
                if (rooms[i][j] == 0) {
                    q.emplace(i, j);
                }
            }
        }
        while (!q.empty()) {
            auto [i, j] = q.front();
            q.pop();
            for (const pair<int, int>& d :
                 vector<pair<int, int>>{{i + 1, j}, {i - 1, j},
                                        {i, j + 1}, {i, j - 1}}) {
                int I, J;
                tie(I, J) = d;
                if (I >= 0 && I < rooms.size() &&
                    J >= 0 && J < rooms[0].size() &&
                    rooms[I][J] == INF) {
                    rooms[I][J] = rooms[i][j] + 1;
                    q.emplace(I, J);
                }
            }
        }
    }
};

// DFS
// static void wallsAndGates(int[][] rooms) {
// 		if (rooms == null || rooms.length == 0)
// 			return;

// 		int m = rooms.length;
// 		int n = rooms[0].length;

// 		for (int i = 0; i < m; i++) {
// 			for (int j = 0; j < n; j++) {
// 				if (rooms[i][j] == 0) {
// 					fill(rooms, i, j, 0);
// 				}
// 			}
// 		}
// }

// static void fill(int[][] rooms, int i, int j, int distance) {
// 		int m = rooms.length;
// 		int n = rooms[0].length;

// 		if (i < 0 || i >= m || j < 0 || j >= n || rooms[i][j] < distance) {
// 			return;
// 		}

// 		rooms[i][j] = distance;

// 		fill(rooms, i - 1, j, distance + 1);
// 		fill(rooms, i, j + 1, distance + 1);
// 		fill(rooms, i + 1, j, distance + 1);
// 		fill(rooms, i, j - 1, distance + 1);
// }