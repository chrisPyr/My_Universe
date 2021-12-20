#ifndef ODE_HPP
#define ODE_HPP

#include "SolarSys.hpp"
class ODE {
private:
  void Euler(SolarSys &sys);
  void RungeKutta(SolarSys &sys);

public:
  void update(SolarSys &sys, long long itr, size_t method);
};
#endif
