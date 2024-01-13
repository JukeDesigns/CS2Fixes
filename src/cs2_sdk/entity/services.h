/**
 * =============================================================================
 * CS2Fixes
 * Copyright (C) 2023 Source2ZE
 * =============================================================================
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License, version 3.0, as published by the
 * Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once
#include <platform.h>
#include "globaltypes.h"
#include <entity/ccsweaponbase.h>

class CCSPlayerPawn;
class CBaseEntity;
class CBasePlayerPawn;

struct CSPerRoundStats_t
{
public:
	DECLARE_SCHEMA_CLASS_INLINE(CSPerRoundStats_t)

	SCHEMA_FIELD(int, m_iKills)
	SCHEMA_FIELD(int, m_iDeaths)
	SCHEMA_FIELD(int, m_iAssists)
	SCHEMA_FIELD(int, m_iDamage)
};

struct CSMatchStats_t : public CSPerRoundStats_t
{
public:
	DECLARE_SCHEMA_CLASS_INLINE(CSMatchStats_t)
};

class CCSPlayerController_ActionTrackingServices
{
public:
	DECLARE_SCHEMA_CLASS(CCSPlayerController_ActionTrackingServices)

	SCHEMA_FIELD(CSMatchStats_t, m_matchStats)
};

class CPlayerPawnComponent
{
public:
	DECLARE_SCHEMA_CLASS(CPlayerPawnComponent);
	uint8_t vtable[0x8];
	uint8_t chainEntity[0x28]; // Unused
	CBasePlayerPawn *pawn; // 0x16
	uint8_t __pad0030[0x6]; // 0x0
};

class CPlayer_MovementServices : public CPlayerPawnComponent
{
public:
	DECLARE_SCHEMA_CLASS(CPlayer_MovementServices);

	SCHEMA_FIELD(CInButtonState, m_nButtons)
	SCHEMA_FIELD(uint64_t, m_nQueuedButtonDownMask)
	SCHEMA_FIELD(uint64_t, m_nQueuedButtonChangeMask)
	SCHEMA_FIELD(uint64_t, m_nButtonDoublePressed)

	// m_pButtonPressedCmdNumber[64]
	SCHEMA_FIELD_POINTER(uint32_t, m_pButtonPressedCmdNumber)
	SCHEMA_FIELD(uint32_t, m_nLastCommandNumberProcessed)
	SCHEMA_FIELD(uint64_t, m_nToggleButtonDownMask)
	SCHEMA_FIELD(float, m_flMaxspeed)
};

class CPlayer_WeaponServices
{
public:
	DECLARE_SCHEMA_CLASS(CPlayer_WeaponServices);

	SCHEMA_FIELD_POINTER(CUtlVector<CHandle<CBasePlayerWeapon>>, m_hMyWeapons)
	SCHEMA_FIELD(CHandle<CBasePlayerWeapon>, m_hActiveWeapon)
};

class CPlayer_MovementServices_Humanoid : public CPlayer_MovementServices
{
public:
	DECLARE_SCHEMA_CLASS(CPlayer_MovementServices_Humanoid);
	SCHEMA_FIELD(bool, m_bDucked)
};

class CCSPlayer_MovementServices : public CPlayer_MovementServices_Humanoid
{
public:
	DECLARE_SCHEMA_CLASS(CCSPlayer_MovementServices);
};

class CCSPlayerController_InGameMoneyServices
{
public:
	DECLARE_SCHEMA_CLASS(CCSPlayerController_InGameMoneyServices);

    SCHEMA_FIELD(int, m_iAccount)
};

class CCSPlayer_ItemServices
{
public:
	DECLARE_SCHEMA_CLASS(CCSPlayer_ItemServices);
	
	virtual ~CCSPlayer_ItemServices() = 0;
private:
	virtual void unk_01() = 0;
	virtual void unk_02() = 0;
	virtual void unk_03() = 0;
	virtual void unk_04() = 0;
	virtual void unk_05() = 0;
	virtual void unk_06() = 0;
	virtual void unk_07() = 0;
	virtual void unk_08() = 0;
	virtual void unk_09() = 0;
	virtual void unk_10() = 0;
	virtual void unk_11() = 0;
	virtual void unk_12() = 0;
	virtual void unk_13() = 0;
	virtual void unk_14() = 0;
	virtual CBaseEntity* _GiveNamedItem(const char* pchName) = 0;
public:
	virtual bool GiveNamedItemBool(const char* pchName) = 0;
	virtual CBaseEntity* GiveNamedItem(const char* pchName) = 0;
	virtual void DropPlayerWeapon(CBasePlayerWeapon* weapon) = 0;
	virtual void StripPlayerWeapons() = 0;
};

// We need an exactly sized class to be able to iterate the vector, our schema system implementation can't do this
class WeaponPurchaseCount_t
{
private:
	virtual void unk01() {};
	uint64_t unk1 = 0; // 0x8
	uint64_t unk2 = 0; // 0x10
	uint64_t unk3 = 0; // 0x18
	uint64_t unk4 = 0; // 0x20
	uint64_t unk5 = -1; // 0x28
public:
	uint16_t m_nItemDefIndex; // 0x30	
	uint16_t m_nCount; // 0x32
private:
	uint32_t unk6 = 0;
};

struct WeaponPurchaseTracker_t
{
public:
	DECLARE_SCHEMA_CLASS_INLINE(WeaponPurchaseTracker_t)

	SCHEMA_FIELD_POINTER(CUtlVector<WeaponPurchaseCount_t>, m_weaponPurchases)
};

class CCSPlayer_ActionTrackingServices
{
public:
	DECLARE_SCHEMA_CLASS(CCSPlayer_ActionTrackingServices)

	SCHEMA_FIELD(WeaponPurchaseTracker_t, m_weaponPurchasesThisRound)
};

struct RnCollisionAttr_t
{ // Unsure, doesn't seem right either for the first few members.
	uint64_t m_nInteractsAs;
	uint64_t m_nInteractsWith;
	uint64_t m_nInteractsExclude;
	uint32_t m_nEntityId;
	uint32_t m_nOwnerId;
	uint16_t m_nHierarchyId;
	uint8_t m_nCollisionGroup;
	uint8_t m_nCollisionFunctionMask;
};

struct alignas(16) trace_t_s2
{
	void *m_pSurfaceProperties;
	void *m_pEnt;
	void *m_pHitbox;
	void *m_hBody;
	void *m_hShape;
	uint64_t contents;
	Vector traceunknown[2];
	uint8_t padding[2];
	RnCollisionAttr_t m_ShapeAttributes;
	Vector startpos;
	Vector endpos;
	Vector planeNormal;
	Vector traceunknown1;
	float traceunknown2;
	float fraction;
	uint8_t traceunknown3[4];
	uint16_t traceunknown4;
	uint8_t traceType;
	bool startsolid;
};

struct touchlist_t {
	Vector deltavelocity;
	trace_t_s2 trace;
};

struct MoveDataUnkSubtickStruct
{
	uint8_t unknown[24];
};

// Size: 0xE8
class CMoveData
{
public:
	CMoveData() = default;
	CMoveData( const CMoveData &source ) : 
		moveDataFlags{source.moveDataFlags},
		m_nPlayerHandle{source.m_nPlayerHandle},
		m_vecAbsViewAngles{ source.m_vecAbsViewAngles},
		m_vecViewAngles{source.m_vecViewAngles},
		m_vecLastMovementImpulses{source.m_vecLastMovementImpulses},
		m_flForwardMove{source.m_flForwardMove},
		m_flSideMove{source.m_flSideMove},
		m_flUpMove{source.m_flUpMove},
		m_flSubtickFraction{source.m_flSubtickFraction},
		m_vecVelocity{source.m_vecVelocity},
		m_vecAngles{source.m_vecAngles},
		m_bGameCodeMovedPlayer{source.m_bGameCodeMovedPlayer},
		m_collisionNormal{source.m_collisionNormal},
		m_groundNormal{source.m_groundNormal},
		m_vecAbsOrigin{source.m_vecAbsOrigin},
		m_nGameModeMovedPlayer{source.m_nGameModeMovedPlayer},
		m_vecOldAngles{source.m_vecOldAngles},
		m_flMaxSpeed{source.m_flMaxSpeed},
		m_flClientMaxSpeed{source.m_flClientMaxSpeed},
		m_flSubtickAccelSpeed{source.m_flSubtickAccelSpeed},
		m_bJumpedThisTick{source.m_bJumpedThisTick},
		m_bShouldApplyGravity{source.m_bShouldApplyGravity},
		m_outWishVel{source.m_outWishVel}
	{
		for (int i = 0; i < source.unknown.Count(); i++)
		{
			this->unknown.AddToTail(source.unknown[i]);
		}
		for (int i = 0; i < source.m_TouchList.Count(); i++)
		{
			this->m_TouchList.AddToTail(source.m_TouchList[i]);
		}

	}
public:
	uint8_t moveDataFlags; // 0x0
	CHandle<CCSPlayerPawn> m_nPlayerHandle; // 0x4 don't know if this is actually a CHandle. <CBaseEntity> is a placeholder
	QAngle m_vecAbsViewAngles; // 0x8 unsure
	QAngle m_vecViewAngles; // 0x14
	Vector m_vecLastMovementImpulses;
	float m_flForwardMove; // 0x20
	float m_flSideMove; // 0x24
	float m_flUpMove; // 0x28
	float m_flSubtickFraction; // 0x38
	Vector m_vecVelocity; // 0x3c
	Vector m_vecAngles; // 0x48
	CUtlVector<MoveDataUnkSubtickStruct> unknown;
	bool m_bGameCodeMovedPlayer; // 0x70
	CUtlVector<touchlist_t> m_TouchList; // 0x78
	Vector m_collisionNormal; // 0x90
	Vector m_groundNormal; // 0x9c unsure
	Vector m_vecAbsOrigin; // 0xa8
	uint8_t padding[4]; // 0xb4 unsure
	bool m_nGameModeMovedPlayer; // 0xb8
	Vector m_vecOldAngles; // 0xbc
	float m_flMaxSpeed; // 0xc8
	float m_flClientMaxSpeed; // 0xcc
	float m_flSubtickAccelSpeed; // 0xd0 Related to ground acceleration subtick stuff with sv_stopspeed and friction
	bool m_bJumpedThisTick; // 0xd4 something to do with basevelocity and the tick the player jumps
	bool m_bShouldApplyGravity; // 0xd5
	Vector m_outWishVel; //0xd8
};
static_assert(sizeof(CMoveData) == 0xE8, "Class didn't match expected size");