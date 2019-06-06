#ifndef COMMON_HPP
#define COMMON_HPP

#ifdef OMP
#include <OpenCAL-OMP/cal3D.h>
#include <OpenCAL-OMP/cal3DRun.h>
#include <OpenCAL-OMP/cal3DIO.h>
#include <OpenCAL-OMP/cal3DUnsafe.h>
#else
#include <OpenCAL/cal3D.h>
#include <OpenCAL/cal3DRun.h>
#include <OpenCAL/cal3DIO.h>
#include <OpenCAL/cal3DUnsafe.h>
#endif
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>


typedef CALreal vec3[3];
static const vec3 G_dir =  {0.0,0.0, -1.0};
static const CALreal G = 9.81;

#define ABS(N) ((N<0)?(-N):(N))



#define DIM 3

#define WALL_WEST 5 //x
#define WALL_EAST 4 //x
#define WALL_SOUTH 3 //y
#define WALL_NORTH 2 //y
#define WALL_FRONT 1 //z
#define WALL_REAR 0 //z

#define N_WALLS 6

//#define BORDER_TO_WALL(B) ({int idx; idx = B+N_WALLS; idx;})

#define swap(x,y) { x = x + y; y = x - y; x = x - y; }


// PHYSICAL CONSTANTS AND FLAGS

#define GRAVITY
//#define G 9.81
#define VISCOELASTIC
#define ETHA 0.0015
#define KA 0.003 //boh
#define FRICTION_COEF_PP 0.003 //boh
#define KN_PP 1000//50000
#define AL_PP 0.001 //boh

#define FRICTION_COEF_PW 0.003 //boh
#define KN_PW 1000//50000
#define AL_PW 0.001 //boh

#define DENSITY (7400.0)

#define PIG 3.141592653589793238
#define PIG43  (PIG *(3.0/4.0))
#define PARTICLE_RADIUS 0.0005
#define PARTICLE_VOLUME  ((PIG43 * PARTICLE_RADIUS) * (PIG43 * PARTICLE_RADIUS) * (PIG43 * PARTICLE_RADIUS))

static const CALreal PARTICLE_MASS = (DENSITY * PARTICLE_VOLUME);

#define DM (2*PARTICLE_RADIUS)
static CALreal dm_2 = DM * DM;


// Particle mass, radius and volume
//#define PI 3.14159265358979
//#define PARTICLE_MASS 0.0001 //1.0471975511966E-06 // -> density = 2000

//#define PARTICLE_VOLUME ((4.0/3.0)*PI*PARTICLE_RADIUS*PARTICLE_RADIUS*PARTICLE_RADIUS)

// Cell side [m], volume [m^3] and max occupancy volume [m^3] according to Kepler's conjecture
#define CELL_SIDE (0.002)
#define CELL_VOLUME (CELL_SIDE*CELL_SIDE*CELL_SIDE)
#define KEPLER_OCCUPANCY_FACTOR (0.74)
#define MAX_OCCUPANCY_VOLUME ((KEPLER_OCCUPANCY_FACTOR)*(CELL_VOLUME))

// Max number of particles per cell (slots) according to Kepler's conjecture
//#define MAX_NUMBER_OF_PARTICLES_PER_CELL  (int)(((MAX_OCCUPANCY_VOLUME)/(PARTICLE_VOLUME))+1)
#define MAX_NUMBER_OF_PARTICLES_PER_CELL 10

// SLOT (or PARTICLE) IDs
#define BORDER_ID -1            // BORDER ID
#define NULL_ID 0               // NO PARTICLE IN SLOT
#define DEFAULT_PARTICLE_ID 1   // DEFAULT PARTICLE ID

// Domain dimensions in m and in cells along x, y and z directions
#define X 0.04
#define Y 0.04
#define Z 0.04
#define X_CELLS (int)((X)/(CELL_SIDE))
#define Y_CELLS (int)((Y)/(CELL_SIDE))
#define Z_CELLS (int)((Z)/(CELL_SIDE))

// Particles are randomly distributed on the CELL_FILL_RATE*MAX_NUMBER_OF_PARTICLES_PER_CELL top layers
#define TOP_LAYERS      (Z_CELLS) - 0.8 * (Z_CELLS)
#define CELL_FILL_RATE  0.75 // 0.59 // 1.0/(MAX_NUMBER_OF_PARTICLES_PER_CELL)

// PHYSICAL TIME AND COMPUTATIONAL STEPS
#define TOTAL_SIMULATION_TIME 1.0 //0.1 //[s]
#define DELTA_T 0.0000005
//#define DELTA_T (0.1 * sqrt(PARTICLE_MASS/KN_PP)) //[s]
#define STEPS (int)((double)(TOTAL_SIMULATION_TIME)/(double)(DELTA_T))
//#define STEPS 10
#define INTEGRITY_CHECK_STEPS STEPS

#define DELTA_T_1_2 (DELTA_T/2)
#define PARTICLE_MASS_DELTA_T_1_2 (DELTA_T_1_2 / PARTICLE_MASS)

#define MOMENT_INERTIA ((0.4 * PARTICLE_MASS) * (0.4 * PARTICLE_MASS))

#define MOMENT_INERTIA_DELTA_T_1_2 (DELTA_T_1_2 / MOMENT_INERTIA)



#endif
