#ifndef VECTOR_UTILS_H
#define VECTOR_UTILS_H

#include <common.hpp>

void calSet3Dr_vec3 (struct CALModel3D* ca,		//!< Pointer to the cellular automaton structure.
                       struct CALSubstate3Dr* Q_x,	//!< Pointer to a 3D real (floating point) substate.
                       struct CALSubstate3Dr* Q_y,	//!< Pointer to a 3D real (floating point) substate.
                       struct CALSubstate3Dr* Q_z,	//!< Pointer to a 3D real (floating point) substate.
                       int i,						//!< Row coordinate of the cell.
                       int j,						//!< Column coordinate of the cell.
                       int k,						//!< Slice coordinate of the cell.
                       vec3 values				//!< initializing value.
                       );

void calSet3Dr_vec3_sv (struct CALModel3D* ca,		//!< Pointer to the cellular automaton structure.
                       struct CALSubstate3Dr* Q_x,	//!< Pointer to a 3D real (floating point) substate.
                       struct CALSubstate3Dr* Q_y,	//!< Pointer to a 3D real (floating point) substate.
                       struct CALSubstate3Dr* Q_z,	//!< Pointer to a 3D real (floating point) substate.
                       int i,						//!< Row coordinate of the cell.
                       int j,						//!< Column coordinate of the cell.
                       int k,						//!< Slice coordinate of the cell.
                       CALreal value				//!< initializing value.
                       );


void calSet3Dr_vec3_slot (struct CALModel3D* ca,	//!< Pointer to the cellular automaton structure.
                       struct CALSubstate3Dr** Q_x,	//!< Pointer to a 3D real (floating point) substate.
                       struct CALSubstate3Dr** Q_y,	//!< Pointer to a 3D real (floating point) substate.
                       struct CALSubstate3Dr** Q_z,	//!< Pointer to a 3D real (floating point) substate.
                       int slot,
                       int i,						//!< Row coordinate of the cell.
                       int j,						//!< Column coordinate of the cell.
                       int k,						//!< Slice coordinate of the cell.
                       vec3 values				//!< initializing value.
                       );

void calSet3Dr_vec3_slot_sv (struct CALModel3D* ca,	//!< Pointer to the cellular automaton structure.
                       struct CALSubstate3Dr** Q_x,	//!< Pointer to a 3D real (floating point) substate.
                       struct CALSubstate3Dr** Q_y,	//!< Pointer to a 3D real (floating point) substate.
                       struct CALSubstate3Dr** Q_z,	//!< Pointer to a 3D real (floating point) substate.
                       int slot,
                       int i,						//!< Row coordinate of the cell.
                       int j,						//!< Column coordinate of the cell.
                       int k,						//!< Slice coordinate of the cell.
                       CALreal value				//!< initializing value.
                       );

void calGet3Dr_vec3 (struct CALModel3D* ca,		//!< Pointer to the cellular automaton structure.
                       struct CALSubstate3Dr* Q_x,	//!< Pointer to a 3D real (floating point) substate.
                       struct CALSubstate3Dr* Q_y,	//!< Pointer to a 3D real (floating point) substate.
                       struct CALSubstate3Dr* Q_z,	//!< Pointer to a 3D real (floating point) substate.
                       int i,						//!< Row coordinate of the cell.
                       int j,						//!< Column coordinate of the cell.
                       int k,						//!< Slice coordinate of the cell.
                       vec3* values				//!< initializing value.
                       );

void calGet3Dr_vec3_slot (struct CALModel3D* ca,	//!< Pointer to the cellular automaton structure.
                       struct CALSubstate3Dr** Q_x,	//!< Pointer to a 3D real (floating point) substate.
                       struct CALSubstate3Dr** Q_y,	//!< Pointer to a 3D real (floating point) substate.
                       struct CALSubstate3Dr** Q_z,	//!< Pointer to a 3D real (floating point) substate.
                       int slot,
                       int i,						//!< Row coordinate of the cell.
                       int j,						//!< Column coordinate of the cell.
                       int k,						//!< Slice coordinate of the cell.
                       vec3* values				//!< initializing value.
                       );

void calGetX3Dr_vec3 (struct CALModel3D* ca,		//!< Pointer to the cellular automaton structure.
                       struct CALSubstate3Dr* Q_x,	//!< Pointer to a 3D real (floating point) substate.
                       struct CALSubstate3Dr* Q_y,	//!< Pointer to a 3D real (floating point) substate.
                       struct CALSubstate3Dr* Q_z,	//!< Pointer to a 3D real (floating point) substate.
                       int i,						//!< Row coordinate of the cell.
                       int j,						//!< Column coordinate of the cell.
                       int k,						//!< Slice coordinate of the cell.
                       vec3* values,				//!< initializing value.
                       int n
                       );

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
                       );

void calInit3Dr_vec3_sv (struct CALModel3D* ca,		//!< Pointer to the cellular automaton structure.
                       struct CALSubstate3Dr* Q_x,	//!< Pointer to a 3D real (floating point) substate.
                       struct CALSubstate3Dr* Q_y,	//!< Pointer to a 3D real (floating point) substate.
                       struct CALSubstate3Dr* Q_z,	//!< Pointer to a 3D real (floating point) substate.
                       int i,						//!< Row coordinate of the cell.
                       int j,						//!< Column coordinate of the cell.
                       int k,						//!< Slice coordinate of the cell.
                       CALreal value				//!< initializing value.
                       );

void calInit3Dr_vec3 (struct CALModel3D* ca,		//!< Pointer to the cellular automaton structure.
                       struct CALSubstate3Dr* Q_x,	//!< Pointer to a 3D real (floating point) substate.
                       struct CALSubstate3Dr* Q_y,	//!< Pointer to a 3D real (floating point) substate.
                       struct CALSubstate3Dr* Q_z,	//!< Pointer to a 3D real (floating point) substate.
                       int i,						//!< Row coordinate of the cell.
                       int j,						//!< Column coordinate of the cell.
                       int k,						//!< Slice coordinate of the cell.
                       vec3 values				//!< initializing value.
                       );

void calInit3Dr_vec3_slot_sv (struct CALModel3D* ca,	//!< Pointer to the cellular automaton structure.
                       struct CALSubstate3Dr** Q_x,	//!< Pointer to a 3D real (floating point) substate.
                       struct CALSubstate3Dr** Q_y,	//!< Pointer to a 3D real (floating point) substate.
                       struct CALSubstate3Dr** Q_z,	//!< Pointer to a 3D real (floating point) substate.
                       int slot,
                       int i,						//!< Row coordinate of the cell.
                       int j,						//!< Column coordinate of the cell.
                       int k,						//!< Slice coordinate of the cell.
                       CALreal value				//!< initializing value.
                       );
void calInit3Dr_vec3_slot (struct CALModel3D* ca,	//!< Pointer to the cellular automaton structure.
                       struct CALSubstate3Dr** Q_x,	//!< Pointer to a 3D real (floating point) substate.
                       struct CALSubstate3Dr** Q_y,	//!< Pointer to a 3D real (floating point) substate.
                       struct CALSubstate3Dr** Q_z,	//!< Pointer to a 3D real (floating point) substate.
                       int slot,
                       int i,						//!< Row coordinate of the cell.
                       int j,						//!< Column coordinate of the cell.
                       int k,						//!< Slice coordinate of the cell.
                       vec3 values				//!< initializing value.
                       );

#endif
