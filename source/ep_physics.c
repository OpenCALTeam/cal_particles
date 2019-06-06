#include <ep_physics.h>
#include <ep_utils.h>
#include <math.h>
#include <stdlib.h>
#include <common.hpp>

// #define AIR_VISCOSITY 1.81e-5


void leap_frog_velocity(struct CALModel3D* ca,
                        int cell_x, int cell_y, int cell_z)
{

    for (int slot = 0; slot < MAX_NUMBER_OF_PARTICLES_PER_CELL; slot++)
    {

        CALint id_PARTICLE_i = calGet3Di(ca, Q.ID[slot],cell_x,cell_y,cell_z);
        if (id_PARTICLE_i > NULL_ID)
        {
            vec3 Fi, moment_i, vi, wi, toAdd;
            calGet3Dr_vec3(ca, Q.vx[slot], Q.vy[slot], Q.vz[slot], cell_x,cell_y,cell_z, &vi);
            calGet3Dr_vec3(ca, Q.Fx[slot], Q.Fy[slot], Q.Fz[slot], cell_x,cell_y,cell_z, &Fi);
            calGet3Dr_vec3(ca, Q.momentx[slot], Q.momenty[slot], Q.momentz[slot], cell_x,cell_y,cell_z, &moment_i);
            calGet3Dr_vec3(ca, Q.wx[slot], Q.wy[slot], Q.wz[slot], cell_x,cell_y,cell_z, &wi);


            multiply_by_scalar_vec3(&toAdd, Fi, PARTICLE_MASS_DELTA_T_1_2);
            add_vec3(&vi, vi, toAdd);
            calSet3Dr_vec3_slot(ca, Q.vx, Q.vy, Q.vz, slot, cell_x,cell_y,cell_z, vi );

            clear_vec3(&toAdd);

            multiply_by_scalar_vec3(&toAdd, moment_i, MOMENT_INERTIA_DELTA_T_1_2);

            add_vec3(&wi, wi, toAdd);
            calSet3Dr_vec3_slot(ca, Q.wx, Q.wy, Q.wz, slot, cell_x,cell_y,cell_z, wi );

        }
    }

}

void leap_frog_positions(struct CALModel3D* ca,
                         int cell_x, int cell_y, int cell_z)
{
    for (int slot = 0; slot < MAX_NUMBER_OF_PARTICLES_PER_CELL; slot++)
    {

        CALint id_PARTICLE_i = calGet3Di(ca, Q.ID[slot],cell_x,cell_y,cell_z);
        if (id_PARTICLE_i > NULL_ID)
        {
            vec3 pi, vi, wi, theta_i, toAdd;
            calGet3Dr_vec3(ca, Q.vx[slot], Q.vy[slot], Q.vz[slot], cell_x,cell_y,cell_z, &vi);
            calGet3Dr_vec3(ca, Q.px[slot], Q.py[slot], Q.pz[slot], cell_x,cell_y,cell_z, &pi);

            calGet3Dr_vec3(ca, Q.wx[slot], Q.wy[slot], Q.wz[slot], cell_x,cell_y,cell_z, &wi);
            calGet3Dr_vec3(ca, Q.thetax[slot], Q.thetay[slot], Q.thetaz[slot], cell_x,cell_y,cell_z, &theta_i);


            multiply_by_scalar_vec3(&toAdd, vi, DELTA_T);
            add_vec3(&pi, pi, toAdd);
            calSet3Dr_vec3_slot(ca, Q.px, Q.py, Q.pz, slot, cell_x,cell_y,cell_z, pi );

            clear_vec3(&toAdd);

            multiply_by_scalar_vec3(&toAdd, wi, DELTA_T);

            add_vec3(&theta_i, theta_i, toAdd);
            calSet3Dr_vec3_slot(ca, Q.thetax, Q.thetay, Q.thetaz, slot, cell_x,cell_y,cell_z, theta_i );

        }
    }
}



void applyForce(struct CALModel3D* ca, int cell_x, int cell_y, int cell_z)
{
    if (!isThereAtLeastAParticle(ca, cell_x, cell_y, cell_z, 0))
        return;

    vec3 F, accGr,  moment_collision;



    for (int slot = 0; slot < MAX_NUMBER_OF_PARTICLES_PER_CELL; slot++)
    {
        CALint id_particle = calGet3Di(ca, Q.ID[slot],cell_x,cell_y,cell_z);
        if ( id_particle > NULL_ID)
        {

            clear_vec3(&F);
            clear_vec3(&accGr);
            clear_vec3(&moment_collision);

            multiply_by_scalar_vec3(&accGr, G_dir, G);

            multiply_by_scalar_vec3(&F, accGr, PARTICLE_MASS);

            totalForceCollisionPP(&collisions, &F, id_particle);
            totalForceCollisionPW(&collisions, &F, id_particle);

            totalMomentCollisionPP(&collisions, &moment_collision, id_particle);
            totalMomentCollisionPW(&collisions, &moment_collision, id_particle);

            calSet3Dr_vec3(ca, Q.Fx[slot], Q.Fy[slot], Q.Fz[slot], cell_x,cell_y,cell_z, F );

            calSet3Dr_vec3(ca, Q.momentx[slot], Q.momenty[slot], Q.momentz[slot], cell_x,cell_y,cell_z, moment_collision );
        }
    }
}
