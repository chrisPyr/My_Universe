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

    def test_ODE_Earth_Sun(self):
        Sun = _system.Planet("Sun",1.989e30,[-1.236499415040757E+06,6.025375268375650E+05 ],[-7.542015186487071E-03,-1.403959979998188E-02],[0, 0])
        Earth = _system.Planet("Earth",5.972e24, [1.411307100326878E+08,4.576889747036675E+07], [-9.511592529337820E+00,2.827120433963588E+01], [0,0])
        Mars = _system.Planet("Mars", 6.4171e23, [-2.350376219138694E+08,-6.689900019557889E+07], [7.614516774239974E+00,-2.122271557472923E+01], [0,0])
        Mercury = _system.Planet("Mercury", 3.302e23, [4.415286198775838E+07,2.022626858016598E+07],[-2.881504773059919E+01,4.685685655172608E+01 ], [0,0])
        Venus = _system.Planet("Venus", 48.685e23, [ 8.711143515075828E+07,-6.282813161880022E+07 ], [2.020064031332229E+01,2.829443154025286E+01],[0,0])
        Jupiter = _system.Planet("Jupiter", 1.898e27, [6.579320580151826E+08, -3.563111248294134E+08], [6.063642398054531E+00, 1.210338130121029E+01], [0,1])
        Saturn = _system.Planet("Saturn", 5.6834e26, [9.939373926025440E+08,-1.103416794862963E+09], [6.637947450410307E+00,6.443751326726574E+00],[0,0])
        Uranus = _system.Planet("Uranus", 86.813e24, [2.185632308223678E+09,1.983667767025198E+09 ], [-4.626811360457690E+00,4.725512851127284E+00 ],[0,0])
        Neptune = _system.Planet("Neptune", 102.409e24, [4.426617174374252E+09,-6.498004322596612E+08 ], [7.533906069327845E-01,5.410577196004814E+00 ], [0,0])
        solarsystem = _system.System()
        solarsystem.addPlanet(Sun)
        solarsystem.addPlanet(Earth)
        solarsystem.addPlanet(Mars)
        solarsystem.addPlanet(Mercury)
        solarsystem.addPlanet(Venus)
        solarsystem.addPlanet(Saturn)
        solarsystem.addPlanet(Jupiter)
        solarsystem.addPlanet(Uranus)
        solarsystem.addPlanet(Neptune)
        ode_solver = _system.ODE()
        ode_solver.update(solarsystem, 86400)
        print(solarsystem[0].pos())
        print(solarsystem[1].pos())

