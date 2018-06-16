/* 
 * Author: Kamil S.
 * Reference: https://en.wikipedia.org/wiki/Mean_shift
 */

#define AREA_RADIUS 1.0f
#define KERNEL_BANDWIDTH 0.35f

#include <utility>
#include <cmath>
#include "matplotlibcpp.h"

using namespace std;
namespace plt = matplotlibcpp;

typedef vector<float> Coord;
typedef vector<Coord> Grid;

Grid *N(Coord center, Grid &points);
bool inside_circle(Coord p1, Coord p2, float radius);
float squared_euclidean_distance(Coord p1, Coord p2);
float gaussian_kernel(float x, float bandwidth);
Coord mean_shift(Coord x, Grid &points);

int main(int argc, char *argv[])
{
    Grid grid;

    for (int z = 0; z < 5; z++)
        for (int zz = 0; zz < 5; zz++)
        {
            vector<float> x;
            x.push_back(z);
            x.push_back(zz);
            grid.push_back(x);
        }

    for (float z = 0.0f; z < 2.0f; z += 0.2f)
        for (float zz = 0.0f; zz < 2.0f; zz += 0.2f)
        {
            vector<float> x;
            x.push_back(z);
            x.push_back(zz);
            grid.push_back(x);
        }

    Coord test;
    test.push_back(2.0f);
    test.push_back(2.0f);

    for (int z = 0; z < 50; z++)
    {
        test = mean_shift(test, grid);
        cout << test[0] << '\t' << test[1] << endl;
    }
    
    plt::scatter(grid);
    plt::show();
}

/*
 * @param center Checks for the neighbors of circle with center 'center'
 * @param points Reference to the whole grid
 * Returns a Grid* object with the neighbors. 
 * It's the responsibility of the user to free the returned Grid.
 */
Grid *N(Coord center, Grid &points) 
{
    Grid *neighbors = new Grid();

    for (Coord p : points) 
        if (inside_circle(center, p, AREA_RADIUS))
            neighbors->push_back(p);

    return neighbors;
}

/*
 * @param p1 Point to compare to
 * @param p2 Center of circle
 * @param radius Radius of circle
 * The "circle" is actually an N-dimensional sphere
 */
bool inside_circle(Coord p1, Coord p2, float radius) 
{
    assert(p1.size() == p2.size());

    float radius_squared = radius * radius;

    float distance = squared_euclidean_distance(p1, p2);

    return distance <= radius_squared;
}

float squared_euclidean_distance(Coord p1, Coord p2) 
{
    assert(p1.size() == p2.size());

    int dimension_size = p1.size();
    float distance = 0, curr_distance;

    for (int x = 0; x < dimension_size; x++)
    {
        curr_distance = p1[x] - p2[x];
        curr_distance *= curr_distance;
        distance += curr_distance;
    }

    return distance;
}

float gaussian_kernel(float x, float bandwidth) 
{
    return exp(x / (2 * (bandwidth * bandwidth)));
}

Coord mean_shift(Coord x, Grid &points) {
    Grid *neighbors = N(x, points);

    int numerator_size = x.size();
    float denominator = 0;
    Coord numerator;

    for (int i = 0; i < numerator_size; i++)
        numerator.push_back(0.0f);

    for (Coord x_i : *neighbors) 
    {
        float distance = squared_euclidean_distance(x, x_i);
        float weight = gaussian_kernel(distance, KERNEL_BANDWIDTH);

        for (int p = 0; p < numerator_size; p++)
            numerator[p] += (weight * x_i[p]);

        denominator += weight;
    }

    for (int i = 0; i < numerator_size; i++)
        numerator[i] /= denominator;

    return numerator;
}