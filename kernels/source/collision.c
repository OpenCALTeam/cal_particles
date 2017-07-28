#include <OpenCAL-CL/calcl3D.h>
#include "model.h"

CALreal calclGetNext3Dr(__CALCL_MODEL_3D, CALint substateNum, int i, int j, int k) {
        __global CALreal * next = calclGetNextSubstate3Dr(MODEL_3D, substateNum);
        return calclGetBufferElement3D(next, calclGetRows(), calclGetColumns(), i, j, k);
}


CALreal __distance (CALreal* p0, CALreal* p1)
{
  return sqrt((p0[0]-p1[0])*(p0[0]-p1[0]) +
              (p0[1]-p1[1])*(p0[1]-p1[1]) +
              (p0[2]-p1[2])*(p0[2]-p1[2]));
}

CALreal __pointPlaneDistance(CALreal* P0, CALreal* Pp, CALreal* n)
{
  return fabs(n[0]*P0[0] + n[1]*P0[1] + n[2]*P0[2] - n[0]*Pp[0] - n[1]*Pp[1] - n[2]*Pp[2]);
}

CALreal scalar(CALreal *v, CALreal *n)
{
  return v[0]*n[0]+v[1]*n[1]+v[2]*n[2];
}

void __orthogonalProjectedPointToPlane(CALreal* Pi, CALreal* Pp, CALreal* n, CALreal* Pj)
{
  CALreal d = - scalar(n, Pp);
  CALreal t = - d - scalar(n, Pi);
  for (int i=0; i<3; i++)
    Pj[i] = t * n[i] + Pi[i];
}

