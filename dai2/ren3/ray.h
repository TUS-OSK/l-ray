#ifndef RAY_H
#define RAY_H
#include "vec3.h"
class Ray {
    public:
        Vec3 origin; //始点
        Vec3 direction; //方向

        //コンストラクタ
        Ray(const Vec3& origin, const Vec3& direction) : origin(origin), direction(direction) {};
};
#endif
