#include "ODE.hpp"
#include "SolarSys.hpp"
#include <cmath>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <utility>

const long double G = 6.67408e-20L;
const long double time_interval = 60;

std::vector<std::vector<long double> > ODE::update(SolarSys &sys, long long itr, size_t method) {
  const long double itr_step = 1 / time_interval;
  std::vector<std::vector<long double> >  pos_table(sys.size() * 366);
  const size_t system_size = sys.size();
  if (1 == method) {
    for (long long i = 0; i < itr * itr_step; ++i) {
      Euler(sys);
      if(i == 0 || (i+1)%(24*60) == 0){
      for (size_t k = 0; k < sys.size(); ++k){
        pos_table[system_size*(i+1)/(24*60) + k].push_back(sys(k).pos()[0]);
        pos_table[system_size*(i+1)/(24*60) + k].push_back(sys(k).pos()[1]);
        pos_table[system_size*(i+1)/(24*60) + k].push_back(sys(k).pos()[2]);
      }
      }
    }
    return pos_table;
  } else if (2 == method) {
    for (long long i = 0; i < itr * itr_step; ++i) {
      RungeKutta(sys);
      if(i == 0 || (i+1)%(24*60) == 0){
      for (size_t k = 0; k < sys.size(); ++k){
        pos_table[system_size*(i+1)/(24*60) + k].push_back(sys(k).pos()[0]);
        pos_table[system_size*(i+1)/(24*60) + k].push_back(sys(k).pos()[1]);
        pos_table[system_size*(i+1)/(24*60) + k].push_back(sys(k).pos()[2]);
      }
      }
    }
    return pos_table;
  } else {
    throw "invalid method choice!!";
  }
}

void ODE::Euler(SolarSys &sys) {

  long double delta_x = 0.0;
  long double delta_y = 0.0;
  long double delta_z = 0.0;
  //auto vec_table = std::make_unique<std::pair<long double, long double>[]>(
  //    sys.size() * sys.size());
  auto vec_table = std::make_unique<std::vector<long double>[]>(
      sys.size() * sys.size());

//#pragma omp parallel for private(delta_x, delta_y)
  for (size_t x = 0; x < sys.size(); ++x) {
    for (size_t y = 0; y < sys.size(); ++y) {
      if (x == y) {
        (vec_table[x * sys.size() + y]).push_back( (sys(x).veloc())[0]);
        (vec_table[x * sys.size() + y]).push_back((sys(x).veloc())[1]);
        (vec_table[x * sys.size() + y]).push_back((sys(x).veloc())[2]);
        continue;
      }

      delta_x = (sys(y).pos())[0] - (sys(x).pos())[0];
      delta_y = (sys(y).pos())[1] - (sys(x).pos())[1];
      delta_z = (sys(y).pos())[2] - (sys(x).pos())[2];
      long double r = delta_x * delta_x + delta_y * delta_y + delta_z * delta_z;
      (vec_table[x * sys.size() + y]).push_back(
          (G * (sys(y).mass()) / r * (delta_x / std::sqrt(r))) * time_interval);
      (vec_table[x * sys.size() + y]).push_back(
          (G * (sys(y).mass()) / r * (delta_y / std::sqrt(r))) * time_interval);
      (vec_table[x * sys.size() + y]).push_back(
          (G * (sys(y).mass()) / r * (delta_z / std::sqrt(r))) * time_interval);
    }
  }


  long double x_vec_sum = 0.0;
  long double y_vec_sum = 0.0;
  long double z_vec_sum = 0.0;

//#pragma omp parallel for private(x_vec_sum, y_vec_sum)
  for (size_t idx = 0; idx < sys.size(); ++idx) {
    x_vec_sum = 0.0;
    y_vec_sum = 0.0;
    z_vec_sum = 0.0;
    (sys(idx).pos())[0] += ((sys(idx).veloc())[0] * time_interval);
    (sys(idx).pos())[1] += ((sys(idx).veloc())[1] * time_interval);
    (sys(idx).pos())[2] += ((sys(idx).veloc())[2] * time_interval);
    for (size_t vec = 0; vec < sys.size(); ++vec) {
      x_vec_sum += (vec_table[idx * sys.size() + vec])[0];
      y_vec_sum += (vec_table[idx * sys.size() + vec])[1];
      z_vec_sum += (vec_table[idx * sys.size() + vec])[2];
    }
    //(sys(idx).pos())[0] += (x_vec_sum * time_interval);
    //(sys(idx).pos())[1] += (y_vec_sum * time_interval);
    (sys(idx).veloc())[0] = x_vec_sum;
    (sys(idx).veloc())[1] = y_vec_sum;
    (sys(idx).veloc())[2] = z_vec_sum;
  }

  // delete[] vec_table;
  return;
}

