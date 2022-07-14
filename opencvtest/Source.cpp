#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>

#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <queue>
#include <stack>
#include <windows.h>
#include <chrono>
#include <thread>

#define CVUI_IMPLEMENTATION
#include "cvui.h"
#define WINDOW_NAME "CVUI Test"

using namespace cv;
using namespace std;

int width = (int)GetSystemMetrics(SM_CXSCREEN);
int height = (int)GetSystemMetrics(SM_CYSCREEN);


//i is the rows
//j is the columns
class Cell {
public:
    int x;
    int y;
    vector<bool> walls = { 1,1,1,1 };
    bool v = false;

    Cell(int x_in, int y_in) {
        x = x_in;
        y = y_in;
    }
    Cell() {
        x = NULL;
        y = NULL;
    }

    void show(Mat img, int cell_size) {
        if (v) {
            cv::rectangle(img, Point(x * cell_size, y * cell_size), Point(x * cell_size + cell_size, y * cell_size + cell_size), Scalar(0, 100, 0), -1);
        }

        if (walls[0] == 1)
            cv::line(img, Point(x * cell_size, y * cell_size), Point(x * cell_size + cell_size, y * cell_size), Scalar(255, 255, 255));
        if (walls[1] == 1)
            cv::line(img, Point(x * cell_size +cell_size-1, y * cell_size+ cell_size), Point(x * cell_size + cell_size-1, y * cell_size), Scalar(255, 255, 255));
        if (walls[2] == 1)
            cv::line(img, Point(x * cell_size, y * cell_size + cell_size-1), Point(x * cell_size + cell_size, y * cell_size+ cell_size-1), Scalar(255, 255, 255));
        if (walls[3] == 1)
            cv::line(img, Point(x * cell_size, y * cell_size), Point(x * cell_size , y * cell_size + cell_size), Scalar(255, 255, 255));
    }

};

class MazeClass {
public:
    vector< vector<Cell>>  map;
    vector<Cell>  row;
    unordered_map<string, vector<Point>> nodeMap;
    Point start= Point(0, 0);
    Point end;
    Cell templateCell;
    Point currCell = start;
    Point next = start;
    Point mapSize;
    stack <Point> stack;
    int cell_size = 0;
    MazeClass(Point p) { // Constructor with parameters
        mapSize = Point(p.x,p.y);
        row.resize(mapSize.y, templateCell);
        map.resize(mapSize.x, row);

    }

    void populate() {
        Cell cell;
        for (int i = 0; i < map.size(); i++) {
            for (int j = 0; j < map[0].size(); j++) {
                cell = Cell(i, j);
                map[i][j] = cell;
            }
        }
    }

 
    Point resize() {
        int x = int(map.size());
        int y = int(map[0].size());
        double xScale = width / x;
        double yScale = height / y;

        if (xScale < yScale)
            cell_size = xScale * .8;
        else
            cell_size = yScale * .8;

        x = int(x * cell_size);
        y = int(y * cell_size);
        return Point(x, y);
    }
    void show_all(Mat img) {
        for (int i = 0; i < mapSize.x; i++) {
            for (int j = 0; j < mapSize.y; j++) {
                map[i][j].show(img, cell_size);
            }
        }
        highlight(img);
    }
    Point nextMove(Point current) {
        srand((unsigned)time(NULL));
        int i = current.x;
        int j = current.y;

        cout << i << j << ": ";
        vector <Point> neighbors;
        vector <Point> touching = { Point(0,-1), Point(1,0), Point(0,1), Point(-1,0) };

        for (int k = 0; k < 4; k++) {                       // loops through all neighbors
            Point thisNeighbor = Point(i, j) + touching[k]; // checks if any neighbors are out of bounds
            if ((thisNeighbor.x >= 0 && thisNeighbor.x < mapSize.x) && (thisNeighbor.y >= 0 && thisNeighbor.y < mapSize.y)) {
                if (map[thisNeighbor.x][thisNeighbor.y].v != true) {                  // if neighbor is new
                    neighbors.push_back(Point(thisNeighbor.x, thisNeighbor.y));   // push to stack?
                    cout << thisNeighbor.x << thisNeighbor.y << "  ";
                }
            }
        }
        cout << endl;

        if (neighbors.size()>0) {
            return neighbors[rand() % neighbors.size()];
        }
        return Point(-1, -1);
    }
    void highlight(Mat img) {
        int x = currCell.x;
        int y = currCell.y;
        cv::rectangle(img, Point(x * cell_size+2, y * cell_size+2), Point(x * cell_size + cell_size-2, y * cell_size + cell_size-2), Scalar(0, 150, 0), -1);
    }

    void removeWalls(Point a, Point b) {
        int x = a.x - b.x;
        int y = a.y - b.y;
        if (x == 1) {
            map[a.x][a.y].walls[3] = false;
            map[b.x][b.y].walls[1] = false;
        }
        else if (x == -1) {
            map[a.x][a.y].walls[1] = false;
            map[b.x][b.y].walls[3] = false;
        }
        if (y == 1) {
            map[a.x][a.y].walls[0] = false;
            map[b.x][b.y].walls[2] = false;
        }
        else if (y == -1) {
            map[a.x][a.y].walls[2] = false;
            map[b.x][b.y].walls[0] = false;
        }

    }

    void makeMove(bool &noMoves) {
        if (next != Point(-1, -1)) {



            removeWalls(next,currCell);
            currCell = next;
            map[currCell.x][currCell.y].v = true;

            stack.push(currCell);


            next = nextMove(currCell);


        }
        else if (stack.size()>0) {
            next = stack.top();
            stack.pop();
            currCell = next;

            next = nextMove(currCell);
        }
        else {
            noMoves = true;
        }
    }

};



int main()
{
    cvui::init(WINDOW_NAME);

    Point mazeSize = Point(10,4);
    Point mazeStart = Point(0, 0);
    Point mazeEnd = Point(mazeSize.x - 1, mazeSize.y - 1);
    MazeClass maze(mazeSize);
    maze.populate();
    
    
    //canvas size/show
    Point canvasSize = maze.resize();
    cv::Mat img = cv::Mat(cv::Size(canvasSize.x+1, canvasSize.y+1), CV_8UC3);
    img = cv::Scalar(0, 0, 0);
    bool noMoves = false;

    while (noMoves == false){
        maze.makeMove(noMoves);
        maze.show_all(img);
        cvui::imshow(WINDOW_NAME, img);
        if (cv::waitKey(20) == 27) 
            break;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return 1;
}
