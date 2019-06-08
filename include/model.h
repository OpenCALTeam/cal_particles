#ifndef MODEL_H
#define MODEL_H

#include <calmodel_utils.h>
#include <vector_utils.h>
#include <collision_pp.h>

//SUBSTATES
struct Substates
{
  struct CALSubstate3Dr **Fx;
  struct CALSubstate3Dr **Fy;
  struct CALSubstate3Dr **Fz;
  struct CALSubstate3Dr **momentx;
  struct CALSubstate3Dr **momenty;
  struct CALSubstate3Dr **momentz;
  struct CALSubstate3Dr **px;
  struct CALSubstate3Dr **py;
  struct CALSubstate3Dr **pz;
  struct CALSubstate3Dr **thetax;
  struct CALSubstate3Dr **thetay;
  struct CALSubstate3Dr **thetaz;
  struct CALSubstate3Dr **vx;
  struct CALSubstate3Dr **vy;
  struct CALSubstate3Dr **vz;
  struct CALSubstate3Dr **wx;
  struct CALSubstate3Dr **wy;
  struct CALSubstate3Dr **wz;
  struct CALSubstate3Di **ID;
  struct CALSubstate3Di *nP;
};


// Main objcts
//extern struct CALModel3D* u_modellu;
//extern struct Substates Q;
//extern struct CALRun3D* a_simulazioni;
//extern CALint initial_nummber_of_particles;
//extern CALreal elapsed_time;
//extern struct Collisions collisions; //da inizializzare

struct CALModel3D* u_modellu;
struct CALRun3D* a_simulazioni;
struct Substates Q;
CALint initial_nummber_of_particles;
CALreal elapsed_time;
struct Collisions collisions; //da inizializzare
struct Wall walls[N_WALLS];

// Functions
void run();

#endif /* MODEL_H */
