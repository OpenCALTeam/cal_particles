#ifndef EP_INIT_H
#define EP_INIT_H

#include <model.h>

void mmiscali_nta_cella(struct CALModel3D* ca, int x_cell, int y_cell, int z_cell);
void cancella_particelle_in_urto(struct CALModel3D* ca, int cell_x, int cell_y, int cell_z);

#endif
