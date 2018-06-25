#include <iostream>
#include <cmath>
class Vec3 {
    public:
        double x; //x成分
        double y; //y成分
        double z; //z成分

        //コンストラクタ
        Vec3() {
            x = y = z = 0;
        };
        Vec3(double _x, double _y, double _z) {
            x = _x;
            y = _y;
            z = _z;
        };

        //ベクトルの長さを返す
        double length() const {
            return std::sqrt(x*x + y*y + z*z);
        };
};


//足し算
Vec3 operator+(const Vec3& v1, const Vec3& v2) {
    return Vec3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}
//引き算
Vec3 operator-(const Vec3& v1, const Vec3& v2) {
    return Vec3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

//スカラー倍
Vec3 operator*(double k, const Vec3& v) {
    return Vec3(k * v.x, k * v.y, k * v.z);
}
Vec3 operator*(const Vec3& v, double k) {
    return k*v;
}

//スカラー倍
Vec3 operator/(double k, const Vec3& v) {
    return Vec3(k / v.x, k / v.y, k / v.z);
}
Vec3 operator/(const Vec3& v, double k) {
    return Vec3(v.x / k, v.y / k, v.z / k);
}

//内積
double dot(const Vec3& v1, const Vec3& v2) {
    return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}



class Ray {
    public:
        Vec3 origin; //始点
        Vec3 direction; //方向

        //コンストラクタ
        Ray(const Vec3& origin, const Vec3& direction) : origin(origin), direction(direction) {};
};


class Sphere {
    public:
        Vec3 center; //中心位置
        double radius; //半径

        //コンストラクタ
        Sphere(const Vec3& center, double radius) : center(center), radius(radius) {};

        //与えられたRayが球に衝突するかどうか判定する
        bool intersect(const Ray& ray) const {
            double d_norm = ray.direction.length();
            double oc_norm = (ray.origin - center).length();

            //二次方程式の係数
            double a = d_norm*d_norm;
            double b = 2*dot(ray.direction, ray.origin - center);
            double c = oc_norm*oc_norm - radius*radius;
            double D = b*b - 4*a*c;

            //解の候補
            double t1 = (-b - std::sqrt(D))/(2*a);
            double t2 = (-b + std::sqrt(D))/(2*a);

            //距離がマイナスになっているものは除外する
            double t = t1;
            if(t < 0) {
                t = t2;
                if(t < 0) return false;
            }

            return true;
        };
};


int main() {
    Sphere sphere(Vec3(0, 0, 0), 1.0);
    Ray ray(Vec3(0, 0, -3), Vec3(0, 0, 1));

    if(sphere.intersect(ray)) {
        std::cout << "hit" << std::endl;
    }

    return 0;
}
