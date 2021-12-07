#!/usr/bin/env/ python3
import _system


if __name__=="__main__":
    #init universe
    universe = _system.System()
    #init ODE solver
    ODEsolver = _system.ODE()
    # user interface
    print("Welcome to Planet Orbit Prediction System!!")
    while 1:
        print("Please select one of the action:")
        print("""
                1. Add entire Solar System(Data are from 2021/10/11)
                2. Add one Planet to current system
                3. Remove a Planet from current system
                4. Start the simulation process
                5. Exit
              """)
        choice = input()
        if choice == '1':
            Sun = _system.Planet("Sun",1.989e30,[-1.236499415040757E+06,6.025375268375650E+05 ],[-7.542015186487071E-03,-1.403959979998188E-02],[0, 0])
            Earth = _system.Planet("Earth",5.972e24, [1.411307100326878E+08,4.576889747036675E+07], [-9.511592529337820E+00,2.827120433963588E+01], [0,0])
            Mars = _system.Planet("Mars", 6.4171e23, [-2.350376219138694E+08,-6.689900019557889E+07], [7.614516774239974E+00,-2.122271557472923E+01], [0,0])
            Mercury = _system.Planet("Mercury", 3.302e23, [4.415286198775838E+07,2.022626858016598E+07],[-2.881504773059919E+01,4.685685655172608E+01 ], [0,0])
            Venus = _system.Planet("Venus", 48.685e23, [ 8.711143515075828E+07,-6.282813161880022E+07 ], [2.020064031332229E+01,2.829443154025286E+01],[0,0])
            Jupiter = _system.Planet("Jupiter", 1.898e27, [6.579320580151826E+08, -3.563111248294134E+08], [6.063642398054531E+00, 1.210338130121029E+01], [0,1])
            Saturn = _system.Planet("Saturn", 5.6834e26, [9.939373926025440E+08,-1.103416794862963E+09], [6.637947450410307E+00,6.443751326726574E+00],[0,0])
            Uranus = _system.Planet("Uranus", 86.813e24, [2.185632308223678E+09,1.983667767025198E+09 ], [-4.626811360457690E+00,4.725512851127284E+00 ],[0,0])
            Neptune = _system.Planet("Neptune", 102.409e24, [4.426617174374252E+09,-6.498004322596612E+08 ], [7.533906069327845E-01,5.410577196004814E+00 ], [0,0])
            universe.addPlanet(Sun)
            universe.addPlanet(Earth)
            universe.addPlanet(Mars)
            universe.addPlanet(Mercury)
            universe.addPlanet(Venus)
            universe.addPlanet(Saturn)
            universe.addPlanet(Jupiter)
            universe.addPlanet(Uranus)
            universe.addPlanet(Neptune)
        elif choice == '4':
            print("Please select method, 1 for Euler method, 2 for Runge Kutta")
            method = input()
            print("Please enter how long you want to predict in terms of seconds")
            time_sec = input()
            ODEsolver.update(universe,int(time_sec),int(method))
        elif choice == '5':
            exit()
        else:
            print("Please choose 1-5")



