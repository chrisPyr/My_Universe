#!/usr/bin/env/ python3
import _system
import time
from datetime import datetime, timedelta
import matplotlib.pyplot as plt
import numpy as np
from matplotlib.animation import FuncAnimation

color = ["red","tomato","gold","blue","gray","orange","chocolate","peru","limegreen","navy"]
name_of_solar = ["Sun","Mercury","Venus","Earth","Moon", "Mars","Jupiter","Saturn","Uranus","Neptune"]

fig = plt.figure()
fig1 = fig.add_subplot(projection = "3d")
def init():
    x_lim = universe[0].pos()[0]
    y_lim = universe[0].pos()[1]
    z_lim = universe[0].pos()[2]
    fig1.set_xlim(x_lim - 0.5E9,x_lim + 0.5E9)
    fig1.set_ylim(y_lim - 0.5E9,y_lim + 0.5E9)
    fig1.set_zlim(z_lim - 0.5E9,z_lim + 0.5E9)
    return

def update(i):
    fig1.clear()
    init()
    system_size = universe.size()
    for k in range(universe.size()):
        fig1.scatter(vec_positions[0][system_size*i +k][0], vec_positions[0][system_size*i+k][1],vec_positions[0][system_size*i+k][2], color = color[k])
        fig1.text(vec_positions[0][system_size*i +k][0],vec_positions[0][system_size*i+k][1],vec_positions[0][system_size*i+k][2], name_of_solar[k] )
    return

def outputTXT():
    d0 = datetime(2021,10,11)
    system_size = universe.size()
    for k in range(system_size):
        file_name = "Orbit_Simu_"+universe[k].name()+"_"+method_type+".txt"
        with open(file_name,"w") as file:
            #wrtie name and mass
            file.write("Name: "+universe[k].name()+"\n"+"Mass:"+str(universe[k].mass())+"\n")
            file.write("*" * 70 + "\n")
            current_day = d0
            for i in range(int(int(time_sec)/86400)+1):
                pos = vec_positions[0][system_size*i +k]
                vec = vec_positions[1][system_size*i +k]
                file.write(str(current_day+timedelta(days=i))+'\n')
                file.write("X  = {:.10e} Y  = {:.10e} Z  = {:.10e}\n".format(pos[0], pos[1], pos[2]))
                file.write("VX = {:.10e} VY = {:.10e} VZ = {:.10e}\n".format(vec[0], vec[1], vec[2]))
            file.write("=" * 70+'\n')
    return

if __name__=="__main__":
    #init universe
    universe = _system.System()
    #init ODE solver
    ODEsolver = _system.ODE() # user interface
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
            universe.addPlanet(Sun)
            universe.addPlanet(Mercury)
            universe.addPlanet(Venus)
            universe.addPlanet(Earth)
            universe.addPlanet(Moon)
            universe.addPlanet(Mars)
            universe.addPlanet(Saturn)
            universe.addPlanet(Jupiter)
            universe.addPlanet(Uranus)
            universe.addPlanet(Neptune)
        elif choice == '2':
            print("Please enter the name of planet")
            name = input()
            print("Please enter the mass of planet")
            mass = input()
            print("Please enter the initial position(x, y) of planet")
            pos =[]
            pos_x = input()
            pos_y = input()
            pos.append(float(pos_x))
            pos.append(float(pos_y))
            print("Please enter the initial velocity(x, y) of planet")
            vec =[]
            vec_x = input()
            vec_y = input()
            vec.append(float(vec_x))
            vec.append(float(vec_y))
            user_planet = _system.Planet(name,float(mass),pos,vec,[0,0])
            universe.addPlanet(user_planet)

        elif choice == '3':
            print("Please enter the name of the planet")
            name = input()
            universe.rmPlanet(name)
        elif choice == '4':
            print("Please select method, 1 for Euler method, 2 for Runge Kutta")
            method = input()
            if method == '1':
                method_type = "Euler"
            else:
                method_type = "Runge_Kutta"
            print("Please enter how long you want to predict in terms of seconds")
            time_sec = input()
            start_time = time.time()
            vec_positions = ODEsolver.update(universe,int(time_sec),int(method))
            end_time = time.time()
            print("time elapsed : {}".format(end_time - start_time))
            print("solved")
            ani = FuncAnimation(fig, update, frames=int(int(time_sec)/86400)+1, init_func=init, interval = 1, repeat = False, save_count = 366)
            print("Start output gif")
            gif_name = "system_"+method_type+".gif"
            ani.save(gif_name, fps = 30)
            print("Start outputing txt file")
            outputTXT()
        elif choice == '5':
            exit()
        else:
            print("Please choose 1-5")



