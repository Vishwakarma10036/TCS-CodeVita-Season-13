#include <bits/stdc++.h>
using namespace std;

int dx[8] = {-1,-1,-1,0,0,1,1,1};
int dy[8] = {-1,0,1,-1,1,-1,0,1};

bool isValid(int x, int y, int n, int m, vector<vector<int>>& blocked) {
    return (x>=0 && y>=0 && x<n && y<m && !blocked[x][y]);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<vector<int>> grid(n, vector<int>(m));
    for(int i=0;i<n;i++)
        for(int j=0;j<m;j++)
            cin >> grid[i][j];

    int sx, sy, dx_, dy_;
    cin >> sx >> sy;
    cin >> dx_ >> dy_;
    sx--; sy--; dx_--; dy_--;

    int k;
    cin >> k;
    vector<vector<int>> blocked(n, vector<int>(m, 0));
    for(int i=0;i<k;i++) {
        int x, y;
        cin >> x >> y;
        blocked[x-1][y-1] = 1;
    }

    vector<vector<int>> dist(n, vector<int>(m, INT_MAX));
    priority_queue<vector<int>, vector<vector<int>>, greater<vector<int>>> pq;
    pq.push({0, sx, sy});
    dist[sx][sy] = 0;

    while(!pq.empty()) {
        auto cur = pq.top(); pq.pop();
        int cost = cur[0], x = cur[1], y = cur[2];

        if(x == dx_ && y == dy_) {
            cout << cost << "\n";
            return 0;
        }

        if(cost > dist[x][y]) continue;

        int maxNeighbour = INT_MIN;
        for(int d=0; d<8; d++) {
            int nx = x + dx[d], ny = y + dy[d];
            if(isValid(nx, ny, n, m, blocked)) {
                maxNeighbour = max(maxNeighbour, grid[nx][ny]);
            }
        }

        for(int d=0; d<8; d++) {
            int nx = x + dx[d], ny = y + dy[d];
            if(!isValid(nx, ny, n, m, blocked)) continue;

            int addCost = 0;
            if(grid[x][y] >= maxNeighbour) {
                addCost = max(0, grid[x][y] - grid[nx][ny] + 1);
            } else if(grid[nx][ny] != maxNeighbour) {
                addCost = max(0, maxNeighbour - grid[nx][ny]);
            }

            int newCost = cost + addCost;
            if(newCost < dist[nx][ny]) {
                dist[nx][ny] = newCost;
                pq.push({newCost, nx, ny});
            }
        }
    }

    cout << -1 << "\n";
    return 0;
}
