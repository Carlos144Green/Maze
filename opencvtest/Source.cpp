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

#define CVUI_IMPLEMENTATION
#include "cvui.h"
#define WINDOW_NAME "CVUI Test"

using namespace cv;
using namespace std;

int width = (int)GetSystemMetrics(SM_CXSCREEN);
int height = (int)GetSystemMetrics(SM_CYSCREEN);


//j is the rows
//i is the columns
class Cell {
public:
    int x;
    int y;
    vector<bool> walls = { 1,1,0,1 };
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
        if (walls[0] == 1)
            cv::line(img, Point(x * cell_size, y * cell_size), Point(x * cell_size + cell_size, y * cell_size), Scalar(255, 255, 255));
        if (walls[1] == 1)
            cv::line(img, Point(x * cell_size +cell_size, y * cell_size+ cell_size), Point(x * cell_size + cell_size, y * cell_size), Scalar(255, 255, 255));
        if (walls[2] == 1)
            cv::line(img, Point(x * cell_size, y * cell_size + cell_size), Point(x * cell_size + cell_size, y * cell_size+ cell_size), Scalar(255, 255, 255));
        if (walls[3] == 1)
            cv::line(img, Point(x * cell_size, y * cell_size), Point(x * cell_size , y * cell_size + cell_size), Scalar(255, 255, 255));
        

        if (v) {
            cv::rectangle(img, Point(x * cell_size, y * cell_size), Point(x * cell_size + cell_size, y * cell_size + cell_size),Scalar(0,100,0),-1);
        }
    }

};

class MazeClass {
public:
    vector< vector<Cell>>  map;
    vector<Cell>  row;
    unordered_map<string, vector<Point>> nodeMap;
    Point start;
    Point end;
    Cell l;
    Point mapSize;
    int cell_size = 0;
    MazeClass(Point p) { // Constructor with parameters
        mapSize = Point(p.y,p.x);
        row.resize(mapSize.x, l);
        map.resize(mapSize.y, row);
    }

    void populate(Point start_xy, Point end_xy, int density) {
        Cell cell;
        for (int j = 0; j < map.size(); j++) {
            for (int i = 0; i < map[0].size(); i++) {
                cell = Cell(i, j);
                map[j][i] = cell;
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
        for (int j = 0; j < mapSize.x; j++) {
            for (int i = 0; i < mapSize.y; i++) {
                map[j][i].show(img, cell_size);
            }
        }
    }
    Point nextMove(Point current) {
        srand((unsigned)time(NULL));
        int j = current.x;
        int i = current.y;

        cout << i << j << ": ";
        vector <Point> neighbors;
        vector <Point> touching = { Point(-1,0), Point(0,1), Point(1,0), Point(0,-1) };

        for (int k = 0; k < 4; k++) {                       // loops through all neighbors
            Point thisNeighbor = Point(j, i) + touching[k]; // checks if any neighbors are out of bounds
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
};



int main()
{
    cvui::init(WINDOW_NAME);

    Point mazeSize = Point(2, 2);
    Point mazeStart = Point(0, 0);
    Point mazeEnd = Point(mazeSize.x - 1, mazeSize.y - 1);
    MazeClass maze(mazeSize);
    maze.populate(Point(0, 0), Point(0, 2), 30);
    
    
    //canvas size/show
    Point canvasSize = maze.resize();
    cv::Mat img = cv::Mat(cv::Size(canvasSize.x+1, canvasSize.y+1), CV_8UC3);
    img = cv::Scalar(0, 0, 0);

    Point next;
    Point current = Point(0,0);

    //while (current != Point(-1, -1)) {
        cout << "ASDA" << endl;
        maze.map[current.y][current.x].v = true;
        //show grid
        maze.show_all(img);
        current = maze.nextMove(current);
    //}
    
    cvui::imshow(WINDOW_NAME,img);
    cv::waitKey(0);

    return 1;
}
