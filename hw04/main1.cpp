#include <cstdio>
#include <cstdlib>
#include <vector>
#include <chrono>
#include <cmath>
#include <array>
constexpr size_t N = 1024; 

using ArrF = std::array<float, N>;


float frand() {
    return (float)rand() / RAND_MAX * 2 - 1;
}

struct Star {
    ArrF px; 
    ArrF py;
    ArrF pz;
    ArrF vx;
    ArrF vy;
    ArrF vz;
    ArrF mass;
};

Star stars;

void init() {
    for (int i = 0; i < 48; i++) {
        stars.px[i] = frand();
        stars.py[i] = frand();
        stars.pz[i] = frand();
        stars.vx[i] = frand();
        stars.vy[i] = frand();
        stars.vz[i] = frand();
        stars.mass[i] = frand() + 1;
    }
}

float G = 0.001;
float eps = 0.001;
float dt = 0.01;
float G_dt = G * dt;

void step() {

    ArrF dx;
    ArrF dy;
    ArrF dz;
    ArrF d2;

    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < N; ++j) {
            dx[j] = stars.px[j] - stars.px[i];
            dx[j] *= dx[j];
        }
        for (size_t j = 0; j < N; ++j) {
            dy[j] = stars.py[j] - stars.py[i];            
            dy[j] *= dy[j];
        }
        for (size_t j = 0; j < N; ++j) {
            dz[j] = stars.pz[j] - stars.pz[i];            
            dz[j] *= dz[j];
        }
        for (size_t j = 0; j < N; ++j) {
            d2[j] = dx[j] + dy[j] + dz[j] + eps * eps;
            d2[j] *= std::sqrt(d2[j]);
            d2[j] = 1 / d2[j];
        }
        
        float vx = 0, vy = 0, vz = 0; 
        float fac = stars.mass[i] * G_dt;
        for (size_t j = 0; j < N; ++j) {
            vx += dx[j] * d2[j] * fac; 
            vy += dy[j] * d2[j] * fac;
            vz += dz[j] * d2[j] * fac;        
        }

        stars.vx[i] += vx;
        stars.vy[i] += vy;
        stars.vz[i] += vz;
    } 
    
    for (size_t i = 0; i < N; ++i) {
        stars.px[i] += stars.vx[i] * dt;
    }
    
    for (size_t i = 0; i < N; ++i) {
        stars.py[i] += stars.vy[i] * dt;
    }
    
    for (size_t i = 0; i < N; ++i) {
        stars.pz[i] += stars.vz[i] * dt;
    }
}

float calc() {
    float energy = 0;

    for (size_t i = 0; i < N; ++i) {
        float v2 = stars.vx[i] * stars.vx[i] + stars.vy[i] + stars.vy[i] + stars.vz[i] * stars.vz[i];
        for (size_t j = 0; j < N; ++j) {
            float dx = stars.px[j] - stars.px[i];
            float dy = stars.py[j] - stars.py[i];            
            float dz = stars.pz[j] - stars.pz[i];
            float d2 = dx * dx + dy * dy + dz * dz + eps * eps;
            energy -= stars.mass[j] * stars.mass[i] * G / std::sqrt(d2) / 2;
        }
        
    }
    return energy;
}

template <class Func>
long benchmark(Func const &func) {
    auto t0 = std::chrono::steady_clock::now();
    func();
    auto t1 = std::chrono::steady_clock::now();
    auto dt = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0);
    return dt.count();
}

int main() {
    init();
    printf("Initial energy: %f\n", calc());
    auto dt = benchmark([&] {
        for (int i = 0; i < 100000; i++)
            step();
    });
    printf("Final energy: %f\n", calc());
    printf("Time elapsed: %ld ms\n", dt);
    return 0;
}
