#include <calmodel_utils.h>

void calSet3Dr_vec3 (struct CALModel3D* ca,		//!< Pointer to the cellular automaton structure.
                       struct CALSubstate3Dr* Q_x,	//!< Pointer to a 3D real (floating point) substate.
                       struct CALSubstate3Dr* Q_y,	//!< Pointer to a 3D real (floating point) substate.
                       struct CALSubstate3Dr* Q_z,	//!< Pointer to a 3D real (floating point) substate.
                       int i,						//!< Row coordinate of the cell.
                       int j,						//!< Column coordinate of the cell.
                       int k,						//!< Slice coordinate of the cell.
                       vec3 values				//!< initializing value.
                       )
{
    calSet3Dr(ca,Q_x,i,j,k,values[0]);
    calSet3Dr(ca,Q_y,i,j,k,values[1]);
    calSet3Dr(ca,Q_z,i,j,k,values[2]);

}

void calSet3Dr_vec3_sv (struct CALModel3D* ca,		//!< Pointer to the cellular automaton structure.
                       struct CALSubstate3Dr* Q_x,	//!< Pointer to a 3D real (floating point) substate.
                       struct CALSubstate3Dr* Q_y,	//!< Pointer to a 3D real (floating point) substate.
                       struct CALSubstate3Dr* Q_z,	//!< Pointer to a 3D real (floating point) substate.
                       int i,						//!< Row coordinate of the cell.
                       int j,						//!< Column coordinate of the cell.
                       int k,						//!< Slice coordinate of the cell.
                       CALreal value				//!< initializing value.
                       )
{
    calSet3Dr(ca,Q_x,i,j,k,value);
    calSet3Dr(ca,Q_y,i,j,k,value);
    calSet3Dr(ca,Q_z,i,j,k,value);

}


void calSet3Dr_vec3_slot (struct CALModel3D* ca,	//!< Pointer to the cellular automaton structure.
                       struct CALSubstate3Dr** Q_x,	//!< Pointer to a 3D real (floating point) substate.
                       struct CALSubstate3Dr** Q_y,	//!< Pointer to a 3D real (floating point) substate.
                       struct CALSubstate3Dr** Q_z,	//!< Pointer to a 3D real (floating point) substate.
                       int slot,
                       int i,						//!< Row coordinate of the cell.
                       int j,						//!< Column coordinate of the cell.
                       int k,						//!< Slice coordinate of the cell.
                       vec3 values				//!< initializing value.
                       )
{
    calSet3Dr(ca,Q_x[slot],i,j,k,values[0]);
    calSet3Dr(ca,Q_y[slot],i,j,k,values[1]);
    calSet3Dr(ca,Q_z[slot],i,j,k,values[2]);

}

void calSet3Dr_vec3_slot_sv (struct CALModel3D* ca,	//!< Pointer to the cellular automaton structure.
                       struct CALSubstate3Dr** Q_x,	//!< Pointer to a 3D real (floating point) substate.
                       struct CALSubstate3Dr** Q_y,	//!< Pointer to a 3D real (floating point) substate.
                       struct CALSubstate3Dr** Q_z,	//!< Pointer to a 3D real (floating point) substate.
                       int slot,
                       int i,						//!< Row coordinate of the cell.
                       int j,						//!< Column coordinate of the cell.
                       int k,						//!< Slice coordinate of the cell.
                       CALreal value				//!< initializing value.
                       )
{
    calSet3Dr(ca,Q_x[slot],i,j,k,value);
    calSet3Dr(ca,Q_y[slot],i,j,k,value);
    calSet3Dr(ca,Q_z[slot],i,j,k,value);

}

void calGet3Dr_vec3 (struct CALModel3D* ca,		//!< Pointer to the cellular automaton structure.
                       struct CALSubstate3Dr* Q_x,	//!< Pointer to a 3D real (floating point) substate.
                       struct CALSubstate3Dr* Q_y,	//!< Pointer to a 3D real (floating point) substate.
                       struct CALSubstate3Dr* Q_z,	//!< Pointer to a 3D real (floating point) substate.
                       int i,						//!< Row coordinate of the cell.
                       int j,						//!< Column coordinate of the cell.
                       int k,						//!< Slice coordinate of the cell.
                       vec3* values				//!< initializing value.
                       )
{
    (*values)[0] = calGet3Dr(ca,Q_x,i,j,k);
    (*values)[1] = calGet3Dr(ca,Q_y,i,j,k);
    (*values)[2] = calGet3Dr(ca,Q_z,i,j,k);

}

