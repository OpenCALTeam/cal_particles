import numpy as np
import matplotlib.pyplot as plt
import operator


def LeSupposizioniDiPaola (elapsed_time,  rot, kin, elastic_pp ):

    ratiokr = []

    for i in range(len(kin)):
        ratiokr.append(kin[i]/ rot[i])

    ratiork = []

    for i in range(len(kin)):
        ratiork.append(rot[i] / kin[i])

    fig = plt.figure()
    plt.subplot(2, 1, 1)
    plt.plot(elapsed_time, ratiokr, 'r-')

    plt.title('kin/rot (red)')

    plt.subplot(2, 1, 2)
    plt.plot(elapsed_time, ratiork, 'b-')
    plt.title('rot/kin (blue)')
    plt.show()

    fig = plt.figure()
    plt.subplot(1, 1, 1)
    plt.plot(kinetic, rotational, 'g-')

    plt.title('kin-rot (green)')
    plt.show()




# Function definition is here
def deltaEnergy( rot_kin, elastic_pw ):
    delta_rot_kin=[]
    delta_elastic_pw=[]
    delta=[]
    for i in range(len(rot_kin) - 1):
        delta_rot_kin.append(rot_kin[i+1]- rot_kin[i])
        #print (rot_kin[i+1]- rot_kin[i] , "   ", elastic_pw[i + 1] - elastic_pw[i])
    for i in range(len(elastic_pw) - 1):
        delta_elastic_pw.append(elastic_pw[i + 1] - elastic_pw[i])

    for i in range(len(elastic_pw)):
        delta.append(abs(elastic_pw[i] - rot_kin[i]))

    fig = plt.figure()
    plt.subplot(3, 1, 1)
    plt.plot( delta_rot_kin, 'r-')
    plt.title('delta_rot_kin (red)')

    plt.subplot(3, 1, 2)
    plt.plot(delta_elastic_pw, 'g-')
    plt.title('delta_elastic_pw (green)')

    plt.subplot(3, 1, 3)
    plt.plot(delta, 'm-')
    plt.title('delta_elastic_pw-rot_kin (magenta)')
    plt.show()



#path ="./total_energy_-1_1_omp.dat"

path ="/home/parcuri/Documenti/total_energy_-1_1_omp.dat"
#path="/home/parcuri/Documenti/DATA_TESI/total_energy_7.dat"
#path ="./total_energy_superball.dat"

step, elapsed_time, kinetic, rotational, potential, elastic_pp, elastic_pw, total_energy = np.loadtxt(path, unpack=True)




import operator
#rot_kin = list(map(operator.add, rotational,kinetic))
#tot_en = list(map(operator.add, rot_kin,elastic_pp))



#deltaEnergy(rot_kin, elastic_pw)

#LeSupposizioniDiPaola(elapsed_time,rotational,kinetic, elastic_pp)



fig = plt.figure()
#for t,k,r,pp,pw,tt in zip(elapsed_time, kinetic,rotational, elastic_pp, elastic_pw, total_energy):
#    print (t,k,r,pp,pw,tt)

plt.subplot(6, 1, 1)
plt.plot(elapsed_time,total_energy,'r-')
plt.title('total energy (red)')
fig.subplots_adjust(left= 0.15,hspace=0.9, wspace=0.4)

plt.subplot(6, 1, 2)
plt.plot(elapsed_time,rotational,'b-')
#plt.plot(elapsed_time,kinetic,'g-')
plt.title('Rotational energy (blue) ')

plt.subplot(6, 1, 3)
plt.plot(elapsed_time,kinetic,'g-')
plt.title('kinetic energy (green)')

plt.subplot(6, 1, 4)
plt.plot(elapsed_time,elastic_pp,'c-')
#plt.plot(elapsed_time,elastic_pw,'k-')
plt.title('elastic particles energy (cyan)')


plt.subplot(6, 1, 5)
plt.plot(elapsed_time,elastic_pw,'k-')
plt.title('elastic wall energy (black)')



plt.subplot(6, 1, 6)

plt.plot(elapsed_time,potential,'m-')
plt.title('Potential energy (magenta)')





plt.show()


 