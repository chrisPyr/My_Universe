#ifndef PLANET_HPP
#define PLANET_HPP

#include <cstddef>
#include <vector>
#include <string>

class Planet {
using vec_dou = std::vector<double>;
private:
  std::string m_name;
  double m_mass;

  vec_dou m_pos;
  vec_dou m_veloc; // velocity along x, y axis
  vec_dou m_acc;   // accelerate along x, y axis
public:
  Planet(std::string name, double mass, vec_dou pos, vec_dou veloc, vec_dou acc)
      : m_name{name}, m_mass{mass}, m_pos{pos}, m_veloc{veloc}, m_acc{acc} {}
  ~Planet() = default;
  std::string name(){  return m_name;};
  double mass() {return  m_mass;};
  std::vector<double> pos() {return m_pos;};
  std::vector<double> veloc() {return m_veloc;};
  std::vector<double> acc() {return m_acc;};
};
#endif
