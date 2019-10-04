import matplotlib.pyplot as plt
import numpy as np
import sys
from PyQt4 import QtGui, QtSvg
import math
from matplotlib.animation import FuncAnimation
import matplotlib.pyplot as plt
import matplotlib.animation as animation

import math
import numpy as np


path ="./particleInfo_2_1_omp.dat"

step, time, id, _x, _y, _z, theta_x, theta_y, theta_z, v_x,  v_y, v_z, w_x, w_y, w_z = np.loadtxt(path, unpack=True)
width = 200
height = 200
deltastep=1000

#print(theta_z[1:100])

print (len(_x))
#nFrame = 100

ray = 0.0005
new_x1 = []
new_y1 = []
new_theta_z1 = []

new_x2 = []
new_y2 = []
new_theta_z2 = []

for j in range(0, len(id)):
    if (id[j] == 1):
        # print (id[j])
        new_x1.append(_x[j])
        new_y1.append(_y[j] - 0.0005)
        new_theta_z1.append(theta_z[j])

    if (id[j] == 2):


        # print (id[j])
        new_x2.append(_x[j])
        new_y2.append(_y[j] - 0.0005)
        new_theta_z2.append(theta_z[j])


nFrame = (int) (len(_x) / deltastep)

nFrame = (int) (nFrame /2)
print (nFrame)


def update(i, ax, plt):
    plt.clf()
    plt.cla()
    ax = plt.axes(xlim=(0.015, 0.04), ylim=(0.01, 0.04 - 0.005))

    b1 = (ray - 0.0001) * math.cos(new_theta_z1[i * deltastep])
    a1 = b1 * math.tan(new_theta_z1[i * deltastep])

    # print(w_x[i], w_y[i], w_z[i])
    ax.add_artist(plt.Circle((new_x1[i * deltastep], new_y1[i * deltastep] ), ray, fill=False, color='k', linewidth=0.5))
    ax.arrow(new_x1[i * deltastep], new_y1[i * deltastep] , b1, a1, width=0.00003, linewidth=0.0003,
                    head_width=0.00015, head_length=0.0001,
                    fc='k', ec='k')

    b2 = (ray - 0.0001) * math.cos(new_theta_z2[i * deltastep])
    a2 = b2 * math.tan(new_theta_z2[i * deltastep])

    ax.add_artist(plt.Circle((new_x2[i * deltastep], new_y2[i * deltastep] ), ray, fill=False, color='k', linewidth=0.5))
    ax.arrow(new_x2[i * deltastep], new_y2[i * deltastep] , b2, a2, width=0.00003, linewidth=0.0003,
                    head_width=0.00015, head_length=0.0001,
                    fc='k', ec='k')


def creategif():
    fig = plt.figure()
    ax = plt.axes(xlim=(0.015, 0.04), ylim=(0.01, 0.04 - 0.005))
    a0, = ax.plot([], [])
    anim = animation.FuncAnimation(fig, update, frames=nFrame,
                                   fargs=(ax, plt,),
                                   interval=5)

    anim.save('coaxial_coll1.gif', writer='imagemagick')
    # show plot
    plt.show()



def createsubplot():
    # fig= plt.figure()

    deltastep = 47000

    deltasteps = [0, 10000, 12000, 15000, 17000, 30000]


    fig, axes = plt.subplots(3, 2, figsize=((0.025 - 0.015) * 1000, (0.025 - 0.015) * 1000))

    fig.set_size_inches((0.025 - 0.015) * 750, (0.025 - 0.015) * 1000)

    index = 0
    for i in range(0, 3):
        for j in range(0, 2):

            b1 = (ray - 0.0001) * math.cos(new_theta_z1[deltasteps[index]])
            a1 = b1 * math.tan(new_theta_z1[deltasteps[index]])

            # print(w_x[i], w_y[i], w_z[i])
            axes[i, j].add_artist(
                plt.Circle((new_x1[deltasteps[index]], new_y1[deltasteps[index]] ), ray, fill=False, color='k',
                           linewidth=0.5))
            axes[i, j].arrow(new_x1[deltasteps[index]], new_y1[deltasteps[index]] , b1, a1, width=0.00003, linewidth=0.0003,
                     head_width=0.00015, head_length=0.0001,
                     fc='k', ec='k')

            b2 = (ray - 0.0001) * math.cos(new_theta_z2[deltasteps[index]])
            a2 = b2 * math.tan(new_theta_z2[deltasteps[index]])

            axes[i, j].add_artist(
                plt.Circle((new_x2[deltasteps[index]], new_y2[deltasteps[index]] ), ray, fill=False, color='k',
                           linewidth=0.5))
            axes[i, j].arrow(new_x2[deltasteps[index]], new_y2[deltasteps[index]] , b2, a2, width=0.00003, linewidth=0.0003,
                     head_width=0.00015, head_length=0.0001,
                     fc='k', ec='k')




            axes[i, j].set_xlim(0.018, 0.035)
            axes[i, j].set_ylim(0.013, 0.035 - 0.005)
            axes[i, j].set_title(str(time[deltasteps[index]]) + " s", y=0.83, fontsize=10)
            axes[i, j].set_xlabel("x")
            axes[i, j].set_ylabel("y", rotation=0, labelpad=10)

            axes[i, j].set_xticks([])
            axes[i, j].set_yticks([])

            # if i == 1 and j == 1 or i == 2 and j == 1 or i == 2 and j == 0:
            #     axes[i, j].plot(_x[deltasteps[2]:deltasteps[index]], _z[deltasteps[2]:deltasteps[index]] - 0.0005,
            #                     'b--', linewidth=0.8)
            # else:
            axes[i, j].plot(new_x1[0:deltasteps[index]], new_y1[0:deltasteps[index]] , 'b--', linewidth=0.7)
            axes[i, j].plot(new_x2[0:deltasteps[index]], new_y2[0:deltasteps[index]] , 'r--', linewidth=0.7)
            index += 1


    plt.savefig("coaxial_coll.svg", format="svg")
    plt.savefig("coaxial_coll.png")
    plt.show()


creategif()
#creategif()
# fig = plt.figure()
# ax = plt.axes()
# ax.set_xlim([0.015, 0.024])
# ax.set_ylim([0.0, 0.024 - 0.015])
#
#
#
#
# for i in range(1, 30000, 700):
#     # ax = plt.axes()
#     # ax.set_xlim([0.015, 0.024])
#     # ax.set_ylim([0.0, 0.024-0.015])
#     b = (0.0005 -0.0001) * math.cos(theta_y[i])
#     a = b * math.tan(theta_y[i])
#
#     #print(w_x[i], w_y[i], w_z[i])
#     ax.add_artist(plt.Circle((_x[i], _z[i] - 0.0005), 0.0005,  fill=False, color='k', linewidth=0.5))
#     ax.arrow(_x[i], _z[i] - 0.0005, b  , a, width = 0.00003,linewidth= 0.0003,head_width=0.00015, head_length=0.0001, fc='k', ec='k')
#     #plt.show()
#
#
#
#
# plt.savefig("biliardball.svg", format="svg")
#
# app = QtGui.QApplication(sys.argv)
# svgWidget = QtSvg.QSvgWidget('superball.svg')
# svgWidget.setGeometry(100,100,width*4,height*4)
# svgWidget.show()
#
# sys.exit(app.exec_())



