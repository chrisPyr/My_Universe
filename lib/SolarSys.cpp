#include "SolarSys.hpp"
#include "ODE.hpp"
#include "Planet.hpp"
#include <iostream>
#include <pybind11/operators.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <string>

void SolarSys::addPlanet(Planet newPlanet) { m_planets.push_back(newPlanet); };

void SolarSys::rmPlanet(std::string name) {
  /* be careful when erase in for-loop*/
  /* This is a wrong version */
  // for(auto itr:m_planets){
  //  m_planets.erase(&itr);
  //}
  for (auto itr = m_planets.begin(); itr != m_planets.end();) {
    if ((itr->name()) == name) {
      itr = m_planets.erase(itr);
    } else {
      ++itr;
    }
  }
  return;
};

void SolarSys::showUniverse() {
  std::cout << std::endl;
  if (m_planets.size() == 0) {
    std::cout << "There's no planet in this universe\n";
  } else {
    for (auto itr : m_planets) {
      std::cout << itr.name() << std::endl;
    }
  }
}

void SolarSys::updatePos() { return; }

namespace py = pybind11;

PYBIND11_MODULE(_system, m) {
  py::class_<Planet>(m, "Planet")
      .def(py::init<std::string, long double, std::vector<long double>,
                    std::vector<long double>, std::vector<long double>>())
      .def("name", &Planet::name)
      .def("mass", &Planet::mass)
      .def("veloc", &Planet::veloc)
      .def("acc", &Planet::acc)
      .def("pos", &Planet::pos);
  py::class_<SolarSys>(m, "System")
      .def(py::init<>())
      .def(py::init<std::vector<Planet>>())
      .def("addPlanet", &SolarSys::addPlanet)
      .def("rmPlanet", &SolarSys::rmPlanet)
      .def("size", &SolarSys::size)
      .def("showUniverse", &SolarSys::showUniverse)
      .def("updatePos", &SolarSys::updatePos)
      .def("__getitem__",
           [](SolarSys &sys, size_t index) { return sys(index); });
  py::class_<ODE>(m, "ODE").def(py::init<>()).def("update", &ODE::update);
}
