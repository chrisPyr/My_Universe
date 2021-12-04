#ifndef ODE_HPP
#define ODE_HPP

#include "SolarSys.hpp"
class ODE {
public:
  void naiveUpdate(SolarSys &sys);
  void update(SolarSys &sys, size_t itr);
};
#endif
