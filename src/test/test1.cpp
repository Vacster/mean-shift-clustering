#include "catch.hpp"
#include "../header/mean_shift.h"
#include <fstream>
#include <map>

#ifdef MS_VISUAL
#include "../header/matplotlibcpp.h"
namespace plt = matplotlibcpp;
#endif

bool double_equals(double a, double b, double epsilon = 0.001)
{
    double diff = (a - b);
    diff *= diff;
    epsilon *= epsilon;
    return diff < epsilon;
}

TEST_CASE( "Dataset1", "[mean_shift]" ) 
{

    GIVEN("A filename with csv data") 
    {

        Grid *grid;
        MinMaxData data;
        WHEN( "Creating grid from file" ) 
        {
            std::filebuf fb;
            if (fb.open("data/dataset1.csv", std::ios::in))
            {
                std::istream is(&fb);
                grid = &grid_from_file(2, is);

                THEN("The grid is populated") 
                {
                    REQUIRE( grid->size() == 400 );
                    REQUIRE( (*grid)[0].size() == 2 );
                }
            }
            REQUIRE( fb.is_open() );
            fb.close();
        }

        WHEN("Getting the MinMax values from the grid") 
        {
            get_grid_min_max(data, grid);

            THEN("The MinMaxData struct it populated correctly") 
            {
                REQUIRE( data.mins[0] == 0.02978 );
                REQUIRE( data.mins[1] == 0.004658 );
                REQUIRE( data.maxs[0] == 4.229498 );
                REQUIRE( data.maxs[1] == 5.385811 );
            }
        }

        WHEN("Applying the mean_shift algorithm in 1 point over 10 iterations")
        {
            Coord test_point;
            test_point.push_back(2.0);
            test_point.push_back(2.0);

            for (int x = 0; x < 10; x++)
                test_point = mean_shift(test_point, *grid);

            THEN("Test point should end in same place every time")
            {
                REQUIRE( double_equals(test_point[0], 2.93276) );
                REQUIRE( double_equals(test_point[1], 2.54771) );
            }
        }

        WHEN("Applying the mean_shift algorithm in 100 points over 20 iterations")
        {
            Grid test_points_grid;
            get_grid_min_max(data, grid);

            double ratio_0 = data.maxs[0] / 10.0;
            double ratio_1 = data.maxs[1] / 10.0;

            for (double x = data.mins[0]; x < data.maxs[0]; x += ratio_0)
            {
                for (double y = data.mins[1]; y < data.maxs[1]; y += ratio_1)
                {
                    Coord test_point;
                    test_point.push_back(x);
                    test_point.push_back(y);
                    test_points_grid.push_back(test_point);
                }
            }

            THEN("Points converge to the same destination every time")
            {
#ifdef MS_VISUAL
                std::map<std::string, std::string> kwargs;
                kwargs["color"] = "red";
                kwargs["s"] = "100";

                plt::scatter(*grid);
                plt::scatter(test_points_grid, kwargs);
                plt::show();
#endif

                for (int z = 0; z < 20; z++)
                    for (auto &coord : test_points_grid)
                        coord = mean_shift(coord, *grid);

#ifdef MS_VISUAL
                plt::scatter(*grid);
                plt::scatter(test_points_grid, kwargs);
                plt::show();
#endif
                REQUIRE( double_equals(test_points_grid[0][0], 1.06683) );
                REQUIRE( double_equals(test_points_grid[0][1], 0.524208) );
                REQUIRE( double_equals(test_points_grid[5][0], 0.957584) );
                REQUIRE( double_equals(test_points_grid[5][1], 2.29361) );    
            }
        }
    }
}