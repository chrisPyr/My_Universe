#ifndef ODE_HPP
#define ODE_HPP

#include "SolarSys.hpp"
#include <vector>
#include <memory>

using namespace std;
class ODE {
private:
  void Euler(SolarSys &sys, unique_ptr<vector<long double> > &vec_table);
  void RungeKutta(SolarSys &sys, unique_ptr<vector<long double>> &acc_table);

public:
  pair<vector<vector<long double> >,vector<vector<long double> > > update(SolarSys &sys, long long itr, size_t method);
};
#endif
