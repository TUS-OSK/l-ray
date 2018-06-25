#include <iostream>
#include <random>


std::random_device rnd_dev;
std::mt19937 mt(rnd_dev());
std::uniform_real_distribution<> dist(0, 1);
inline double rnd() {
    return dist(mt);
}


int main() {
    const int N = 1000000;

    double sum = 0;
    double pdf = 1.0/(2*2*2);
    for(int i = 0; i < N; i++) {
        double x = rnd();
        double y = rnd();
        double z = rnd();
        if(x*x + y*y + z*z < 1) sum += 1/pdf;
    }
    std::cout << sum/N << std::endl;
}
