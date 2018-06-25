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


//物体集合
Accel accel;
//光源の方向
Vec3 lightDir = normalize(Vec3(1, 1, -1));


//与えられたrayの方向から来る光の強さを計算する
//depthは再帰の深さを表す
Vec3 getColor(const Ray& ray, int depth = 0) {
    //100回以上の反射は追跡しない
    if(depth > 100) return Vec3(0, 0, 0);

    Hit hit;
    if(accel.intersect(ray, hit)) {
        //Diffuse
        if(hit.hitSphere->material == 0) {
            //Shadow Rayを生成
            Ray shadowRay(hit.hitPos + 0.001*hit.hitNormal, lightDir);
            Hit hit_shadow;
            //Shadow Rayが当たったら影をつける
            if(accel.intersect(shadowRay, hit_shadow)) {
                return Vec3(0, 0, 0);
            }
            else {
                double I = std::max(dot(lightDir, hit.hitNormal), 0.0);
                return I * hit.hitSphere->color;
            }
        }
        //Mirror
        else if(hit.hitSphere->material == 1) {
            //反射レイを生成
            Ray nextRay(hit.hitPos + 0.001*hit.hitNormal, reflect(ray.direction, hit.hitNormal));
            //反射レイを使って自分自身を呼び出す
            return getColor(nextRay, depth + 1);
        }
        else {
            return Vec3(0, 0, 0);
        }
    }
    //空の色
    else {
        return Vec3(0, 0, 0);
    }
}


int main() {
    Image img(512, 512);
    Camera cam(Vec3(0, 0, -3), Vec3(0, 0, 1));

    //緑色の球
    accel.add(std::make_shared<Sphere>(Vec3(0, 0, 0), 1.0, Vec3(0, 1, 0), 0));
    //ミラーの床
    accel.add(std::make_shared<Sphere>(Vec3(0, -10001, 0), 10000, Vec3(1, 1, 1), 1));

    //100回のSSAA
#pragma omp parallel for schedule(dynamic, 1)
    for(int k = 0; k < 100; k++) {
        for(int i = 0; i < img.width; i++) {
            for(int j = 0; j < img.height; j++) {
                double u = (2.0*(i + rnd()) - img.width)/img.width;
                double v = (2.0*(j + rnd()) - img.height)/img.height;
                Ray ray = cam.getRay(u, v);

                //色を計算する
                Vec3 color = getColor(ray);

                //画素に値を書き込む
                img.setPixel(i, j, img.getPixel(i, j) + 1/100.0*color);
            }
        }
    }
    img.ppm_output();
}
