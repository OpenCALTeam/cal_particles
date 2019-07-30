from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
import numpy as np
from itertools import product, combinations

# import the math module
import math

numberOfParticle=2

def omega_velocity( id, w_2, v_2, theta_2 ):
    w = []
    for i in range(0, len(w_2)):
        w.append(math.sqrt(w_2[i]))
    v = []
    for i in range(0, len(v_2)):
        v.append(math.sqrt(v_2[i]))

    theta = []
    for i in range(0, len(theta_2)):
        theta.append(math.sqrt(theta_2[i]))

    for i in range(1, numberOfParticle+1):

        new_theta = []
        new_time = []
        new_v = []
        new_w = []

        for j in range(0, len(id)):
            if (id[j] == i):
                new_theta.append(theta[j])
                new_time.append((time[j]))
                new_v.append(v[j])
                new_w.append(w[j])


        plt.subplot(3, 1, 1)
        plt.plot(new_time, new_w, 'm-')
        plt.title('omega p'+str(i))

        plt.subplot(3, 1, 2)
        plt.plot(new_time, new_v, 'b-')
        plt.title('vel p'+str(i))

        plt.subplot(3, 1, 3)
        plt.plot(new_time, new_theta, 'g-')
        plt.title('theta p' + str(i))
        plt.show()

def velocity( v_x, v_y, v_z ):

    for i in range(1, numberOfParticle+1):

        new_v_x = []
        new_time = []
        new_v_y = []
        new_v_z = []

        for j in range(0, len(id)):
            if (id[j] == i):
                new_v_x.append(v_x[j])
                new_time.append(time[j])
                new_v_y.append(v_y[j])
                new_v_z.append(v_z[j])

        plt.subplot(3, 1, 1)
        plt.plot(new_time, new_v_x, 'm-')
        plt.title('vel_x p'+str(i))

        plt.subplot(3, 1, 2)
        plt.plot(new_time, new_v_y, 'b-')
        plt.title('vel_y p'+str(i))

        plt.subplot(3, 1, 3)
        plt.plot(new_time, new_v_z, 'g-')
        plt.title('vel_z p' + str(i))
        plt.show()

def omega( w_x, w_y, w_z ):

    for i in range(1, numberOfParticle+1):

        new_w_x = []
        new_time = []
        new_w_y = []
        new_w_z = []

        for j in range(0, len(id)):
            if (id[j] == i):
                new_w_x.append(w_x[j])
                new_time.append(time[j])
                new_w_y.append(w_y[j])
                new_w_z.append(w_z[j])


        plt.subplot(3, 1, 1)
        plt.plot(new_time, new_w_x, 'm-')
        plt.title('w_x p'+str(i))

        plt.subplot(3, 1, 2)
        plt.plot(new_time, new_w_y, 'b-')
        plt.title('w_y p'+str(i))

        plt.subplot(3, 1, 3)
        plt.plot(new_time, new_w_z, 'g-')
        plt.title('w_z p' + str(i))
        plt.show()


def theta( theta_x, theta_y, theta_z ):

    for i in range(1, numberOfParticle + 1):

        new_theta_x = []
        new_time = []
        new_theta_y = []
        new_theta_z = []

        for j in range(0, len(id)):
            if (id[j] == i):
                new_theta_x.append(theta_x[j])
                new_time.append(time[j])
                new_theta_y.append(theta_y[j])
                new_theta_z.append(theta_z[j])

        plt.subplot(3, 1, 1)
        plt.plot(new_time, new_theta_x, 'm-')
        plt.title('theta_x p'+str(i))

        plt.subplot(3, 1, 2)
        plt.plot(new_time, new_theta_y, 'b-')
        plt.title('theta_y p'+str(i))

        plt.subplot(3, 1, 3)
        plt.plot(new_time, new_theta_z, 'g-')
        plt.title('theta_z p' + str(i))
        plt.show()



path ="/home/parcuri/Dropbox/Materie Varie (Personale)/Magistrale/DEM/workspace/cal_particles/data/particleInfo_10_1_omp.dat"
#path ="/home/parcuri/Dropbox/Materie Varie (Personale)/Magistrale/DEM/workspace/cal_particles/data/particle_info_superball.dat"
#path = "/home/parcuri/Documenti/DATA_TESI/particleInfo_7_1.dat"
step, time, id, _x, _y, _z, theta_x, theta_y, theta_z, v_x,  v_y, v_z, w_x, w_y, w_z = np.loadtxt(path, unpack=True)


v_2 = v_x**2+v_y**2+v_z**2
w_2 = w_x**2+w_y**2+w_z**2

theta_2 = theta_x**2 + theta_y**2+ theta_z**2

#omega_velocity(id, w_2, v_2, theta_2)

velocity(v_x, v_y, v_z)
omega(w_x, w_y, w_z)

theta(theta_x, theta_y, theta_z)



for i in range(1,numberOfParticle+1):

    fig = plt.figure()
    plt.subplot(4, 1, 1)

    new_x =[]
    new_y = []
    new_time = []
    new_v_2 = []
    new_w_2 = []

    for j in range(0,len(id)):
        if(id[j] == i ):
            #print (id[j])
            new_x.append(_x[j])
            new_y.append(_y[j])
            new_time.append((time[j]))
            new_v_2.append(v_2[j])
            new_w_2.append(w_2[j])


    plt.plot(new_x,new_y,'r-')
    plt.title('trajectory (x,y) p'+ str(i))
    fig.subplots_adjust(left= 0.15,hspace=1.0, wspace=0.4)

    # [::2]
    plt.subplot(4, 1, 2)
    plt.plot(new_time,new_w_2,'m-')
    plt.title('omega^2 p'+ str(i))

    plt.subplot(4, 1, 3)
    plt.plot(new_time,new_v_2,'b-')
    plt.title('vel^2 p'+ str(i))

    plt.subplot(4, 1, 4)
    plt.plot(new_time,new_x,'r-')
    plt.title('(elapsed time, x) p' + str(i))

    plt.show()




