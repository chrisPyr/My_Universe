#ifndef PLANET_HPP
#define PLANET_HPP

#include <cstddef>
#include <string>
#include <vector>

class Planet {
  using vec_dou = std::vector<long double>;

private:
  std::string m_name;
  long double m_mass;
  vec_dou m_pos;
  vec_dou m_veloc; // velocity along x, y axis
  vec_dou m_acc;   // accelerate along x, y axis

public:
  Planet(std::string name, double mass, vec_dou pos, vec_dou veloc, vec_dou acc)
      : m_name{name}, m_mass{mass}, m_pos{pos}, m_veloc{veloc}, m_acc{acc} {}
  Planet(Planet const &other)
      : m_name{other.m_name}, m_mass{other.m_mass}, m_pos{other.m_pos},
        m_veloc{other.m_veloc}, m_acc{other.m_acc} {}
  Planet &operator=(Planet const &other) {
    if (&other != this) {
      m_name = other.m_name;
      m_mass = other.m_mass;
      m_pos = other.m_pos;
      m_veloc = other.m_veloc;
      m_acc = other.m_acc;
    }
    return *this;
  }
  ~Planet() = default;
  std::string name() { return m_name; };
  long double mass() { return m_mass; };
  std::vector<long double> &pos() { return m_pos; };
  std::vector<long double> &veloc() { return m_veloc; };
  std::vector<long double> &acc() { return m_acc; };
};
#endif
