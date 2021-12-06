#include "ODE.hpp"
#include "SolarSys.hpp"
#include <cmath>
#include <iostream>
#include <utility>

const double G = 6.67408e-20;
const double time_interval = 1;

void ODE::update(SolarSys &sys, size_t itr) {
  for (size_t i = 0; i < itr; ++i) {
    naiveUpdate(sys);
  }
}

void ODE::naiveUpdate(SolarSys &sys) {

  double delta_x = 0.0;
  double delta_y = 0.0;
  std::pair<double, double> *vec_table =
      new std::pair<double, double>[sys.size() * sys.size()];
  for (size_t x = 0; x < sys.size(); ++x) {
    for (size_t y = 0; y < sys.size(); ++y) {
      if (x == y) {
        (vec_table[x * sys.size() + y]).first = (sys(x).veloc())[0];
        (vec_table[x * sys.size() + y]).second = (sys(x).veloc())[1];
        continue;
      }

      delta_x = (sys(y).pos())[0] - (sys(x).pos())[0];
      delta_y = (sys(y).pos())[1] - (sys(x).pos())[1];
      double r = delta_x * delta_x + delta_y * delta_y;
      (vec_table[x * sys.size() + y]).first =
          (G * (sys(y).mass()) / r * (delta_x / std::sqrt(r))) * time_interval;
      (vec_table[x * sys.size() + y]).second =
          (G * (sys(y).mass()) / r * (delta_y / std::sqrt(r))) * time_interval;
    }
  }

  double x_vec_sum = 0.0;
  double y_vec_sum = 0.0;
  for (size_t idx = 0; idx < sys.size(); ++idx) {
    x_vec_sum = 0.0;
    y_vec_sum = 0.0;
    (sys(idx).pos())[0] += ((sys(idx).veloc())[0] * time_interval);
    (sys(idx).pos())[1] += ((sys(idx).veloc())[1] * time_interval);
    for (size_t vec = 0; vec < sys.size(); ++vec) {
      x_vec_sum += (vec_table[idx * sys.size() + vec]).first;
      y_vec_sum += (vec_table[idx * sys.size() + vec]).second;
    }
    //(sys(idx).pos())[0] += (x_vec_sum * time_interval);
    //(sys(idx).pos())[1] += (y_vec_sum * time_interval);
    (sys(idx).veloc())[0] = x_vec_sum;
    (sys(idx).veloc())[1] = y_vec_sum;
  }

  delete[] vec_table;
  return;
}
