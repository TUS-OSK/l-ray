#include "vec3.h"
#include "image.h"


int main() {
    Image img(512, 512);
    for(int i = 0; i < img.width; i++) {
        for(int j = 0; j < img.height; j++) {
            //-10 ~ 10に変換する
            double x = 10*(2.0*i - img.width)/img.width;
            double y = 10*(2.0*j - img.height)/img.height;

            //楕円の式
            double f = x*x/(3*3) + y*y/(2*2);

            //内側を白色にする
            if(f < 1.0f) {
                img.setPixel(i, j, Vec3(1, 1, 1));
            }
            else {
                img.setPixel(i, j, Vec3(0, 0, 0));
            }
        }
    }
    img.ppm_output();
}
