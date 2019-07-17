#ifndef INIT_PP_H
#define INIT_PP_H

#include <model.h>


bool addRandomParticlePosition(struct CALModel3D* ca, int * nextIDParticle);
bool addParticleWithPosition(struct CALModel3D *ca, vec3 p_0, vec3 v_0, vec3 w_0, int * nextIDParticle);
bool addParticleWithFullConfiguration(struct CALModel3D *ca, vec3 p_0, vec3 v_0, vec3 w_0, vec3 theta_0,vec3 F_0, vec3 moment_0, CALint particle_id, int * numberOfParticles);
void addStructuredParticles(struct CALModel3D *ca, int * nextIDParticle);
#endif
