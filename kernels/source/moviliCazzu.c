#include <OpenCAL-CL/calcl3D.h>
#include "model.h"

CALint calclGetNext3Di(__CALCL_MODEL_3D, CALint substateNum, int i, int j, int k) {
    __global CALint * next = calclGetNextSubstate3Di(MODEL_3D, substateNum);
    return calclGetBufferElement3D(next, calclGetRows(), calclGetColumns(), i, j, k);
}

//void pezziala(int slot, __CALCL_MODEL_3D, int cell_x, int cell_y, int cell_z)
//{
//    calclSet3Dr(MODEL_3D,Q(FX,slot),cell_x,cell_y,cell_z,0.0);
//    calclSet3Dr(MODEL_3D,Q(FY,slot),cell_x,cell_y,cell_z,0.0);
//    calclSet3Dr(MODEL_3D,Q(FZ,slot),cell_x,cell_y,cell_z,0.0);
//    calclSet3Dr(MODEL_3D,Q(PX,slot),cell_x,cell_y,cell_z,0.0);
//    calclSet3Dr(MODEL_3D,Q(PY,slot),cell_x,cell_y,cell_z,0.0);
//    calclSet3Dr(MODEL_3D,Q(PZ,slot),cell_x,cell_y,cell_z,0.0);
//    calclSet3Dr(MODEL_3D,Q(VX,slot),cell_x,cell_y,cell_z,0.0);
//    calclSet3Dr(MODEL_3D,Q(VY,slot),cell_x,cell_y,cell_z,0.0);
//    calclSet3Dr(MODEL_3D,Q(VZ,slot),cell_x,cell_y,cell_z,0.0);
//    calclSet3Di(MODEL_3D,Q(ID,slot),cell_x,cell_y,cell_z,NULL_ID);
//}

//void sucala(int destination_slot, int source_slot, __CALCL_MODEL_3D, int cell_x, int cell_y, int cell_z, int n)
//{
//    calclSet3Dr(MODEL_3D,Q(FX,destination_slot),cell_x,cell_y,cell_z, calclGetX3Dr(MODEL_3D,Q(FX,source_slot),cell_x,cell_y,cell_z,n));
//    calclSet3Dr(MODEL_3D,Q(FY,destination_slot),cell_x,cell_y,cell_z, calclGetX3Dr(MODEL_3D,Q(FY,source_slot),cell_x,cell_y,cell_z,n));
//    calclSet3Dr(MODEL_3D,Q(FZ,destination_slot),cell_x,cell_y,cell_z, calclGetX3Dr(MODEL_3D,Q(FZ,source_slot),cell_x,cell_y,cell_z,n));
//    calclSet3Dr(MODEL_3D,Q(PX,destination_slot),cell_x,cell_y,cell_z, calclGetX3Dr(MODEL_3D,Q(PX,source_slot),cell_x,cell_y,cell_z,n));
//    calclSet3Dr(MODEL_3D,Q(PY,destination_slot),cell_x,cell_y,cell_z, calclGetX3Dr(MODEL_3D,Q(PY,source_slot),cell_x,cell_y,cell_z,n));
//    calclSet3Dr(MODEL_3D,Q(PZ,destination_slot),cell_x,cell_y,cell_z, calclGetX3Dr(MODEL_3D,Q(PZ,source_slot),cell_x,cell_y,cell_z,n));
//    calclSet3Dr(MODEL_3D,Q(VX,destination_slot),cell_x,cell_y,cell_z, calclGetX3Dr(MODEL_3D,Q(VX,source_slot),cell_x,cell_y,cell_z,n));
//    calclSet3Dr(MODEL_3D,Q(VY,destination_slot),cell_x,cell_y,cell_z, calclGetX3Dr(MODEL_3D,Q(VY,source_slot),cell_x,cell_y,cell_z,n));
//    calclSet3Dr(MODEL_3D,Q(VZ,destination_slot),cell_x,cell_y,cell_z, calclGetX3Dr(MODEL_3D,Q(VZ,source_slot),cell_x,cell_y,cell_z,n));
//    calclSet3Di(MODEL_3D,Q(ID,destination_slot),cell_x,cell_y,cell_z, calclGetX3Di(MODEL_3D,Q(ID,source_slot),cell_x,cell_y,cell_z,n));
//}

