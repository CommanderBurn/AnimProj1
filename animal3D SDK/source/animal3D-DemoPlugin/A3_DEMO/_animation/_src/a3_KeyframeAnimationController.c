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
	
	a3_KeyframeAnimationController.c
	Implementation of keyframe animation controller.
*/

#include "../a3_KeyframeAnimationController.h"

#include <string.h>


// macros to help with names
#define A3_CLIPCTRL_DEFAULTNAME		("unnamed clip ctrl")
#define A3_CLIPCTRL_SEARCHNAME		((ctrlName && *ctrlName) ? ctrlName : A3_CLIPCTRL_DEFAULTNAME)


//-----------------------------------------------------------------------------

// initialize clip controller
a3i32 a3clipControllerInit(a3_ClipController* clipCtrl_out, const a3byte ctrlName[a3keyframeAnimation_nameLenMax], const a3_ClipPool* clipPool, const a3ui32 clipIndex_pool, const a3i32 playback_step, const a3f64 playback_stepPerSec)
{
	a3i32 const ret = a3clipControllerSetClip(clipCtrl_out, clipPool, clipIndex_pool, playback_step, playback_stepPerSec);
	if (ret >= 0)
	{
		strncpy(clipCtrl_out->name, A3_CLIPCTRL_SEARCHNAME, a3keyframeAnimation_nameLenMax);
		return ret;
	}
	return -1;
}

// update clip controller
a3i32 a3clipControllerUpdate(a3_ClipController* clipCtrl, a3f64 dt)
{
	a3f64 t, t0, t1;
	a3boolean useFlags = false;
	//clipCtrl->playback_sec = -1;
	t0 = clipCtrl->clipPool->sample[clipCtrl->keyframe[clipCtrl->keyframeIndex].sampleIndex0].time_sec;
	t1 = clipCtrl->clipPool->sample[clipCtrl->keyframe[clipCtrl->keyframeIndex].sampleIndex1].time_sec;
	clipCtrl->clipTime_sec += clipCtrl->playback_sec * dt;
	t = clipCtrl->clipTime_sec;


	if (clipCtrl && clipCtrl->clipPool)
	{
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-1: IMPLEMENT ME
//-----------------------------------------------------------------------------
		if (clipCtrl->playback_sec == 0)
		{
			return 0;
		}
		if (clipCtrl->playback_sec > 0)
		{
			if (t > clipCtrl->clip->duration_sec)
			{
				clipCtrl->keyframeIndex = clipCtrl->clip->keyframeIndex_first;
				t0 = clipCtrl->clipPool->sample[clipCtrl->keyframe[clipCtrl->keyframeIndex].sampleIndex0].time_sec;
				t1 = clipCtrl->clipPool->sample[clipCtrl->keyframe[clipCtrl->keyframeIndex].sampleIndex1].time_sec;
				clipCtrl->clipTime_sec = t - clipCtrl->clip->duration_sec;
				t = clipCtrl->clipTime_sec;
				if (useFlags)
				{
					switch (clipCtrl->clip->transitionForward->flag)
					{
					case 0:
						clipCtrl->playback_sec = 0;
						break;
					case 1:
						clipCtrl->playback_sec = 1;
						break;
					case 2:
						clipCtrl->playback_sec = -1;
						break;

					}
				}
			}
			while (t >= t1 || t < t0)
			{
				clipCtrl->keyframeIndex += 1;
				t0 = clipCtrl->clipPool->sample[clipCtrl->keyframe[clipCtrl->keyframeIndex].sampleIndex0].time_sec;
				t1 = clipCtrl->clipPool->sample[clipCtrl->keyframe[clipCtrl->keyframeIndex].sampleIndex1].time_sec;
			}
		}
		else
		{
			if (t < 0)
			{
				clipCtrl->keyframeIndex = clipCtrl->clip->keyframeIndex_final;
				t0 = clipCtrl->clipPool->sample[clipCtrl->keyframe[clipCtrl->keyframeIndex].sampleIndex0].time_sec;
				t1 = clipCtrl->clipPool->sample[clipCtrl->keyframe[clipCtrl->keyframeIndex].sampleIndex1].time_sec;
				clipCtrl->clipTime_sec = t + clipCtrl->clip->duration_sec;
				t = clipCtrl->clipTime_sec;
				if (useFlags)
				{
					switch (clipCtrl->clip->transitionReverse->flag)
					{
					case 0:
						clipCtrl->playback_sec = 0;
						break;
					case 1:
						clipCtrl->playback_sec = -1;
						break;
					case 2:
						clipCtrl->playback_sec = 1;
						break;


					}
				}
			}
			while (t >= t1 || t < t0)
			{
				clipCtrl->keyframeIndex -= 1;
				t0 = clipCtrl->clipPool->sample[clipCtrl->keyframe->sampleIndex0 + clipCtrl->keyframeIndex].time_sec;
				t1 = clipCtrl->clipPool->sample[clipCtrl->keyframe->sampleIndex1 + clipCtrl->keyframeIndex].time_sec;
			}
		}
		clipCtrl->clipParam = (t) / clipCtrl->clip->duration_sec;
		clipCtrl->keyframeParam = (t - t0) / (t1 - t0);
//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-1
//-----------------------------------------------------------------------------
	}
	return -1;
}


//-----------------------------------------------------------------------------