void calGet3Dr_vec3_slot (struct CALModel3D* ca,	//!< Pointer to the cellular automaton structure.
                       struct CALSubstate3Dr** Q_x,	//!< Pointer to a 3D real (floating point) substate.
                       struct CALSubstate3Dr** Q_y,	//!< Pointer to a 3D real (floating point) substate.
                       struct CALSubstate3Dr** Q_z,	//!< Pointer to a 3D real (floating point) substate.
                       int slot,
                       int i,						//!< Row coordinate of the cell.
                       int j,						//!< Column coordinate of the cell.
                       int k,						//!< Slice coordinate of the cell.
                       vec3* values				//!< initializing value.
                       )
{
    (*values)[0] = calGet3Dr(ca,Q_x[slot],i,j,k);
    (*values)[1] = calGet3Dr(ca,Q_y[slot],i,j,k);
    (*values)[2] = calGet3Dr(ca,Q_z[slot],i,j,k);

}

void calGetX3Dr_vec3 (struct CALModel3D* ca,		//!< Pointer to the cellular automaton structure.
                       struct CALSubstate3Dr* Q_x,	//!< Pointer to a 3D real (floating point) substate.
                       struct CALSubstate3Dr* Q_y,	//!< Pointer to a 3D real (floating point) substate.
                       struct CALSubstate3Dr* Q_z,	//!< Pointer to a 3D real (floating point) substate.
                       int i,						//!< Row coordinate of the cell.
                       int j,						//!< Column coordinate of the cell.
                       int k,						//!< Slice coordinate of the cell.
                       vec3* values,				//!< initializing value.
                       int n
                       )
{
    (*values)[0] = calGetX3Dr(ca,Q_x,i,j,k, n);
    (*values)[1] = calGetX3Dr(ca,Q_y,i,j,k, n);
    (*values)[2] = calGetX3Dr(ca,Q_z,i,j,k, n);

}

void calGetX3Dr_vec3_slot (struct CALModel3D* ca,	//!< Pointer to the cellular automaton structure.
                       struct CALSubstate3Dr** Q_x,	//!< Pointer to a 3D real (floating point) substate.
                       struct CALSubstate3Dr** Q_y,	//!< Pointer to a 3D real (floating point) substate.
                       struct CALSubstate3Dr** Q_z,	//!< Pointer to a 3D real (floating point) substate.
                       int slot,
                       int i,						//!< Row coordinate of the cell.
                       int j,						//!< Column coordinate of the cell.
                       int k,						//!< Slice coordinate of the cell.
                       vec3* values,				//!< initializing value.
                       int n
                       )
{
    (*values)[0] = calGetX3Dr(ca,Q_x[slot],i,j,k,n);
    (*values)[1] = calGetX3Dr(ca,Q_y[slot],i,j,k,n);
    (*values)[2] = calGetX3Dr(ca,Q_z[slot],i,j,k,n);

}


void calInit3Dr_vec3_sv (struct CALModel3D* ca,		//!< Pointer to the cellular automaton structure.
                       struct CALSubstate3Dr* Q_x,	//!< Pointer to a 3D real (floating point) substate.
                       struct CALSubstate3Dr* Q_y,	//!< Pointer to a 3D real (floating point) substate.
                       struct CALSubstate3Dr* Q_z,	//!< Pointer to a 3D real (floating point) substate.
                       int i,						//!< Row coordinate of the cell.
                       int j,						//!< Column coordinate of the cell.
                       int k,						//!< Slice coordinate of the cell.
                       CALreal value				//!< initializing value.
                       )
{
    calInit3Dr(ca,Q_x,i,j,k,value);
    calInit3Dr(ca,Q_y,i,j,k,value);
    calInit3Dr(ca,Q_z,i,j,k,value);

}

