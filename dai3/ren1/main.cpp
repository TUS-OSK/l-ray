#include "vec3.h"
#include "ray.h"
#include "sphere.h"
#include "image.h"
#include "camera.h"
#include "accel.h"


int main() {
    Image img(512, 512);
    Camera cam(Vec3(0, 0, -3), Vec3(0, 0, 1));

    Accel accel;
    accel.add(std::make_shared<Sphere>(Vec3(0, 0, 0), 1.0));
    accel.add(std::make_shared<Sphere>(Vec3(0, -10001, 0), 10000));

    for(int i = 0; i < img.width; i++) {
        for(int j = 0; j < img.height; j++) {
            double u = (2.0*i - img.width)/img.width;
            double v = (2.0*j - img.height)/img.height;
            Ray ray = cam.getRay(u, v);

            Hit hit;
            if(accel.intersect(ray, hit)) {
                img.setPixel(i, j, (hit.hitNormal + Vec3(1, 1, 1))/2.0);
            }
            else {
                img.setPixel(i, j, Vec3(0, 0, 0));
            }
        }
    }
    img.ppm_output();
}
