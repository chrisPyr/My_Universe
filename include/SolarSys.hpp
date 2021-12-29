#ifndef SOLARSYS_HPP
#define SOLARSYS_HPP

#include "Planet.hpp"
#include <cstddef>
#include <stdexcept>
#include <string>
#include <vector>

class SolarSys {
private:
  using vec_planet = std::vector<Planet>;
  vec_planet m_planets; // all planets in this system

public:
  /* Remember rule of five!!*/
  /* constructor*/
  SolarSys() = default;
  SolarSys(vec_planet initPlanets) : m_planets{initPlanets} {};
  /* copy constructor*/
  SolarSys(SolarSys const &other) : m_planets{other.m_planets} {};
  /* copy assignment constructor*/
  SolarSys &operator=(SolarSys const &other) {
    if (&other != this) {
      m_planets.clear();
      m_planets = other.m_planets;
    }
    return *this;
  };
  /* move constructor */
  SolarSys(SolarSys &&other) : m_planets{std::move(other.m_planets)} {};
  /* move assignment constructor*/
  SolarSys &operator=(SolarSys &&other) {
    if (&other != this) {
      m_planets.clear();
      m_planets = other.m_planets;
    }
    return *this;
  }
  ~SolarSys() = default;
  void addPlanet(Planet newPlanet);
  void rmPlanet(std::string name);
  void showUniverse();
  void updatePos();
  size_t size() { return m_planets.size(); };
  Planet &operator()(size_t idx) {
    if (idx + 1 > m_planets.size()) {
      throw std::invalid_argument("out of bound");
    }
    return m_planets[idx];
  };
};
#endif
