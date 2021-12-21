import sys
import os
import unittest
import time

import _system

class SystemTest(unittest.TestCase):

    def test_create(self):

        planet1 = _system.Planet("chris",188.33,[122.2, 333, 0],[123.2, 333, 0],[124.2, 333,0])
        self.assertEqual("chris",planet1.name())
        self.assertEqual(188.33,planet1.mass())
        self.assertEqual([122.2, 333,0],planet1.pos())
        self.assertEqual([123.2, 333,0],planet1.veloc())
        self.assertEqual([124.2, 333,0],planet1.acc())

        solarsystem = _system.System()
        solarsystem.addPlanet(planet1)
        self.assertEqual("chris", solarsystem[0].name())
        self.assertEqual(188.33,solarsystem[0].mass())
        self.assertEqual([122.2, 333,0],solarsystem[0].pos())
        self.assertEqual([123.2, 333,0],solarsystem[0].veloc())
        self.assertEqual([124.2, 333,0],solarsystem[0].acc())

    def test_del(self):
        planet1 = _system.Planet("chris",188.33,[122.2, 333,0],[123.2, 333,0],[124.2, 333,0])
        solarsystem = _system.System()
        solarsystem.addPlanet(planet1)
        solarsystem.rmPlanet("chris")
        self.assertEqual(0,solarsystem.size())

    def test_ODE_Earth_Sun(self):
        solarsystem = _system.System()
        Sun = _system.Planet("Sun",1.9885e30,[-1.236499415040757E+06,6.025375268375650E+05,2.397910269693015E+04],[-7.542015186487071E-03,-1.403959979998188E-02,2.893371660177392E-04],[0, 0,0])
        Mercury = _system.Planet("Mercury", 3.302e23, [4.415286198775838E+07,2.022626858016598E+07,-2.535867922431055E+06],[-2.881504773059919E+01,4.685685655172608E+01,6.472910517061404E+00 ], [0,0,0])
        Venus = _system.Planet("Venus", 48.685e23, [ 8.711143515075828E+07,-6.282813161880022E+07,-5.944677998897284E+06  ], [2.020064031332229E+01,2.829443154025286E+01,-7.773061165994584E-01],[0,0,0])
        Earth = _system.Planet("Earth",5.97219e24, [1.411307100326878E+08,4.576889747036675E+07,2.139201455830224E+04], [-9.511592529337820E+00,2.827120433963588E+01,9.056609748014921E-06], [0,0,0])
        Moon = _system.Planet("Moon", 7.349e22, [1.410633228773884E+08,4.540868585933193E+07,1.163627374554798E+04], [-8.469646392105485E+00,2.804815679241358E+01,-9.271979073803571E-02],[0,0,0])
        Mars = _system.Planet("Mars", 6.4171e23, [-2.350376219138694E+08,-6.689900019557889E+07,4.344403777444564E+06], [7.614516774239974E+00,-2.122271557472923E+01,-6.311609004978642E-01], [0,0,0])
        Jupiter = _system.Planet("Jupiter", 1.89818722e27, [6.579320644969507E+08,-3.563111492541205E+08,-1.324135219521098E+07], [6.063731542774859E+00,1.210325656944901E+01,-1.858641558465068E-01], [0,0,0])
        Saturn = _system.Planet("Saturn", 5.6834e26, [9.939374042448882E+08,-1.103416617098590E+09,-2.038638425098956E+07], [6.636742621025345E+00,6.444332174823212E+00,-3.761572957513724E-01],[0,0,0])
        Uranus = _system.Planet("Uranus", 86.813e24, [2.185632317815626E+09,1.983667763989631E+09,-2.094785240820575E+07 ], [-4.626778904354935E+00,4.725500261780994E+00,7.752831643121549E-02 ],[0,0,0])
        Neptune = _system.Planet("Neptune", 102.409e24, [4.426617237941353E+09,-6.498004061384301E+08,-8.863447761366978E+07 ], [7.533974997978995E-01,5.409900514160937E+00,-1.281455072120732E-01 ], [0,0,0])
        solarsystem.addPlanet(Sun)
        solarsystem.addPlanet(Mercury)
        solarsystem.addPlanet(Venus)
        solarsystem.addPlanet(Earth)
        solarsystem.addPlanet(Moon)
        solarsystem.addPlanet(Mars)
        solarsystem.addPlanet(Saturn)
        solarsystem.addPlanet(Jupiter)
        solarsystem.addPlanet(Uranus)
        solarsystem.addPlanet(Neptune)
        ode_solver = _system.ODE()
        ode_solver.update(solarsystem, 86400, 1)
        ode_solver.update(solarsystem, 86400, 2)

