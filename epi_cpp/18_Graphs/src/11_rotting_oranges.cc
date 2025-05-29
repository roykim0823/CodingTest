// You are given an m x n grid where each cell can have one of three values:

//     0 representing an empty cell,
//     1 representing a fresh orange, or
//     2 representing a rotten orange.

// Every minute, any fresh orange that is 4-directionally adjacent to a rotten orange becomes rotten.

// Return the minimum number of minutes that must elapse until no cell has a fresh orange. If this is impossible, return -1.

#include <vector>
#include <queue>

using namespace std;

class Solution {
public:
    int orangesRotting(vector<vector<int>>& grid) {
        
        //vector<vector<bool>> visited(grid.size(), {grid[0].size(), false});
        int elapsed_time = 0;
        for(int row = 0; row < grid.size(); ++row) {
            for(int col=0; col< grid[0].size(); ++col) {
                if (grid[row][col]==2) {
                    dfs(grid, row, col, 2);
                }

            }
        }
        for(int row = 0; row < grid.size(); ++row) {
            for(int col=0; col< grid[0].size(); ++col) {
                if (grid[row][col]==1) {
                    return -1;
                }

                // update the maximum time 
                elapsed_time = max(elapsed_time, grid[row][col]-2);
            }
        }
        return elapsed_time;    
    }
    void dfs (vector<vector<int>>& grid, int row, int col, int time) {
        constexpr int next_r[4] = {1, -1, 0, 0};
        constexpr int next_c[4] = {0, 0,  1,-1};

        // update minimum time
        grid[row][col] = time;

        for (int i=0; i<4; ++i) {
            int new_r = row+next_r[i];
            int new_c = col+next_c[i];
            if (new_r>=0 && new_r < grid.size() && new_c>=0 && new_c<grid[0].size() && 
                (grid[new_r][new_c]==1 || grid[new_r][new_c] > time+1)) {
                    dfs(grid, new_r, new_c, time +1);
            }
        }
    }
};