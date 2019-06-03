#ifndef GLOBAL_FUNCTION_H
#define GLOBAL_FUNCTION_H
#include <model.h>
#include <ep_physics.h>
#include <ep_collision.h>
#include <ep_movili.h>
#include <ep_movili_cazzu.h>
#include <utils_io.h>
#include <ep_utils.h>

void updateF(struct CALModel3D* ca);

void updateMoment(struct CALModel3D* ca);
void updateP(struct CALModel3D* ca);

void updateTheta(struct CALModel3D* ca);

void updateV(struct CALModel3D* ca);
void updateW(struct CALModel3D* ca);

void transitionFunction(struct CALModel3D* modello);

void printID (struct CALModel3D* model);

#endif