SolarSys RK4Step(SolarSys sys_rep, size_t round) {
  const size_t system_size = sys_rep.size();
  long double time_in;
  if (2 == round) {
    time_in = time_interval;
  } else {
    time_in = time_interval / 2;
  }
  long double delta_x = 0.0;
  long double delta_y = 0.0;
  // auto vec_table =
  //    new std::pair<long double, long double>[system_size * system_size];
  // auto acc_table =
  //    new std::pair<long double, long double>[system_size * system_size];
  auto vec_table = std::make_unique<std::pair<long double, long double>[]>( system_size * system_size);
  auto acc_table = std::make_unique<std::pair<long double, long double>[]>(
      system_size * system_size);

  for (size_t x = 0; x < system_size; ++x) {
    for (size_t y = 0; y < system_size; ++y) {
      const size_t index = x * system_size + y;
      if (x == y) {
        (vec_table[index]).first = (sys_rep(x).veloc())[0];
        (vec_table[index]).second = (sys_rep(x).veloc())[1];
        (acc_table[index]).first = 0.0;
        (acc_table[index]).second = 0.0;
        continue;
      }

      delta_x = (sys_rep(y).pos())[0] - (sys_rep(x).pos())[0];
      delta_y = (sys_rep(y).pos())[1] - (sys_rep(x).pos())[1];
      long double r = delta_x * delta_x + delta_y * delta_y;
      (acc_table[index]).first =
          (G * (sys_rep(y).mass()) / r * (delta_x / std::sqrt(r)));
      (acc_table[index]).second =
          (G * (sys_rep(y).mass()) / r * (delta_y / std::sqrt(r)));
      (vec_table[index]).first = (acc_table[index]).first * time_in;
      (vec_table[index]).second = (acc_table[index]).second * time_in;
    }
  }

  long double x_vec_sum, y_vec_sum, x_acc_sum, y_acc_sum;
  for (size_t idx = 0; idx < system_size; ++idx) {
    x_vec_sum = 0.0;
    y_vec_sum = 0.0;
    x_acc_sum = 0.0;
    y_acc_sum = 0.0;
    (sys_rep(idx).pos())[0] += ((sys_rep(idx).veloc())[0] * (time_in));
    (sys_rep(idx).pos())[1] += ((sys_rep(idx).veloc())[1] * (time_in));
    for (size_t vec = 0; vec < system_size; ++vec) {
      const size_t index = idx * system_size + vec;
      x_vec_sum += (vec_table[index]).first;
      y_vec_sum += (vec_table[index]).second;
      x_acc_sum += (acc_table[index]).first;
      y_acc_sum += (acc_table[index]).second;
    }
    (sys_rep(idx).veloc())[0] = x_vec_sum;
    (sys_rep(idx).veloc())[1] = y_vec_sum;
    (sys_rep(idx).acc())[0] = x_acc_sum;
    (sys_rep(idx).acc())[1] = y_acc_sum;
  }

  // delete [] vec_table;
  // delete [] acc_table;
  return sys_rep;
}

