#include <iostream>
#include <random>


std::random_device rnd_dev;
std::mt19937 mt(rnd_dev());
std::uniform_real_distribution<> dist(0, 1);
inline double rnd() {
    return dist(mt);
}


int main() {
    const int N = 10000;

    double sum = 0;
    for(int i = 0; i < N; i++) {
        double x = rnd();
        double y = rnd();
        sum += (x*y)/std::pow(x + y + 1, 2.0);
    }
    std::cout << sum/N << std::endl;
}
