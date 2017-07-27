#include <OpenCAL-CL/calcl3D.h>
#include "model.h"

__kernel void resetF(__CALCL_MODEL_3D) {

    calclThreadCheck3D();

    int cell_x = calclGlobalRow();
    int cell_y = calclGlobalColumn();
    int cell_z = calclGlobalSlice();
    CALreal F[3];
    CALreal gravity_force = -PARTICLE_MASS*G;

    F[0] = 0.0;
    F[1] = 0.0;
    F[2] = gravity_force;

    for (int slot = 0; slot < MAX_NUMBER_OF_PARTICLES_PER_CELL; slot++)
        if (calclGet3Di(MODEL_3D,P(ID,slot),cell_x,cell_y,cell_z) > NULL_ID)
            {
                calclSet3Dr(MODEL_3D, Q(FX,slot),cell_x,cell_y,cell_z,F[0]);

                calclSet3Dr(MODEL_3D, Q(FY,slot),cell_x,cell_y,cell_z,F[1]);
                calclSet3Dr(MODEL_3D, Q(FZ,slot),cell_x,cell_y,cell_z,F[2]);

            }



}
