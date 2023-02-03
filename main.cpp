#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <time.h>
#include <random>


#define WIDTH  1920 
#define HEIGHT 1080

#define POINT_RADIUS 7
#define POINT_COUNT 20

//            0xAABBGGRR
#define BLUE  0x00FF0000
#define GREEN 0x0000FF00
#define RED   0x000000FF


static std::random_device rd; 

typedef uint32_t Color;

struct Point
{
    int x, y;
    Color color;
};
typedef struct Point Point;


static Color image[HEIGHT][WIDTH];
static std::vector<Point> points;


void save_image_as_ppm(std::string path) 
{
    std::ofstream output_file;
    output_file.open(path, std::ios::binary);

    if (!output_file) {
        printf("output file did not open correctly\n");
        exit(1);
    }

    output_file << "P6\n" << WIDTH << " " << HEIGHT << "\n255\n";

    for (size_t i = 0; i < HEIGHT; i++) {
        for (size_t j = 0; j < WIDTH; j++) {
            Color pixel = image[i][j];
            uint8_t bytes[3] = {
                (uint8_t)(pixel&0x0000FF),
                (uint8_t)((pixel&0x00FF00)>>8),
                (uint8_t)((pixel&0xFF0000)>>16)
            };
            for (int x = 0; x < 3; x++) { output_file << bytes[x]; }
        }
    }

    output_file.close();
}

Color generate_color()
{
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> uni(0, UINT32_MAX);

    return uni(rng);

    // return rand() % UINT32_MAX;
    // int RR = rand() % 256;
    // int GG = rand() % 256;
    // int BB = rand() % 256;

    // return ((((((0x00000000|BB)<<8)|GG)<<8)|RR)<<8);
}

void generate_points(int point_count) 
{
    for (int i = 0; i < point_count; i++)
    {
        int random_x = rand() % WIDTH + 1;
        int random_y = rand() % HEIGHT + 1;
        
        Point point;
        point.x = random_x;
        point.y = random_y;
        point.color = generate_color();

        points.push_back(point);
    }
}

void draw_points(const int radius)
{
    std::for_each(points.begin(), points.end(), [&](Point const point){
        int dx_up = point.x - radius, dy_up = point.y - radius;
        int dx_down = point.x + radius, dy_down = point.y + radius;

        for (int j = dy_up; j < dy_down; j++)
        {
            for (int k = dx_up; k < dx_down; k++)
            {
                int dx = point.x - k;
                int dy = point.y - j;
                if (radius * radius >= dx * dx + dy * dy) image[j][k] = 0x000000;
            }
        }
    });
}

double calculate_distance(const Point point, const int x, const int y)
{
    int dx = point.x - x;
    int dy = point.y - y;

    return sqrt((double)(dx * dx + dy * dy));
}


void make_diagram()
{
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            if (image[i][j] != 0x000000) {
                double min_distance = WIDTH + HEIGHT;
                Point min_distance_point;
                std::for_each(points.begin(), points.end(), [&](Point const point) {
                    if (point.color != 0x000000){
                        double distance = calculate_distance(point, j, i);
                        if (distance < min_distance){
                            min_distance = distance;
                            min_distance_point = point;
                        }
                    }
                });
                
                image[i][j] = min_distance_point.color;
            }
        }
    }
}




int main()
{
    srand(time(NULL));

    for (size_t i = 0; i < HEIGHT; i++) {
        for (size_t j = 0; j < WIDTH; j++) {
            image[i][j] = 0xFFFFFF; 
        }
    }

    generate_points(POINT_COUNT);

    draw_points(POINT_RADIUS);

    make_diagram();

    save_image_as_ppm("output.ppm");

    return 0;
}