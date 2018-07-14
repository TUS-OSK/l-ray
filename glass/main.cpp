#include <iostream>
#include <random>
#include <omp.h>
#include "vec3.h"
#include "ray.h"
#include "sphere.h"
#include "image.h"
#include "camera.h"
#include "accel.h"
#include "util.h"


std::random_device rnd_dev;
std::mt19937 mt(rnd_dev());
std::uniform_real_distribution<> dist(0, 1);
inline double rnd() {
    return dist(mt);
}


//半球面でのランダムな方向を返す
Vec3 randomHemisphere(const Vec3& n) {
  double u = rnd();
  double v = rnd();
  
  double y = u;
  double x = std::sqrt(1 - u*u)*std::cos(2*M_PI*v);
  double z = std::sqrt(1 - u*u)*std::sin(2*M_PI*v);

  Vec3 xv, zv;
  orthonormalBasis(n, xv, zv);
  
  return x*xv + y*n + z*zv;
}
//半球面でのコサインに比例したランダムな方向を返す
Vec3 randomCosineHemisphere(double &pdf, const Vec3& n) {
    double u = rnd();
    double v = rnd();

    double theta = 0.5*std::acos(1 - 2*u);
    double phi = 2*M_PI*v;
    pdf = 1/M_PI * std::cos(theta);

    double x = std::cos(phi)*std::sin(theta);
    double y = std::cos(theta);
    double z = std::sin(phi)*std::sin(theta);
    Vec3 xv, zv;
    orthonormalBasis(n, xv, zv);
    return x*xv + y*n + z*zv;
}


//フレネル項
double fresnel(const Vec3& d, const Vec3& n, double n1, double n2) {
    double f0 = std::pow((n1 - n2)/(n1 + n2), 2.0);
    return f0 + (1 - f0)*std::pow(dot(-d, n), 5.0);
}



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
            return hit.hitSphere->color * getColor(nextRay, depth + 1);
        }
        //Glass
        else if(hit.hitSphere->material == 2) {
            //ガラスに入射しているか?
            bool isEntering = dot(-ray.direction, hit.hitNormal) > 0;

            //法線
            Vec3 n = isEntering ? hit.hitNormal : -hit.hitNormal;
            
            //屈折率
            double n1, n2;
            n1 = isEntering ? 1.0 : 1.5;
            n2 = isEntering ? 1.5 : 1.0;
            double eta = n1/n2;

            //フレネル項
            double fr = fresnel(-ray.direction, n, n1, n2);

            //反射
            if(rnd() < fr) {
                Ray nextRay(Vec3(hit.hitPos + 0.001*n), reflect(ray.direction, n));
                return getColor(nextRay, depth + 1);
            }
            //屈折
            else {
                Vec3 wt;
                if(refract(-ray.direction, n, n1, n2, wt)) {
                    Ray nextRay(hit.hitPos - 0.001*n, wt);
                    return eta*eta * getColor(nextRay, depth + 1);
                }
                //全反射
                else {
                    Ray nextRay(hit.hitPos + 0.001*n, reflect(ray.direction, n));
                    return getColor(nextRay, depth + 1);
                }
            }
        }
        else {
            return Vec3(0, 0, 0);
        }
    }
    //空の色
    else {
        return 5*Vec3(1, 1, 1) * std::pow(std::max(dot(ray.direction, normalize(Vec3(1, 1, 1))), 0.0), 64.0);
    }
}


int main() {
    const int N = 1000;
    Image img(512, 512);
    Camera cam(Vec3(0, 0, -3), Vec3(0, 0, 1));

    //緑色の球
    accel.add(std::make_shared<Sphere>(Vec3(0, 0, 0), 1.0, Vec3(1, 1, 1), 2));
    //白色の床
    accel.add(std::make_shared<Sphere>(Vec3(0, -10001, 0), 10000, Vec3(0.8, 0.8, 0.8), 0));

    //100回のSSAA
    for(int k = 0; k < N; k++) {
        for(int i = 0; i < img.width; i++) {
#pragma omp parallel for schedule(dynamic, 1)
            for(int j = 0; j < img.height; j++) {
                double u = (2.0*(i + rnd()) - img.width)/img.width;
                double v = (2.0*(j + rnd()) - img.height)/img.height;
                Ray ray = cam.getRay(u, v);

                //色を計算する
                Vec3 color = getColor(ray);

                //画素に値を書き込む
                img.setPixel(i, j, img.getPixel(i, j) + 1.0/N*color);
            }
        }
        std::cout << progressbar(k, N) << " " << percentage(k, N) << "\r" << std::flush;
    }
    img.gamma_correction();
    img.ppm_output();
}
