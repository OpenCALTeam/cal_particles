#include <boundary.h>

void mb(struct CALModel3D* ca, struct Substates *Q, int cell_x, int cell_y, int cell_z)
{
//  CALreal x, y, z, nx, ny, nz;
  vec3 p, n;

  if (cell_x == 0)
    {
      p[0] = cnfg.PARTICLE_RADIUS;
      p[1] = (cell_y * CELL_SIDE) + CELL_SIDE/2;
      p[2] = (cell_z * CELL_SIDE) + CELL_SIDE/2;

      n[0] = 1;
      n[1] = 0;
      n[2] = 0;

      //dovrei azzerare anche momento theta ecc?
      for (int boundary_slot = MAX_NUMBER_OF_PARTICLES_PER_CELL - 1; boundary_slot >= 0; boundary_slot--)
        if (calGet3Di(ca, Q->ID[boundary_slot], cell_x, cell_y, cell_z) >= NULL_ID ) //TODO PERCHÈ SI FA?
          {
            calInit3Dr_vec3_slot_sv(ca, Q->Fx, Q->Fy, Q->Fz, boundary_slot, cell_x,cell_y,cell_z, 0.0 );

            calInit3Dr_vec3_slot(ca, Q->px, Q->py, Q->pz, boundary_slot, cell_x,cell_y,cell_z, p );
            calInit3Dr_vec3_slot(ca, Q->vx, Q->vy, Q->vz, boundary_slot, cell_x,cell_y,cell_z, n );

            calInit3Di(ca, Q->ID[boundary_slot],cell_x,cell_y,cell_z,BORDER_ID);
            break;
          }
    }

  if (cell_x == X_CELLS-1)
    {
      p[0] = X_CELLS * CELL_SIDE - cnfg.PARTICLE_RADIUS;
      p[1] = (cell_y * CELL_SIDE) + CELL_SIDE/2;
      p[2] = (cell_z * CELL_SIDE) + CELL_SIDE/2;

      n[0] = -1;
      n[1] = 0;
      n[2] = 0;

      for (int boundary_slot = MAX_NUMBER_OF_PARTICLES_PER_CELL - 1; boundary_slot >= 0; boundary_slot--)
        if (calGet3Di(ca, Q->ID[boundary_slot], cell_x, cell_y, cell_z) >= NULL_ID )
          {
            calInit3Dr_vec3_slot_sv(ca, Q->Fx, Q->Fy, Q->Fz, boundary_slot, cell_x,cell_y,cell_z, 0.0 );

            calInit3Dr_vec3_slot(ca, Q->px, Q->py, Q->pz, boundary_slot, cell_x,cell_y,cell_z, p );
            calInit3Dr_vec3_slot(ca, Q->vx, Q->vy, Q->vz, boundary_slot, cell_x,cell_y,cell_z, n );

            calInit3Di(ca, Q->ID[boundary_slot],cell_x,cell_y,cell_z, BORDER_ID);
            break;
          }
    }

  if (cell_y == 0)
    {
      p[0] = (cell_x * CELL_SIDE) + CELL_SIDE/2;
      p[1] = cnfg.PARTICLE_RADIUS;
      p[2] = (cell_z * CELL_SIDE) + CELL_SIDE/2;

      n[0] = 0;
      n[1] = 1;
      n[2] = 0;

      for (int boundary_slot = MAX_NUMBER_OF_PARTICLES_PER_CELL - 1; boundary_slot >= 0; boundary_slot--)
        if (calGet3Di(ca, Q->ID[boundary_slot], cell_x, cell_y, cell_z) >= NULL_ID )
          {
            calInit3Dr_vec3_slot_sv(ca, Q->Fx, Q->Fy, Q->Fz, boundary_slot, cell_x,cell_y,cell_z, 0.0 );

            calInit3Dr_vec3_slot(ca, Q->px, Q->py, Q->pz, boundary_slot, cell_x,cell_y,cell_z, p );
            calInit3Dr_vec3_slot(ca, Q->vx, Q->vy, Q->vz, boundary_slot, cell_x,cell_y,cell_z, n );

            calInit3Di(ca, Q->ID[boundary_slot],cell_x,cell_y,cell_z, BORDER_ID);
            break;
          }
    }

  if (cell_y == Y_CELLS-1)
    {
      p[0] = (cell_x * CELL_SIDE) + CELL_SIDE/2;
      p[1] = Y_CELLS * CELL_SIDE - cnfg.PARTICLE_RADIUS;
      p[2] = (cell_z * CELL_SIDE) + CELL_SIDE/2;

      n[0] = 0;
      n[1] = -1;
      n[2] = 0;

      for (int boundary_slot = MAX_NUMBER_OF_PARTICLES_PER_CELL - 1; boundary_slot >= 0; boundary_slot--)
        if (calGet3Di(ca, Q->ID[boundary_slot], cell_x, cell_y, cell_z) >= NULL_ID )
          {
            calInit3Dr_vec3_slot_sv(ca, Q->Fx, Q->Fy, Q->Fz, boundary_slot, cell_x,cell_y,cell_z, 0.0 );

            calInit3Dr_vec3_slot(ca, Q->px, Q->py, Q->pz, boundary_slot, cell_x,cell_y,cell_z, p );
            calInit3Dr_vec3_slot(ca, Q->vx, Q->vy, Q->vz, boundary_slot, cell_x,cell_y,cell_z, n );

            calInit3Di(ca, Q->ID[boundary_slot],cell_x,cell_y,cell_z,BORDER_ID);
            break;
          }
    }

  if (cell_z == 0)
    {
      p[0] = (cell_x * CELL_SIDE) + CELL_SIDE/2;
      p[1] = (cell_y * CELL_SIDE) + CELL_SIDE/2;
      p[2] = cnfg.PARTICLE_RADIUS;

      n[0] = 0;
      n[1] = 0;
      n[2] = 1;

      for (int boundary_slot = MAX_NUMBER_OF_PARTICLES_PER_CELL - 1; boundary_slot >= 0; boundary_slot--)
        if (calGet3Di(ca, Q->ID[boundary_slot], cell_x, cell_y, cell_z) >= NULL_ID )
          {
            calInit3Dr_vec3_slot_sv(ca, Q->Fx, Q->Fy, Q->Fz, boundary_slot, cell_x,cell_y,cell_z, 0.0 );

            calInit3Dr_vec3_slot(ca, Q->px, Q->py, Q->pz, boundary_slot, cell_x,cell_y,cell_z, p );
            calInit3Dr_vec3_slot(ca, Q->vx, Q->vy, Q->vz, boundary_slot, cell_x,cell_y,cell_z, n );

            calInit3Di(ca, Q->ID[boundary_slot],cell_x,cell_y,cell_z,BORDER_ID);
            break;
          }
    }

  if (cell_z == Z_CELLS-1)
    {
      p[0] = (cell_x * CELL_SIDE) + CELL_SIDE/2;
      p[1] = (cell_y * CELL_SIDE) + CELL_SIDE/2;
      p[2] = Z_CELLS * CELL_SIDE - cnfg.PARTICLE_RADIUS;

      n[0] = 0;
      n[1] = 0;
      n[2] = -1;

      for (int boundary_slot = MAX_NUMBER_OF_PARTICLES_PER_CELL - 1; boundary_slot >= 0; boundary_slot--)
        if (calGet3Di(ca, Q->ID[boundary_slot], cell_x, cell_y, cell_z) >= NULL_ID )
          {
            calInit3Dr_vec3_slot_sv(ca, Q->Fx, Q->Fy, Q->Fz, boundary_slot, cell_x,cell_y,cell_z, 0.0 );

            calInit3Dr_vec3_slot(ca, Q->px, Q->py, Q->pz, boundary_slot, cell_x,cell_y,cell_z, p );
            calInit3Dr_vec3_slot(ca, Q->vx, Q->vy, Q->vz, boundary_slot, cell_x,cell_y,cell_z, n );

            calInit3Di(ca, Q->ID[boundary_slot],cell_x,cell_y,cell_z,BORDER_ID);
            break;
          }
    }
}


void boundaryCellsSerial(struct CALModel3D* ca)
{
  for (int x_cell = 0; x_cell < ca->rows; x_cell++)
    for (int y_cell = 0; y_cell < ca->columns; y_cell++)
      for (int z_cell = 0; z_cell < ca->slices; z_cell++)
        {
//#define Z_HOLE
#ifdef Z_HOLE
          if (z_cell==0 && !(x_cell < 5 || x_cell > 14) && !(y_cell < 5 || y_cell > 14))
            continue;
#endif
          mb(ca,&Q,x_cell,y_cell,z_cell);
        }
}
