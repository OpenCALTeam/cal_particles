#include <init_pp.h>

unsigned int _seed = 1;

int getFirstFreeSlot(struct CALModel3D *ca, int cell_x,int cell_y,int cell_z)
{
    int freeSlot = -1;
    for (int slot = 0; slot < cnfg.MAX_NUMBER_OF_PARTICLES_PER_CELL; ++slot) {
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
    CALreal cy = /*((CALreal)rand_r(&_seed)/(CALreal)(RAND_MAX)) * (CELL_SIDE);*/ 0.001;
    CALreal cz = ((CALreal)rand_r(&_seed)/(CALreal)(RAND_MAX)) * (CELL_SIDE);

    //generiamo una porzione
    CALint cell_x =((CALreal)rand_r(&_seed)/(CALreal)(RAND_MAX)) * (ca->rows-1)+1;
    CALint cell_y =((CALreal)rand_r(&_seed)/(CALreal)(RAND_MAX)) * (ca->columns-1)+1;
    CALint cell_z =((CALreal)rand_r(&_seed)/(CALreal)(RAND_MAX)) * (ca->slices-1)+1;


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

        //        calInit3Di(ca,Q.nP,cell_x,cell_y,cell_z,calGet3Di(ca,Q.nP,cell_x,cell_y,cell_z)+1);

        (*nextIDParticle)++;
        return true;
    }

    return false;

}

//aggiunge una particella con una posizone random in una posione random del CA
void addStructuredParticles(struct CALModel3D *ca, int * nextIDParticle)
{

    vec3 pos;
    clear_vec3(&pos);

    pos[0] = cnfg.DM*2;
//    pos[1] = cnfg.DM*2;
//    pos[2] = cnfg.DM*2;

    while (pos[0] < (X_CELLS * CELL_SIDE - cnfg.DM*2))
    {
        pos[1] = cnfg.DM*2;
        while (pos[1] < (X_CELLS * CELL_SIDE - cnfg.DM*2))
        {
            pos[2] = (X_CELLS * CELL_SIDE - cnfg.DM*2)*0.35;
            while (pos[2] >cnfg.DM*2)
            {
                int cell_x = pos[0]/CELL_SIDE;
                int cell_y = pos[1]/CELL_SIDE;
                int cell_z = pos[2]/CELL_SIDE;

                int slot = getFirstFreeSlot(ca, cell_x,cell_y,cell_z);


                if (slot != -1)
                {


                    printf( "aggiunta particella a pos : %f %f %f con id %d \n", pos[0],pos[1],pos[2], *nextIDParticle);

                    calInit3Dr(ca,Q.Fx[slot],cell_x,cell_y,cell_z,0.0);
                    calInit3Dr(ca,Q.Fy[slot],cell_x,cell_y,cell_z,0.0);
                    calInit3Dr(ca,Q.Fz[slot],cell_x,cell_y,cell_z,0.0);

                    calInit3Dr(ca,Q.px[slot],cell_x,cell_y,cell_z,pos[0]);
                    calInit3Dr(ca,Q.py[slot],cell_x,cell_y,cell_z,pos[1]);
                    calInit3Dr(ca,Q.pz[slot],cell_x,cell_y,cell_z,pos[2]);

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

                    //        calInit3Di(ca,Q.nP,cell_x,cell_y,cell_z,calGet3Di(ca,Q.nP,cell_x,cell_y,cell_z)+1);

                    (*nextIDParticle)++;

                }
                pos[2]-= cnfg.DM*2;
            }
            pos[1]+= cnfg.DM*2;
        }
        pos[0]+= cnfg.DM*2;
    }



}

