#include <boundary.h>
#include <ep_collision.h>
#include <ep_movili_cazzu.h>
#include <ep_physics.h>
#include <ep_utils.h>
#include <init.h>
#include <model.h>
#include <utils_io.h>
#include <sim_stop.h>
#include <stdlib.h>

#include<init_pp.h>

#include <globalFunctions.h>


void run()
{
  u_modellu = calCADef3D(X_CELLS,Y_CELLS,Z_CELLS,CAL_MOORE_NEIGHBORHOOD_3D,CAL_SPACE_TOROIDAL,CAL_NO_OPT);

  Q.Fx = (struct CALSubstate3Dr**)malloc(sizeof(struct CALSubstate3Dr*)*MAX_NUMBER_OF_PARTICLES_PER_CELL);
  Q.Fy = (struct CALSubstate3Dr**)malloc(sizeof(struct CALSubstate3Dr*)*MAX_NUMBER_OF_PARTICLES_PER_CELL);
  Q.Fz = (struct CALSubstate3Dr**)malloc(sizeof(struct CALSubstate3Dr*)*MAX_NUMBER_OF_PARTICLES_PER_CELL);

  Q.momentx = (struct CALSubstate3Dr**)malloc(sizeof(struct CALSubstate3Dr*)*MAX_NUMBER_OF_PARTICLES_PER_CELL);
  Q.momenty = (struct CALSubstate3Dr**)malloc(sizeof(struct CALSubstate3Dr*)*MAX_NUMBER_OF_PARTICLES_PER_CELL);
  Q.momentz = (struct CALSubstate3Dr**)malloc(sizeof(struct CALSubstate3Dr*)*MAX_NUMBER_OF_PARTICLES_PER_CELL);

  Q.px = (struct CALSubstate3Dr**)malloc(sizeof(struct CALSubstate3Dr*)*MAX_NUMBER_OF_PARTICLES_PER_CELL);
  Q.py = (struct CALSubstate3Dr**)malloc(sizeof(struct CALSubstate3Dr*)*MAX_NUMBER_OF_PARTICLES_PER_CELL);
  Q.pz = (struct CALSubstate3Dr**)malloc(sizeof(struct CALSubstate3Dr*)*MAX_NUMBER_OF_PARTICLES_PER_CELL);

  Q.thetax = (struct CALSubstate3Dr**)malloc(sizeof(struct CALSubstate3Dr*)*MAX_NUMBER_OF_PARTICLES_PER_CELL);
  Q.thetay = (struct CALSubstate3Dr**)malloc(sizeof(struct CALSubstate3Dr*)*MAX_NUMBER_OF_PARTICLES_PER_CELL);
  Q.thetaz = (struct CALSubstate3Dr**)malloc(sizeof(struct CALSubstate3Dr*)*MAX_NUMBER_OF_PARTICLES_PER_CELL);

  Q.vx = (struct CALSubstate3Dr**)malloc(sizeof(struct CALSubstate3Dr*)*MAX_NUMBER_OF_PARTICLES_PER_CELL);
  Q.vy = (struct CALSubstate3Dr**)malloc(sizeof(struct CALSubstate3Dr*)*MAX_NUMBER_OF_PARTICLES_PER_CELL);
  Q.vz = (struct CALSubstate3Dr**)malloc(sizeof(struct CALSubstate3Dr*)*MAX_NUMBER_OF_PARTICLES_PER_CELL);

  Q.wx = (struct CALSubstate3Dr**)malloc(sizeof(struct CALSubstate3Dr*)*MAX_NUMBER_OF_PARTICLES_PER_CELL);
  Q.wy = (struct CALSubstate3Dr**)malloc(sizeof(struct CALSubstate3Dr*)*MAX_NUMBER_OF_PARTICLES_PER_CELL);
  Q.wz = (struct CALSubstate3Dr**)malloc(sizeof(struct CALSubstate3Dr*)*MAX_NUMBER_OF_PARTICLES_PER_CELL);

  Q.ID = (struct CALSubstate3Di**)malloc(sizeof(struct CALSubstate3Di*)*MAX_NUMBER_OF_PARTICLES_PER_CELL);

  for(int slot=0;slot<MAX_NUMBER_OF_PARTICLES_PER_CELL;slot++)
    {
      Q.Fx[slot] = calAddSubstate3Dr(u_modellu);
      Q.Fy[slot] = calAddSubstate3Dr(u_modellu);
      Q.Fz[slot] = calAddSubstate3Dr(u_modellu);

      Q.momentx[slot] = calAddSubstate3Dr(u_modellu);
      Q.momenty[slot] = calAddSubstate3Dr(u_modellu);
      Q.momentz[slot] = calAddSubstate3Dr(u_modellu);

      Q.px[slot] = calAddSubstate3Dr(u_modellu);
      Q.py[slot] = calAddSubstate3Dr(u_modellu);
      Q.pz[slot] = calAddSubstate3Dr(u_modellu);

      Q.thetax[slot] = calAddSubstate3Dr(u_modellu);
      Q.thetay[slot] = calAddSubstate3Dr(u_modellu);
      Q.thetaz[slot] = calAddSubstate3Dr(u_modellu);

      Q.vx[slot] = calAddSubstate3Dr(u_modellu);
      Q.vy[slot] = calAddSubstate3Dr(u_modellu);
      Q.vz[slot] = calAddSubstate3Dr(u_modellu);

      Q.wx[slot] = calAddSubstate3Dr(u_modellu);
      Q.wy[slot] = calAddSubstate3Dr(u_modellu);
      Q.wz[slot] = calAddSubstate3Dr(u_modellu);

      Q.ID[slot] = calAddSubstate3Di(u_modellu);

      calInitSubstate3Dr(u_modellu,Q.Fx[slot],0.0);
      calInitSubstate3Dr(u_modellu,Q.Fy[slot],0.0);
      calInitSubstate3Dr(u_modellu,Q.Fz[slot],0.0);

      calInitSubstate3Dr(u_modellu,Q.momentx[slot],0.0);
      calInitSubstate3Dr(u_modellu,Q.momenty[slot],0.0);
      calInitSubstate3Dr(u_modellu,Q.momentz[slot],0.0);

      calInitSubstate3Dr(u_modellu,Q.px[slot],0.0);
      calInitSubstate3Dr(u_modellu,Q.py[slot],0.0);
      calInitSubstate3Dr(u_modellu,Q.pz[slot],0.0);

      calInitSubstate3Dr(u_modellu,Q.thetax[slot],0.0);
      calInitSubstate3Dr(u_modellu,Q.thetay[slot],0.0);
      calInitSubstate3Dr(u_modellu,Q.thetaz[slot],0.0);

      calInitSubstate3Dr(u_modellu,Q.vx[slot],0.0);
      calInitSubstate3Dr(u_modellu,Q.vy[slot],0.0);
      calInitSubstate3Dr(u_modellu,Q.vz[slot],0.0);

      calInitSubstate3Dr(u_modellu,Q.wx[slot],0.0);
      calInitSubstate3Dr(u_modellu,Q.wy[slot],0.0);
      calInitSubstate3Dr(u_modellu,Q.wz[slot],0.0);
      calInitSubstate3Di(u_modellu,Q.ID[slot],NULL_ID);
    }


  // Boundary
  boundaryCellsSerial(u_modellu);

  // Initial conditions
  initial_nummber_of_particles = 0;
  elapsed_time = 0.0;
//  mmiscali_nta_cella_seriale(u_modellu);
//  cancella_particelle_in_urto(u_modellu);

  for (int i = 0; i < 300; ++i) {
      addRandomParticlePosition(u_modellu, &initial_nummber_of_particles);
  }



  printf( "initial_nummber_of_particles = %d\n", initial_nummber_of_particles);

  collisions.N_PARTICLES = initial_nummber_of_particles;
  initCollisionsPP(&collisions);

  initWalls(walls);
  initCollisionsPW(&collisions);

  // Simulation
  a_simulazioni = calRunDef3D(u_modellu,0,CAL_RUN_LOOP,CAL_UPDATE_EXPLICIT);
  calRunAddGlobalTransitionFunc3D(a_simulazioni, transitionFunction);
  calRunAddStopConditionFunc3D(a_simulazioni, caminalu);

  calRunAddFinalizeFunc3D(a_simulazioni, cleanupCollisions);


#ifdef VERBOSE
  printf("The 3D particles computational model\n");
#ifdef OMP
  printf("OpenMP parallel execution enabled!\n");
#endif
  printf("A system of %d particles will be simulated for %f s, subdivided in %d steps, each one corresponding to %f s\n", initial_nummber_of_particles, TOTAL_SIMULATION_TIME, STEPS, DELTA_T);
#endif
}
