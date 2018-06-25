#include <iostream>
#include <random>


std::random_device rnd_device;
std::mt19937 mt(rnd_device());
std::uniform_real_distribution<> dist(0, 1);
inline double rnd() {
    return dist(mt);
}


int main() {
    const int N = 10000;

    int a = 0;
    for(int i = 0; i < N; i++) {
        double x = 2*rnd();
        double y = 2*rnd();
        if((x - 1)*(x - 1) + (y - 1)*(y - 1) < 1) a++;
    }

    std::cout << 4 * (double)a/N << std::endl;
}
