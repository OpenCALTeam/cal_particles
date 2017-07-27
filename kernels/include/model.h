#ifndef model_h
#define model_h

// PHYSICAL CONSTANTS AND FLAGS
#define KN 100//50000
#define GRAVITY
#define G 9.81
#define VISCOELASTIC
#define ETHA 0.0015

// Particle mass, radius and volume
#define PI 3.14159265358979
#define PARTICLE_MASS 0.0001 //1.0471975511966E-06 // -> density = 2000
#define PARTICLE_RADIUS 0.0005
#define PARTICLE_VOLUME ((4.0/3.0)*PI*PARTICLE_RADIUS*PARTICLE_RADIUS*PARTICLE_RADIUS)

// Cell side [m], volume [m^3] and max occupancy volume [m^3] according to Kepler's conjecture
#define CELL_SIDE (0.002)
#define CELL_VOLUME (CELL_SIDE*CELL_SIDE*CELL_SIDE)
#define KEPLER_OCCUPANCY_FACTOR (0.74)
#define MAX_OCCUPANCY_VOLUME ((KEPLER_OCCUPANCY_FACTOR)*(CELL_VOLUME))

// Max number of particles per cell (slots) according to Kepler's conjecture
#define MAX_NUMBER_OF_PARTICLES_PER_CELL  (int)(((MAX_OCCUPANCY_VOLUME)/(PARTICLE_VOLUME))+1)

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
#define DELTA_T (0.1 * sqrt(PARTICLE_MASS/KN)) //[s]
#define STEPS (int)((double)(TOTAL_SIMULATION_TIME)/(double)(DELTA_T))
#define INTEGRITY_CHECK_STEPS STEPS


#define ID 0
#define P(S,slot) ((S)+((slot)))

#define FX 0
#define FY 1
#define FZ 2
#define PX 3
#define PY 4
#define PZ 5
#define VX 6
#define VY 7
#define VZ 8
#define NUM_SUBSTATES 9
#define Q(S,slot) ((S)+((slot)*(NUM_SUBSTATES)))









#endif
