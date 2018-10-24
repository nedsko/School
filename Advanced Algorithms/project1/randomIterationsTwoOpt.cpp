/**
 *  Authors: Jacob Hallman and Nedo Skobalj
 *   (Random  initialization  and  2-opt  with limited number of iterations)
 */
#include <iostream>
#include <vector>
#include <cmath>
#include <tuple>
#include <limits>
#include <stack>
#include <algorithm>

struct Point {
    double a;
    double b;
    int   id;
};

struct Edge {
    int firstNode;
    int secondNode;
    int distance;

    void print() {
        std::cout << firstNode << " " << secondNode << " Distance: " << distance << std::endl; 
    }

    void print_ids() {
        std::cout << firstNode << "to " << secondNode << ", " << " Distance: " << distance << std::endl; 
    }
};

// Edge distance compare operator
bool operator< (const Edge& a, const Edge& b) {
    return a.distance < b.distance;
}

// Structure for union by rank (rank associated with depth of tree which allows us to keep the resulting tree small)
struct Subset {
    int parent;
    int rank;
};


// Function declarations
int distance(Point, Point);
std::vector<int> greedyAlgorithm();
std::vector<int> randomizeVector(std::vector<int> &);

int twoOpt();
void twoOptSwap(int&, int&);
int tourDistance(std::vector<int>);


// Global variables
std::vector <Point> points;
std::vector <Edge> edges;

std::vector<std::vector<int>> distMat;
std::vector<int> tour;
std::vector<int> newTour;
std::vector<int> bestTour;

int nrOfPoints = 0;

using namespace std;
int main() {
    cin >> nrOfPoints;

    for(int i = 0; i < nrOfPoints; ++i) {
        Point point;
        cin >> point.a;
        cin >> point.b;
        point.id = i;
        points.push_back(point);
    }


    distMat.resize(nrOfPoints, vector<int>(nrOfPoints));

    for (int i = 0; i < distMat.size(); ++i) {
        for(int j = 0; j < distMat[i].size(); ++j) {
            distMat[i][j] = 0;
        }
    }

    for(int i = 0; i < nrOfPoints; ++i) {
        for(int j = i+1; j < nrOfPoints; ++j) {
            int dist = distance(points[i], points[j]);
            Edge edge;
            edge.firstNode = i;
            edge.secondNode = j;
            edge.distance = dist;
            distMat[i][j] = dist;
            distMat[j][i] = dist;
            edges.push_back(edge);
        }
    }

    vector<int> path(nrOfPoints);
    for(int i = 0; i < nrOfPoints; ++i) {
        path[i] = i;
    }


    int bestDistance = numeric_limits<int>::max();
    int currentDistance = numeric_limits<int>::max();

    tour = randomizeVector(path);
    bestTour = tour;
    for(int i = 0; i < 12; ++i) {
        tour = randomizeVector(path);
        newTour.resize(tour.size());
        currentDistance = twoOpt();
        if(bestDistance > currentDistance) {
            bestDistance = currentDistance;
            bestTour = tour;
        }
    }
    tour = greedyAlgorithm();
    if(tourDistance(tour) < bestDistance) {
        bestTour = tour;
    }

    for (int i = 0; i < tour.size(); ++i)
    {
        cout << bestTour[i] << endl;
    }
    

    return 0;
};

/**
 * @brief      Perform the swap of edges.
 *
 * @param      i     edge i
 * @param      j     edge j
 */
void twoOptSwap(int &i, int &j) {
    // take route[0] to route[i-1] and add them in order to newRoute
    for (int k = 0;  k <= i-1; ++k) {
        newTour[k] = tour[k];
    }
    int decrementer = 0;
    // take route[i] to route[j] and add them in reverse order to newRoute
    for (int k = i; k <= j; ++k) {
        newTour[k] = tour[j-decrementer];
        decrementer++;
    }
    // take route[j+1] to end and add them in order to newRoute
    for (int k = j+1;  k < tour.size(); ++k) {
        newTour[k] = tour[k];
    }
}


int twoOpt() {
    int tourSize = nrOfPoints;

    int improvement = 0;
    // Compute total tour distance
    int bestDist = tourDistance(tour);

    int diff = 0;
    vector<int> optimalSwap(2);
    

    while(improvement < 2) {
        for (int i = 0; i < tourSize - 1; ++i) {
           for (int j = i + 1; j < tourSize; ++j) {
                int indexI = i-1;
                int indexJ = j+1;
                if (i == 0) {
                    indexI = 0;
                }
                if (indexJ == tourSize) {
                    indexJ = j;
                }
                // kolla alla möjliga valueswaps sen gör den som är mest value.
                int oldPartTour = distMat[tour[indexI]][tour[indexI+1]] + distMat[tour[j]][tour[indexJ]];
                int newPartTour = distMat[tour[indexI]][tour[j]] + distMat[tour[indexI+1]][tour[indexJ]];
                if(oldPartTour > newPartTour){
                    diff = newPartTour - oldPartTour;
                    optimalSwap[0] = i;
                    optimalSwap[1] = j;
                } else {
                    continue;
                }

                // Update newTour with performed swap
                twoOptSwap(optimalSwap[0], optimalSwap[1]);

                int newDist = tourDistance(newTour);
                
                if (newDist < bestDist) {
                    improvement = 0;
                    tour = newTour;
                    bestDist = newDist;
                }   

            }
        }
        improvement++;
    }

    return tourDistance(tour);
}

int tourDistance(vector<int> seq) {
    int res = 0;
    for (int i = 0; i < tour.size()-1; ++i)
    {
        res += distMat[seq[i]][seq[i+1]];
    }
    res += distMat[seq[nrOfPoints-1]][seq[0]];
    return res;
    
}
 
/**
 * @brief      A greedy algoritm choosing the shortest path at every vertice.
 *
 * @return     Return chosen path.
 */
vector<int> greedyAlgorithm() {
    // Define path vector and initialize
    vector<int> path(nrOfPoints);
    vector<bool> isVisited(nrOfPoints);

    path[0] = 0;
    isVisited[0] = true;

    int best;
    int sum = 0;
    for(int i = 1; i < nrOfPoints; ++i) {
        best = -1;
        for(int j = 0; j < nrOfPoints; ++j) {
            
            if( (!isVisited[j] &&  best == -1) || (!isVisited[j]  && (distance(points[path[i-1]], points[j]) < distance(points[path[i-1]], points[best])) ) ) {
                best = j;
            }
        }
        path[i] = best;
        isVisited[best] = true;

    }

    return path;
}

vector<int> randomizeVector(vector<int> &path) {
    random_shuffle(path.begin(), path.end());
    return path;
}

//Compute euclidian distance between two points (round to nearest integer)
int distance(Point firstPoint, Point secondPoint) {
    return sqrt(pow((secondPoint.a - firstPoint.a), 2) + pow((secondPoint.b - firstPoint.b), 2)) + 0.5;
}