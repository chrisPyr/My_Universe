#include "ODE.hpp"
#include "SolarSys.hpp"
#include <cmath>
#include <iostream>
#include <utility>
#include <stdexcept>

const double G = 6.67408e-20;
const double time_interval = 1;

void ODE::update(SolarSys &sys, size_t itr, size_t method) {
  if(1 == method){
    for (size_t i = 0; i < itr; ++i) {
      Euler(sys);
    }
  }else if(2 == method){
    for(size_t i = 0; i < itr; ++i){
      RungeKutta(sys);
    }
  }else{
    throw "invalid method choice!!";
  }
}

void ODE::Euler(SolarSys &sys) {

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

SolarSys RK4Step(SolarSys sys_rep, size_t round){
  double time_in;
  if(2 == round){
    time_in = time_interval;
  }else{
    time_in = time_interval/2;
  }
  SolarSys temp_sys = sys_rep;
  double delta_x = 0.0;
  double delta_y = 0.0;
  std::pair<double, double> *vec_table =
      new std::pair<double, double>[sys_rep.size() * sys_rep.size()];
  std::pair<double, double> *acc_table =
      new std::pair<double, double>[sys_rep.size() * sys_rep.size()];
  for (size_t x = 0; x < sys_rep.size(); ++x) {
    for (size_t y = 0; y < sys_rep.size(); ++y) {
      if (x == y) {
        (vec_table[x * sys_rep.size() + y]).first = (sys_rep(x).veloc())[0];
        (vec_table[x * sys_rep.size() + y]).second = (sys_rep(x).veloc())[1];
        (acc_table[x * sys_rep.size() + y]).first = 0.0;
        (acc_table[x * sys_rep.size() + y]).second = 0.0;
        continue;
      }

      delta_x = (sys_rep(y).pos())[0] - (sys_rep(x).pos())[0];
      delta_y = (sys_rep(y).pos())[1] - (sys_rep(x).pos())[1];
      double r = delta_x * delta_x + delta_y * delta_y;
      (acc_table[x * sys_rep.size() + y]).first =
          (G * (sys_rep(y).mass()) / r * (delta_x / std::sqrt(r)));
      (acc_table[x * sys_rep.size() + y]).second =
          (G * (sys_rep(y).mass()) / r * (delta_y / std::sqrt(r)));
      (vec_table[x * sys_rep.size() + y]).first =
          (acc_table[x * sys_rep.size() + y]).first * time_in;
      (vec_table[x * sys_rep.size() + y]).second =
          (acc_table[x * sys_rep.size() + y]).second * time_in;
    }
  }

  double x_vec_sum = 0.0;
  double y_vec_sum = 0.0;
  double x_acc_sum = 0.0;
  double y_acc_sum = 0.0;
  for (size_t idx = 0; idx < sys_rep.size(); ++idx) {
    x_vec_sum = 0.0;
    y_vec_sum = 0.0;
    x_acc_sum = 0.0;
    y_acc_sum = 0.0;
    (sys_rep(idx).pos())[0] += ((sys_rep(idx).veloc())[0] * (time_in));
    (sys_rep(idx).pos())[1] += ((sys_rep(idx).veloc())[1] * (time_in));
    for (size_t vec = 0; vec < sys_rep.size(); ++vec) {
      x_vec_sum += (vec_table[idx * sys_rep.size() + vec]).first;
      y_vec_sum += (vec_table[idx * sys_rep.size() + vec]).second;
      x_acc_sum += (acc_table[idx * sys_rep.size() + vec]).first;
      y_acc_sum += (acc_table[idx * sys_rep.size() + vec]).second;
    }
    (sys_rep(idx).veloc())[0] = x_vec_sum;
    (sys_rep(idx).veloc())[1] = y_vec_sum;
    (sys_rep(idx).acc())[0] = x_acc_sum;
    (sys_rep(idx).acc())[1] = y_acc_sum;
  }
  delete [] vec_table;
  delete [] acc_table;
  return sys_rep;

}

void ODE::RungeKutta(SolarSys &sys){

  //RK4 parameters
  SolarSys k0 = sys; //veloc k1
  SolarSys k1 = RK4Step(k0,1); //acc k1, veloc k2
  SolarSys acc_k2 = RK4Step(k1,1); //acc k2
  SolarSys temp_k1(k1);
  for(size_t i =0; i<sys.size();++i){
    (temp_k1(i).pos())[0] = (k0(i).pos())[0];
    (temp_k1(i).pos())[1] = (k0(i).pos())[1];
  }
  SolarSys k2 = RK4Step(temp_k1,1); //acc k2, veloc k3
  SolarSys acc_k3 = RK4Step(k2,1); //acc k3
  SolarSys temp_k2(k2);
  for(size_t i =0; i<sys.size();++i){
    (temp_k2(i).pos())[0] = (k0(i).pos())[0];
    (temp_k2(i).pos())[1] = (k0(i).pos())[1];
  }
  SolarSys k3 = RK4Step(temp_k2,2); //veloc k4
  SolarSys k4 = RK4Step(k3,1); //acc k4

  for(size_t i = 0; i<sys.size(); ++i){
    (sys(i).pos())[0]+=((k0(i).veloc())[0] +2*(k1(i).veloc())[0] + 2*(k2(i).veloc())[0] + (k3(i).veloc())[0])/6;
    (sys(i).pos())[1]+=((k0(i).veloc())[1] +2*(k1(i).veloc())[1] + 2*(k2(i).veloc())[1] + (k3(i).veloc())[1])/6;
    (sys(i).veloc())[0]+=((k1(i).acc())[0] +2*(acc_k2(i).acc())[0] + 2*(acc_k3(i).acc())[0] + (k4(i).acc())[0])/6;
    (sys(i).veloc())[1]+=((k1(i).acc())[1] +2*(acc_k2(i).acc())[1] + 2*(acc_k3(i).acc())[1] + (k4(i).acc())[1])/6;
  }
}
