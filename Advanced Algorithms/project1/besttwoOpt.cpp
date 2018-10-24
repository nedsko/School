/**
 * Authors: Jacob Hallman and Nedo Skobalj
 * (Greedy  algorithm  with  2-opt  once  +Random initialization and 2-opt with time limit)
 * Run this in kattis by dragging the file into the big box...
 */
#include <iostream>
#include <vector>
#include <cmath>
#include <tuple>
#include <limits>
#include <stack>
#include <algorithm>
#include <time.h>

// Struct to store vertices in graph
struct Point {
	double a;
	double b;
	int   id;
};

// Struct to represent an edge between two vertices where they are represented as indexes
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

// Function declarations
int distance(Point, Point);
std::vector<int> greedyAlgorithm();

void twoOpt();
void twoOptSwap(int&, int&);
int tourDistance(std::vector<int>);
void randomizeTour(std::vector<int> &tour);


// Global variables
std::vector <Point> points;
std::vector <Edge> edges;

// Distance matrix
std::vector<std::vector<int>> distMat;
// Store tours here
std::vector<int> tour;
std::vector<int> newTour;
std::vector<int> resultTour;

// Iterations and total number of vertices
const int iters = 13;
int nrOfPoints = 0;

using namespace std;
int main() {
	// Initiate clock
	clock_t startTime = clock();
	cin >> nrOfPoints;

	tour.resize(nrOfPoints);
	resultTour.resize(nrOfPoints);

	// Read vertices
	for(int i = 0; i < nrOfPoints; ++i) {
		Point point;
		cin >> point.a;
		cin >> point.b;
		tour[i] = i;
		point.id = i;
		points.push_back(point);
	}

	// Resize distance matrix to size nrOfPoints*nrOfPoints
	distMat.resize(nrOfPoints, vector<int>(nrOfPoints));

	// Set initial distance matrix distances to 0
	for (int i = 0; i < distMat.size(); ++i) {
		for(int j = 0; j < distMat[i].size(); ++j) {
			distMat[i][j] = 0;
		}
	}

	// Calculate all possible edge combinations and their distances and store values in edges + distance matrix
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

	newTour.resize(tour.size());
	// Set the current lowest cost to int max value
	int currentLowestCost = numeric_limits<int>::max();
	int iterLowestCost = 0;
	// Perform greedy once and calculate distance for this tour
	tour = greedyAlgorithm();
	iterLowestCost = tourDistance(tour);
	resultTour = tour;
	currentLowestCost = iterLowestCost;
	// Set clock variables
	clock_t currTime;
	float timeInMs = 0.0;
	// Loop as long as possible to utilize number of iterations as much as possible
	while (timeInMs < 1.85) {
		// Randomize tour and perform twoOpt
		randomizeTour(tour);
		twoOpt();
		iterLowestCost = tourDistance(tour);
		// Replace current tour if new is better
		if (iterLowestCost < currentLowestCost) {
			resultTour = tour;
			currentLowestCost = iterLowestCost;
		}
		currTime = clock();
		timeInMs = ((float)(currTime - startTime))/CLOCKS_PER_SEC;
	}
	
	// Print result
	for (int i = 0; i < resultTour.size(); ++i) {
		cout << resultTour[i] << endl;
	}

	return 0;
};

/**
 * @brief      Perform the swap of edges.
 *
 * @param      i     vertice i
 * @param      j     vertice j
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

/**
 * @brief      Perform 2-opt and update global tour as we go
 */
void twoOpt() {
	int tourSize = nrOfPoints;

	int improvement = 0;

	// Compute total tour distance
	int bestDist = tourDistance(tour);

	while (improvement < 2) {
		// Loop through edge combinations
		for (int i = 0; i < tourSize - 1; ++i) {
		    for (int j = i + 1; j < tourSize; ++j) {
				int indexI = i-1;
				int indexJ = j+1;
				// Perform checks for out of bounds
				if (i == 0) {
					indexI = 0;
				}
				if (indexJ == tourSize) {
					indexJ = j;
				}
				// Check if swap is worth doing
				int oldPartTour = distMat[tour[indexI]][tour[indexI+1]] + distMat[tour[j]][tour[indexJ]];
				int newPartTour = distMat[tour[indexI]][tour[j]] + distMat[tour[indexI+1]][tour[indexJ]];
				if (oldPartTour < newPartTour) {
		        	continue;
		        }

		        // Swap and update tour
				twoOptSwap(i, j);
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
}
/**
 * @brief      Compute distance of seq.
 *
 * @param[in]  seq   The sequence to calculate distance for.
 *
 * @return     Total sequence distance.
 */
int tourDistance(vector<int> seq) {

	int res = 0;
	// Loop through sequence and lookup distances in distance matrix
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

	// Start from 0 and set to visited
	path[0] = 0;
	isVisited[0] = true;

	int best;
	for(int i = 1; i < nrOfPoints; ++i) {
		best = -1;
		for(int j = 0; j < nrOfPoints; ++j) {
			
			// Check if not visited and if distance is better
			if( (!isVisited[j] &&  best == -1) || (!isVisited[j]  && (distance(points[path[i-1]], points[j]) < distance(points[path[i-1]], points[best])) ) ) {
				best = j;
			}
		}
		path[i] = best;
		isVisited[best] = true;

	}

	return path;
}

/**
 * @brief      Compute euclidian distance between two points (round to nearest integer)
 *
 * @param[in]  firstPoint   The first vertice
 * @param[in]  secondPoint  The second vertice
 *
 * @return     Distance between vertices
 */
int distance(Point firstPoint, Point secondPoint) {
	return sqrt(pow((secondPoint.a - firstPoint.a), 2) + pow((secondPoint.b - firstPoint.b), 2)) + 0.5;
}
/**
 * @brief      Randomize a new tour
 *
 * @param      tour  The tour to randomize
 */
void randomizeTour(vector<int> &tour) {
	srand(time(0));
	random_shuffle(tour.begin(), tour.end());
}