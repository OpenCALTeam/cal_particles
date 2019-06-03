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



//void applyForce(CALreal* F, CALreal* p0, CALreal* v0, CALreal m, CALreal t, CALreal* pf, CALreal* vf)
//{
////  CALreal F[3];
////  F[0] =  m*0 - 6*M_PI*AIR_VISCOSITY*PARTICLE_RADIUS*v0[0];
////  F[1] =  m*0 - 6*M_PI*AIR_VISCOSITY*PARTICLE_RADIUS*v0[1];
////  F[2] = -m*G - 6*M_PI*AIR_VISCOSITY*PARTICLE_RADIUS*v0[2];

//  CALreal a[3];

//  a[0] = F[0]/m;
//  a[1] = F[1]/m;
//  a[2] = F[2]/m;

//  for (int i=0; i<3; i++)
//    {
//      vf[i] = v0[i]+a[i]*t;
//      //pf[i] = p0[i] + v0[i]*t + 0.5*a[i]*t*t;
//      pf[i] = p0[i] + vf[i]*t;
//    }

//  CALreal displacement = distance(p0, pf);
//  if (displacement >= PARTICLE_RADIUS)
//    {
//      printf("ERROR: a particle displacemnt is greater than CELL_SIDE.\n");
//#ifdef VERBOSE
//      printf("F = (%f, %f, %f)\n", F[0], F[1], F[2]);
//      printf("a = (%f, %f, %f)\n", a[0], a[1], a[2]);
//      printf("p0 = (%f, %f, %f)\n", p0[0], p0[1], p0[2]);
//      printf("pf = (%f, %f, %f)\n", pf[0], pf[1], pf[2]);
//      printf("particle displacemnt = %f\n", displacement);
//      printf("CELL_SIDE = %f \n", CELL_SIDE);
//#endif
//      exit(EXIT_FAILURE);
//    }
//}



void applyForce(struct CALModel3D* ca, int cell_x, int cell_y, int cell_z)
{
    if (!ncestiArmenuNaParticella(ca, cell_x, cell_y, cell_z, 0))
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

            totalMomentCollisionPP(&collisions, &moment_collision, id_particle);

            calSet3Dr_vec3(ca, Q.Fx[slot], Q.Fy[slot], Q.Fz[slot], cell_x,cell_y,cell_z, F );

            calSet3Dr_vec3(ca, Q.momentx[slot], Q.momenty[slot], Q.momentz[slot], cell_x,cell_y,cell_z, moment_collision );
        }
    }
}
