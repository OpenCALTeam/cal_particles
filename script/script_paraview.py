from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
import numpy as np
import sys
from itertools import product, combinations

steps= 1000
numberOfParicles= 2708
ray = 0.0005
np.set_printoptions(threshold=sys.maxsize)

path= "/run/user/1000/gvfs/sftp:host=160.97.63.93,user=parcuri/home/parcuri/nfshome/workspace/cal_particles/data/simulazione -1/particleInfo_-1_1_omp.dat"
#path ="/run/user/1000/gvfs/sftp:host=telesio,user=parcuri/home/parcuri/nfshome/workspace/cal_particles/data/simulazione -1/merged.dat"

step, time, id, _x, _y, _z, theta_x, theta_y, theta_z, v_x,  v_y, v_z, w_x, w_y, w_z = np.genfromtxt(path, max_rows=numberOfParicles*(1+steps), unpack=True)

print ("ho letto "+str(len(step))+ " righe")
start= 0
end = numberOfParicles
for i in range(0,steps):

    file = open("/run/user/1000/gvfs/sftp:host=160.97.63.93,user=parcuri/home/parcuri/nfshome/workspace/cal_particles/data/simulazione -1/steps/step"+str(i)+".vtk", "w")
    file.write("# vtk DataFile Version 2.0\nUnstructured grid legacy vtk file with point scalar data\nASCII\nDATASET UNSTRUCTURED_GRID\nPOINTS "+str(numberOfParicles)+ " double\n")
    start = numberOfParicles * i
    end = numberOfParicles * (i + 1)

    for j in range(start,end):
        file.write('%.9f %.9f %.9f \n' % (_x[j], _y[j], _z[j]))

    file.write("POINT_DATA "+str(numberOfParicles)+"\nSCALARS radii double\nLOOKUP_TABLE default\n")
    for j in range(start,end):
        file.write('%.5f\n'%ray )

    print (len(step[start:end]))

    file.close()



