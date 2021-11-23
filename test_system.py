import sys
import os
import unittest
import time

import _system

class SystemTest(unittest.TestCase):

    def test_create(self):

        planet1 = _system.Planet("chris",188.33,[122.2, 333],[123.2, 333],[124.2, 333])
        self.assertEqual("chris",planet1.name())
        self.assertEqual(188.33,planet1.mass())
        self.assertEqual([122.2, 333],planet1.pos())
        self.assertEqual([123.2, 333],planet1.veloc())
        self.assertEqual([124.2, 333],planet1.acc())

        solarsystem = _system.System()
        solarsystem.addPlanet(planet1)
        self.assertEqual("chris", solarsystem[0].name())
        self.assertEqual(188.33,solarsystem[0].mass())
        self.assertEqual([122.2, 333],solarsystem[0].pos())
        self.assertEqual([123.2, 333],solarsystem[0].veloc())
        self.assertEqual([124.2, 333],solarsystem[0].acc())

    def test_del(self):
        planet1 = _system.Planet("chris",188.33,[122.2, 333],[123.2, 333],[124.2, 333])
        solarsystem = _system.System()
        solarsystem.addPlanet(planet1)
        solarsystem.rmPlanet("chris")
        self.assertEqual(0,solarsystem.size())

    def test_ODE(self):
        planet1 = _system.Planet("chris",188.33,[122.2, 333],[123.2, 333],[124.2, 333])
        solarsystem = _system.System()
        solarsystem.addPlanet(planet1)
        ode_solver = _system.ODE()
        ode_solver.naiveUpdate(solarsystem)

