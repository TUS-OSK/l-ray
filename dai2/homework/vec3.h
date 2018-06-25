#ifndef VEC3_H
#define VEC3_H
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

        //方向を逆にしたベクトルを返す
        Vec3 operator-() const {
            return Vec3(-x, -y, -z);
        };

        //ベクトルの長さを返す
        double length() const {
            return std::sqrt(x*x + y*y + z*z);
        };

        //ターミナルに出力する
        void print() const {
            std::cout << "(" << x << ", " << y << ", " << z << ")" << std::endl;
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
//外積
Vec3 cross(const Vec3& v1, const Vec3& v2) {
    return Vec3(v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x);
}

//正規化
Vec3 normalize(const Vec3& v) {
    return v/v.length();
}

//反射
Vec3 reflect(const Vec3& d, const Vec3& n) {
    return d - 2*dot(d, n)*n;
}
#endif
