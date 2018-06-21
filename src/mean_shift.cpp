/* 
 * Author: Kamil S.
 * Reference: https://en.wikipedia.org/wiki/Mean_shift
 */

#include "header/mean_shift.h"
#include "header/matplotlibcpp.h"
#include <algorithm>

using namespace std;

double AREA_RADIUS;
double KERNEL_BANDWIDTH;

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

/*
 * @param dimensions Optional parameter specifying how many dimensions there are,
 *                   if it isn't specified then the default value is 2.
 * @param stream Optional parameter specifying the istream from with which to read
 *               the file, if it isn't specified then the default value is std::cin.
 * Creates a new Grid in heap, populates it from the stream and then returns a 
 * reference to it. It's up to the callee to free it.
 * The file must be a CSV with each row being a different point and the first
 * row representing the AREA_RADIUS and KERNEL_BANDWIDTH
 */
Grid &grid_from_file(int dimensions, istream &stream)
{
    Grid *grid = new Grid();
    stream >> AREA_RADIUS;
    stream >> KERNEL_BANDWIDTH;
    
    double curr;
    while (!stream.eof())
    {
        Coord coord;
        for (int x = 0; x < dimensions; x++)
        {
            stream >> curr;
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

/*
 * https://en.wikipedia.org/wiki/Euclidean_distance#Squared_Euclidean_distance
 */
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

/*
 * @param x Center point from with which to calculate the mean shift
 * @param points The whole grid from which to calculate the neighbors
 * @return Returns the Coord to where x should shift to.
 * https://en.wikipedia.org/wiki/Mean_shift
 */
Coord mean_shift(Coord x, Grid &points) {
    Grid *neighbors = get_neighbors(x, points);

    int numerator_size = x.size();
    double denominator = 0;
    Coord numerator(numerator_size);

    fill(numerator.begin(), numerator.end(), 0.0f);

    for (Coord x_i : *neighbors) 
    {
        double distance = squared_euclidean_distance(x, x_i);
        double weight = gaussian_kernel(distance, KERNEL_BANDWIDTH);

        for (int p = 0; p < numerator_size; p++)
            numerator[p] += (weight * x_i[p]);

        denominator += weight;
    }

    for (double &num_i : numerator)
        num_i /= denominator;
        
    free(neighbors);
    return numerator;
}

/*
 * @param grid The grid to calculate the mins and max of
 * Iterates over the whole grid looking for the min and max
 * points of each component and returns them in a struct
 */
void get_grid_min_max(MinMaxData &data, Grid *grid)
{
    bool first = true;
    for (Coord coord : *grid)
    {
        int index = 0;
        for (auto it = coord.begin(); it != coord.end(); it++, index++)
        {
            if (first)
            {
                data.mins.push_back(numeric_limits<double>::max());
                data.maxs.push_back(numeric_limits<double>::lowest());
            }

            if (*it < data.mins[index])
                data.mins[index] = *it;

            if (*it > data.maxs[index])
                data.maxs[index] = *it;
        }
        first = false;
    }
}