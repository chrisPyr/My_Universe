#ifndef PLANET_HPP
#define PLANET_HPP

#include <cstddef>
#include <vector>

class Planet {
  using std::vector<double> vec_dou private : double m_mass;
  std::vector<double> m_pos;
  std::vector<double> m_veloc; // velocity along x, y axis
  std::vector<double> m_acc;   // accelerate along x, y axis
public:
  Planet(double mass, vec_dou pos, vec_dou veloc, vec_dou acc)
      : m_mass{mass}, m_pos{pos}, m_veloc{veloc}, m_acc{acc} {}
};
#endif
