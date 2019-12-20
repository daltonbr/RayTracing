#include <iostream>
#include "sphere.h"
#include "hitable_list.h"
#include "float.h"
#include "camera.h"
#include "material.h"

// Image converting
#define STB_IMAGE_IMPLEMENTATION
#include "../vendor/stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../vendor/stb/stb_image_write.h"

void generate_png(const char* inputFileName, const char* outputFileName )
{
    int x, y, n;
    unsigned char *data = stbi_load(inputFileName, &x, &y, &n, 0);
    stbi_write_png(outputFileName, x, y, 3, data, 0);
}

vec3 color(const ray& r, hitable *world, int depth)
{
    hit_record rec;
    if (world->hit(r, 0.001, MAXFLOAT, rec))  // acne problem solved
    {
        ray scattered;
        vec3 attenuation;
        if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered))
        {
            return attenuation * color(scattered, world, depth + 1);
        }
        else
        {
            return vec3(0, 0, 0);
        }
    }
    else
    {
        vec3 unit_direction = unit_vector(r.direction());
        float t = 0.5 * (unit_direction.y() + 1.0);
        // Lerping from white to blue in y
        return (1.0 - t) * vec3(1, 1, 1) + t * vec3(0.5, 0.7, 1.0);
    }
}

// Generating the content of an image file in .ppm format

int main()
{
    // const char* inputFileName = (const char*)"foo.png";//"chapter5b.ppm";
    // const char* outputFileName = (const char*)"foo.out.png";
    // generate_png(inputFileName, outputFileName);

    int nx = 2000;
    int ny = 1000;
    int ns = 1000;
    std::cout << "P3\n" << nx << " " << ny << "\n255\n";

    hitable *list[5];
    list[0] = new sphere(vec3(0, 0, -1), 0.5, new lambertian(vec3(0.5, 0.0, 1.0)));
    list[1] = new sphere(vec3(0.75, -0.25, -1.5), 0.25, new lambertian(vec3(0.1, 0.7, 0.3)));
    // A giant sphere to act as a ground (or a planet)
    
    list[2] = new sphere(vec3(0, -100.5, -1), 100, new lambertian(vec3(0.8, 0.6, 0.2)));
    list[3] = new sphere(vec3(1, 0, -1), 0.5, new metal(vec3(0.8, 0.6, 0.2), 0.3));
    list[4] = new sphere(vec3(-1, 0, -1), 0.5, new metal(vec3(0.8, 0.8, 0.8), 0.8));
    // rose gold sphere
    //list[5] = new sphere(vec3(-0.25, -0.125, -0.25), 0.125, new metal(vec3(0.718, 0.431, 0.475)));
    hitable *world = new hitable_list(list, 5);

    camera cam;

    for (int j = ny - 1; j >= 0; j--)
    {
        for (int i = 0; i < nx; i++)
        {
            vec3 col = vec3(0, 0, 0);
            for (int s = 0; s < ns; s++)
            {
                float u = float(i + drand48()) / float(nx);
                float v = float(j + drand48()) / float(ny);
                ray r = cam.get_ray(u, v);
                vec3 p = r.point_at_parameter(2.0);
                col += color(r, world, 0);
            }
            col /= float(ns);
            col = vec3( sqrt(col[0]), sqrt(col[1]), sqrt(col[2]) );
            int ir = int (255.99 * col[0]);
            int ig = int (255.99 * col[1]);
            int ib = int (255.99 * col[2]);
            
            std::cout << ir << " " << ig << " " << ib << "\n";
        }
    }
}
