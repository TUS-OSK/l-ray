#include <iostream>
#include <cmath>


class Figure {
    public:
        Figure() {};
        virtual double area() const = 0;
};


class Rectangle : public Figure {
    public:
        //左下(x1, y1)と右上(x2, y2)の座標
        double x1, y1, x2, y2;

        Rectangle(double _x1, double _y1, double _x2, double _y2) {
            x1 = _x1;
            y1 = _y1;
            x2 = _x2;
            y2 = _y2;
        };

        double area() const {
            return (x2 - x1)*(y2 - y1);
        };
};


class Triangle : public Figure {
    public:
        //3点の(x, y)座標
        double x1, y1;
        double x2, y2;
        double x3, y3;

        Triangle(double _x1, double _y1, double _x2, double _y2, double _x3, double _y3) {
            x1 = _x1;
            y1 = _y1;
            x2 = _x2;
            y2 = _y2;
            x3 = _x3;
            y3 = _y3;
        };

        double area() const {
            //方向ベクトルp1, p2を求める
            double p1x = x2 - x1;
            double p1y = y2 - y1;
            double p2x = x3 - x1;
            double p2y = y3 - y1;

            //面積はabs(外積)*1/2
            double cross = p1x*p2y - p1y*p2x;
            return std::abs(cross)/2.0;
        };
};


int main() {
    Rectangle rect(1, 1, 2, 2);
    Triangle tri(0, 0, 1, 0, 0, 1);

    std::cout << rect.area() << std::endl;
    std::cout << tri.area() << std::endl;

    return 0;
}

