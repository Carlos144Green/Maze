#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>

#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <queue>


using namespace std;
typedef vector< vector<int> > Matrix;
typedef vector<int> Row;

class Point {
public:
    int x, y;
    Point() {
        x = 0;
        y = 0;
    }
    Point(int a, int b) { 
        x = a;
        y = b; 
    }
    //bool operator<(const Point& ob) const {
    //    return x < ob.x || (x == ob.x && y < ob.y);
    //}
    //bool operator>(const Point& ob) const {
    //    return x > ob.x || (x == ob.x && y > ob.y);
    //}
    bool operator==(const Point& ob) const {
        return x == ob.x && y == ob.y;
    }
};

class MazeClass {       
public:
    Matrix map;
    Row row;
    unordered_map<string, vector<Point>> nodeMap;
    Point start;
    Point end;

    MazeClass(Point p) { // Constructor with parameters
        row.resize(p.x, 0);
        map.resize(p.y, row);
    }

    void show() {
        for (int i = 0; i < map[0].size(); ++i) {
            for (int j = 0; j < map.size(); ++j)
                cout << map[j][i] << "";
            cout << endl;
        }
    }

    void populate(Point start_xy, Point end_xy, int density) {
        srand((unsigned)time(NULL));

        for (int i=0; i < map.size(); i++) {
            for (int j = 0; j < map[0].size(); j++) 
                if (rand() % 100 <= density) 
                    map[i][j] = 1;
            
            map[start_xy.y][start_xy.x] = 2;
            map[end_xy.y][end_xy.x] = 2;
            start = start_xy;
            end = end_xy;
        }
    }

    void makeNodeMap() {
        Point p = Point(1, 1);
        for (int i = 0; i < map.size(); i++) {                  // x moving
            for (int j = 0; j < map[0].size(); j++) {           // y moving
                if (map[i][j] != 1) {                           // if we aren't on a wall
                    vector<Point> vec1;                         // initialize append queue
                    string key = to_string(i) + " " + to_string(j);     // set key as "i j"
                    if (i - 1 >= 0 && map[i - 1][j] != 1)       // and if we arent right of a wall
                        vec1.push_back(Point(i-1, j));          // push it to our append queue 

                    if (j - 1 >= 0 && map[i][j-1] != 1)         
                        vec1.push_back(Point(i, j-1));          

                    if (i + 1 < map.size() && map[i + 1][j] != 1)      
                        vec1.push_back(Point(i + 1, j));        

                    if (j + 1 < map[0].size() && map[i][j+1] != 1)      
                        vec1.push_back(Point(i, j+1));          

                    nodeMap[key] = vec1;               // put empty vec in element to initialize it
                    //nodeMap[key].insert(nodeMap[key].end(), vec1.begin(), vec1.end());
                }
            }
        }
    }

    void showNodeMap() {
        unordered_map<string, vector<Point>> ::iterator  itr;
        for (itr = nodeMap.begin(); itr != nodeMap.end(); ++itr) {
            cout << itr->first << '\t';
            for (int i = 0; i < itr->second.size(); i++) {
                cout << itr->second[i].x << itr->second[i].y << " ";
            }
            cout << endl;
        }
    }

    void BFS() {
        queue<Point> q;
        q.push(start);
        Point tile;
        string key;

        while (q.size()) {
            tile = q.front();
            q.pop();
            key = to_string(tile.x) + " " + to_string(tile.y);

            for (int i = 0; i < nodeMap[key].size(); i++) {
                q.push(nodeMap[key][i]);
            }
            cout << key<<endl;
        }
    }
};

int main()
{
    MazeClass maze(Point(5, 4));
    maze.populate(Point(0, 0), Point(4, 3), 30);
    maze.show();
    maze.makeNodeMap();
    maze.showNodeMap();
    maze.BFS();
}
