/*
===========================================================================
Copyright (C) 2015 the OpenMoHAA team

This file is part of OpenMoHAA source code.

OpenMoHAA source code is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

OpenMoHAA source code is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with OpenMoHAA source code; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
===========================================================================
*/

// actor_idle.cpp

#include "actor.h"

void Actor::InitIdle
	(
	GlobalFuncs_t *func
	)

{
	func->BeginState					= &Actor::Begin_Idle;
	func->ThinkState					= &Actor::Think_Idle;
	func->PassesTransitionConditions	= &Actor::PassesTransitionConditions_Idle;
	func->IsState						= &Actor::IsIdleState;
}

void Actor::Begin_Idle
	(
	void
	)

{
	m_csMood = m_csIdleMood;
	ClearPath();
}

void Actor::Think_Idle
	(
	void
	)

{
	if (RequireThink())
	{
		UpdateEyeOrigin();
		m_pszDebugState = "";
		CheckForThinkStateTransition();
		IdleThink();
	}
}

void Actor::IdleThink
(
	void
)

{
	//FIXME: revision
	IdlePoint();
	IdleLook();
	if (PathExists() && PathComplete())
	{
		ClearPath();
	}
	if (m_bAutoAvoidPlayer && !PathExists())
	{
		SetPathToNotBlockSentient((Sentient *)G_GetEntity(0));
	}
	if (!PathExists())
	{
		Anim_Idle();
		IdleTurn();
		PostThink(true);
	}
	else
	{
		//FIXME: macros
		Anim_WalkTo(2);

		if (PathDist() <= 128.0)
		{
			IdleTurn();
			PostThink(true);
		}
		else
		{
			FaceMotion();
		}
	}

}


/*
===============
Actor::PassesTransitionConditions_Idle

Should actor transition think state to idle ?
===============
*/
bool Actor::PassesTransitionConditions_Idle
(
	void
)

{

	if (m_bEnableEnemy)
	{
		if (level.inttime > m_iEnemyCheckTime + 500)
			UpdateEnemyInternal();
	}

	if (m_bLockThinkState)
		return false;

	if (!m_Enemy && !m_iCuriousTime)
		return true;

	return false;
}

bool Actor::IsIdleState
(
	int state
)

{
	return state == THINKSTATE_IDLE;
}