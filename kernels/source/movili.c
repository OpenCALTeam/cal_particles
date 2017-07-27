#include <OpenCAL-CL/calcl3D.h>
#include "model.h"

void applyForce(CALreal* F, CALreal* p0, CALreal* v0, CALreal m, CALreal t, CALreal* pf, CALreal* vf)
{
//  CALreal F[3];
//  F[0] =  m*0 - 6*M_PI*AIR_VISCOSITY*PARTICLE_RADIUS*v0[0];
//  F[1] =  m*0 - 6*M_PI*AIR_VISCOSITY*PARTICLE_RADIUS*v0[1];
//  F[2] = -m*G - 6*M_PI*AIR_VISCOSITY*PARTICLE_RADIUS*v0[2];

  CALreal a[3];

  a[0] = F[0]/m;
  a[1] = F[1]/m;
  a[2] = F[2]/m;

  for (int i=0; i<3; i++)
    {
      vf[i] = v0[i]+a[i]*t;
      //pf[i] = p0[i] + v0[i]*t + 0.5*a[i]*t*t;
      pf[i] = p0[i] + vf[i]*t;
    }

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
//      //exit(EXIT_FAILURE);
//    }
}

__kernel void movili(__CALCL_MODEL_3D){

    calclThreadCheck3D();

    int cell_x = calclGlobalRow();
    int cell_y = calclGlobalColumn();
    int cell_z = calclGlobalSlice();

    CALreal F0[3];
    CALreal p0[3];
    CALreal v0[3];
    CALreal pf[3];
    CALreal vf[3];

    for (int slot = 0; slot < MAX_NUMBER_OF_PARTICLES_PER_CELL; slot++)
      if (calclGet3Di(MODEL_3D, P(ID,slot),cell_x,cell_y,cell_z) > NULL_ID)
        {
          F0[0] = calclGet3Dr(MODEL_3D,Q(FX,slot),cell_x,cell_y,cell_z);
          F0[1] = calclGet3Dr(MODEL_3D,Q(FY,slot),cell_x,cell_y,cell_z);
          F0[2] = calclGet3Dr(MODEL_3D,Q(FZ,slot),cell_x,cell_y,cell_z);

          p0[0] = pf[0] = calclGet3Dr(MODEL_3D,Q(PX,slot),cell_x,cell_y,cell_z);
          p0[1] = pf[1] = calclGet3Dr(MODEL_3D,Q(PY,slot),cell_x,cell_y,cell_z);
          p0[2] = pf[2] = calclGet3Dr(MODEL_3D,Q(PZ,slot),cell_x,cell_y,cell_z);

          v0[0] = vf[0] = calclGet3Dr(MODEL_3D,Q(VX,slot),cell_x,cell_y,cell_z);
          v0[1] = vf[1] = calclGet3Dr(MODEL_3D,Q(VY,slot),cell_x,cell_y,cell_z);
          v0[2] = vf[2] = calclGet3Dr(MODEL_3D,Q(VZ,slot),cell_x,cell_y,cell_z);

          applyForce(F0, p0, v0, PARTICLE_MASS, DELTA_T, pf, vf);

          calclSet3Dr(MODEL_3D,Q(PX,slot),cell_x,cell_y,cell_z,pf[0]);
          calclSet3Dr(MODEL_3D,Q(PY,slot),cell_x,cell_y,cell_z,pf[1]);
          calclSet3Dr(MODEL_3D,Q(PZ,slot),cell_x,cell_y,cell_z,pf[2]);

          calclSet3Dr(MODEL_3D,Q(VX,slot),cell_x,cell_y,cell_z,vf[0]);
          calclSet3Dr(MODEL_3D,Q(VY,slot),cell_x,cell_y,cell_z,vf[1]);
          calclSet3Dr(MODEL_3D,Q(VZ,slot),cell_x,cell_y,cell_z,vf[2]);
        }

}
