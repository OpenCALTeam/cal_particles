#include <sim_stop.h>
#include <ep_utils.h>

CALbyte caminalu(struct CALModel3D* modello)
{
  CALint step = a_simulazioni->step;

  if (step < cnfg.STEPS)
      return CAL_FALSE;

  return CAL_TRUE;
}
