/*
	Copyright 2011-2025 Daniel S. Buckstein

	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use this file except in compliance with the License.
	You may obtain a copy of the License at

		http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.
*/

/*
	animal3D SDK: Minimal 3D Animation Framework
	By Daniel S. Buckstein
	
	a3_Kinematics.c
	Implementation of kinematics solvers.
*/

#include "../a3_Kinematics.h"


//-----------------------------------------------------------------------------

// single FK helpers
static inline void a3kinematicsSolveForwardSingle(const a3_HierarchyState* hierarchyState, const a3ui32 index, const a3ui32 parentIndex)
{
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-2: IMPLEMENT ME
//-----------------------------------------------------------------------------

	// T[this_object] = T[parent_object] * T[this_local]
	a3real4x4Product(
		hierarchyState->objectSpace->hpose_base[index].transformMat.m,		// Result: this node object-space.
		hierarchyState->objectSpace->hpose_base[parentIndex].transformMat.m,// Left-hand: parent node object-space.
		hierarchyState->localSpace->hpose_base[index].transformMat.m		// Right-hand: this node local space.
	);

//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-2
//-----------------------------------------------------------------------------
}
static inline void a3kinematicsSolveForwardRoot(const a3_HierarchyState* hierarchyState, const a3ui32 index)
{
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-2: IMPLEMENT ME
//-----------------------------------------------------------------------------

	// T[root_object] = T[root_local]
	hierarchyState->objectSpace->hpose_base[index] = hierarchyState->localSpace->hpose_base[index];

//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-2
//-----------------------------------------------------------------------------
}

// partial FK solver
a3i32 a3kinematicsSolveForwardPartial(const a3_HierarchyState* hierarchyState, const a3ui32 firstIndex, const a3ui32 nodeCount)
{
	if (hierarchyState && hierarchyState->hierarchy &&
		firstIndex < hierarchyState->hierarchy->numNodes && nodeCount)
	{
		// implement forward kinematics algorithm: 
		//	- for all nodes starting at first index
		//		- if node is not root (has parent node)
		//			- object matrix = parent object matrix * local matrix
		//		- else
		//			- copy local matrix to object matrix
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-2: IMPLEMENT ME
//-----------------------------------------------------------------------------

		const a3_HierarchyNode* itr = hierarchyState->hierarchy->nodes + firstIndex;
		const a3_HierarchyNode* const end = itr + nodeCount;
		for (; itr < end; ++itr)
		{
			if (itr->parentIndex >= 0)
				a3kinematicsSolveForwardSingle(hierarchyState, itr->index, itr->parentIndex);
			else
				a3kinematicsSolveForwardRoot(hierarchyState, itr->index);
		}
		return (a3i32)(end - itr);

//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-2
//-----------------------------------------------------------------------------
	}
	return -1;
}


//-----------------------------------------------------------------------------

// single IK helpers
static inline void a3kinematicsSolveInverseSingle(const a3_HierarchyState* hierarchyState, const a3ui32 index, const a3ui32 parentIndex)
{
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-3: IMPLEMENT ME
//-----------------------------------------------------------------------------
	a3real4x4Product(
		hierarchyState->localSpace->hpose_base[index].transformMat.m,
		hierarchyState->objectSpaceInv->hpose_base[parentIndex].transformMat.m,
		hierarchyState->objectSpace->hpose_base[index].transformMat.m
		);

//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-3
//-----------------------------------------------------------------------------
}
static inline void a3kinematicsSolveInverseRoot(const a3_HierarchyState* hierarchyState, const a3ui32 index)
{
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-3: IMPLEMENT ME
//-----------------------------------------------------------------------------

	hierarchyState->localSpace->hpose_base[index] = hierarchyState->objectSpace->hpose_base[index];


//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-3
//-----------------------------------------------------------------------------
}

