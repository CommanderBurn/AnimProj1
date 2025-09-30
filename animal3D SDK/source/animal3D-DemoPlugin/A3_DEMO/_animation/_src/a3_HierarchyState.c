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
	
	a3_HierarchyState.c
	Implementation of transform hierarchy state.
*/

#include "../a3_HierarchyState.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>


//-----------------------------------------------------------------------------

// initialize pose set given an initialized hierarchy and key pose count
a3i32 a3hierarchyPoseGroupCreate(a3_HierarchyPoseGroup *poseGroup_out, const a3_Hierarchy *hierarchy, const a3ui32 poseCount)
{
	// validate params and initialization states
	//	(output is not yet initialized, hierarchy is initialized)
	if (poseGroup_out && hierarchy && !poseGroup_out->hierarchy && hierarchy->nodes)
	{
		// determine memory requirements
		a3ui32 const nodeCount = hierarchy->numNodes;
		a3ui32 const hposeCount = poseCount, hposeSpace = sizeof(a3_HierarchyPose) * hposeCount;
		a3ui32 const sposeCount = hposeCount * nodeCount, sposeSpace = sizeof(a3_SpatialPose) * sposeCount;
		a3ui32 const channelSpace = sizeof(a3_SpatialPoseChannel) * nodeCount;
		a3ui32 const orderSpace = sizeof(a3_SpatialPoseEulerOrder) * nodeCount;
		a3ui32 const memreq = hposeSpace + sposeSpace + channelSpace + orderSpace;
		a3index i;

		// allocate everything (one malloc)
		poseGroup_out->hpose = (a3_HierarchyPose*)malloc(memreq);
		poseGroup_out->hpose->hpose_base = poseGroup_out->pose = (a3_SpatialPose*)(poseGroup_out->hpose + hposeCount);
		poseGroup_out->channel = (a3_SpatialPoseChannel*)(poseGroup_out->pose + sposeCount);
		poseGroup_out->order = (a3_SpatialPoseEulerOrder*)(poseGroup_out->channel + nodeCount);

		// set pointers
		for (i = 1; i < hposeCount; ++i)
		{
			poseGroup_out->hpose[i].hpose_base = poseGroup_out->hpose[i - 1].hpose_base + nodeCount;
			poseGroup_out->hpose[i].hpose_index = i * nodeCount;
		}

		// reset all data
		a3hierarchyPoseReset(poseGroup_out->hpose, sposeCount);
		memset(poseGroup_out->channel, a3poseChannel_none, channelSpace);
		memset(poseGroup_out->order, a3poseEulerOrder_xyz, orderSpace);
		poseGroup_out->hierarchy = hierarchy;
		poseGroup_out->hposeCount = hposeCount;
		poseGroup_out->poseCount = sposeCount;

		// done
		return 1;
	}
	return -1;
}

// release pose set
a3i32 a3hierarchyPoseGroupRelease(a3_HierarchyPoseGroup *poseGroup)
{
	// validate param exists and is initialized
	if (poseGroup && poseGroup->hierarchy)
	{
		// release everything (one free)
		free(poseGroup->hpose);

		// reset pointers
		poseGroup->hierarchy = 0;
		poseGroup->hpose = 0;
		poseGroup->pose = 0;
		poseGroup->channel = 0;
		poseGroup->order = 0;

		// done
		return 1;
	}
	return -1;
}

// load binary
a3i32 a3hierarchyPoseGroupLoadBinary(a3_HierarchyPoseGroup* poseGroup, a3_FileStream const* fileStream)
{
	FILE* fp;
	a3ui32 ret = 0;
	a3ui32 dataSize = 0;
	if (poseGroup && fileStream)
	{
		if (poseGroup->hierarchy && !poseGroup->pose)
		{
			fp = fileStream->stream;
			if (fp)
			{
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-OPTIONAL: IMPLEMENT ME
//-----------------------------------------------------------------------------
				//fread
				// ->create


//-----------------------------------------------------------------------------
//****END-TO-DO-OPTIONAL
//-----------------------------------------------------------------------------
			}
			return ret;
		}
	}
	return -1;
}

// save binary
a3i32 a3hierarchyPoseGroupSaveBinary(a3_HierarchyPoseGroup const* poseGroup, a3_FileStream const* fileStream)
{
	FILE* fp;
	a3ui32 ret = 0;
	if (poseGroup && fileStream)
	{
		if (poseGroup->hierarchy && poseGroup->pose)
		{
			fp = fileStream->stream;
			if (fp)
			{
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-OPTIONAL: IMPLEMENT ME
//-----------------------------------------------------------------------------
				//fwrite
				// -> 


//-----------------------------------------------------------------------------
//****END-TO-DO-OPTIONAL
//-----------------------------------------------------------------------------
			}
			return ret;
		}
	}
	return -1;
}


//-----------------------------------------------------------------------------

