#include <iostream>
#include "sphere.h"
#include "hitable_list.h"
#include "float.h"
#include "camera.h"

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

vec3 random_in_unit_sphere()
{
    vec3 p;
    do
    {
        p = 2.0 * vec3(drand48(), drand48(), drand48()) - vec3(1, 1, 1);
    } while (p.squared_length() >= 1.0);
    return p;
}

vec3 color(const ray& r, hitable *world)
{
    hit_record rec;
    if (world->hit(r, 0.0, MAXFLOAT, rec))
    {
        vec3 target = rec.p + rec.normal + random_in_unit_sphere();
        return 0.5 * color(ray(rec.p, target - rec.p), world);
    }
    else
    {
        vec3 unit_direction = unit_vector(r.direction());
        float t = 0.5 * (unit_direction.y() + 1.0);
        // Lerping from white to blue in y
        return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
    }
}

// Generating the content of an image file in .ppm format

int main()
{
    // const char* inputFileName = (const char*)"foo.png";//"chapter5b.ppm";
    // const char* outputFileName = (const char*)"foo.out.png";
    // generate_png(inputFileName, outputFileName);

    int nx = 200;
    int ny = 100;
    int ns = 100;
    std::cout << "P3\n" << nx << " " << ny << "\n255\n";

    hitable *list[2];
    list[0] = new sphere(vec3(0, 0, -1), 0.5);
    // A giant sphere to act as a ground (or a planet)
    list[1] = new sphere(vec3(0, -100.5, -1), 100);
    hitable *world = new hitable_list(list, 2);

    camera cam;

    for (int j = ny - 1; j >= 0; j--)
    {
        for (int i = 0; i < nx; i++)
        {
            vec3 col(0, 0, 0);
            for (int s = 0; s < ns; s++)
            {
                float u = float(i + drand48()) / float(nx);
                float v = float(j + drand48()) / float(ny);
                ray r = cam.get_ray(u, v);
                vec3 p = r.point_at_parameter(2.0);
                col += color(r, world);
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
