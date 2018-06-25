#include "vec3.h"
#include "image.h"


int main() {
    Image img(512, 512);
    for(int i = 0; i < img.width; i++) {
        for(int j = 0; j < img.height; j++) {
            double r = (double)i/img.width;
            double g = (double)j/img.height;
            double b = 1.0;
            img.setPixel(i, j, Vec3(r, g, b));
        }
    }
    img.ppm_output();
}