// initialize hierarchy state given an initialized hierarchy
a3i32 a3hierarchyStateCreate(a3_HierarchyState *state_out, const a3_Hierarchy *hierarchy)
{
	// validate params and initialization states
	//	(output is not yet initialized, hierarchy is initialized)
	if (state_out && hierarchy && !state_out->hierarchy && hierarchy->nodes)
	{
		// determine memory requirements
		a3ui32 const nodeCount = hierarchy->numNodes;
		a3ui32 const hposeCount = sizeof(state_out->hpose) / sizeof(a3_HierarchyPose);
		a3ui32 const sposeCount = hposeCount * nodeCount;
		a3ui32 const memreq = sizeof(a3_SpatialPose) * sposeCount;
		a3index i;

		// allocate everything (one malloc)
		state_out->hpose->hpose_base = (a3_SpatialPose*)malloc(memreq);

		// set pointers
		state_out->hierarchy = hierarchy;
		for (i = 1; i < hposeCount; ++i)
		{
			state_out->hpose[i].hpose_base = state_out->hpose[i - 1].hpose_base + nodeCount;
			state_out->hpose[i].hpose_index = i * nodeCount;
		}

		// reset all data
		a3hierarchyPoseReset(state_out->hpose, sposeCount);

		// done
		return 1;
	}
	return -1;
}

// release hierarchy state
a3i32 a3hierarchyStateRelease(a3_HierarchyState *state)
{
	// validate param exists and is initialized
	if (state && state->hierarchy)
	{
		a3ui32 const hposeCount = sizeof(state->hpose) / sizeof(a3_HierarchyPose);
		a3ui32 i;

		// release everything (one free)
		free(state->hpose->hpose_base);

		// reset pointers
		state->hierarchy = 0;
		for (i = 0; i < hposeCount; ++i)
		{
			state->hpose[i].hpose_base = 0;
			state->hpose[i].hpose_index = 0;
		}

		// done
		return 1;
	}
	return -1;
}


//-----------------------------------------------------------------------------

// update inverse local-space matrices
a3i32 a3hierarchyStateUpdateLocalInverse(const a3_HierarchyState* state)
{
	if (state && state->hierarchy)
	{
		a3index i = 0;
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-2: IMPLEMENT ME
//-----------------------------------------------------------------------------
		for (i = 0; i < state->hierarchy->numNodes; ++i)
		{
			a3real4x4TransformInverse(state->localSpaceInv->hpose_base[i].transformMat.m, state->localSpace->hpose_base[i].transformMat.m);
		}


//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-2
//-----------------------------------------------------------------------------
		return i;
	}
	return -1;
}

// update inverse object-space matrices
a3i32 a3hierarchyStateUpdateObjectInverse(const a3_HierarchyState* state)
{
	if (state && state->hierarchy)
	{
		a3index i = 0;
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-2: IMPLEMENT ME
//-----------------------------------------------------------------------------
		
		for (i = 0; i < state->hierarchy->numNodes; ++i)
		{
			a3real4x4TransformInverse(state->objectSpaceInv->hpose_base[i].transformMat.m, state->objectSpace->hpose_base[i].transformMat.m);
		}

//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-2
//-----------------------------------------------------------------------------
		return i;
	}
	return -1;
}

// update inverse object-space bind-to-current matrices
a3i32 a3hierarchyStateUpdateObjectBindToCurrent(const a3_HierarchyState* state, const a3_HierarchyState* state_bind)
{
	if (state && state->hierarchy && state_bind && state_bind->hierarchy)
	{
		a3index i = 0;
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-2: IMPLEMENT ME
//-----------------------------------------------------------------------------
		for (i = 0; i < state->hierarchy->numNodes; ++i)
		{
			a3real4x4Product(state->objectSpaceBindToCurrent->hpose_base[i].transformMat.m, state_bind->objectSpace->hpose_base[i].transformMat.m,state_bind->objectSpaceInv->hpose_base[i].transformMat.m);
		}


//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-2
//-----------------------------------------------------------------------------
		return i;
	}
	return -1;
}


//-----------------------------------------------------------------------------

