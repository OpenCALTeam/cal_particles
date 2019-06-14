#ifndef ENERGY_H
#define ENERGY_H

#include<model.h>

void compute_total_energy(struct CALModel3D* ca, int cell_x, int cell_y, int cell_z);
void compute_kinetic_energy(struct CALModel3D* ca, int cell_x, int cell_y, int cell_z);
void compute_rotational_energy(struct CALModel3D* ca, int cell_x, int cell_y, int cell_z);

void compute_potential_energy(struct CALModel3D* ca, int cell_x, int cell_y, int cell_z);

void compute_elastic_energy_pp(struct Collisions* collisions, struct CollisionPP* collision_ij,
                               const int i, const int j, CALreal overlap, CALreal dij,
                               vec3  pi, vec3 pj, vec3 theta_i, vec3 theta_j);

void total_elastic_energy_pp(struct CALModel3D* ca, int cell_x, int cell_y, int cell_z);
void total_elastic_energy_pw(struct CALModel3D* ca, int cell_x, int cell_y, int cell_z);

void setToZeroEnergy (struct CALModel3D* ca, int cell_x, int cell_y, int cell_z);

#endif