__kernel void collision(__CALCL_MODEL_3D){

    calclThreadCheck3D();

    int cell_x = calclGlobalRow();
    int cell_y = calclGlobalColumn();
    int cell_z = calclGlobalSlice();


    CALreal kn = KN;
    CALreal etha = ETHA;
    CALreal pi[3], vi[3], delta_Fi[3];
    CALreal pj[3], vj[3], delta_Fj[3], Nj[3];
    CALreal rij[3], dij, enij[3], vij[3], vnij;
    CALreal delta_n;

    for (int slot = 0; slot < MAX_NUMBER_OF_PARTICLES_PER_CELL; slot++)
      if (calclGet3Di(MODEL_3D, P(ID,slot),cell_x,cell_y,cell_z) > NULL_ID)
        {
          pi[0] = calclGet3Dr(MODEL_3D,Q(PX,slot),cell_x,cell_y,cell_z);
          pi[1] = calclGet3Dr(MODEL_3D,Q(PY,slot),cell_x,cell_y,cell_z);
          pi[2] = calclGet3Dr(MODEL_3D,Q(PZ,slot),cell_x,cell_y,cell_z);
  #ifdef VISCOELASTIC
          vi[0] = calclGet3Dr(MODEL_3D,Q(VX,slot),cell_x,cell_y,cell_z);
          vi[1] = calclGet3Dr(MODEL_3D,Q(VY,slot),cell_x,cell_y,cell_z);
          vi[2] = calclGet3Dr(MODEL_3D,Q(VZ,slot),cell_x,cell_y,cell_z);
  #endif
          delta_Fi[0] = 0.0;
          delta_Fi[1] = 0.0;
          delta_Fi[2] = 0.0;
          // inner particle-particle collision
          for (int inner_slot=slot+1; inner_slot<MAX_NUMBER_OF_PARTICLES_PER_CELL; inner_slot++)
            {
              if (calclGet3Di(MODEL_3D, P(ID,inner_slot),cell_x,cell_y,cell_z) == NULL_ID)
                continue;

              pj[0] = calclGet3Dr(MODEL_3D,Q(PX,inner_slot),cell_x,cell_y,cell_z);
              pj[1] = calclGet3Dr(MODEL_3D,Q(PY,inner_slot),cell_x,cell_y,cell_z);
              pj[2] = calclGet3Dr(MODEL_3D,Q(PZ,inner_slot),cell_x,cell_y,cell_z);
  #ifdef VISCOELASTIC
              vj[0] = calclGet3Dr(MODEL_3D,Q(VX,inner_slot),cell_x,cell_y,cell_z);
              vj[1] = calclGet3Dr(MODEL_3D,Q(VY,inner_slot),cell_x,cell_y,cell_z);
              vj[2] = calclGet3Dr(MODEL_3D,Q(VZ,inner_slot),cell_x,cell_y,cell_z);
  #endif
              delta_Fj[0] = 0.0;
              delta_Fj[1] = 0.0;
              delta_Fj[2] = 0.0;


              if (calclGet3Di(MODEL_3D, P(ID,inner_slot),cell_x,cell_y,cell_z) > NULL_ID)
                {
                  dij = __distance(pi, pj);
                  if (dij < 2*PARTICLE_RADIUS)
                    {
                      for (int k=0; k<3; k++)
                        {
                          rij[k] = pj[k] - pi[k];
                          enij[k] = rij[k] / dij;
                          delta_n = 2*PARTICLE_RADIUS - dij;
                          delta_Fi[k] += -kn * delta_n * enij[k];
                          delta_Fj[k] +=  kn * delta_n * enij[k];
  #ifdef VISCOELASTIC
                          vij[k] = vi[k] - vj[k];
  #endif
                        }
  #ifdef VISCOELASTIC
                      vnij = 0.0;
                      for (int k=0; k<3; k++)
                        vnij += vij[k] * enij[k];

                      for (int k=0; k<3; k++)
                        {
                          delta_Fi[k] += -etha * vnij * enij[k];
                          delta_Fj[k] +=  etha * vnij * enij[k];
                        }
  #endif
                      // update phase
                      calclSet3Dr(MODEL_3D,Q(FX,inner_slot),cell_x,cell_y,cell_z,calclGetNext3Dr(MODEL_3D,Q(FX,inner_slot),cell_x,cell_y,cell_z) + delta_Fj[0]);
                      calclSet3Dr(MODEL_3D,Q(FY,inner_slot),cell_x,cell_y,cell_z,calclGetNext3Dr(MODEL_3D,Q(FY,inner_slot),cell_x,cell_y,cell_z) + delta_Fj[1]);
                      calclSet3Dr(MODEL_3D,Q(FZ,inner_slot),cell_x,cell_y,cell_z,calclGetNext3Dr(MODEL_3D,Q(FZ,inner_slot),cell_x,cell_y,cell_z) + delta_Fj[2]);
                    } //if dij < 2*PARTICLE_RADIUS
                } // if > NULL_ID


              if (calclGet3Di(MODEL_3D, P(ID,inner_slot),cell_x,cell_y,cell_z) == BORDER_ID)
                {
                  Nj[0] = calclGet3Dr(MODEL_3D,Q(VX,inner_slot),cell_x,cell_y,cell_z);
                  Nj[1] = calclGet3Dr(MODEL_3D,Q(VY,inner_slot),cell_x,cell_y,cell_z);
                  Nj[2] = calclGet3Dr(MODEL_3D,Q(VZ,inner_slot),cell_x,cell_y,cell_z);
  #ifdef VISCOELASTIC
                  vj[0] = 0.0;
                  vj[1] = 0.0;
                  vj[2] = 0.0;
  #endif
                  dij = __pointPlaneDistance(pi, pj, Nj);

                  if (dij < PARTICLE_RADIUS)
                    {

                      __orthogonalProjectedPointToPlane(pi, pj, Nj, pj);

                      for (int k=0; k<3; k++)
                        {
                          rij[k] = pj[k] - pi[k];
                          enij[k] = rij[k] / dij;
                          delta_n = PARTICLE_RADIUS - dij;
                          delta_Fi[k] += -kn * delta_n * enij[k];
  #ifdef VISCOELASTIC
                          vij[k] = vi[k] - vj[k];
  #endif
                        }
  #ifdef VISCOELASTIC

                      vnij = 0.0;
                      for (int k=0; k<3; k++)
                        vnij += vij[k] * enij[k];

                      for (int k=0; k<3; k++)
                        delta_Fi[k] += -etha * vnij * enij[k];
  #endif
                    } //if dij < PARTICLE_RADIUS

                } // if BORDER_ID

            } //for inner_slot AND if

          // update phase
          calclSet3Dr(MODEL_3D,Q(FX,slot),cell_x,cell_y,cell_z,calclGetNext3Dr(MODEL_3D,Q(FX,slot), cell_x,cell_y,cell_z) + delta_Fi[0]);
          calclSet3Dr(MODEL_3D,Q(FY,slot),cell_x,cell_y,cell_z,calclGetNext3Dr(MODEL_3D,Q(FY,slot), cell_x,cell_y,cell_z) + delta_Fi[1]);
          calclSet3Dr(MODEL_3D,Q(FZ,slot),cell_x,cell_y,cell_z,calclGetNext3Dr(MODEL_3D,Q(FZ,slot), cell_x,cell_y,cell_z) + delta_Fi[2]);
        }





    //outer_collision
//    CALreal kn = KN;
//    CALreal etha = ETHA;
//    CALreal pi[3], vi[3], delta_Fi[3];
//    CALreal pj[3], vj[3], Nj[3];
//    CALreal rij[3], dij, enij[3], vij[3], vnij;
//    CALreal delta_n;
    CALint sizeof_X = calclGetNeighborhoodSize();

    for (int slot = 0; slot < MAX_NUMBER_OF_PARTICLES_PER_CELL; slot++)
      if (calclGet3Di(MODEL_3D, P(ID,slot),cell_x,cell_y,cell_z) > NULL_ID)
        {
          pi[0] = calclGet3Dr(MODEL_3D,Q(PX,slot),cell_x,cell_y,cell_z);
          pi[1] = calclGet3Dr(MODEL_3D,Q(PY,slot),cell_x,cell_y,cell_z);
          pi[2] = calclGet3Dr(MODEL_3D,Q(PZ,slot),cell_x,cell_y,cell_z);
  #ifdef VISCOELASTIC
          vi[0] = calclGet3Dr(MODEL_3D,Q(VX,slot),cell_x,cell_y,cell_z);
          vi[1] = calclGet3Dr(MODEL_3D,Q(VY,slot),cell_x,cell_y,cell_z);
          vi[2] = calclGet3Dr(MODEL_3D,Q(VZ,slot),cell_x,cell_y,cell_z);
  #endif
          delta_Fi[0] = 0.0;
          delta_Fi[1] = 0.0;
          delta_Fi[2] = 0.0;

          // outer particle-particle collision
          for (int n = 1; n< sizeof_X; n++)
            {
              for (int outer_slot=0; outer_slot<MAX_NUMBER_OF_PARTICLES_PER_CELL; outer_slot++)
                {
                  if (calclGetX3Di(MODEL_3D, P(ID,outer_slot),cell_x,cell_y,cell_z,n) == NULL_ID)
                    continue;

                  pj[0] = calclGetX3Dr(MODEL_3D,Q(PX,outer_slot),cell_x,cell_y,cell_z,n);
                  pj[1] = calclGetX3Dr(MODEL_3D,Q(PY,outer_slot),cell_x,cell_y,cell_z,n);
                  pj[2] = calclGetX3Dr(MODEL_3D,Q(PZ,outer_slot),cell_x,cell_y,cell_z,n);
  #ifdef VISCOELASTIC
                  vj[0] = calclGetX3Dr(MODEL_3D,Q(VX,outer_slot),cell_x,cell_y,cell_z,n);
                  vj[1] = calclGetX3Dr(MODEL_3D,Q(VY,outer_slot),cell_x,cell_y,cell_z,n);
                  vj[2] = calclGetX3Dr(MODEL_3D,Q(VZ,outer_slot),cell_x,cell_y,cell_z,n);
  #endif
                  if (calclGetX3Di(MODEL_3D, P(ID,outer_slot),cell_x,cell_y,cell_z,n) > NULL_ID)
                    {
                      dij = __distance(pi, pj);
                      if (dij < 2*PARTICLE_RADIUS)
                        {
                          for (int k=0; k<3; k++)
                            {
                              rij[k] = pj[k] - pi[k];
                              enij[k] = rij[k] / dij;
                              delta_n = 2*PARTICLE_RADIUS - dij;
                              delta_Fi[k] += -kn * delta_n * enij[k];
  #ifdef VISCOELASTIC
                              vij[k] = vi[k] - vj[k];
  #endif
                            }
  #ifdef VISCOELASTIC
                          vnij = 0.0;
                          for (int k=0; k<3; k++)
                            vnij += vij[k] * enij[k];

                          for (int k=0; k<3; k++)
                            delta_Fi[k] += -etha * vnij * enij[k];
  #endif
                        } //if dij < 2*PARTICLE_RADIUS
                    } // if > NULL_ID

                  if (calclGetX3Di(MODEL_3D, P(ID,outer_slot),cell_x,cell_y,cell_z,n) == BORDER_ID)
                    {
                      Nj[0] = calclGetX3Dr(MODEL_3D,Q(VX,outer_slot),cell_x,cell_y,cell_z,n);
                      Nj[1] = calclGetX3Dr(MODEL_3D,Q(VY,outer_slot),cell_x,cell_y,cell_z,n);
                      Nj[2] = calclGetX3Dr(MODEL_3D,Q(VZ,outer_slot),cell_x,cell_y,cell_z,n);
      #ifdef VISCOELASTIC
                      vj[0] = 0.0;
                      vj[1] = 0.0;
                      vj[2] = 0.0;
      #endif
                      dij = __pointPlaneDistance(pi, pj, Nj);
                      if (dij < PARTICLE_RADIUS)
                        {
                          __orthogonalProjectedPointToPlane(pi, pj, Nj, pj);
                          for (int k=0; k<3; k++)
                            {
                              rij[k] = pj[k] - pi[k];
                              enij[k] = rij[k] / dij;
                              delta_n = PARTICLE_RADIUS - dij;
                              delta_Fi[k] += -kn * delta_n * enij[k];
      #ifdef VISCOELASTIC
                              vij[k] = vi[k] - vj[k];
      #endif
                            }
      #ifdef VISCOELASTIC
                          vnij = 0.0;
                          for (int k=0; k<3; k++)
                            vnij += vij[k] * enij[k];

                          for (int k=0; k<3; k++)
                            delta_Fi[k] += -etha * vnij * enij[k];
      #endif
                        } //if dij < PARTICLE_RADIUS
                    } // if BORDER_ID

                } // for outer_slot
            } //for n = 1 ...

          // update phase
          calclSet3Dr(MODEL_3D,Q(FX,slot),cell_x,cell_y,cell_z,calclGetNext3Dr(MODEL_3D,Q(FX,slot), cell_x,cell_y,cell_z) + delta_Fi[0]);
          calclSet3Dr(MODEL_3D,Q(FY,slot),cell_x,cell_y,cell_z,calclGetNext3Dr(MODEL_3D,Q(FY,slot), cell_x,cell_y,cell_z) + delta_Fi[1]);
          calclSet3Dr(MODEL_3D,Q(FZ,slot),cell_x,cell_y,cell_z,calclGetNext3Dr(MODEL_3D,Q(FZ,slot), cell_x,cell_y,cell_z) + delta_Fi[2]);
        }


}
