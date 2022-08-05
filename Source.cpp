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
#include <numeric>

#define CVUI_IMPLEMENTATION
#include "cvui.h"
#define WINDOW_NAME "CVUI Test"

HWND hMainWindow;

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

class Maze {
public:
    vector<vector<Cell>> cellMaze;
    vector<Cell>  row;
    Point start;
    Point end;
    Cell templateCell;
    Point currCell;
    Point next;
    Point mazeSize;
    stack <Point> stack;
    vector <Point> touching = { Point(0,-1), Point(1,0), Point(0,1), Point(-1,0) };
    Point canvasSize;
    queue <Point> q;
    bool hasMoves = true;


    int cell_size = 0;
    Maze(Point p, Point mazeStart, Point mazeEnd) { // Constructor with parameters
        mazeSize = Point(p.x,p.y);
        row.reserve(mazeSize.y);
        row.resize(mazeSize.y, templateCell);
        cellMaze.reserve(mazeSize.x);
        cellMaze.resize(mazeSize.x, row);
        start = mazeStart;
        end = mazeEnd;
        currCell = end;
        next = end;
        q.push(start);
        populate();
    }

    void populate() {
        Cell cell;
        for (int i = 0; i < mazeSize.x; i++) {
            for (int j = 0; j < mazeSize.y; j++) {
                cell = Cell(i, j);
                cellMaze[i][j] = cell;
            }
        }
    }

 
    Point resize() {
        int x = int(mazeSize.x);
        int y = int(mazeSize.y);
        double xScale = width / x;
        double yScale = height / y;

        if (xScale < yScale)
            cell_size = xScale * .7;
        else
            cell_size = yScale * .7;

        x = int(x * cell_size);
        y = int(y * cell_size);
        canvasSize = Point(x+1, y+100);
        return canvasSize;
    }
    void drawFrame(Mat img) {

        for (int i = 0; i < mazeSize.x; i++) {
            for (int j = 0; j < mazeSize.y; j++) {
                cellMaze[i][j].show(img, cell_size);
            }
        }
        highlight(img);

    }
    Point nextMove(Point current) {
        
        int i = current.x;
        int j = current.y;
        srand((unsigned)time(NULL));

        //cout << i << j << ": ";
        vector <Point> neighbors;

        for (int k = 0; k < 4; k++) {                       // loops through all neighbors
            Point thisNeighbor = Point(i, j) + touching[k]; // checks if any neighbors are out of bounds
            if ((thisNeighbor.x >= 0 && thisNeighbor.x < mazeSize.x) && (thisNeighbor.y >= 0 && thisNeighbor.y < mazeSize.y)) {
                if (cellMaze[thisNeighbor.x][thisNeighbor.y].v != true) {                  // if neighbor is new
                    neighbors.push_back(thisNeighbor);   // push to stack?
                    //cout << thisNeighbor.x << thisNeighbor.y << "  ";
                }
            }
        }

        if (neighbors.size()>0) {
            return neighbors[rand() % neighbors.size()];
        }
        return Point(-1, -1);
    }
    void highlight(Mat img) {
        int x = currCell.x;
        int y = currCell.y;
        cv::rectangle(img, Point(x * cell_size + 2, y * cell_size + 2), Point(x * cell_size + cell_size - 2, y * cell_size + cell_size - 2), Scalar(0, 150, 0), -1);
    }

    void removeWalls(Point a, Point b) {
        int x = a.x - b.x;
        int y = a.y - b.y;
        if (x == 1) {
            cellMaze[a.x][a.y].walls[3] = false;
            cellMaze[b.x][b.y].walls[1] = false;
        }
        else if (x == -1) {
            cellMaze[a.x][a.y].walls[1] = false;
            cellMaze[b.x][b.y].walls[3] = false;
        }
        if (y == 1) {
            cellMaze[a.x][a.y].walls[0] = false;
            cellMaze[b.x][b.y].walls[2] = false;
        }
        else if (y == -1) {
            cellMaze[a.x][a.y].walls[2] = false;
            cellMaze[b.x][b.y].walls[0] = false;
        }
    }

    bool generateMaze() {
        if (next != Point(-1, -1)) {
            removeWalls(next, currCell);
            currCell = next;
            cellMaze[currCell.x][currCell.y].v = true;

            stack.push(currCell);
            next = nextMove(currCell);
        }
        else if (stack.size() > 0) {
            next = stack.top();
            stack.pop();
            currCell = next;
            next = nextMove(currCell);
        }
        else {
            hasMoves = false;
            for (int i = 0; i < mazeSize.x; i++) {
                for (int j = 0; j < mazeSize.y; j++) {
                    cellMaze[i][j].v = false;

                }
            }
        }
        return hasMoves;
    }

