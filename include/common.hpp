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
#include <limits.h>

#define TEST_CASE 1
#define INTEGRATION_METHOD 1

#define TEST_CASE_SUPERBALL 0
#define TEST_CASE_TWO_PP_VEL 1
#define TEST_CASE_TWO_PP_VEL_OMEGA 2

#define TEST_CASE_VEL_WALL 3
#define TEST_CASE_VEL_OMEGA_WALL 4
#define TEST_CASE_VEL_WALL_VERTICAL 5
#define TEST_CASE_VEL_WALL_DIAGONAL 6
#define TEST_CASE_VEL_WALL_DIAGONAL_GRAVITY 7

#define OPTIMITAZION_ACTIVE_CELLS 1


#define LEAP_FROG 1
#define EULER_FORWARD_BACKWARD 2



#if TEST_CASE == TEST_CASE_SUPERBALL
#define TOTAL_NUMBER_PARTICLE 1
#elif TEST_CASE == TEST_CASE_TWO_PP_VEL || TEST_CASE == TEST_CASE_TWO_PP_VEL_OMEGA
#define TOTAL_NUMBER_PARTICLE 2
#else
#define TOTAL_NUMBER_PARTICLE 300
#endif



typedef CALreal vec3[3];
static const vec3 G_dir =  {0.0,0.0, -1.0};
//static const CALreal G = 0;

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

#if TEST_CASE == TEST_CASE_SUPERBALL || TEST_CASE == TEST_CASE_VEL_WALL_DIAGONAL

//#define KA 0.28571 //boh
//#define FRICTION_COEF_PP 1000
//#define KN_PP 14000
//#define AL_PP 1

//#define FRICTION_COEF_PW 1000
//#define KN_PW 14000
//#define AL_PW 0

//static const CALreal PARTICLE_MASS = 1.0472e-6;
//#define PARTICLE_RADIUS 0.0005

//#define DELTA_T 0.0000172/50

#elif TEST_CASE == TEST_CASE_TWO_PP_VEL_OMEGA || TEST_CASE == TEST_CASE_TWO_PP_VEL

//#define KA 0.28571
//#define FRICTION_COEF_PP 1000
//#define KN_PP 5000
//#define AL_PP 0

//#define FRICTION_COEF_PW 1000
//#define KN_PW 5000
//#define AL_PW 0


//#define PARTICLE_RADIUS 5e-4

//#define DENSITY (7500.0)

//#define PIG 3.141592653589793238
//#define PIG43  (PIG *(3.0/4.0))




//#define DELTA_T 5e-7


#endif





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

#define PIG 3.141592653589793238
#define PIG43  (PIG *(3.0/4.0))


#endif
