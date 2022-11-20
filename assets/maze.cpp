#include <sys/wait.h>

#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <queue>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

class Maze {
   public:
    void maze(vector<vector<char>> &map);  // Definition for singly-linked list.
    void showMaze(vector<vector<char>> &map);

   private:
    void _maze(vector<vector<char>> &map, int i, int j);  //Use DFS
    void refineMaze(vector<vector<char>> &map, float percent);
    int countVisitedNeighbor(vector<vector<char>> &map, int i, int j);
    void shuffle(int a[], int n);
    void swap(int &a, int &b);
    int randInt(int min, int max);
};
void Maze::maze(vector<vector<char>> &map) {
    //U unvisited, ' ' visited
    for (int i = 0; i < map.size(); ++i) {
        for (int j = 0; j < map[0].size(); ++j) {
            map[i][j] = 'W';
        }
    }
    _maze(map, 0, 0);
    refineMaze(map, 40);
}

void Maze::showMaze(vector<vector<char>> &map) {
    for (int i = 0; i < map.size(); i++) {              //height
        for (int j = map[i].size() - 1; j >= 0; j--) {  //width
            cout << map[i][j];
        }
        cout << endl;
    }
}
void Maze::_maze(vector<vector<char>> &map, int i, int j) {
    int direct[][2] = {{0, 1}, {0, -1}, {-1, 0}, {1, 0}};
    int visitOrder[] = {0, 1, 2, 3};
    //out of boundary
    if (i < 0 || j < 0 || i >= map.size() || j >= map[0].size())
        return;
#ifdef DEBUG
    cout << "(" << i << ", " << j << ")" << endl;
#endif
    //visited, go back to the coming direction, return
    if (map[i][j] == ' ')
        return;

    //some neighbors are visited in addition to the coming direction, return
    //this is to avoid circles in maze
    if (countVisitedNeighbor(map, i, j) > 1)
        return;

    map[i][j] = ' ';  // visited

    //shuffle the visitOrder
    shuffle(visitOrder, 4);

    for (int k = 0; k < 4; ++k) {
        int ni = i + direct[visitOrder[k]][0];
        int nj = j + direct[visitOrder[k]][1];
        _maze(map, ni, nj);
    }
}
void Maze::refineMaze(vector<vector<char>> &map, float percent) {
    vector<vector<int>> wallsList;

    if (percent > 100 || percent < 15) return;
    for (int i = 0; i < map.size(); i++)
        for (int j = 0; j < map[i].size(); j++)
            if (map[i][j] == 'W') wallsList.push_back({i, j});

    int m;  //temporal
    percent = (100 - percent) / 100;
    int originalSize = wallsList.size();

    while ((float)wallsList.size() / originalSize > percent) {
        m = randInt(0, wallsList.size() - 1);
        map[wallsList[m][0]][wallsList[m][1]] = ' ';
        wallsList.erase(wallsList.begin() + m);
    }

    // horizontal laterals
    for (int i = 0; i < map.size() ; i++) {
        map[i].emplace(map[i].begin(), 'W');
        map[i].push_back('W');  //width was updated so it is within the bounds.
    }

    // vertical laterals
    std::vector<char> tmp(map[1].size(), 'W');
    map.emplace(map.begin(), tmp);
    map.emplace_back(tmp);

}
int Maze::countVisitedNeighbor(vector<vector<char>> &map, int i, int j) {
    int direct[][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    int count = 0;
    for (int k = 0; k < 4; ++k) {
        int ni = i + direct[k][0];
        int nj = j + direct[k][1];
        //out of boundary
        if (ni < 0 || nj < 0 || ni >= map.size() || nj >= map[0].size())
            continue;
        if (map[ni][nj] == ' ')
            count++;  //visited
    }
    return count;
}
void Maze::shuffle(int a[], int n) {
    for (int i = 0; i < n; ++i) {
        swap(a[i], a[rand() % n]);
    }
}
void Maze::swap(int &a, int &b) {
    int c = a;
    a = b;
    b = c;
}
int Maze::randInt(int min, int max) {
    return std::rand() % (max + 1 - min) + min;
}