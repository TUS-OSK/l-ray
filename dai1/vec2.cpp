#include <cstdio>


class Vec2 {
  public:
    double x; //x座標
    double y; //y座標
    
    //コンストラクタ
    Vec2(double _x, double _y) {
      x = _x;
      y = _y;
    };
    
    //出力
    void print() const {
      printf("(%f, %f)\n", x, y);
    };
};


//演算子+をオーバーロード
Vec2 operator+(const Vec2& left, const Vec2& right) {
  return Vec2(left.x + right.x, left.y + right.y);
}
//演算子*をオーバーロード
//Vec2 * double
Vec2 operator*(const Vec2& left, double k) {
    return Vec2(left.x*k, left.y*k);
}
//double * Vec2
Vec2 operator*(double k, const Vec2& right) {
    return Vec2(k*right.x, k*right.y);
}


int main() {
  Vec2 v1(1, 1);
  Vec2 v2(2, 2);
  
  Vec2 v3 = v1 + v2;
  v3.print();

  Vec2 v4 = 3*v3;
  v4.print();
  
  return 0;
}