// partial IK solver
a3i32 a3kinematicsSolveInversePartial(const a3_HierarchyState* hierarchyState, const a3ui32 firstIndex, const a3ui32 nodeCount)
{
	if (hierarchyState && hierarchyState->hierarchy &&
		firstIndex < hierarchyState->hierarchy->numNodes && nodeCount)
	{
		// implement inverse kinematics algorithm: 
		//	- for all nodes starting at first index
		//		- if node is not root (has parent node)
		//			- local matrix = inverse parent object matrix * object matrix
		//		- else
		//			- copy object matrix to local matrix
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-3: IMPLEMENT ME
//-----------------------------------------------------------------------------
		const a3_HierarchyNode* itr = hierarchyState->hierarchy->nodes + firstIndex;
		const a3_HierarchyNode* const end = itr + nodeCount;
		for (; itr < end; ++itr)
		{
			if (itr->parentIndex >= 0)
				a3kinematicsSolveInverseSingle(hierarchyState, itr->index, itr->parentIndex);
			else
				a3kinematicsSolveInverseRoot(hierarchyState, itr->index);
		}
		return (a3i32)(end - itr);


//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-3
//-----------------------------------------------------------------------------
	}
	return -1;
}


//-----------------------------------------------------------------------------

void a3kinematicsUpdateHierarchyStateFK(a3_HierarchyState* activeHS,
	a3_HierarchyState const* baseHS, a3_HierarchyPoseGroup const* poseGroup)
{
	if (activeHS->hierarchy == baseHS->hierarchy &&
		activeHS->hierarchy == poseGroup->hierarchy)
	{
		// FK pipeline
		//	-> concatenate base pose
		//	-> convert poses to local-space matrices
		//	-> perform recursive FK
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-2: IMPLEMENT ME
//-----------------------------------------------------------------------------

		a3hierarchyPoseConcat(activeHS->localSpace,	// local: goal to calculate
			activeHS->animPose,						// holds current sample pose
			baseHS->localSpace,						// holds base pose (animPose is all identity poses)
			activeHS->hierarchy->numNodes);
		a3hierarchyPoseConvert(activeHS->localSpace,
			activeHS->hierarchy->numNodes,
			poseGroup->channel,
			poseGroup->order);
		a3kinematicsSolveForward(activeHS);

//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-2
//-----------------------------------------------------------------------------
	}
}

void a3kinematicsUpdateHierarchyStateIK(a3_HierarchyState* activeHS,
	a3_HierarchyState const* baseHS, a3_HierarchyPoseGroup const* poseGroup)
{
	if (activeHS->hierarchy == baseHS->hierarchy &&
		activeHS->hierarchy == poseGroup->hierarchy)
	{
		// IK pipeline
		//	-> perform recursive IK
		//	-> restore local-space matrices to poses
		//	-> deconcatenate base pose
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-3: IMPLEMENT ME
//-----------------------------------------------------------------------------


		a3kinematicsSolveInverse(activeHS);
		a3hierarchyPoseRestore(activeHS->localSpace,
			activeHS->hierarchy->numNodes,
			poseGroup->channel,
			poseGroup->order);
		a3hierarchyPoseDeconcat(activeHS->animPose,	// local: goal to calculate
			activeHS->localSpace,						// holds current sample pose
			baseHS->localSpace,						// holds base pose (animPose is all identity poses)
			activeHS->hierarchy->numNodes);

//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-3
//-----------------------------------------------------------------------------
	}
}

void a3kinematicsUpdateHierarchyStateSkin(a3_HierarchyState* activeHS,
	a3_HierarchyState const* baseHS)
{
	if (activeHS->hierarchy == baseHS->hierarchy)
	{
		// FK pipeline extended for skinning and other applications
		//	-> update local-space inverse matrices
		//	-> update object-space inverse matrices
		//	-> update transform from base to current
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-2: IMPLEMENT ME
//-----------------------------------------------------------------------------

		a3hierarchyStateUpdateLocalInverse(activeHS);
		a3hierarchyStateUpdateObjectInverse(activeHS);
		a3hierarchyStateUpdateObjectBindToCurrent(activeHS, baseHS);

//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-2
//-----------------------------------------------------------------------------
	}
}


