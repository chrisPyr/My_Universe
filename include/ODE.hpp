#ifndef ODE_HPP
#define ODE_HPP

#include "SolarSys.hpp"
#include <vector>
#include <memory>
class ODE {
private:
  void Euler(SolarSys &sys, std::unique_ptr<std::vector<long double> > &vec_table);
  void RungeKutta(SolarSys &sys);

public:
  std::pair<std::vector<std::vector<long double> >,std::vector<std::vector<long double> > > update(SolarSys &sys, long long itr, size_t method);
};
#endif
