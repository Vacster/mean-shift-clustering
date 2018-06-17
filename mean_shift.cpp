/* 
 * Author: Kamil S.
 * Reference: https://en.wikipedia.org/wiki/Mean_shift
 */

#define AREA_RADIUS 0.5
#define KERNEL_BANDWIDTH 0.35

#include <utility>
#include <cmath>
#include "matplotlibcpp.h"

using namespace std;
namespace plt = matplotlibcpp;

typedef vector<double> Coord;
typedef vector<Coord> Grid;

Grid *get_neighbors(Coord center, Grid &points);
Grid &grid_from_file(int dimensions = 2);
bool inside_circle(Coord p1, Coord p2, double radius);
double squared_euclidean_distance(Coord p1, Coord p2);
double gaussian_kernel(double x, double bandwidth);
Coord mean_shift(Coord x, Grid &points);

int main(int argc, char *argv[])
{
    Grid &grid = grid_from_file();
    Grid test_points_grid;
    Grid final_position_grid;

    Coord test;
    test.push_back(2.0f);
    test.push_back(2.0f);

    for (int z = 0; z < 50; z++)
    {
        test = mean_shift(test, grid);
        test_points_grid.push_back(Coord(test));
    }
    final_position_grid.push_back(Coord(test));
    cout << test[0] << '\t' << test[1] << endl;

    map<string, string> kwargs;
    kwargs["color"] = "red";
    kwargs["s"] = "100";
    kwargs["facecolors"] = "none";
    plt::scatter(grid);
    plt::scatter(test_points_grid, kwargs);
    kwargs["color"] = "green";
    kwargs["s"] = "150";
    plt::scatter(final_position_grid, kwargs);
    plt::show();
}

/*
 * @param center Checks for the neighbors of circle with center 'center'
 * @param points Reference to the whole grid
 * Returns a Grid* object with the neighbors. 
 * It's the responsibility of the user to free the returned Grid.
 */
Grid *get_neighbors(Coord center, Grid &points) 
{
    Grid *neighbors = new Grid();

    for (Coord p : points) 
        if (inside_circle(center, p, AREA_RADIUS))
            neighbors->push_back(p);

    return neighbors;
}

Grid &grid_from_file(int dimensions)
{
    Grid *grid = new Grid();

    double curr;
    while (!cin.eof())
    {
        Coord coord;
        for (int x = 0; x < dimensions; x++)
        {
            cin >> curr;
            coord.push_back(curr);
        }
        grid->push_back(coord);
    }

    return *grid;
}

/*
 * @param p1 Point to compare to
 * @param p2 Center of circle
 * @param radius Radius of circle
 * The "circle" is actually an N-dimensional sphere
 */
bool inside_circle(Coord p1, Coord p2, double radius) 
{
    assert(p1.size() == p2.size());

    double radius_squared = radius * radius;

    double distance = squared_euclidean_distance(p1, p2);

    return distance <= radius_squared;
}

double squared_euclidean_distance(Coord p1, Coord p2) 
{
    assert(p1.size() == p2.size());

    int dimension_size = p1.size();
    double distance = 0, curr_distance;

    for (int x = 0; x < dimension_size; x++)
    {
        curr_distance = p1[x] - p2[x];
        curr_distance *= curr_distance;
        distance += curr_distance;
    }

    return distance;
}

double gaussian_kernel(double x, double bandwidth) 
{
    return exp(x / (2 * (bandwidth * bandwidth)));
}

Coord mean_shift(Coord x, Grid &points) {
    Grid *neighbors = get_neighbors(x, points);

    int numerator_size = x.size();
    double denominator = 0;
    Coord numerator;

    for (int i = 0; i < numerator_size; i++)
        numerator.push_back(0.0f);

    for (Coord x_i : *neighbors) 
    {
        double distance = squared_euclidean_distance(x, x_i);
        double weight = gaussian_kernel(distance, KERNEL_BANDWIDTH);

        for (int p = 0; p < numerator_size; p++)
            numerator[p] += (weight * x_i[p]);

        denominator += weight;
    }

    for (int i = 0; i < numerator_size; i++)
        numerator[i] /= denominator;

    free(neighbors);
    return numerator;
}