//-----------------------------------------------------------------------------

// helper to resolve single-joint IK after solver
static void a3kinematicsResolvePostIK(a3_HierarchyState* activeHS,
	a3_HierarchyState const* baseHS, a3_HierarchyPoseGroup const* poseGroup,
	a3ui32 const nodeIndex, a3real4x4 const j2obj)
{
	// post-IK resolution for single affected joint
	//	-> reassign resolved transform to object-space
	//	-> compute object-space inverse matrix
	//	-> compute local-space matrix
	//	-> restore local-space matrix to pose
	//	-> deconcatenate base pose
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-3: IMPLEMENT ME
//-----------------------------------------------------------------------------

	a3real4x4SetReal4x4(activeHS->objectSpace->hpose_base[nodeIndex].transformMat.m, j2obj);
	
	a3real4x4GetInverse(activeHS->objectSpaceInv->hpose_base[nodeIndex].transformMat.m, j2obj);

	a3ui32 parentIndex = activeHS->hierarchy->nodes[nodeIndex].parentIndex;
	
	a3real4x4Product(activeHS->localSpace->hpose_base[nodeIndex].transformMat.m, activeHS->objectSpaceInv->hpose_base[parentIndex].transformMat.m, j2obj);

	a3spatialPoseRestore
	(&activeHS->localSpace->hpose_base[nodeIndex],
		poseGroup->channel[nodeIndex],
		poseGroup->order[nodeIndex]);
	//Deconcat
	a3spatialPoseDeconcat
	(&activeHS->animPose->hpose_base[nodeIndex],	// local: goal to calculate
		&activeHS->localSpace->hpose_base[nodeIndex],						// holds current sample pose
		&baseHS->localSpace->hpose_base[nodeIndex]						// holds base pose (animPose is all identity poses)
		);

//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-3
//-----------------------------------------------------------------------------
}
void a3kinematicsUpdateLookAtIK(a3_HierarchyState const* sceneGraphState,
	a3_HierarchyState* activeHS, a3_HierarchyState const* baseHS, a3_HierarchyPoseGroup const* poseGroup,
	a3ui32 const sceneGraphIndex_hierarchyObj, a3ui32 const sceneGraphIndex_effector,
	a3ui32 const hierarchyObjIndex_affected, a3_Basis const basis_hierarchyObj, a3_Basis const basis_affected)
{
	a3mat3 m_hierarchyObj, m_affected;
	if (!a3basisToMat3(m_hierarchyObj.m, basis_hierarchyObj))
		return;
	if (!a3basisToMat3(m_affected.m, basis_affected))
		return;

	if ((!sceneGraphState || !activeHS || !baseHS || !poseGroup) ||
		(activeHS->hierarchy != baseHS->hierarchy) ||
		(activeHS->hierarchy != poseGroup->hierarchy))
		return;
	
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-3: IMPLEMENT ME
//-----------------------------------------------------------------------------
	
	//First:
	//Move eveything into  the space of the skeleton/hierarchy
	a3real4x4 product;

	//a3ui32 parentIndex = sceneGraphState->hierarchy->nodes[sceneGraphIndex_hierarchyObj].parentIndex;
	a3real4x4Product(product, sceneGraphState->objectSpaceInv->hpose_base[sceneGraphIndex_hierarchyObj].transformMat.m, sceneGraphState->objectSpace->hpose_base[sceneGraphIndex_effector].transformMat.m);
	a3real4x4SetReal4x4(sceneGraphState->localSpace->hpose_base[sceneGraphIndex_effector].transformMat.m, product);

	// look at target
	//Main:
	//Solver: build an orthonormal basis
	//Joint-to-object
	//1. Direction basis = target - joint position
	a3real3 dir;
	a3vec4 lookPos = sceneGraphState->localSpace->hpose_base[sceneGraphIndex_effector].transformMat.v3;
	a3vec4 eyePos = activeHS->objectSpace->hpose_base[hierarchyObjIndex_affected].transformMat.v3;
	a3real3Diff(dir,lookPos.v,eyePos.v);
	a3real3Normalize(dir);
	//2. side basis = known up x direction basis
	a3real3 upVec;
	a3real3Set(upVec, 0, 1, 0);
	
	a3real3 side;
	a3real3Cross(side, upVec, dir);
	a3real3Normalize(side);
	//3. up basis = direction x side
	a3real3 up;
	a3real3Cross(up, dir, side);
	//4. normalizle all
	//Last:
	a3real3x3 R;
	a3real3x3SetMajors(R, side, up, dir);
	a3real4x4SetReal3x3(product, R);
	a3real4SetReal4(product[3], activeHS->objectSpace->hpose_base[hierarchyObjIndex_affected].transformMat.v3.v);
	a3kinematicsResolvePostIK(activeHS, baseHS, poseGroup, hierarchyObjIndex_affected, product);

//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-3
//-----------------------------------------------------------------------------
}


