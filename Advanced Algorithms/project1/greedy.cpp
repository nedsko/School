/**
 * Authors: Jacob Hallman and Nedo Skobalj
 * (Greedy nearest neighbor)
 */
#include <iostream>
#include <vector>
#include <cmath>

struct Point {
    double a;
    double b;
};

int distance(Point, Point);
std::vector<int> greedyAlgorithm(std::vector<Point>);

using namespace std;
int main() {
    int nrOfPoints = 0;
    std::vector <Point> points;


    cin >> nrOfPoints;

    double a,b;
    for(int i = 0; i < nrOfPoints; ++i) {
        cin >> a >> b;
        Point point;
        point.a = a;
        point.b = b;
        points.push_back(point);
    }


    vector<int> result = greedyAlgorithm(points);

    for(int i = 0; i < result.size(); ++i) {
        cout << result[i] << endl;
    }
    

    return 0;
};

vector<int> greedyAlgorithm(vector<Point> points) {
    // Define path vector and initialize
    vector<int> path(points.size());
    vector<bool> isVisited(points.size());

    path[0] = 0;
    isVisited[0] = true;

    int best;
    for(int i = 1; i < points.size(); ++i) {
        best = -1;
        for(int j = 0; j < points.size(); ++j) {
            if( (!isVisited[j] &&  best == -1) || (!isVisited[j]  && (distance(points[path[i-1]], points[j]) < distance(points[path[i-1]], points[best])) ) ) {
                best = j;
            }
        }
        path[i] = best;
        isVisited[best] = true;

    }

    return path;
}

// Compute euclidian distance between two points (round to nearest integer)
int distance(Point firstPoint, Point secondPoint) {
    return sqrt(pow((secondPoint.a - firstPoint.a), 2) + pow((secondPoint.b - firstPoint.b), 2));
}