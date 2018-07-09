#ifndef SAMPLER_H
#define SAMPLER_H
#include <random>
std::random_device rnd_dev;
std::mt19937 mt(rnd_dev());
std::uniform_real_distribution<> dist(0, 1);
inline double rnd() {
    return dist(mt);
}


//半径1の円上でサンプリングする
Vec3 sampleDisk() {
    double theta = 2*M_PI*rnd();
    return Vec3(std::cos(theta), std::sin(theta), 0);
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
#endif
