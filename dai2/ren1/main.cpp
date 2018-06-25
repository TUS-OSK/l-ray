#include <iostream>
#include "vec3.h"
#include "ray.h"
#include "sphere.h"


int main() {
    Sphere sphere(Vec3(0, 0, 0), 1.0);
    Ray ray(Vec3(0, 0, -3), Vec3(0, 0, 1));

    if(sphere.intersect(ray)) {
        std::cout << "hit" << std::endl;
    }
}
