#include <utility>
#include <vector>
#include <iostream>
#include <cmath>

typedef std::vector<double> Coord;
typedef std::vector<Coord> Grid;

struct MinMaxData {
    std::vector<double> mins;
    std::vector<double> maxs;
};

Grid *get_neighbors(Coord center, Grid &points);
Grid &grid_from_file(int dimensions = 2, std::istream &stream = std::cin);
Coord mean_shift(Coord x, Grid &points);
bool inside_circle(Coord p1, Coord p2, double radius);
double squared_euclidean_distance(Coord p1, Coord p2);
void get_grid_min_max(MinMaxData &data, Grid *grid);

inline double gaussian_kernel(double x, double bandwidth) {
    return exp(x / (2 * (bandwidth * bandwidth)));
}