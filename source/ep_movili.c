#include <ep_movili.h>
#include <ep_physics.h>
#include <ep_utils.h>

void movili(struct CALModel3D* ca, int cell_x, int cell_y, int cell_z)
{
  if (!ncestiArmenuNaParticella(ca, cell_x, cell_y, cell_z, 0))
    return;

  vec3 F0, p0,v0,pf,vf;


  for (int slot = 0; slot < MAX_NUMBER_OF_PARTICLES_PER_CELL; slot++)
    if (calGet3Di(ca, Q.ID[slot],cell_x,cell_y,cell_z) > NULL_ID)
      {
        calGet3Dr_vec3(ca, Q.Fx[slot], Q.Fy[slot], Q.Fz[slot], cell_x,cell_y,cell_z, &F0 );

        calGet3Dr_vec3(ca, Q.px[slot], Q.py[slot], Q.pz[slot], cell_x,cell_y,cell_z, &p0 );
        calGet3Dr_vec3(ca, Q.px[slot], Q.py[slot], Q.pz[slot], cell_x,cell_y,cell_z, &pf );

        calGet3Dr_vec3(ca, Q.vx[slot], Q.vy[slot], Q.vz[slot], cell_x,cell_y,cell_z, &v0 );
        calGet3Dr_vec3(ca, Q.vx[slot], Q.vy[slot], Q.vz[slot], cell_x,cell_y,cell_z, &vf );

        applyForce(F0, p0, v0, PARTICLE_MASS, DELTA_T, pf, vf);

        calSet3Dr_vec3(ca, Q.px[slot], Q.py[slot], Q.pz[slot], cell_x,cell_y,cell_z, pf );
        calSet3Dr_vec3(ca, Q.vx[slot], Q.vy[slot], Q.vz[slot], cell_x,cell_y,cell_z, vf );
      }
}
