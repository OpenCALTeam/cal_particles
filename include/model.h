#ifndef MODEL_H
#define MODEL_H

#include <calmodel_utils.h>

//SUBSTATES
struct Substates
{
  struct CALSubstate3Dr **Fx;
  struct CALSubstate3Dr **Fy;
  struct CALSubstate3Dr **Fz;
  struct CALSubstate3Dr **px;
  struct CALSubstate3Dr **py;
  struct CALSubstate3Dr **pz;
  struct CALSubstate3Dr **vx;
  struct CALSubstate3Dr **vy;
  struct CALSubstate3Dr **vz;
  struct CALSubstate3Di **ID;
};

// Main objcts
extern struct CALModel3D* u_modellu;
extern struct Substates Q;
extern struct CALRun3D* a_simulazioni;
extern CALint initial_nummber_of_particles;
extern CALreal elapsed_time;

// Functions
void partilu();

#endif /* MODEL_H */
