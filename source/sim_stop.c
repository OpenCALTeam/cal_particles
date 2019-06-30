#include <sim_stop.h>
#include <ep_utils.h>
#include <restart_manager.h>

CALbyte caminalu(struct CALModel3D* modello)
{
  CALint step = a_simulazioni->step;

  if(step == STEP_SNAPSHOT)
  {
      char path[256];
      sprintf(path, "./snapshots/particle_configuration%d.dat", step);
      saveConfigurationParticles(modello, step, path);

      sprintf(path, "./snapshots/collisions_wall_configuration%d.dat", step);
      saveConfigurationCollisionsWall(&collisions, step, path);

      sprintf(path, "./snapshots/collisions_particles_configuration%d.dat", step);
      saveConfigurationCollisionsParticles(&collisions, step, path);

  }

  if (step < cnfg.STEPS)
      return CAL_FALSE;

  return CAL_TRUE;
}