//aggiunge una particella con una posizone precisa del CA
bool addParticleWithPosition(struct CALModel3D *ca, vec3 p_0, vec3 v_0, vec3 w_0, int * nextIDParticle)
{

    int cell_x = p_0[0]/CELL_SIDE;
    int cell_y = p_0[1]/CELL_SIDE;
    int cell_z = p_0[2]/CELL_SIDE;


    int slot = getFirstFreeSlot(ca, cell_x,cell_y,cell_z);


    if (slot != -1)
    {
        printf( "aggiunta particella a pos : %f %f %f con id %d \n", p_0[0],p_0[1],p_0[2], *nextIDParticle);

        calInit3Dr(ca,Q.Fx[slot],cell_x,cell_y,cell_z,0.0);
        calInit3Dr(ca,Q.Fy[slot],cell_x,cell_y,cell_z,0.0);
        calInit3Dr(ca,Q.Fz[slot],cell_x,cell_y,cell_z,0.0);


        calInit3Dr(ca,Q.thetax[slot],cell_x,cell_y,cell_z,0.0);
        calInit3Dr(ca,Q.thetay[slot],cell_x,cell_y,cell_z,0.0);
        calInit3Dr(ca,Q.thetaz[slot],cell_x,cell_y,cell_z,0.0);


        calInit3Dr_vec3(ca,Q.px[slot],Q.py[slot], Q.pz[slot], cell_x, cell_y, cell_z, p_0 );
        calInit3Dr_vec3(ca,Q.vx[slot],Q.vy[slot], Q.vz[slot], cell_x, cell_y, cell_z, v_0 );
        calInit3Dr_vec3(ca,Q.wx[slot],Q.wy[slot], Q.wz[slot], cell_x, cell_y, cell_z, w_0 );
//        printf( "aggiunta particella a vel : %f %f %f con id %d \n", v_0[0],v_0[1],v_0[2], *nextIDParticle);

        calInit3Di(ca,Q.ID[slot],cell_x,cell_y,cell_z,*nextIDParticle);

        (*nextIDParticle)++;
        return true;
    }

    return false;

}

bool addParticleWithFullConfiguration(struct CALModel3D *ca, vec3 p_0, vec3 v_0, vec3 w_0, vec3 theta_0,vec3 F_0, vec3 moment_0, CALint particle_id, int * numberOfParticles)
{

    int cell_x = p_0[0]/CELL_SIDE;
    int cell_y = p_0[1]/CELL_SIDE;
    int cell_z = p_0[2]/CELL_SIDE;


    int slot = getFirstFreeSlot(ca, cell_x,cell_y,cell_z);


    if (slot != -1)
    {
        printf( "aggiunta particella a pos : %.6f %.6f %.6f con id %d \n", p_0[0],p_0[1],p_0[2], particle_id);

        //        calInit3Dr(ca,Q.Fx[slot],cell_x,cell_y,cell_z,0.0);
        //        calInit3Dr(ca,Q.Fy[slot],cell_x,cell_y,cell_z,0.0);
        //        calInit3Dr(ca,Q.Fz[slot],cell_x,cell_y,cell_z,0.0);

        calInit3Dr_vec3(ca,Q.thetax[slot],Q.thetay[slot], Q.thetaz[slot], cell_x, cell_y, cell_z, theta_0 );
        calInit3Dr_vec3(ca,Q.px[slot],Q.py[slot], Q.pz[slot], cell_x, cell_y, cell_z, p_0 );
        calInit3Dr_vec3(ca,Q.vx[slot],Q.vy[slot], Q.vz[slot], cell_x, cell_y, cell_z, v_0 );
        calInit3Dr_vec3(ca,Q.wx[slot],Q.wy[slot], Q.wz[slot], cell_x, cell_y, cell_z, w_0 );

        calInit3Dr_vec3(ca,Q.Fx[slot],Q.Fy[slot], Q.Fz[slot], cell_x, cell_y, cell_z, F_0 );
        calInit3Dr_vec3(ca,Q.momentx[slot],Q.momenty[slot], Q.momentz[slot], cell_x, cell_y, cell_z, moment_0 );

        calInit3Di(ca,Q.ID[slot],cell_x,cell_y,cell_z,particle_id);

        (*numberOfParticles)++;

        return true;
    }

    return false;

}