void calInit3Dr_vec3 (struct CALModel3D* ca,		//!< Pointer to the cellular automaton structure.
                       struct CALSubstate3Dr* Q_x,	//!< Pointer to a 3D real (floating point) substate.
                       struct CALSubstate3Dr* Q_y,	//!< Pointer to a 3D real (floating point) substate.
                       struct CALSubstate3Dr* Q_z,	//!< Pointer to a 3D real (floating point) substate.
                       int i,						//!< Row coordinate of the cell.
                       int j,						//!< Column coordinate of the cell.
                       int k,						//!< Slice coordinate of the cell.
                       vec3 values				//!< initializing value.
                       )
{
    calInit3Dr(ca,Q_x,i,j,k,values[0]);
    calInit3Dr(ca,Q_y,i,j,k,values[1]);
    calInit3Dr(ca,Q_z,i,j,k,values[2]);

}

void calInit3Dr_vec3_slot_sv (struct CALModel3D* ca,	//!< Pointer to the cellular automaton structure.
                       struct CALSubstate3Dr** Q_x,	//!< Pointer to a 3D real (floating point) substate.
                       struct CALSubstate3Dr** Q_y,	//!< Pointer to a 3D real (floating point) substate.
                       struct CALSubstate3Dr** Q_z,	//!< Pointer to a 3D real (floating point) substate.
                       int slot,
                       int i,						//!< Row coordinate of the cell.
                       int j,						//!< Column coordinate of the cell.
                       int k,						//!< Slice coordinate of the cell.
                       CALreal value				//!< initializing value.
                       )
{
    calInit3Dr(ca,Q_x[slot],i,j,k,value);
    calInit3Dr(ca,Q_y[slot],i,j,k,value);
    calInit3Dr(ca,Q_z[slot],i,j,k,value);

}

void calInit3Dr_vec3_slot (struct CALModel3D* ca,	//!< Pointer to the cellular automaton structure.
                       struct CALSubstate3Dr** Q_x,	//!< Pointer to a 3D real (floating point) substate.
                       struct CALSubstate3Dr** Q_y,	//!< Pointer to a 3D real (floating point) substate.
                       struct CALSubstate3Dr** Q_z,	//!< Pointer to a 3D real (floating point) substate.
                       int slot,
                       int i,						//!< Row coordinate of the cell.
                       int j,						//!< Column coordinate of the cell.
                       int k,						//!< Slice coordinate of the cell.
                       vec3 values				//!< initializing value.
                       )
{
    calInit3Dr(ca,Q_x[slot],i,j,k,values[0]);
    calInit3Dr(ca,Q_y[slot],i,j,k,values[1]);
    calInit3Dr(ca,Q_z[slot],i,j,k,values[2]);

}

void calGetNext3Dr_vec3(struct CALModel3D* ca,		//!< Pointer to the cellular automaton structure.
                        struct CALSubstate3Dr* Q_x,	//!< Pointer to a 3D real (floating point) substate.
                        struct CALSubstate3Dr* Q_y,	//!< Pointer to a 3D real (floating point) substate.
                        struct CALSubstate3Dr* Q_z,	//!< Pointer to a 3D real (floating point) substate.
                          int i,					//!< Row coordinate of the cell.
                          int j,					//!< Column coordinate of the cell
                          int k,						//!< Slice coordinate of the central cell.
                          vec3* values
                      )
{
    (*values)[0] = calGetNext3Dr(ca,Q_x,i,j,k);
    (*values)[1] = calGetNext3Dr(ca,Q_y,i,j,k);
    (*values)[2] = calGetNext3Dr(ca,Q_z,i,j,k);
}

void calGetNext3Dr_vec3_slot(struct CALModel3D* ca,		//!< Pointer to the cellular automaton structure.
                             struct CALSubstate3Dr** Q_x,	//!< Pointer to a 3D real (floating point) substate.
                             struct CALSubstate3Dr** Q_y,	//!< Pointer to a 3D real (floating point) substate.
                             struct CALSubstate3Dr** Q_z,	//!< Pointer to a 3D real (floating point) substate.
                          int slot,
                          int i,					//!< Row coordinate of the cell.
                          int j,					//!< Column coordinate of the cell
                          int k,						//!< Slice coordinate of the central cell.
                          vec3* values
                      )
{
    (*values)[0] = calGetNext3Dr(ca,Q_x[slot],i,j,k);
    (*values)[1] = calGetNext3Dr(ca,Q_y[slot],i,j,k);
    (*values)[2] = calGetNext3Dr(ca,Q_z[slot],i,j,k);
}



