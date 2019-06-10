#ifndef EP_PHYSICS_H
#define EP_PHYSICS_H

#include <model.h>

//void applyForce(CALreal* F, CALreal* p0, CALreal* v0, CALreal m, CALreal t, CALreal* pf, CALreal* vf);
void applyForce(struct CALModel3D* ca, int cell_x, int cell_y, int cell_z);
void leap_frog_velocity(struct CALModel3D* ca, int cell_x, int cell_y, int cell_z);
void leap_frog_positions(struct CALModel3D* ca, int cell_x, int cell_y, int cell_z);

void euler_backward_forward_position(struct CALModel3D* ca, int cell_x, int cell_y, int cell_z);
void euler_backward_forward_velocity(struct CALModel3D* ca,int cell_x, int cell_y, int cell_z);
#endif