__kernel void moviliCazzu(__CALCL_MODEL_3D){

    calclThreadCheck3D();

    int cell_x = calclGlobalRow();
    int cell_y = calclGlobalColumn();
    int cell_z = calclGlobalSlice();

    CALreal px, py, pz;
    CALint  new_cell_x, new_cell_y, new_cell_z;

    //pezziali
    for (int slot = 0; slot < MAX_NUMBER_OF_PARTICLES_PER_CELL; slot++)
        if (calclGet3Di(MODEL_3D, P(ID,slot),cell_x,cell_y,cell_z) > NULL_ID)
            {
                px = calclGet3Dr(MODEL_3D,Q(PX,slot),cell_x,cell_y,cell_z);
                py = calclGet3Dr(MODEL_3D,Q(PY,slot),cell_x,cell_y,cell_z);
                pz = calclGet3Dr(MODEL_3D,Q(PZ,slot),cell_x,cell_y,cell_z);

                new_cell_x = px/CELL_SIDE;
                new_cell_y = py/CELL_SIDE;
                new_cell_z = pz/CELL_SIDE;

                if ((cell_x != new_cell_x) || (cell_y != new_cell_y) || (cell_z != new_cell_z))
                    // pezziala(slot, MODEL_3D,cell_x,cell_y,cell_z);
                    {
                        calclSet3Dr(MODEL_3D,Q(FX,slot),cell_x,cell_y,cell_z,0.0);
                        calclSet3Dr(MODEL_3D,Q(FY,slot),cell_x,cell_y,cell_z,0.0);
                        calclSet3Dr(MODEL_3D,Q(FZ,slot),cell_x,cell_y,cell_z,0.0);
                        calclSet3Dr(MODEL_3D,Q(PX,slot),cell_x,cell_y,cell_z,0.0);
                        calclSet3Dr(MODEL_3D,Q(PY,slot),cell_x,cell_y,cell_z,0.0);
                        calclSet3Dr(MODEL_3D,Q(PZ,slot),cell_x,cell_y,cell_z,0.0);
                        calclSet3Dr(MODEL_3D,Q(VX,slot),cell_x,cell_y,cell_z,0.0);
                        calclSet3Dr(MODEL_3D,Q(VY,slot),cell_x,cell_y,cell_z,0.0);
                        calclSet3Dr(MODEL_3D,Q(VZ,slot),cell_x,cell_y,cell_z,0.0);
                        calclSet3Di(MODEL_3D,P(ID,slot),cell_x,cell_y,cell_z,NULL_ID);
                    }
            }

    //sucali
    for (int n=1; n< calclGetNeighborhoodSize(); n++)
        for (int source_slot = 0; source_slot < MAX_NUMBER_OF_PARTICLES_PER_CELL; source_slot++)
            if (calclGetX3Di(MODEL_3D,P(ID,source_slot),cell_x,cell_y,cell_z,n) > NULL_ID)
                {
                    px = calclGetX3Dr(MODEL_3D,Q(PX,source_slot),cell_x,cell_y,cell_z,n);
                    py = calclGetX3Dr(MODEL_3D,Q(PY,source_slot),cell_x,cell_y,cell_z,n);
                    pz = calclGetX3Dr(MODEL_3D,Q(PZ,source_slot),cell_x,cell_y,cell_z,n);

                    new_cell_x = px/CELL_SIDE;
                    new_cell_y = py/CELL_SIDE;
                    new_cell_z = pz/CELL_SIDE;

                    if ((cell_x == new_cell_x) && (cell_y == new_cell_y) && (cell_z == new_cell_z))
                        {
                            CALbyte sucked = CAL_FALSE;
                            int destination_slot;
                            for (destination_slot = 0; destination_slot < MAX_NUMBER_OF_PARTICLES_PER_CELL; destination_slot++)
                                if (calclGetNext3Di(MODEL_3D,P(ID,destination_slot),cell_x,cell_y,cell_z) == NULL_ID)
                                    {
                                        //sucala(destination_slot,source_slot,MODEL_3D,cell_x,cell_y,cell_z,n);
                                        calclSet3Dr(MODEL_3D,Q(FX,destination_slot),cell_x,cell_y,cell_z, calclGetX3Dr(MODEL_3D,Q(FX,source_slot),cell_x,cell_y,cell_z,n));
                                        calclSet3Dr(MODEL_3D,Q(FY,destination_slot),cell_x,cell_y,cell_z, calclGetX3Dr(MODEL_3D,Q(FY,source_slot),cell_x,cell_y,cell_z,n));
                                        calclSet3Dr(MODEL_3D,Q(FZ,destination_slot),cell_x,cell_y,cell_z, calclGetX3Dr(MODEL_3D,Q(FZ,source_slot),cell_x,cell_y,cell_z,n));
                                        calclSet3Dr(MODEL_3D,Q(PX,destination_slot),cell_x,cell_y,cell_z, calclGetX3Dr(MODEL_3D,Q(PX,source_slot),cell_x,cell_y,cell_z,n));
                                        calclSet3Dr(MODEL_3D,Q(PY,destination_slot),cell_x,cell_y,cell_z, calclGetX3Dr(MODEL_3D,Q(PY,source_slot),cell_x,cell_y,cell_z,n));
                                        calclSet3Dr(MODEL_3D,Q(PZ,destination_slot),cell_x,cell_y,cell_z, calclGetX3Dr(MODEL_3D,Q(PZ,source_slot),cell_x,cell_y,cell_z,n));
                                        calclSet3Dr(MODEL_3D,Q(VX,destination_slot),cell_x,cell_y,cell_z, calclGetX3Dr(MODEL_3D,Q(VX,source_slot),cell_x,cell_y,cell_z,n));
                                        calclSet3Dr(MODEL_3D,Q(VY,destination_slot),cell_x,cell_y,cell_z, calclGetX3Dr(MODEL_3D,Q(VY,source_slot),cell_x,cell_y,cell_z,n));
                                        calclSet3Dr(MODEL_3D,Q(VZ,destination_slot),cell_x,cell_y,cell_z, calclGetX3Dr(MODEL_3D,Q(VZ,source_slot),cell_x,cell_y,cell_z,n));
                                        calclSet3Di(MODEL_3D,P(ID,destination_slot),cell_x,cell_y,cell_z, calclGetX3Di(MODEL_3D,P(ID,source_slot),cell_x,cell_y,cell_z,n));

                                        sucked = CAL_TRUE;
                                        break;
                                    }
                            //                if (sucked == CAL_FALSE)
                            //                  {
                            //                    printf("ERROR: unable to suck a particle.\n");
                            //  #ifdef VERBOSE
                            //                    printf("cell_capacity: %d\n", MAX_NUMBER_OF_PARTICLES_PER_CELL);
                            //                    printf("current_step: %d\n", a_simulazioni->step);
                            //                    printf("source_slot: %d\n", source_slot);
                            //                    printf("destination_slot: %d\n", destination_slot);
                            //  #endif
                            //                    exit(EXIT_FAILURE);
                            //                  }

                        }

                }



}
