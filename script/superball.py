import matplotlib.pyplot as plt
import numpy as np
import sys
from PyQt4 import QtGui, QtSvg
import math
from matplotlib.animation import FuncAnimation
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import matplotlib.image as image
import math
import numpy as np
from matplotlib.offsetbox import OffsetImage,AnchoredOffsetbox

import os
from PIL import Image

path ="./particleInfo_0_1_omp.dat"

width = 200
height = 200
deltastep=1000


nFrame = 440


step, time, id, _x, _y, _z, theta_x, theta_y, theta_z, v_x,  v_y, v_z, w_x, w_y, w_z = np.genfromtxt(path, max_rows=deltastep*nFrame, unpack=True)

print (len(_x))
#nFrame = (int) (len(_x) / deltastep)



def update(i, ax, plt):
    plt.clf()
    plt.cla()
    ax = plt.axes(xlim=(0.015, 0.025), ylim=(0.0, 0.025 - 0.015))

    b = (0.0005 - 0.0001) * math.cos(theta_y[i * deltastep])
    a = b * math.tan(theta_y[i * deltastep])

    # print(w_x[i], w_y[i], w_z[i])
    ax.add_artist(plt.Circle((_x[i * deltastep], _z[i * deltastep] - 0.0005), 0.0005, fill=False, color='k', linewidth=0.5))
    ax.arrow(_x[i * deltastep], _z[i * deltastep] - 0.0005, b, a, width=0.00003, linewidth=0.0003,
                    head_width=0.00015, head_length=0.0001,
                    fc='k', ec='k')
    plt.xlabel('x', fontsize=12)
    plt.ylabel('z', fontsize=12, rotation=0)
    plt.text(0.00, 0.024 - 0.015, "ciao", fontsize=30)







def create_animation():
    fig = plt.figure()
    ax = plt.axes(xlim=(0.015, 0.024), ylim=(0.0, 0.024 - 0.015))
    a0, = ax.plot([], [])


    anim = animation.FuncAnimation(fig, update,frames=nFrame,
                                   fargs=(ax,plt, ),
                                   interval=10)

    anim.save('superball.gif', writer='imagemagick')
    # show plot
    plt.show()



def createsubplot():

    #fig= plt.figure()

    deltastep = 47000

    deltasteps=[0, 70000, 110000, 130000, 200000, 270000]

    filename = 'right_arrow.png'
    arrow = image.imread(filename)

    fig, axes = plt.subplots(3, 2, figsize=((0.025 - 0.015)*1000,(0.025 - 0.015)*1000))

    fig.set_size_inches((0.025 - 0.015)*750,(0.025 - 0.015)*1000)

    index = 0
    for i in range(0,3):
        for j in range(0,2):
            b = (0.0005 - 0.0001) * math.cos(theta_y[ deltasteps[index]])
            a = b * math.tan(theta_y[deltasteps[index]])
            print (_x[deltasteps[index]],_y[deltasteps[index]], _z[deltasteps[index]])
            print ("vel")
            print(v_x[deltasteps[index]], v_y[deltasteps[index]], v_y[deltasteps[index]])

            print("omega")
            print(w_x[deltasteps[index]], w_y[deltasteps[index]], w_z[deltasteps[index]])


            axes[i, j].add_patch(plt.Circle((_x[deltasteps[index]], _z[deltasteps[index]] - 0.0005), 0.0005, fill=False, color='k', linewidth=0.8))
            #axes[i, j].add_patch(circle2)
            axes[i, j].arrow(_x[deltasteps[index]], _z[deltasteps[index]] - 0.0005, b, a, width=0.00003, linewidth=0.0003,
                     head_width=0.00015, head_length=0.0001,
                     fc='k', ec='k')

            axes[i, j].set_xlim(0.015, 0.025)
            axes[i, j].set_ylim(0.0, 0.025 - 0.015)
            axes[i, j].set_title (str(time[deltasteps[index]])+" s", y=0.83, fontsize=10)
            axes[i, j].set_xlabel("x")
            axes[i, j].set_ylabel("z", rotation=0, labelpad=10)

            axes[i, j].set_xticks([])
            axes[i, j].set_yticks([])

            if i == 1 and j == 1 or i == 2 and j == 1 or i == 2 and j == 0:
                axes[i, j].plot(_x[deltasteps[2]:deltasteps[index]], _z[deltasteps[2]:deltasteps[index]] - 0.0005, 'b--', linewidth=0.8)
            else:
                axes[i, j].plot(_x[0:deltasteps[index]],_z[0:deltasteps[index]]-0.0005, 'b--', linewidth=0.8)
            index += 1

    imagebox = OffsetImage(arrow, zoom=1 * 0.72)
    ab = AnchoredOffsetbox(loc=1, child=imagebox, frameon=False)
    #axes[0,0].imshow(arrow, origin='upper', zorder=-1, aspect='auto')
    #axes[0,0].add_image(arrow)
    plt.savefig("superball_subplot.svg", format= "svg")
    plt.savefig("superball_subplot.png")
    plt.show()









def createsvg():
    fig = plt.figure()
    ax = plt.axes()
    ax.set_xlim([0.015, 0.024])
    ax.set_ylim([0.0, 0.024 - 0.015])




    for i in range(1, 30000, 700):
        # ax = plt.axes()
        # ax.set_xlim([0.015, 0.024])
        # ax.set_ylim([0.0, 0.024-0.015])
        b = (0.0005 -0.0001) * math.cos(theta_y[i])
        a = b * math.tan(theta_y[i])

        #print(w_x[i], w_y[i], w_z[i])
        ax.add_artist(plt.Circle((_x[i], _z[i] - 0.0005), 0.0005,  fill=False, color='k', linewidth=0.5))
        ax.arrow(_x[i], _z[i] - 0.0005, b  , a, width = 0.00003,linewidth= 0.0003,head_width=0.00015, head_length=0.0001, fc='k', ec='k')
        #plt.show()




    plt.savefig("superball.svg", format="svg")

    app = QtGui.QApplication(sys.argv)
    svgWidget = QtSvg.QSvgWidget('superball.svg')
    svgWidget.setGeometry(100,100,width*4,height*4)
    svgWidget.show()

    sys.exit(app.exec_())

createsubplot()

