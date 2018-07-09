#include <iostream>
#include <omp.h>
#include "vec3.h"
#include "ray.h"
#include "sphere.h"
#include "image.h"
#include "camera.h"
#include "accel.h"
#include "sampler.h"
#include "util.h"


//物体集合
Accel accel;


//与えられたrayの方向から来る光の強さを計算する
//depthは再帰の深さを表す
Vec3 getColor(const Ray& ray, int depth = 0) {
    //100回以上の反射は追跡しない
    if(depth > 100) return Vec3(0, 0, 0);

    Hit hit;
    if(accel.intersect(ray, hit)) {
        //Diffuse
        if(hit.hitSphere->material == 0) {
            //反射レイを生成
            double pdf;
            Ray nextRay(hit.hitPos + 0.001*hit.hitNormal, randomCosineHemisphere(pdf, hit.hitNormal));

            //コサイン項
            double cos_term = std::max(dot(nextRay.direction, hit.hitNormal), 0.0);

            return 1/pdf * cos_term * hit.hitSphere->color/M_PI * getColor(nextRay, depth + 1);
        }
        //Mirror
        else if(hit.hitSphere->material == 1) {
            //反射レイを生成
            Ray nextRay(hit.hitPos + 0.001*hit.hitNormal, reflect(ray.direction, hit.hitNormal));
            return getColor(nextRay, depth + 1);
        }
        else {
            return Vec3(0, 0, 0);
        }
    }
    //空の色
    else {
        return Vec3(1, 1, 1);
    }
}


int main() {
    const int N = 100;
    Image img(512, 512);
    ThinLensCamera cam(Vec3(0, 0, -5), Vec3(0, 0, 1), 1.0, Vec3(0, 0, 0), 5.6);

    accel.add(std::make_shared<Sphere>(Vec3(-1, 0, 0), 1.0, Vec3(0.8, 0.2, 0.2), 0));
    accel.add(std::make_shared<Sphere>(Vec3(1, 0, 2), 1.0, Vec3(0.2, 0.8, 0.2), 0));
    accel.add(std::make_shared<Sphere>(Vec3(3, 0, 4), 1.0, Vec3(0.0, 0.2, 0.8), 0));
    accel.add(std::make_shared<Sphere>(Vec3(0, -10001, 0), 10000, Vec3(0.8, 0.8, 0.8), 0));

    for(int k = 0; k < N; k++) {
#pragma omp parallel for schedule(dynamic, 1)
        for(int i = 0; i < img.width; i++) {
            for(int j = 0; j < img.height; j++) {
                double u = (2.0*(i + rnd()) - img.width)/img.width;
                double v = (2.0*(j + rnd()) - img.height)/img.height;
                double w;
                Ray ray = cam.getRay(u, v, w);

                //色を計算する
                Vec3 color = w*getColor(ray);

                //画素に値を書き込む
                img.setPixel(i, j, img.getPixel(i, j) + 1.0/N*color);
            }
            if(omp_get_thread_num() == 0) {
                progress_output(k, N);
            }
        }
    }
    img.gamma_correction();
    img.ppm_output();
}
