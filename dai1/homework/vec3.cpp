#include <cstdio>
#include <cmath>
class Vec3 {
    public:
        double x;
        double y;
        double z;

        Vec3() {
            x = y = z = 0;
        };
        Vec3(double _x, double _y, double _z) {
            x = _x;
            y = _y;
            z = _z;
        };

        double length() const {
            return std::sqrt(x*x + y*y + z*z);
        };
};

Vec3 operator+(const Vec3& v1, const Vec3& v2) {
    return Vec3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}
Vec3 operator-(const Vec3& v1, const Vec3& v2) {
    return Vec3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

Vec3 operator*(double k, const Vec3& v) {
    return Vec3(k * v.x, k * v.y, k * v.z);
}
Vec3 operator*(const Vec3& v, double k) {
    return k*v;
}

Vec3 operator/(double k, const Vec3& v) {
    return Vec3(k / v.x, k / v.y, k / v.z);
}
Vec3 operator/(const Vec3& v, double k) {
    return Vec3(v.x / k, v.y / k, v.z / k);
}

double dot(const Vec3& v1, const Vec3& v2) {
    return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}


int main() {
    Vec3 v1(1, 1, 1);
    Vec3 v2(2, 2, 2);
    Vec3 v3 = v1 + v2;
    printf("%f\n", dot(v1, v2));
    printf("%f\n", v3.length());
    return 0;
}
