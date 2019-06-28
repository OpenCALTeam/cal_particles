#ifndef RESTART_MANAGER_H
#define RESTART_MANAGER_H
#include<model.h>


void saveConfigurationParticles(struct CALModel3D *ca, CALint step, char* path);

void saveConfigurationCollisionsParticles(struct Collisions * collisions, CALint step, char* path);

void saveConfigurationCollisionsWall(struct Collisions * collisions, CALint step, char* path);


int loadSnapshotParticles (struct CALModel3D *ca, char* file_name, int *numberOfParticles);
void loadSnapshotCollisionsParticles (struct Collisions * collisions, char* file_name);

void loadSnapshotCollisionsWalls (struct Collisions * collisions, char* file_name);
#endif