void a3kinematicsUpdateLimbIK(a3_HierarchyState const* sceneGraphState,
	a3_HierarchyState* activeHS, a3_HierarchyState const* baseHS, a3_HierarchyPoseGroup const* poseGroup,
	a3ui32 const sceneGraphIndex_hierarchyObj, a3ui32 const sceneGraphIndex_effector_end, a3ui32 const sceneGraphIndex_constraint,
	a3ui32 const hierarchyObjIndex_affected_end, a3ui32 const hierarchyObjIndex_affected_hinge, a3ui32 const hierarchyObjIndex_affected_base,
	a3_Basis const basis_hierarchyObj, a3_Basis const basis_affected_end, a3_Basis const basis_affected_hinge, a3_Basis const basis_affected_base)
{
	a3mat3 m_hierarchyObj, m_affected_end, m_affected_hinge, m_affected_base;
	if (!a3basisToMat3(m_hierarchyObj.m, basis_hierarchyObj))
		return;
	if (!a3basisToMat3(m_affected_end.m, basis_affected_end))
		return;
	if (!a3basisToMat3(m_affected_hinge.m, basis_affected_hinge))
		return;
	if (!a3basisToMat3(m_affected_base.m, basis_affected_base))
		return;

	if ((!sceneGraphState || !activeHS || !baseHS || !poseGroup) ||
		(activeHS->hierarchy != baseHS->hierarchy) ||
		(activeHS->hierarchy != poseGroup->hierarchy))
		return;
	
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-3: IMPLEMENT ME
//-----------------------------------------------------------------------------

	//First:
	//	wrist
	//	pole vector constraint
	a3real4x4 product; 
	a3real4x4Product(product, sceneGraphState->objectSpaceInv->hpose_base[sceneGraphIndex_hierarchyObj].transformMat.m, sceneGraphState->objectSpace->hpose_base[sceneGraphIndex_effector_end].transformMat.m);
	a3real4x4SetReal4x4(sceneGraphState->localSpace->hpose_base[sceneGraphIndex_effector_end].transformMat.m, product);
	

	//Main:
	//a3real4x4 T;
	//Solve joint-to-object for end, hinge,base
	// -> end position*
	
	// -> hinge position*
	
	//1. base joint to end effector vector (and distance)
	a3vec4 base = sceneGraphState->objectSpace->hpose_base[hierarchyObjIndex_affected_base].transformMat.v3;
	a3vec4 end = sceneGraphState->objectSpace->hpose_base[sceneGraphIndex_effector_end].transformMat.v3;

	a3real3 d; //effector displacement
	a3real3Diff(d, end.v, base.v);

	a3real3 dNorm;
	a3real3SetReal3(dNorm, d);
	a3real3Normalize(dNorm);
	
	//2. base joint to pole vector constaint
	
	a3vec4 constraint = sceneGraphState->objectSpace->hpose_base[sceneGraphIndex_constraint].transformMat.v3;


	a3real4 c;
	a3real3Diff(c, constraint.v, base.v);
	//3. plane normal = (base to pole) x (base to end)
	a3real4 n;
	a3real3Cross(n, c, d);

	a3real4 nNorm;
	a3real3SetReal3(nNorm, n);
	a3real3Normalize(nNorm);

	a3real4 h; //is normalized
	a3real3Cross(h, nNorm, dNorm);

	a3real4 hNorm;
	a3real3SetReal3(hNorm, h);
	a3real3Normalize(hNorm);

	//4. geometric (Heron's formula) or algebraic (law of cosines)
	// ->solve elbow pos
	a3real B = a3real3Length(d);
	a3real L1 = a3real3Distance(constraint.v, base.v);
	a3real L2 = a3real3Distance(end.v, constraint.v);

	a3f32 s = 0.5f * (B + L1 + L2);
	
	a3real A = a3sqrtf(a3absolute(s * (s - B) * (s - L1) * (s - L2)));
	
	a3real H = (2 * A) / B;

	a3real D = a3sqrtf(a3absolute((L1 * L1) - (H * H)));

	a3real4 DProd, HProd;
	a3real3ProductS(DProd, dNorm, D);
	a3real3ProductS(HProd, hNorm, H);

	a3real4 p; //elbow's offset from base effector
	a3real3Add(p, base.v);
	a3real3Add(p, DProd);
	a3real3Add(p, HProd);

	a3real4 t0;
	a3real3Diff(t0, p, base.v);
	

	a3real4 t0Norm;
	a3real3SetReal3(t0Norm, t0);
	a3real3Normalize(t0Norm);

	a3real4 t1;
	a3real3Diff(t1, end.v, p);

	a3real4 t1Norm;
	a3real3SetReal3(t1Norm, t1);
	a3real3Normalize(t1Norm);

	a3real4 b0;
	a3real3Cross(b0, t0Norm, nNorm);

	a3real4 b0Norm;
	a3real3SetReal3(b0Norm, b0);
	a3real3Normalize(b0Norm);

	a3real4 b1;
	a3real3Cross(b1, t1Norm, nNorm);

	a3real4 b1Norm;
	a3real3SetReal3(b1Norm, b1);
	a3real3Normalize(b1Norm);


	t0Norm[3] = 0;
	b0Norm[3] = 0;
	nNorm[3] = 0;
	p[3] = 1;
	a3real4x4 worldTjoint0;
	a3real4x4SetMajors(worldTjoint0, t0Norm, b0Norm, nNorm, p);
	
	
	t1Norm[3] = 0;
	b1Norm[3] = 0;
	nNorm[3] = 0;
	p[3] = 1;
	a3real4x4 worldTjoint1;
	a3real4x4SetMajors(worldTjoint1, t1Norm, b1Norm, nNorm, p);


	//a3real3x3 R;
	//a3real3x3SetMajors(R, base.v, constraint.v, end.v);

	//5. "look at" solve shoulder and elbow rotations
	a3kinematicsUpdateLookAtIK(sceneGraphState, activeHS, baseHS, poseGroup, sceneGraphIndex_hierarchyObj, sceneGraphIndex_constraint, hierarchyObjIndex_affected_hinge, basis_hierarchyObj, basis_affected_end);
	a3kinematicsUpdateLookAtIK(sceneGraphState, activeHS, baseHS, poseGroup, sceneGraphIndex_hierarchyObj, sceneGraphIndex_constraint, hierarchyObjIndex_affected_hinge, basis_hierarchyObj, basis_affected_base);

	//Last:
	//	work from root to leaf
	a3kinematicsResolvePostIK(activeHS, baseHS, poseGroup, hierarchyObjIndex_affected_base, worldTjoint0);
	a3kinematicsResolvePostIK(activeHS, baseHS, poseGroup, hierarchyObjIndex_affected_hinge, worldTjoint1);
	a3kinematicsResolvePostIK(activeHS, baseHS, poseGroup, hierarchyObjIndex_affected_end, product);
//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-3
//-----------------------------------------------------------------------------
}


//-----------------------------------------------------------------------------
