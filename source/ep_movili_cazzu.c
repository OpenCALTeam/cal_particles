#include <ep_movili_cazzu.h>
#include <ep_utils.h>
#include <stdlib.h>


void makeSlotEmpty(int slot, struct CALModel3D* ca, int cell_x, int cell_y, int cell_z)
{
    calSet3Dr_vec3_sv(ca, Q.Fx[slot], Q.Fy[slot], Q.Fz[slot], cell_x,cell_y,cell_z, 0.0 );
    calSet3Dr_vec3_sv(ca, Q.px[slot], Q.py[slot], Q.pz[slot], cell_x,cell_y,cell_z, 0.0 );
    calSet3Dr_vec3_sv(ca, Q.vx[slot], Q.vy[slot], Q.vz[slot], cell_x,cell_y,cell_z, 0.0 );
    calSet3Dr_vec3_sv(ca, Q.wx[slot], Q.wy[slot], Q.wz[slot], cell_x,cell_y,cell_z, 0.0 );
    calSet3Dr_vec3_sv(ca, Q.thetax[slot], Q.thetay[slot], Q.thetaz[slot], cell_x,cell_y,cell_z, 0.0 );

    calSet3Di(ca,Q.ID[slot],cell_x,cell_y,cell_z,NULL_ID);

    //    calSet3Di(ca,Q.nP,cell_x,cell_y,cell_z,calGet3Di(ca,Q.nP,cell_x,cell_y,cell_z)-1);
}

void moveParticleToNeighbor(int destination_slot, int source_slot, struct CALModel3D* ca, int cell_x, int cell_y, int cell_z, int n)
{
    vec3 F, p, v, theta, w;

    calGetX3Dr_vec3_slot(ca, Q.Fx, Q.Fy, Q.Fz, source_slot, cell_x,cell_y,cell_z, &F, n );
    calGetX3Dr_vec3_slot(ca, Q.px, Q.py, Q.pz, source_slot, cell_x,cell_y,cell_z, &p, n );
    calGetX3Dr_vec3_slot(ca, Q.vx, Q.vy, Q.vz, source_slot, cell_x,cell_y,cell_z, &v, n );
    calGetX3Dr_vec3_slot(ca, Q.thetax, Q.thetay, Q.thetaz, source_slot, cell_x,cell_y,cell_z, &theta, n );
    calGetX3Dr_vec3_slot(ca, Q.wx, Q.wy, Q.wz, source_slot, cell_x,cell_y,cell_z, &w, n );


    calSet3Dr_vec3_slot(ca, Q.Fx, Q.Fy, Q.Fz, destination_slot, cell_x,cell_y,cell_z, F );
    calSet3Dr_vec3_slot(ca, Q.px, Q.py, Q.pz, destination_slot, cell_x,cell_y,cell_z, p );
    calSet3Dr_vec3_slot(ca, Q.vx, Q.vy, Q.vz, destination_slot, cell_x,cell_y,cell_z, v );
    calSet3Dr_vec3_slot(ca, Q.thetax, Q.thetay, Q.thetaz, destination_slot, cell_x,cell_y,cell_z, theta );
    calSet3Dr_vec3_slot(ca, Q.wx, Q.wy, Q.wz, destination_slot, cell_x,cell_y,cell_z, w );


    calSet3Di(ca,Q.ID[destination_slot],cell_x,cell_y,cell_z, calGetX3Di(ca,Q.ID[source_slot],cell_x,cell_y,cell_z,n));

    //    calSet3Di(ca,Q.nP,cell_x,cell_y,cell_z,calGet3Di(ca,Q.nP,cell_x,cell_y,cell_z)+1);
}