    void BFS(Mat img, int &state) {
        if (q.size() != 0) {
            Point node = q.front();
            q.pop();
            if (cellMaze[node.x][node.y].v == false) {
                cellMaze[node.x][node.y].v = true;
                for (int k = 0; k < 4; k++) {                       // loops through all neighbors
                    if (cellMaze[node.x][node.y].walls[k] == false) {

                        q.push(touching[k] + node);
                    }
                }
            }
        }
        else {
            state++;
        }
    }
};

string getMazePath() {
    //should open window for file selection of maze image of any format.
    //for now it is just hard coded
    string filePath = "C:\\Users\\cuartasca\\Pictures\\maze9.jpg";
    return filePath;
}
//pullImgAndResize(){}

void importMaze(string filePath) {
    Mat gray;
    Mat blur;
    vector<Vec2f> lines;
    ////////////////////////////////////////////////
    //pull img, copy, and resize copy
    Mat img = imread(filePath, IMREAD_COLOR);
    Point imgSize = Point(img.size().width, img.size().height);
    resize(img, img, Size(650, 650 * img.size().height / img.size().width), 0, 0, INTER_LINEAR);
    Point resizedSize = Point(img.size().width, img.size().height);
    Mat imgText = img.clone();
    string text = "Crop Maze OR Press Enter";
    Size size = cv::getTextSize(text,cv::FONT_HERSHEY_DUPLEX, 1, 2, 0);
    int textX = (resizedSize.x - size.width) / 2;
    int textY = (resizedSize.y + size.height) / 2;


    cv::putText(imgText, text, Point(textX, textY), cv::FONT_HERSHEY_DUPLEX, 1, Scalar(30, 150, 0),2);
    Rect ROIrectangle = cv::selectROI("ROI Selection", imgText, true);
    //cout << ROIrectangle << endl;
    Mat ROI;
    if (ROIrectangle.area() != 0) {
        ROI = img(ROIrectangle);
    }
    else {
        ROI = img;
    }
    cv::destroyWindow("ROI Selection");
    cv::cvtColor(ROI, gray, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(gray, blur, Size(5, 5), 1);
    ////////////////////////////////////////////////
    //row color frequency
    vector<float> rowSpectrum;
    cv::Scalar tempVal;
    //cout << "height" << endl;
    for (int i = 0; i < ROI.size().height; i++) {
        tempVal = cv::mean(blur.row(i));
        //cout << tempVal.val[0] << endl;
        rowSpectrum.push_back(float(tempVal.val[0]));
    }
    ////////////////////////////////////////////////
    //dirty row data
    Mat out = Mat(rowSpectrum).reshape(1);
    Scalar mean = cv::mean(rowSpectrum);
    Point tempPoint;
    vector<Point> allPassingRows;
    float last= 255;
    for (int j = 0; j < ROI.size().height; j++){
        if (rowSpectrum[j] < mean.val[0]) {
            if (last > mean.val[0]) {
                tempPoint.x = j;
            }
            if (j == ROI.size().height - 1) {
                tempPoint.y = j;

                allPassingRows.push_back(tempPoint);
            }
        }
        else {
            if (last < mean.val[0]) {
                tempPoint.y = j;
                allPassingRows.push_back(tempPoint);
                tempPoint = Point(0, 0);
            }
        }
        last = rowSpectrum[j];
    }
    ////////////////////////////////////////////////
    // show dirty row data
    //for (int i = 0; i < allPassingRows.size(); i++) {
    //    //cout <<"row "<<i<<": "<< allPassingRows[i] << endl;
    //    cv::line(ROI, Point(0, int((allPassingRows[i].x + allPassingRows[i].y)/2)), Point(ROI.size().width, int((allPassingRows[i].x + allPassingRows[i].y) / 2)), Scalar(100, 255, 0),1);
    //}
    ////////////////////////////////////////////////
    //width color frequency
    vector<Point> allPassingCols;
    vector<float> colSpectrum;
    for (int i = 0; i < ROI.size().width; i++) {
        tempVal = cv::mean(blur.col(i));
        //cout << tempVal.val[0] << endl;

        colSpectrum.push_back(float(tempVal.val[0]));
    }

    ////////////////////////////////////////////////
    // dirty width data
    tempPoint = Point(0,0);
    mean = cv::mean(colSpectrum);
    for (int j = 0; j < ROI.size().width; j++) {
        if (colSpectrum[j] < mean.val[0]-50) {
            if (last > mean.val[0] - 50) {
                tempPoint.x = j;
            }
            if (j == ROI.size().width - 1) {
                tempPoint.y = j;
                allPassingCols.push_back(tempPoint);
            }
        }
        else {
            if (last < mean.val[0] - 50) {
                tempPoint.y = j;
                allPassingCols.push_back(tempPoint);
                tempPoint = Point(0, 0);
            }
        }
        last = colSpectrum[j];
    }
    ////////////////////////////////////////////////
    //narrowing columns to a pixel
    vector<int> colLines;
    cout << endl;
    for (int i = 0; i < allPassingCols.size(); i++) {
        colLines.push_back((allPassingCols[i].x + allPassingCols[i].y) / 2);
        //cout << colLines[i] << endl;
    }
    ////////////////////////////////////////////////
    // getting line spacing between columns
    vector<int> lineSpacing;
    int smallestSpace= colLines[1]- colLines[0];
    for (int i = 1; i < allPassingCols.size(); i++) {
        lineSpacing.push_back(colLines[i] - colLines[i - 1]);
        smallestSpace = min(smallestSpace, colLines[i] - colLines[i - 1]);
    }
    ////////////////////////////////////////////////
    // getting standard deviation for spacing
    vector<double> diff(lineSpacing.size());
    double sum = accumulate(lineSpacing.begin(), lineSpacing.end(), 0.0);

    double means = sum / lineSpacing.size();
    transform(lineSpacing.begin(), lineSpacing.end(), diff.begin(), [means](double x) { return x - means; });
    double sq_sum = std::inner_product(diff.begin(), diff.end(), diff.begin(), 0.0);
    double stdev = std::sqrt(sq_sum / lineSpacing.size());
    cout << stdev << endl;
    ////////////////////////////////////////////////
    // show dirty row data
    //for (int i = 0; i < colLines.size(); i++) {
    //    cv::line(ROI, Point(colLines[i], 0), Point(colLines[i], ROI.size().height), Scalar(0,100, 255), 2);
    //}
    ////////////////////////////////////////////////
    // cleans the column data, adds missing columns 
    double maxFit;
    vector<int> newColLines;

    for (int i = 0; i < lineSpacing.size(); i++)
    {
        newColLines.push_back(colLines[i]);

        if (lineSpacing[i]-smallestSpace*1>stdev*1) {//  maybe swap to means
            maxFit = lineSpacing[i] / (double)smallestSpace;
            int maxFitint = round(maxFit);
            cout << "maxFit " << maxFit << endl;

            if (abs(maxFit-maxFitint) < .2) {
                maxFit = maxFitint;
            }
            else {
                cout << "problem" << endl;
            }
            for (int j = 0; j < maxFit;j++) {
                newColLines.push_back(colLines[i] + (lineSpacing[i] / maxFit)*j);

            }
        }
    }
    newColLines.push_back(colLines.back());
    ////////////////////////////////////////////////
    // prints columns
    for (int i = 0; i < newColLines.size(); i++) {
        cv::line(ROI, Point(newColLines[i], 0), Point(newColLines[i], ROI.size().height), Scalar(100, 255, 0), 1);
    }

    cvui::imshow("test", img);
}

void setup() {
    //srand((unsigned)time(NULL));
    cvui::init(WINDOW_NAME);
}
int drawButtons(Mat img, Maze maze, Point canvasSize) {
    img = cv::Scalar(0, 0, 0);
    // More buttons will be added later, I plan on having the buttons gray out after use but that will be a problem for later.
    if (cvui::button(img, 100, canvasSize.y - 60, "Generate Maze")) {
        return 1;
    }

    if (cvui::button(img, 100, canvasSize.y - 30, "Open File")) {
        return 2;
    }
    if (cvui::button(img, 250, canvasSize.y - 60, "Start BFS")) {
        return 5;
    }
    else
        return 0;

}
void stateMachine(Mat img, Maze &maze,int &state, int buttonInput) {
    if (buttonInput != 0)
        state = buttonInput;
    switch (state)
    {
        case 0:
            break;
        case 1:
            if (maze.generateMaze() == false)
                state++;
                break;
            break;
        case 2:
            importMaze(getMazePath());

            state++;
            break;
        case 3:

            break;        
        case 4:

            break;
        case 5:
            maze.BFS(img, state);
            break;
    default:
        break;
    }
}

int main()
{
    setup();
    Point mazeSize = Point(60, 40);
    Point mazeStart = Point(0, 0);
    Point mazeEnd = Point(mazeSize.x - 1, mazeSize.y - 1);
    Maze maze(mazeSize, mazeStart, mazeEnd);

    Point canvasSize = maze.resize();
    cv::Mat img = cv::Mat(cv::Size(canvasSize.x, canvasSize.y), CV_8UC3);
    img = cv::Scalar(0, 0, 0);
    int buttonInput = 0;
    int state = 0;
    while (true) {
        buttonInput = drawButtons(img, maze, canvasSize);
        stateMachine(img, maze, state, buttonInput);
        maze.drawFrame(img);

        cvui::imshow(WINDOW_NAME, img);
        if (cv::waitKey(1) == 27)
            break;
    }

    return 1;
}