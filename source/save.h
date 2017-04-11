
#pragma once

#ifndef SAVE_H
#define SAVE_H

#include <3ds.h>
#include <fcntl.h>
#include <malloc.h>
#include <wchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define SAVE_VC_RGBY_LENGTH 32768

#include "dictionnary.h"

typedef struct SaveInfo {
	u8 OT_LENGTH;
	u8 PKMN_PARTY_DATA_LENGTH;
	u8 BOXCOUNT;
	u8 BOXSLOTS;
	u8 BOX_LENGTH;
	u8 PKMN_DATA_STR_LENGTH;
	u16 BOX_DATA_LENGTH;

	ushort* dictionnary;

	u16 OFS_TID;
	u16 OFS_STORAGE_DATA_1;
	u16 OFS_STORAGE_DATA_2;
	u16 OFS_PARTY_DATA;
	u16 OFS_CHECKSUM;
	u16 OFS_CURRENTBOX_INDEX;
	u16 OFS_CURRENTBOX;
} SaveInfo;


typedef struct SaveRGBY {
	u8* data;
	bool isJapanese;
	SaveInfo info;

	u8 versionID;
	u8 langID;
	u8 countryID;
	u8 regionID;
	u8 consoleRegionID;

} SaveRGBY;


typedef enum {
	INFOSAVE_G1_US_OT_LENGTH = 7,
	INFOSAVE_G1_US_BOXCOUNT = 12,
	INFOSAVE_G1_US_BOXSLOTS = 20,

	INFOSAVE_G1_JP_OT_LENGTH = 5,
	INFOSAVE_G1_JP_BOXCOUNT = 8,
	INFOSAVE_G1_JP_BOXSLOTS = 30,

	INFOSAVE_G1_PKMN_DATA_PARTY_SIZE =  44,
	INFOSAVE_G1_PKMN_DATA_STORED_SIZE = 33,
	INFOSAVE_G1_US_PKMN_DATA_STRING_LENGTH = 11,
	INFOSAVE_G1_JP_PKMN_DATA_STRING_LENGTH = 6,
} InfoSaveG1;

typedef enum {

	// Save Offset
	OFFSET_G1_OT                  = 0x2598,
	OFFSET_G1_STORAGE1            = 0x4000,
	OFFSET_G1_STORAGE2            = 0x6000,

	// Specific Save version offset
	OFFSET_G1_JP_TID              = 0x25FB,
	OFFSET_G1_JP_PARTY_DATA       = 0x2ED5,
	OFFSET_G1_JP_CHECKSUM         = 0x3594,
	OFFSET_G1_JP_CURRENTBOX_INDEX = 0x2842,
	OFFSET_G1_JP_CURRENTBOX       = 0x302D,

	OFFSET_G1_US_TID              = 0x2605,
	OFFSET_G1_US_PARTY_DATA       = 0x2F2C,
	OFFSET_G1_US_CHECKSUM         = 0x3523,
	OFFSET_G1_US_CURRENTBOX_INDEX = 0x284C,
	OFFSET_G1_US_CURRENTBOX       = 0x30C0,

	// Pkmn Data Offset
    OFFSET_G1_PKMN_RAW_SPECIES_ID = 0x00,

    OFFSET_G1_PKMN_TYPE1          = 0x05,
    OFFSET_G1_PKMN_TYPE2          = 0x06,

    OFFSET_G1_PKMN_MOVE1          = 0x08,
    OFFSET_G1_PKMN_MOVE2          = 0x09,
    OFFSET_G1_PKMN_MOVE3          = 0x0A,
    OFFSET_G1_PKMN_MOVE4          = 0x0B,

    OFFSET_G1_PKMN_TID            = 0x0C,

    OFFSET_G1_PKMN_EV_HP          = 0x11,
    OFFSET_G1_PKMN_EV_ATK         = 0x13,
    OFFSET_G1_PKMN_EV_DEF         = 0x15,
    OFFSET_G1_PKMN_EV_SPE         = 0x17,
    OFFSET_G1_PKMN_EV_SPC         = 0x19,

    OFFSET_G1_PKMN_IVS            = 0x1B,

    OFFSET_G1_PKMN_MOVE1PP        = 0x1D,
    OFFSET_G1_PKMN_MOVE2PP        = 0x1E,
    OFFSET_G1_PKMN_MOVE3PP        = 0x1F,
    OFFSET_G1_PKMN_MOVE4PP        = 0x20


} OffsetG1;

typedef enum {
	EV_HP,
	EV_ATK,
	EV_DEF,
	EV_SPE,
	EV_SPC
} EV_Gen1;

typedef enum {
	IV_HP,
	IV_ATK,
	IV_DEF,
	IV_SPE,
	IV_SPC
} IV_Gen1;


bool openSaveArch(FS_Archive *out, u64 id);
u64 getProgramID();
void fsStart();
void fsEnd();
bool isHBL();

SaveRGBY Gen1_SaveOpenWithBytes(u8* data);
u16 Gen1_SaveGetTID(SaveRGBY save);
u8 Gen1_SaveTotalPkmnInParty(SaveRGBY save);
u8 Gen1_SaveTotalBox(SaveRGBY save);
void Gen1_SaveGetOT(SaveRGBY save, wchar_t* otname);
u8 Gen1_SaveGetTotalPkmnInBox(SaveRGBY save, int box);
void Gen1_SaveGetPkmnInParty(SaveRGBY save, int index, u8* pkmn);
void Gen1_SaveGetPkmnInBox(SaveRGBY save, int box, int index, u8* pkmn);
u8 Gen1_SaveGetTotalPkmnInBox(SaveRGBY save, int box);

u8 Gen1_PkmnDataSize(SaveRGBY save);
u8 Gen1_PkmnGetType1(u8* pkmn);
u8 Gen1_PkmnGetType2(u8* pkmn);
u8 Gen1_PkmnGetMove(u8 *pkmn, u8 move);
u16 Gen1_PkmnGetTID(u8 *pkmn);
u8 Gen1_PkmnGetEV(u8 *pkmn, EV_Gen1 typeEV);
u8 Gen1_PkmnGetIV(u8 *pkmn, IV_Gen1 typeIV);
u8 Gen1_PkmnGetNationalDexID(u8 *pkmn);

void Gen1_PkmnGetOT(SaveRGBY save, u8* pkmn, wchar_t* otname);
void Gen1_PkmnGetNickname(SaveRGBY save, u8* pkmn, wchar_t* nickname);
#endif
