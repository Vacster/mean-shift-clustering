#define AREA_RADIUS 1.0
#define KERNEL_BANDWIDTH 0.6

#include <utility>
#include <vector>
#include <iostream>
#include <cmath>

typedef std::vector<double> Coord;
typedef std::vector<Coord> Grid;

Grid *get_neighbors(Coord center, Grid &points);
Grid &grid_from_file(int dimensions = 2, std::istream &stream = std::cin);
Coord mean_shift(Coord x, Grid &points);
bool inside_circle(Coord p1, Coord p2, double radius);
double squared_euclidean_distance(Coord p1, Coord p2);

inline double gaussian_kernel(double x, double bandwidth) {
    return exp(x / (2 * (bandwidth * bandwidth)));
}