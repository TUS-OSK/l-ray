#include <iostream>
#include <random>
#include <omp.h>
#include "vec3.h"
#include "ray.h"
#include "sphere.h"
#include "image.h"
#include "camera.h"
#include "accel.h"


std::random_device rnd_dev;
std::mt19937 mt(rnd_dev());
std::uniform_real_distribution<> dist(0, 1);
inline double rnd() {
    return dist(mt);
}


int main() {
    Image img(512, 512);
    Camera cam(Vec3(0, 0, -3), Vec3(0, 0, 1));

    //光源の方向
    Vec3 lightDir = normalize(Vec3(1, 1, -1));

    Accel accel;
    //緑色の球
    accel.add(std::make_shared<Sphere>(Vec3(0, 0, 0), 1.0, Vec3(0, 1, 0)));
    //白色の床
    accel.add(std::make_shared<Sphere>(Vec3(0, -10001, 0), 10000, Vec3(0.9, 0.9, 0.9)));

    //100回のSSAA
#pragma omp parallel for schedule(dynamic, 1)
    for(int k = 0; k < 100; k++) {
        for(int i = 0; i < img.width; i++) {
            for(int j = 0; j < img.height; j++) {
                double u = (2.0*(i + rnd()) - img.width)/img.width;
                double v = (2.0*(j + rnd()) - img.height)/img.height;
                Ray ray = cam.getRay(u, v);

                Vec3 color;
                Hit hit;
                if(accel.intersect(ray, hit)) {
                    //ShadowRayを生成
                    Ray shadowRay(hit.hitPos + 0.001*hit.hitNormal, lightDir);

                    Hit hit_shadow;
                    //ShadowRayが物体に当たったら影をつける
                    if(accel.intersect(shadowRay, hit_shadow)) {
                        color = Vec3(0, 0, 0);
                    }
                    else {
                        double I = std::max(dot(lightDir, hit.hitNormal), 0.0);
                        color = I*hit.hitSphere->color;
                    }
                }
                //空の色
                else {
                    color = Vec3(0, 0, 0);
                }

                //画素に値を書き込む
                img.setPixel(i, j, img.getPixel(i, j) + 1/100.0*color);
            }
        }
    }
    img.ppm_output();
}
