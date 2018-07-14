#ifndef ACCEL_H
#define ACCEL_H
#include <vector>
#include <memory>
#include "ray.h"
#include "hit.h"
#include "sphere.h"
class Accel {
    public:
        //Sphereのshared_ptrを要素に持つ配列
        std::vector<std::shared_ptr<Sphere>> spheres;

        Accel() {};

        //Sphereのshared_ptrを末尾に追加する
        void add(std::shared_ptr<Sphere> p) {
            spheres.push_back(p);
        };

        //与えられたRayと衝突計算を行い、結果をhitに格納する
        bool intersect(const Ray& ray, Hit& hit) const {
            bool isHit = false;

            //hitの距離を大きな値で初期化しておく
            hit.t = 1000000;

            //全ての物体に対し衝突計算をする
            Hit hit_each;
            for(auto p : spheres) {
                if(p->intersect(ray, hit_each)) {
                    //衝突距離が短いものが見つかったらhitをそれで置き換える
                    if(hit_each.t < hit.t) {
                        isHit = true;
                        hit = hit_each;
                    }
                }
            }

            return isHit;
        };
};
#endif