void moveParticles(struct CALModel3D* ca, int cell_x, int cell_y, int cell_z)
{
    vec3 p;
    CALint  new_cell_x, new_cell_y, new_cell_z;

    if(cell_x > ca->rows || cell_y > ca->columns || cell_z > ca->slices )
    {
        printf("\\\\\\\\\\\\\\\\\\\\\ ci muoviamo male (%d,%d;%d)\n", cell_x,cell_y,cell_z );
    }

    //pezziali
    for (int slot = 0; slot < cnfg.MAX_NUMBER_OF_PARTICLES_PER_CELL; slot++)
        if (calGet3Di(ca, Q.ID[slot],cell_x,cell_y,cell_z) > NULL_ID)
        {
            calGet3Dr_vec3(ca, Q.px[slot], Q.py[slot], Q.pz[slot], cell_x,cell_y,cell_z, &p );

            new_cell_x = p[0]/CELL_SIDE;
            new_cell_y = p[1]/CELL_SIDE;
            new_cell_z = p[2]/CELL_SIDE;



            if ((cell_x != new_cell_x) || (cell_y != new_cell_y) || (cell_z != new_cell_z))
            {
                makeSlotEmpty(slot, ca,cell_x,cell_y,cell_z);

#if OPTIMITAZION_ACTIVE_CELLS == 1
                printf("(%d,%d;%d) è ORA attiva\n", new_cell_x, new_cell_y, new_cell_z);
                if(new_cell_x > ca->rows || new_cell_y > ca->columns || new_cell_z > ca->slices
                        || new_cell_x < 0 || new_cell_y < 0 || new_cell_z < 0)
                {
                    printf("sono %d parto da (%d,%d;%d) \n", calGet3Di(ca, Q.ID[slot],cell_x,cell_y,cell_z), cell_x,cell_y,cell_z);
                    printf("(%d,%d;%d) è ORA attiva MA DOVE VAI \n", new_cell_x, new_cell_y, new_cell_z);
                    printf("lo spazio è (%d, %d, %d) \n", ca->rows, ca->columns, ca->slices);
//                    exit(EXIT_SUCCESS);
                }
                else
                    calAddActiveCell3D(ca, new_cell_x, new_cell_y, new_cell_z);
#endif
            }
        }


}

void pickupParticles(struct CALModel3D* ca, int cell_x, int cell_y, int cell_z)
{
    //sucali
    vec3 p;
    CALint  new_cell_x, new_cell_y, new_cell_z;


    if(cell_x > ca->rows || cell_y > ca->columns || cell_z > ca->slices )
        printf("\\\\\\\\\\\\\\\\\\\\\ preleviamo male (%d,%d;%d)\n", cell_x,cell_y,cell_z );
    for (int n=1; n<ca->sizeof_X; n++)
        for (int source_slot = 0; source_slot < cnfg.MAX_NUMBER_OF_PARTICLES_PER_CELL; source_slot++)
        {
            int id_particle = calGetX3Di(ca,Q.ID[source_slot],cell_x,cell_y,cell_z,n);
            if (id_particle > NULL_ID)
            {
                calGetX3Dr_vec3_slot(ca, Q.px, Q.py, Q.pz, source_slot, cell_x,cell_y,cell_z, &p, n );

                new_cell_x = p[0]/CELL_SIDE;
                new_cell_y = p[1]/CELL_SIDE;
                new_cell_z = p[2]/CELL_SIDE;


                if ((cell_x == new_cell_x) && (cell_y == new_cell_y) && (cell_z == new_cell_z))
                {
                    CALbyte sucked = CAL_FALSE;
                    int destination_slot;
                    for (destination_slot = 0; destination_slot < cnfg.MAX_NUMBER_OF_PARTICLES_PER_CELL; destination_slot++)
                        if (calGetNext3Di(ca,Q.ID[destination_slot],cell_x,cell_y,cell_z) == NULL_ID)
                        {
                            printf("sono la particella %d e vado da source= %d dest=%d cella (%d,%d,%d) vicino %d \n",id_particle, source_slot, destination_slot, cell_x,cell_y,cell_z, n );
                            moveParticleToNeighbor(destination_slot,source_slot,ca,cell_x,cell_y,cell_z,n);
                            sucked = CAL_TRUE;
                            break;
                        }
                    if (sucked == CAL_FALSE)
                    {
                        printf("ERROR: unable to suck a particle.\n");
#ifdef VERBOSE
                        printf("cell_capacity: %d\n", cnfg.MAX_NUMBER_OF_PARTICLES_PER_CELL);
                        printf("current_step: %d\n", a_simulazioni->step);
                        printf("source_slot: %d\n", source_slot);
                        printf("destination_slot: %d\n", destination_slot);
#endif
                        exit(EXIT_FAILURE);
                    }
                }
            }
        }
}
