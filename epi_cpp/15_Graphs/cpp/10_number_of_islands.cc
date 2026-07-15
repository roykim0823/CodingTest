// Given an m x n 2D binary grid grid which represents a map of '1's (land) and '0's (water), return the number of islands.

// An island is surrounded by water and is formed by connecting adjacent lands horizontally or vertically. You may assume all four edges of the grid are all surrounded by water.

 

// Example 1:

// Input: grid = [
//   ["1","1","1","1","0"],
//   ["1","1","0","1","0"],
//   ["1","1","0","0","0"],
//   ["0","0","0","0","0"]
// ]
// Output: 1

// Example 2:

// Input: grid = [
//   ["1","1","0","0","0"],
//   ["1","1","0","0","0"],
//   ["0","0","1","0","0"],
//   ["0","0","0","1","1"]
// ]
// Output: 3
#include <vector>
#include <queue>

using namespace std;

class Solution {
public:

    int numIslands(vector<vector<char>>& grid) {
        int island_cnt = 0;

        //vector<vector<bool>> visited(grid.size(), {grid[0].size(), false});

        for(int row = 0; row < grid.size(); ++row) {
            for(int col=0; col< grid[0].size(); ++col) {
                if (grid[row][col]=='1') {
                    
                    ++island_cnt;
                    // BFS search
                    bfs(grid, row, col);
                }

            }
        }
        return island_cnt;    
    }
    void bfs (vector<vector<char>>& grid, int row, int col) {
        //constexpr int next_r[4] = {1, -1, 0, 0};
        vector<int> next_r = {1, -1, 0, 0};
        //constexpr int next_c[4] = {0, 0,  1,-1};
        vector<int> next_c = {0, 0, 1, -1};
        
        std:queue<pair<int, int>> queue;
        queue.push({row, col});
        grid[row][col] = '0';

        while(!queue.empty()) {
            auto [r, c] = queue.front();
            queue.pop();

            for (int i=0; i<4; ++i) {
                int new_r = r+next_r[i];
                int new_c = c+next_c[i];
                if (new_r>=0 && new_r < grid.size() && new_c>=0 && 
                    new_c<grid[0].size() && grid[new_r][new_c]=='1') {
                    grid[new_r][new_c] = '0';
                    queue.push({new_r, new_c});
                }
            }
        }

    }
};