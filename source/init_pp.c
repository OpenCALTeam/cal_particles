#include <init_pp.h>

unsigned int _seed = 1;

int getFirstFreeSlot(struct CALModel3D *ca, int cell_x,int cell_y,int cell_z)
{
    int freeSlot = -1;
    for (int slot = 0; slot < MAX_NUMBER_OF_PARTICLES_PER_CELL; ++slot) {
        if (calGet3Di(ca,Q.ID[slot],cell_x,cell_y,cell_z) == NULL_ID)
        {
            freeSlot = slot;
        }
    }
    return freeSlot;

}

//aggiunge una particella con una posizone random in una posione random del CA
bool addRandomParticlePosition(struct CALModel3D *ca, int * nextIDParticle)
{
    CALreal cx = ((CALreal)rand_r(&_seed)/(CALreal)(RAND_MAX)) * (CELL_SIDE);
    CALreal cy = 0.001;
    CALreal cz = ((CALreal)rand_r(&_seed)/(CALreal)(RAND_MAX)) * (CELL_SIDE);

    //generiamo una porzione
    CALint cell_x =((CALreal)rand_r(&_seed)/(CALreal)(RAND_MAX)) * ca->rows;
    CALint cell_y =((CALreal)rand_r(&_seed)/(CALreal)(RAND_MAX)) * ca->columns;
    CALint cell_z =((CALreal)rand_r(&_seed)/(CALreal)(RAND_MAX)) * ca->slices;


    int slot = getFirstFreeSlot(ca, cell_x,cell_y,cell_z);


    if (slot != -1)
    {
        CALreal px = CELL_SIDE * (cell_x + cx);
        CALreal py = CELL_SIDE * (cell_y + cy);
        CALreal pz = CELL_SIDE * (cell_z + cz);

        printf( "aggiunta particella a pos : %f %f %f con id %d \n", px,py,pz, *nextIDParticle);

        calInit3Dr(ca,Q.Fx[slot],cell_x,cell_y,cell_z,0.0);
        calInit3Dr(ca,Q.Fy[slot],cell_x,cell_y,cell_z,0.0);
        calInit3Dr(ca,Q.Fz[slot],cell_x,cell_y,cell_z,0.0);

        calInit3Dr(ca,Q.px[slot],cell_x,cell_y,cell_z,px);
        calInit3Dr(ca,Q.py[slot],cell_x,cell_y,cell_z,py);
        calInit3Dr(ca,Q.pz[slot],cell_x,cell_y,cell_z,pz);

        calInit3Dr(ca,Q.thetax[slot],cell_x,cell_y,cell_z,0.0);
        calInit3Dr(ca,Q.thetay[slot],cell_x,cell_y,cell_z,0.0);
        calInit3Dr(ca,Q.thetaz[slot],cell_x,cell_y,cell_z,0.0);


        calInit3Dr(ca,Q.vx[slot],cell_x,cell_y,cell_z,0.0);
        calInit3Dr(ca,Q.vy[slot],cell_x,cell_y,cell_z,0.0);
        calInit3Dr(ca,Q.vz[slot],cell_x,cell_y,cell_z,0.0);

        calInit3Dr(ca,Q.wx[slot],cell_x,cell_y,cell_z,0.0);
        calInit3Dr(ca,Q.wy[slot],cell_x,cell_y,cell_z,0.0);
        calInit3Dr(ca,Q.wz[slot],cell_x,cell_y,cell_z,0.0);

        calInit3Di(ca,Q.ID[slot],cell_x,cell_y,cell_z,*nextIDParticle);

        (*nextIDParticle)++;
        return true;
    }

    return false;

}
