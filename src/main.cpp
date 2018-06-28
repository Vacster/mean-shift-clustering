#include <map>
#include "header/matplotlibcpp.h"
#include "header/mean_shift.h"

namespace plt = matplotlibcpp;
using namespace std;

int main(int argc, char *argv[])
{   
    Grid &grid = grid_from_file(2);
    Grid test_points_grid;

    for (int x = 0; x < 20; x++)
    {
        for (int y = 0; y < 20; y++)
        {
            double val_x = static_cast<double>((x % 8) - 2);
            double val_y = static_cast<double>((y % 10) - 2);
            Coord test;
            test.push_back(val_x);
            test.push_back(val_y);
            test_points_grid.push_back(test);
        }
    }

    plt::ion();
    map<string, string> kwargs;
    kwargs["color"] = "red";
    kwargs["s"] = "100";

    for (int z = 0; z < 40; z++)
    {
        for (auto &coord : test_points_grid)
            coord = mean_shift(coord, grid);

        plt::clf(); //Can't remove just test_points_grid so it must all be redrawn
        plt::scatter(grid);
        plt::scatter(test_points_grid, kwargs);
        plt::draw();
        plt::pause(0.0001);
    }
    plt::ioff();
    plt::show();
    free(&grid);
}