SolarSys RK4Step_sub(SolarSys sys_rep, SolarSys base, size_t type){
  const size_t system_size = sys_rep.size();
  long double time_in;
  if(1==type){
    time_in = time_interval/2;
  }else{
    time_in = time_interval;
  }


  for(size_t x =0 ;x<system_size;++x){
    sys_rep(x).pos()[0] = base(x).pos()[0] + sys_rep(x).veloc()[0] * time_in;
    sys_rep(x).pos()[1] = base(x).pos()[1] + sys_rep(x).veloc()[1] * time_in;
    sys_rep(x).pos()[2] = base(x).pos()[2] + sys_rep(x).veloc()[2] * time_in;
  }
  long double delta_x = 0.0;
  long double delta_y = 0.0;
  long double delta_z = 0.0;

  auto acc_table = std::make_unique<std::vector<long double>[]>(
      system_size * system_size);

//#pragma omp parallel
//  {
//#pragma omp parallel for private(delta_x, delta_y) collapse(2)
  for (size_t x = 0; x < system_size; ++x) {
    for (size_t y = 0; y < system_size; ++y) {
      const size_t index = x * system_size + y;
      if (x == y) {
        (acc_table[index]).push_back(0.0);
        (acc_table[index]).push_back(0.0);
        (acc_table[index]).push_back(0.0);
        continue;
      }

      delta_x = (sys_rep(y).pos())[0] - (sys_rep(x).pos())[0];
      delta_y = (sys_rep(y).pos())[1] - (sys_rep(x).pos())[1];
      delta_z = (sys_rep(y).pos())[2] - (sys_rep(x).pos())[2];
      long double r = delta_x * delta_x + delta_y * delta_y + delta_z * delta_z;
      (acc_table[index]).push_back(
          (G * (sys_rep(y).mass()) / r * (delta_x / std::sqrt(r))));
      (acc_table[index]).push_back(
          (G * (sys_rep(y).mass()) / r * (delta_y / std::sqrt(r))));
      (acc_table[index]).push_back(
          (G * (sys_rep(y).mass()) / r * (delta_z / std::sqrt(r))));
    }
  }
//#pragma omp for
  //compute kx, ky
  for(size_t x = 0; x < system_size; ++x){
    sys_rep(x).veloc()[0] = base(x).veloc()[0] + sys_rep(x).acc()[0] * time_in;
    sys_rep(x).veloc()[1] = base(x).veloc()[1] + sys_rep(x).acc()[1] * time_in;
    sys_rep(x).veloc()[2] = base(x).veloc()[2] + sys_rep(x).acc()[2] * time_in;
  }

  long double x_acc_sum, y_acc_sum, z_acc_sum;
//#pragma omp for private(x_acc_sum, y_acc_sum)
  for (size_t idx = 0; idx < system_size; ++idx) {
    x_acc_sum = 0.0;
    y_acc_sum = 0.0;
    z_acc_sum = 0.0;
    for (size_t vec = 0; vec < system_size; ++vec) {
      const size_t index = idx * system_size + vec;
      x_acc_sum += (acc_table[index])[0];
      y_acc_sum += (acc_table[index])[1];
      z_acc_sum += (acc_table[index])[2];
    }
    (sys_rep(idx).acc())[0] = x_acc_sum;
    (sys_rep(idx).acc())[1] = y_acc_sum;
    (sys_rep(idx).acc())[2] = z_acc_sum;
  }
//}

  return sys_rep;
}

