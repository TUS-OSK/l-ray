#ifndef SPHERE_H
#define SPHERE_H
#include "vec3.h"
#include "ray.h"
#include "hit.h"
class Sphere {
    public:
        Vec3 center; //中心位置
        double radius; //半径
        Vec3 color; //色

        Sphere(const Vec3& center, double radius, const Vec3& color) : center(center), radius(radius), color(color) {};

        bool intersect(const Ray& ray, Hit& hit) const {
            double a = ray.direction.length2();
            double b = 2*dot(ray.direction, ray.origin - center);
            double c = (ray.origin - center).length2() - radius*radius;
            double D = b*b - 4*a*c;
            if(D < 0) return false;

            double t1 = (-b - std::sqrt(D))/(2*a);
            double t2 = (-b + std::sqrt(D))/(2*a);
            
            double t = t1;
            if(t < 0) {
                t = t2;
                if(t < 0) return false;
            }

            //衝突距離
            hit.t = t;
            //衝突位置
            hit.hitPos = ray.origin + t*ray.direction;
            //衝突した位置の法線
            hit.hitNormal = normalize(hit.hitPos - center);
            //衝突した球へのポインタ
            hit.hitSphere = this;
            return true;
        };
};
#endif
