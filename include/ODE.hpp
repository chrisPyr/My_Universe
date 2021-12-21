#ifndef ODE_HPP
#define ODE_HPP

#include "SolarSys.hpp"
#include <vector>
class ODE {
private:
  void Euler(SolarSys &sys);
  void RungeKutta(SolarSys &sys);

public:
  std::vector<std::vector<long double> > update(SolarSys &sys, long long itr, size_t method);
};
#endif