void ODE::RungeKutta(SolarSys &sys) { 
  // RK4 parameters
  //std::cout.precision(17);
  //SolarSys k0 = sys;                // veloc k1
  //SolarSys k1 = RK4Step(k0, 1);     // acc k1, veloc k2
  //SolarSys acc_k2 = RK4Step(k1, 1); // acc k2
  //SolarSys temp_k1(k1);
  //for (size_t i = 0; i < sys.size(); ++i) {
  //  (temp_k1(i).pos())[0] = (k0(i).pos())[0];
  //  (temp_k1(i).pos())[1] = (k0(i).pos())[1];
  //}
  //SolarSys k2 = RK4Step(temp_k1, 1); // veloc k3
  //SolarSys acc_k3 = RK4Step(k2, 1);  // acc k3
  //SolarSys temp_k2(k2);
  //for (size_t i = 0; i < sys.size(); ++i) {
  //  (temp_k2(i).pos())[0] = (k0(i).pos())[0];
  //  (temp_k2(i).pos())[1] = (k0(i).pos())[1];
  //}
  //SolarSys k3 = RK4Step(temp_k2, 2); // veloc k4
  //SolarSys k4 = RK4Step(k3, 1);      // acc k4

  //for (size_t i = 0; i < sys.size(); ++i) {
  //  (sys(i).pos())[0] += ((k0(i).veloc())[0] + 2 * (k1(i).veloc())[0] +
  //                        2 * (k2(i).veloc())[0] + (k3(i).veloc())[0]) *
  //                       time_interval / 6.0;
  //  (sys(i).pos())[1] += ((k0(i).veloc())[1] + 2 * (k1(i).veloc())[1] +
  //                        2 * (k2(i).veloc())[1] + (k3(i).veloc())[1]) *
  //                       time_interval / 6.0;
  //  (sys(i).veloc())[0] += ((k1(i).acc())[0] + 2 * (acc_k2(i).acc())[0] +
  //                          2 * (acc_k3(i).acc())[0] + (k4(i).acc())[0]) *
  //                         time_interval / 6.0;
  //  (sys(i).veloc())[1] += ((k1(i).acc())[1] + 2 * (acc_k2(i).acc())[1] +
  //                          2 * (acc_k3(i).acc())[1] + (k4(i).acc())[1]) *
  //                         time_interval / 6.0;
  //}

  const size_t system_size = sys.size();
  long double delta_x = 0.0;
  long double delta_y = 0.0;
  long double delta_z = 0.0;

  auto acc_table = std::make_unique<std::vector<long double>[]>(
      system_size * system_size);

  for (size_t x = 0; x < system_size; ++x) {
    for (size_t y = 0; y < system_size; ++y) {
      const size_t index = x * system_size + y;
      if (x == y) {
        (acc_table[index]).push_back(0.0);
        (acc_table[index]).push_back(0.0);
        (acc_table[index]).push_back(0.0);
        continue;
      }

      delta_x = (sys(y).pos())[0] - (sys(x).pos())[0];
      delta_y = (sys(y).pos())[1] - (sys(x).pos())[1];
      delta_z = (sys(y).pos())[2] - (sys(x).pos())[2];
      long double r = delta_x * delta_x + delta_y * delta_y + delta_z * delta_z;
      (acc_table[index]).push_back(
          (G * (sys(y).mass()) / r * (delta_x / std::sqrt(r))));
      (acc_table[index]).push_back(
          (G * (sys(y).mass()) / r * (delta_y / std::sqrt(r))));
      (acc_table[index]).push_back(
          (G * (sys(y).mass()) / r * (delta_z / std::sqrt(r))));
    }
  }
  long double x_acc_sum, y_acc_sum, z_acc_sum;
  for (size_t idx = 0; idx < system_size; ++idx) {
    x_acc_sum = 0.0;
    y_acc_sum = 0.0;
    z_acc_sum = 0.0;
    for (size_t vec = 0; vec < system_size; ++vec) {
      const size_t index = idx * system_size + vec;
      x_acc_sum += (acc_table[index])[0];
      y_acc_sum += (acc_table[index])[1];
      z_acc_sum += (acc_table[index])[2];
    }
    (sys(idx).acc())[0] = x_acc_sum;
    (sys(idx).acc())[1] = y_acc_sum;
    (sys(idx).acc())[2] = z_acc_sum;
  }
  std::cout.precision(17);

  SolarSys k1 = sys; //k1x, k1y, k1vx, k1vy
  SolarSys k2 = RK4Step_sub(k1,sys,1); //k2x, k2y, k2vx, k2vy
  SolarSys k3 = RK4Step_sub(k2,sys,1); //k3x, k3y, k3vy, k3vy
  SolarSys k4 = RK4Step_sub(k3,sys,2); //k4x, k4y, k4vy, k4vy

//#pragma omp parallel for
  for (size_t i = 0; i < sys.size(); ++i) {
    (sys(i).pos())[0] += ((k1(i).veloc())[0] + 2 * (k2(i).veloc())[0] +
                          2 * (k3(i).veloc())[0] + (k4(i).veloc())[0]) *
                         time_interval / 6.0;
    (sys(i).pos())[1] += ((k1(i).veloc())[1] + 2 * (k2(i).veloc())[1] +
                          2 * (k3(i).veloc())[1] + (k4(i).veloc())[1]) *
                         time_interval / 6.0;
    (sys(i).pos())[2] += ((k1(i).veloc())[2] + 2 * (k2(i).veloc())[2] +
                          2 * (k3(i).veloc())[2] + (k4(i).veloc())[2]) *
                         time_interval / 6.0;
    (sys(i).veloc())[0] += ((k1(i).acc())[0] + 2 * (k2(i).acc())[0] +
                            2 * (k3(i).acc())[0] + (k4(i).acc())[0]) *
                           time_interval / 6.0;
    (sys(i).veloc())[1] += ((k1(i).acc())[1] + 2 * (k2(i).acc())[1] +
                            2 * (k3(i).acc())[1] + (k4(i).acc())[1]) *
                           time_interval / 6.0;
    (sys(i).veloc())[2] += ((k1(i).acc())[2] + 2 * (k2(i).acc())[2] +
                            2 * (k3(i).acc())[2] + (k4(i).acc())[2]) *
                           time_interval / 6.0;
  }
}

