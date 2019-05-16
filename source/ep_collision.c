#include <ep_collision.h>
#include <ep_utils.h>
#include <math.h>

#include <model.h>

void inner_collision(struct CALModel3D* ca, int cell_x, int cell_y, int cell_z)
{
  CALreal kn = KN;
  CALreal etha = ETHA;
  vec3 pi, vi, delta_Fi;
  vec3 pj, vj, delta_Fj, Nj;
  vec3 rij, enij, vij;
  CALreal  dij, vnij;
  CALreal delta_n;

  for (int slot = 0; slot < MAX_NUMBER_OF_PARTICLES_PER_CELL; slot++)
    if (calGet3Di(ca, Q.ID[slot],cell_x,cell_y,cell_z) > NULL_ID)
      {
        calGet3Dr_vec3(ca, Q.px[slot], Q.py[slot], Q.pz[slot], cell_x,cell_y,cell_z, &pi );
#ifdef VISCOELASTIC
        calGet3Dr_vec3(ca, Q.vx[slot], Q.vy[slot], Q.vz[slot], cell_x,cell_y,cell_z, &vi );
#endif
        delta_Fi[0] = 0.0;
        delta_Fi[1] = 0.0;
        delta_Fi[2] = 0.0;

        // inner particle-particle collision
        for (int inner_slot=slot+1; inner_slot<MAX_NUMBER_OF_PARTICLES_PER_CELL; inner_slot++)
          {
            if (calGet3Di(ca, Q.ID[inner_slot],cell_x,cell_y,cell_z) == NULL_ID)
              continue;

            calGet3Dr_vec3(ca, Q.px[inner_slot], Q.py[inner_slot], Q.pz[inner_slot], cell_x,cell_y,cell_z, &pj );
#ifdef VISCOELASTIC
            calGet3Dr_vec3(ca, Q.vx[inner_slot], Q.vy[inner_slot], Q.vz[inner_slot], cell_x,cell_y,cell_z, &vj );
#endif
            delta_Fj[0] = 0.0;
            delta_Fj[1] = 0.0;
            delta_Fj[2] = 0.0;

            if (calGet3Di(ca, Q.ID[inner_slot],cell_x,cell_y,cell_z) > NULL_ID)
              {
                dij = distance(pi, pj);
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
                    calSet3Dr(ca,Q.Fx[inner_slot],cell_x,cell_y,cell_z,calGetNext3Dr(ca,Q.Fx[inner_slot],cell_x,cell_y,cell_z) + delta_Fj[0]);
                    calSet3Dr(ca,Q.Fy[inner_slot],cell_x,cell_y,cell_z,calGetNext3Dr(ca,Q.Fy[inner_slot],cell_x,cell_y,cell_z) + delta_Fj[1]);
                    calSet3Dr(ca,Q.Fz[inner_slot],cell_x,cell_y,cell_z,calGetNext3Dr(ca,Q.Fz[inner_slot],cell_x,cell_y,cell_z) + delta_Fj[2]);
                  } //if dij < 2*PARTICLE_RADIUS
              } // if > NULL_ID


            if (calGet3Di(ca, Q.ID[inner_slot],cell_x,cell_y,cell_z) == BORDER_ID)
              {
                calGet3Dr_vec3(ca, Q.vx[inner_slot], Q.vy[inner_slot], Q.vz[inner_slot], cell_x,cell_y,cell_z, &Nj );
#ifdef VISCOELASTIC
                vj[0] = 0.0;
                vj[1] = 0.0;
                vj[2] = 0.0;
#endif
                dij = pointPlaneDistance(pi, pj, Nj);
                if (dij < PARTICLE_RADIUS)
                  {
                    orthogonalProjectedPointToPlane(pi, pj, Nj, pj);
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
        calSet3Dr(ca,Q.Fx[slot],cell_x,cell_y,cell_z,calGetNext3Dr(ca,Q.Fx[slot], cell_x,cell_y,cell_z) + delta_Fi[0]);
        calSet3Dr(ca,Q.Fy[slot],cell_x,cell_y,cell_z,calGetNext3Dr(ca,Q.Fy[slot], cell_x,cell_y,cell_z) + delta_Fi[1]);
        calSet3Dr(ca,Q.Fz[slot],cell_x,cell_y,cell_z,calGetNext3Dr(ca,Q.Fz[slot], cell_x,cell_y,cell_z) + delta_Fi[2]);
      }
}

void outer_collision(struct CALModel3D* ca, int cell_x, int cell_y, int cell_z)
{
  CALreal kn = KN;
  CALreal etha = ETHA;
  CALreal pi[3], vi[3], delta_Fi[3];
  CALreal pj[3], vj[3], Nj[3];
  CALreal rij[3], dij, enij[3], vij[3], vnij;
  CALreal delta_n;

  for (int slot = 0; slot < MAX_NUMBER_OF_PARTICLES_PER_CELL; slot++)
    if (calGet3Di(ca, Q.ID[slot],cell_x,cell_y,cell_z) > NULL_ID)
      {
        calGet3Dr_vec3(ca, Q.px[slot], Q.py[slot], Q.pz[slot], cell_x,cell_y,cell_z, &pi );
#ifdef VISCOELASTIC
        calGet3Dr_vec3(ca, Q.vx[slot], Q.vy[slot], Q.vz[slot], cell_x,cell_y,cell_z, &vi );
#endif
        delta_Fi[0] = 0.0;
        delta_Fi[1] = 0.0;
        delta_Fi[2] = 0.0;

        // outer particle-particle collision
        for (int n = 1; n<ca->sizeof_X; n++)
          {
            for (int outer_slot=0; outer_slot<MAX_NUMBER_OF_PARTICLES_PER_CELL; outer_slot++)
              {
                if (calGetX3Di(ca, Q.ID[outer_slot],cell_x,cell_y,cell_z,n) == NULL_ID)
                  continue;

                calGetX3Dr_vec3(ca, Q.px[outer_slot], Q.py[outer_slot], Q.pz[outer_slot], cell_x,cell_y,cell_z, &pj, n );
#ifdef VISCOELASTIC
                calGetX3Dr_vec3(ca, Q.vx[outer_slot], Q.vy[outer_slot], Q.vz[outer_slot], cell_x,cell_y,cell_z, &vj, n );
#endif
                if (calGetX3Di(ca, Q.ID[outer_slot],cell_x,cell_y,cell_z,n) > NULL_ID)
                  {
                    dij = distance(pi, pj);
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

                if (calGetX3Di(ca, Q.ID[outer_slot],cell_x,cell_y,cell_z,n) == BORDER_ID)
                  {
                    calGetX3Dr_vec3_slot(ca, Q.vx, Q.vy, Q.vz, outer_slot, cell_x,cell_y,cell_z, &Nj, n );

    #ifdef VISCOELASTIC
                    vj[0] = 0.0;
                    vj[1] = 0.0;
                    vj[2] = 0.0;
    #endif
                    dij = pointPlaneDistance(pi, pj, Nj);
                    if (dij < PARTICLE_RADIUS)
                      {
                        orthogonalProjectedPointToPlane(pi, pj, Nj, pj);
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
        calSet3Dr(ca,Q.Fx[slot],cell_x,cell_y,cell_z,calGetNext3Dr(ca,Q.Fx[slot], cell_x,cell_y,cell_z) + delta_Fi[0]);
        calSet3Dr(ca,Q.Fy[slot],cell_x,cell_y,cell_z,calGetNext3Dr(ca,Q.Fy[slot], cell_x,cell_y,cell_z) + delta_Fi[1]);
        calSet3Dr(ca,Q.Fz[slot],cell_x,cell_y,cell_z,calGetNext3Dr(ca,Q.Fz[slot], cell_x,cell_y,cell_z) + delta_Fi[2]);
      }
}
