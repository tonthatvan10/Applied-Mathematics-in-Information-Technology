#include <iostream>
#include <cmath>
#include <iomanip>

using namespace std;

// Hàm mục tiêu
double func(double x) {
    if (fabs(15 * x - 2) < 1e-8) return INFINITY;
    return (exp(3 * x) + 12 * x * x + 5 * x) / (15 * x - 2) - 25 * x;
}

// Đạo hàm
double grad(double x) {
    double v = 15 * x - 2;
    if (fabs(v) < 1e-8) return INFINITY;

    double u = exp(3 * x) + 12 * x * x + 5 * x;
    double u_prime = 3 * exp(3 * x) + 24 * x + 5;
    double v_prime = 15;

    double df = (u_prime * v - u * v_prime) / (v * v) - 25;
    return df;
}

// Gradient Descent cơ bản
double gradientDescent(double x0, double gamma) {
    double x = x0;
    int iter = 0, max_iter = 100000;

    while (iter++ < max_iter) {
        double grad_x = grad(x);
        if (!isfinite(grad_x)) break;

        double new_x = x - gamma * grad_x;
        if (fabs(new_x - x) < 1e-6) break;
        x = new_x;
    }
    return x;
}

int main() {
    double x_init = 2.0;

    cout << scientific << setprecision(6);
    for (int i = 0; i < 4; i++) {
        double gamma = pow(10, -i);
        cout << "==============================" << endl;
        cout << "Gamma = " << gamma << endl;

        double x_gd = gradientDescent(x_init, gamma);
        cout << "[Gradient Descent] x = " << x_gd 
             << ", f(x) = " << func(x_gd) << endl;
    }

    return 0;
}