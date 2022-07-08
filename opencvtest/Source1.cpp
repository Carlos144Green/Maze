//#include <opencv2/imgcodecs.hpp>
//#include <opencv2/highgui.hpp>
//#include <opencv2/imgproc.hpp>
//#include <iostream>
//#include <vector>
//#include <unordered_map>
//#include <string>
//
//#include <stdlib.h>
//#include <time.h>
//#include <stdio.h>
//#include <queue>
//#include <stack>
//#include <windows.h>
//
//#define CVUI_IMPLEMENTATION
//#include "cvui.h"
//#define WINDOW_NAME "CVUI Test"
//
//using namespace cv;
//using namespace std;
//
//int width = (int)GetSystemMetrics(SM_CXSCREEN);
//int height = (int)GetSystemMetrics(SM_CYSCREEN);
//
//
////j is the rows
////i is the columns
//
//class Cell {
//public:
//    int x;
//    int y;
//    vector<bool> walls = { 1,1,1,1 };
//    bool v = false;
//
//
//    Cell() {
//    }
//    Cell(int x_in, int y_in) {
//        x = x_in;
//        y = y_in;
//    }
//
//
//};
//
//class MazeClass {
//public:
//    vector< vector<Cell>>  map;
//    vector<Cell>  row;
//    unordered_map<string, vector<Point>> nodeMap;
//    Point start;
//    Point end;
//
//    MazeClass(Point p) { // Constructor with parameters
//        row.resize(p.x, 0);
//        map.resize(p.y, row);
//    }
//
//    void show() {
//        for (int i = 0; i < map[0].size(); ++i) {
//            for (int j = 0; j < map.size(); ++j)
//                cout << map[j][i] << "";
//            cout << endl;
//        }
//    }
//    void populate(Point start_xy, Point end_xy, int density) {
//        srand((unsigned)time(NULL));
//        Cell cell;
//        for (int j = 0; j < map.size(); j++) {
//            for (int i = 0; i < map[0].size(); i++) {
//                cell = Cell(i, j);
//                map[j][i] = cell;
//            }
//
//        }
//    }
//    void makeNodeMap() {
//        Point p = Point(1, 1);
//        for (int i = 0; i < map.size(); i++) {                  // x moving
//            for (int j = 0; j < map[0].size(); j++) {           // y moving
//                if (map[i][j] != 1) {                           // if we aren't on a wall
//                    vector<Point> vec1;                         // initialize append queue
//                    string key = to_string(i) + " " + to_string(j);     // set key as "i j"
//                    if (i - 1 >= 0 && map[i - 1][j] != 1)       // and if we arent right of a wall
//                        vec1.push_back(Point(i - 1, j));          // push it to our append queue 
//
//                    if (j - 1 >= 0 && map[i][j - 1] != 1)
//                        vec1.push_back(Point(i, j - 1));
//
//                    if (i + 1 < map.size() && map[i + 1][j] != 1)
//                        vec1.push_back(Point(i + 1, j));
//
//                    if (j + 1 < map[0].size() && map[i][j + 1] != 1)
//                        vec1.push_back(Point(i, j + 1));
//
//                    nodeMap[key] = vec1;               // put empty vec in element to initialize it
//                    //nodeMap[key].insert(nodeMap[key].end(), vec1.begin(), vec1.end());
//                }
//            }
//        }
//    }
//    void showNodeMap() {
//        unordered_map<string, vector<Point>> ::iterator  itr;
//        for (itr = nodeMap.begin(); itr != nodeMap.end(); ++itr) {
//            cout << itr->first << '\t';
//            for (int i = 0; i < itr->second.size(); i++) {
//                cout << itr->second[i].x << itr->second[i].y << " ";
//            }
//            cout << endl;
//        }
//    }
//    void BFS() {
//        queue<Point> q;
//        vector<Point> seenList;
//        q.push(start);
//        Point tile;
//        string key;
//
//        while (q.size()) {
//            tile = q.front();
//            q.pop();
//            key = to_string(tile.x) + " " + to_string(tile.y);
//
//            if (find(seenList.begin(), seenList.end(), tile) == seenList.end()) {
//                cout << key << endl;
//                for (int i = 0; i < nodeMap[key].size(); i++) {
//                    q.push(nodeMap[key][i]);
//                }
//
//                seenList.push_back(tile);
//            }
//
//        }
//    }
//    Point TBFS(queue <Point>& q, vector <Point>& v, bool& found, int& generation, int& count) {
//        Point tile = Point(-2, -2);
//        if (found != 1) {
//            if (q.size()) {
//                tile = q.front();
//
//
//                q.pop();
//                string key = to_string(tile.x) + " " + to_string(tile.y);
//
//                if (find(v.begin(), v.end(), tile) == v.end()) {
//
//
//
//
//
//                    for (int i = 0; i < nodeMap[key].size(); i++) {
//                        q.push(nodeMap[key][i]);
//                    }
//
//
//                    count--;
//                    if (count == 0) {
//                        generation++;
//                        count = q.size();
//                    }
//
//                    v.push_back(tile);
//                    map[tile.x][tile.y] = generation;
//
//                }
//
//                if (tile.y == end.x && tile.x == end.y) {
//                    found = 1;
//                    cout << "FOUND!!!" << endl;
//                }
//            }
//        }
//        return tile;
//    }
//    Point TDFS(stack <Point>& q, vector <Point>& v, bool& found, int& generation, int& count, Point old) {
//        Point tile = Point(-2, -2);
//        if (found != 1) {
//            if (q.size()) {
//                tile = q.top();
//                q.pop();
//                string key = to_string(tile.x) + " " + to_string(tile.y);
//
//                if (find(v.begin(), v.end(), tile) == v.end()) {
//                    for (int i = 0; i < nodeMap[key].size(); i++) {
//                        q.push(nodeMap[key][i]);
//                    }
//
//
//                    if (abs(old.x - tile.x) > 1 || abs(old.y - tile.y) > 1) {
//                        generation++;
//                    }
//                    //
//
//                    v.push_back(tile);
//                    map[tile.x][tile.y] = generation;
//
//                }
//
//                if (tile.y == end.x && tile.x == end.y) {
//                    found = 1;
//                    cout << "FOUND!!!" << endl;
//                }
//            }
//        }
//        return tile;
//    }
//};
//
//Point resize(vector<vector<int>> map, double& cell_size) {
//    int x = int(map.size());
//    int y = int(map[0].size());
//    double xScale = width / x;
//    double yScale = height / y;
//
//    if (xScale < yScale)
//        cell_size = xScale * .8;
//    else
//        cell_size = yScale * .8;
//
//    x = int(x * cell_size);
//    y = int(y * cell_size);
//    return Point(x, y);
//}
//void draw(vector<vector<int>> map, Mat& img, double cell_size, Point tile = Point(-1, -1)) {
//    int fill = 0;
//    Point start;
//    cv::Scalar color;
//
//    if (tile == Point(-1, -1)) {
//        start = Point(0, 0);
//    }
//    else {
//        start = tile;
//    }
//    if (tile != Point(-2, -2)) {
//        for (int i = start.x; i < map.size(); i++) {
//            for (int j = start.y; j < map[0].size(); j++) {
//                if (map[i][j] == 1) {
//                    fill = -1;
//                    color = cv::Scalar(200, 200, 200);
//                }
//                else if (map[i][j] == 2) {
//                    fill = -1;
//                    color = cv::Scalar(10, 200, 10);
//                }
//                else if (map[i][j] == 0) {
//                    fill = 1;
//                    color = cv::Scalar(0, 0, 0);
//                }
//                else {
//                    fill = 1;
//                    color = cv::Scalar(17 * map[i][j] % 255, 50 * map[i][j] % 255, 39 * map[i][j] % 255);
//                }
//
//                cv::rectangle(img, Point(i * cell_size + 1, j * cell_size + 1), Point((i + 1) * cell_size - 2, (j + 1) * cell_size - 2), color, fill);
//                if (start == tile) {
//                    i = map.size();
//                    j = map[0].size();
//                }
//            }
//        }
//    }
//}
//
//int main()
//{
//    cvui::init(WINDOW_NAME);
//    double cell_size;
//
//    Point mazeSize = Point(80, 80);
//    Point mazeStart = Point(0, 0);
//    Point mazeEnd = Point(mazeSize.x - 1, mazeSize.y - 1);
//    MazeClass maze(mazeSize);
//    maze.populate(Point(0, 0), Point(0, 2), 30);
//    //maze.show();
//    //maze.makeNodeMap();
//    //maze.showNodeMap();
//
// /*   Point canvasSize = resize(maze.map, cell_size);
//    cv::Mat img = cv::Mat(cv::Size(canvasSize.x, canvasSize.y), CV_8UC3);
//    img = cv::Scalar(0, 0, 0);
//
//
//    cvui::imshow(WINDOW_NAME,img);
//    cv::waitKey(0);*/
//    return 1;
//}
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
////int main()
////{
////    cvui::init(WINDOW_NAME);
////    double cell_size = 100;
////    
////    MazeClass maze(Point(80, 80));
////    maze.populate(Point(0, 0), Point(0, 2), 30);
////    //maze.show();
////    maze.makeNodeMap();
////    //maze.showNodeMap();
////    
////    //maze.BFS();
////    Point xy = resize(maze.map, cell_size);
////    cv::Mat img = cv::Mat(cv::Size(xy.x, xy.y), CV_8UC3);
////    img = cv::Scalar(0, 0, 0);
////
////    draw(maze.map, img, cell_size);
////
////    //queue<Point> q;
////    stack<Point> q;
////    vector<Point> v;
////    q.push(maze.start);
////    bool found = 0;
////    int gen = 3;
////    int count = 1;
////    Point tile = maze.start;
////
////    //string path = "test.png";
////    //Mat img = imread(path);
////    //namedWindow("Trackbars", (600, 220));
////
////
////    
////    while (true) {
////        tile = maze.TDFS(q, v, found, gen, count, tile);
////        draw(maze.map, img, cell_size, tile);
////
////
////        /////////////////////Button//////////////////////
////        if (cvui::button(img, 100, 40, "Button")) {
////            cout << " button was clicked" << endl;
////        }
////        cvui::imshow(WINDOW_NAME, img);
////
////        if (cv::waitKey(20) == 27) {
////            break;
////        }
////    }
////    return 0;
////}
