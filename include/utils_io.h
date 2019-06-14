#ifndef UTILS_IO_H
#define UTILS_IO_H

#include <model.h>

void printSummary(struct CALModel3D* ca);

void saveParticles(struct CALModel3D* ca, CALint step, CALreal elapsed_time, double CPU_time, char* path);
void saveTotalEnergy(struct CALModel3D *ca, CALint step, CALreal elapsed_time, FILE * f);

#endif