// load HTR file, read and store complete pose group and hierarchy
a3i32 a3hierarchyPoseGroupLoadHTR(a3_HierarchyPoseGroup* poseGroup_out, a3_Hierarchy* hierarchy_out, const a3byte* resourceFilePath)
{
	if (poseGroup_out && !poseGroup_out->hierarchy && hierarchy_out && !hierarchy_out->nodes && resourceFilePath && *resourceFilePath)
	{
		//-----------------------------------------------------------------------------
		//****TO-DO-ANIM-PROJECT-2: IMPLEMENT ME
		//-----------------------------------------------------------------------------
				//Read in file
				//Get the header to determine poseGroup
				//Load file data to the node of hierarchy
				// ->Node, TranslationX, TranslationY, TranslationZ, RotationX, RotationY, RotationZ, Length


				//Source: MotionCaptureFileFormatsExplained.pdf
				//Shout out to Tristan for his help in tutoring.
		FILE* file = fopen(resourceFilePath, "r");
			char line[256];
			fgets(line, 256, file);

			if (strstr(line,"[Header]"))
			{
				a3real conversion;
				while (!strstr(line,"[SegmentNames&Hierarchy]"))
				{
					
					fgets(line, 256, file);
					if (strstr(line, "FileType"))
					{
						if (!strstr(line, "HTR"))
						{
							return -1;
						}
					}
					if (strstr(line, "DataType"))
					{
						if (!strstr(line, "HTRS"))
						{
							return -1;
						}
					}
					if (strstr(line, "FileVersion"))
					{
						if (!strstr(line, "1"))
						{
							return -1;
						}
					}
					a3ui32 numNodes;
					if (strstr(line, "NumSegments"))
					{
						char num[256];
						sscanf(line, "%s %d", num, &numNodes);
						a3hierarchyCreate(hierarchy_out, numNodes, NULL);
					}
					a3ui32 numFrames;
					if (strstr(line, "NumFrames"))
					{
						char num[256];
						sscanf(line, "%s %d", num, &numFrames);
						a3hierarchyPoseGroupCreate(poseGroup_out, hierarchy_out, numFrames);
					}
					if (strstr(line, "EulerRotationOrder"))
					{
						*poseGroup_out->order = 5;
					}

					if (strstr(line, "CalibrationUnits"))
					{
						if (strstr(line, "mm"))
						{
							conversion = 0.1f;
						}
					}
					if (strstr(line, "RotationUnits"))
					{
						if (strstr(line, "Degrees"))
						{
							//Insert degree logic
						}
					}
					if (strstr(line, "GlobalAxisofGravity"))
					{
						if (strstr(line, "Y"))
						{
							//SetVerticalGravity
						}
					}
					if (strstr(line, "BoneLengthAxisY"))
					{
						//Insert Length Logic
					}
					a3real scaleFactor;
					if (strstr(line, "ScaleFactor"))
					{
						char num[256];
						sscanf(line, "%s %f", &num, &scaleFactor);
						conversion *= scaleFactor;
					}

				}

			}
			int i = 0;
			if (strstr(line,"[SegmentNames&Hierarchy]"))
			{
				char child[256];
				char parent[256];
				fgets(line, 256, file);
				a3ret parIndex;
				while (!strstr(line, "[BasePosition]"))
				{
						sscanf(line, "%s %s", child , parent);
						parIndex = a3hierarchyGetNodeIndex(hierarchy_out, parent);
						a3hierarchySetNode(hierarchy_out, i,parIndex, child);
						i++;
						fgets(line, 256, file);
					
				}
			}
			if (strstr(line, "[BasePosition]"))
			{
					//fgets(line,256, file);
					char node[256];
					a3f32 Tx, Ty, Tz, Rx, Ry, Rz, S;
					while (fscanf(file, "%s %f %f %f %f %f %f %f", node,&Tx,&Ty,&Tz,&Rx,&Ry,&Rz,&S))
					{
						if (strstr(node, "#"))
							return 1;
						a3ret currPose = a3hierarchyGetNodeIndex(hierarchy_out, node);
						a3spatialPoseSetTranslation(poseGroup_out->pose + currPose, Tx, Ty, Tz);
						a3spatialPoseSetRotation(poseGroup_out->pose + currPose,Rx,Ry,Rz );
						a3spatialPoseSetScale(poseGroup_out->pose + currPose,S,S,S);

					}

					//Fill in variable logic
			}


	}
	return -1;
}
//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-2
//-----------------------------------------------------------------------------

// load BVH file, read and store complete pose group and hierarchy
a3i32 a3hierarchyPoseGroupLoadBVH(a3_HierarchyPoseGroup* poseGroup_out, a3_Hierarchy* hierarchy_out, const a3byte* resourceFilePath)
{
	if (poseGroup_out && !poseGroup_out->hierarchy && hierarchy_out && !hierarchy_out->nodes && resourceFilePath && *resourceFilePath)
	{
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-OPTIONAL: IMPLEMENT ME
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
//****END-TO-DO-OPTIONAL
//-----------------------------------------------------------------------------
	}
	return -1;
}

// save HTR file, read and store complete pose group and hierarchy
a3i32 a3hierarchyPoseGroupSaveHTR(const a3_HierarchyPoseGroup* poseGroup_in, const a3_Hierarchy* hierarchy_in, const a3byte* resourceFilePath)
{
	if (poseGroup_in && poseGroup_in->hierarchy && hierarchy_in && hierarchy_in->nodes && resourceFilePath && *resourceFilePath)
	{
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-OPTIONAL: IMPLEMENT ME
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
//****END-TO-DO-OPTIONAL
//-----------------------------------------------------------------------------
	}
	return -1;
}

// save BVH file, read and store complete pose group and hierarchy
a3i32 a3hierarchyPoseGroupSaveBVH(const a3_HierarchyPoseGroup* poseGroup_in, const a3_Hierarchy* hierarchy_in, const a3byte* resourceFilePath)
{
	if (poseGroup_in && poseGroup_in->hierarchy && hierarchy_in && hierarchy_in->nodes && resourceFilePath && *resourceFilePath)
	{
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-OPTIONAL: IMPLEMENT ME
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
//****END-TO-DO-OPTIONAL
//-----------------------------------------------------------------------------
	}
	return -1;
}


//-----------------------------------------------------------------------------
