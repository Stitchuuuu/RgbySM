#include "save.h"

u64 programID = 0;


const u8 SpeciesID[] = { 0x00, 0x70, 0x73, 0x20, 0x23, 0x15, 0x64, 0x22, 0x50, 0x02, 0x67, 0x6C, 0x66, 0x58, 0x5E, 0x1D, 0x1F, 0x68, 0x6F, 0x83, 0x3B, 0x97, 0x82, 0x5A, 0x48, 0x5C, 0x7B, 0x78, 0x09, 0x7F, 0x72, 0x00, 0x00, 0x3A, 0x5F, 0x16, 0x10, 0x4F, 0x40, 0x4B, 0x71, 0x43, 0x7A, 0x6A, 0x6B, 0x18, 0x2F, 0x36, 0x60, 0x4C, 0x00, 0x7E, 0x00, 0x7D, 0x52, 0x6D, 0x00, 0x38, 0x56, 0x32, 0x80, 0x00, 0x00, 0x00, 0x53, 0x30, 0x95, 0x00, 0x00, 0x00, 0x54, 0x3C, 0x7C, 0x92, 0x90, 0x91, 0x84, 0x34, 0x62, 0x00, 0x00, 0x00, 0x25, 0x26, 0x19, 0x1A, 0x00, 0x00, 0x93, 0x94, 0x8C, 0x8D, 0x74, 0x75, 0x00, 0x00, 0x1B, 0x1C, 0x8A, 0x8B, 0x27, 0x28, 0x85, 0x88, 0x87, 0x86, 0x42, 0x29, 0x17, 0x2E, 0x3D, 0x3E, 0x0D, 0x0E, 0x0F, 0x00, 0x55, 0x39, 0x33, 0x31, 0x57, 0x00, 0x00, 0x0A, 0x0B, 0x0C, 0x44, 0x00, 0x37, 0x61, 0x2A, 0x96, 0x8F, 0x81, 0x00, 0x00, 0x59, 0x00, 0x63, 0x5B, 0x00, 0x65, 0x24, 0x6E, 0x35, 0x69, 0x00, 0x5D, 0x3F, 0x41, 0x11, 0x12, 0x79, 0x01, 0x03, 0x49, 0x00, 0x76, 0x77, 0x00, 0x00, 0x00, 0x00, 0x4D, 0x4E, 0x13, 0x14, 0x21, 0x1E, 0x4A, 0x89, 0x8E, 0x00, 0x51, 0x00, 0x00, 0x04, 0x07, 0x05, 0x08, 0x06, 0x00, 0x00, 0x00, 0x00, 0x2B, 0x2C, 0x2D, 0x45, 0x46, 0x47, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

bool isHBL() {
    return programID != 0x000400000EC10000;
}

u64 getProgramID() {
	return programID;
}


void fsStart() {
	APT_GetProgramID(&programID);
    if(isHBL()) {
        Handle fsHandle;
        srvGetServiceHandleDirect(&fsHandle, "fs:USER");
        FSUSER_Initialize(fsHandle);
        fsUseSession(fsHandle);
    }
}

void fsEnd() {
    if(isHBL())
        fsEndUseSession();
}

bool openSaveArch(FS_Archive *out, u64 id) {
	/*
	if (!isHBL()) { //If we're using CIA
		u32 cardPath[3] = {MEDIATYPE_GAME_CARD, id, id >> 32}; //Card
		if (R_FAILED(FSUSER_OpenArchive(out, ARCHIVE_USER_SAVEDATA, (FS_Path){PATH_BINARY, 0xC, cardPath}))) { //If that fails, try digital
			u32 sdPath[3] = {MEDIATYPE_SD, id, id >> 32};
			if (R_FAILED(FSUSER_OpenArchive(out, ARCHIVE_USER_SAVEDATA, (FS_Path){PATH_BINARY, 0xC, sdPath})))
				return false;
			else
				return true;
		}
		else
			return true;
	}
	*/
	if (!isHBL()) {

	}
	else {
		if (R_SUCCEEDED(FSUSER_OpenArchive(out, ARCHIVE_SAVEDATA, fsMakePath(PATH_EMPTY, ""))))
			return true;
		else
			return false;
	}

	return false;
}

bool Gen1_SaveIsJapanese(SaveRGBY save) {

	int offsets[] = {0x2ED5, 0x302D};
	int nbOffsets = 2;

	for (int i = 0; i < nbOffsets; i++)
	{
		int ofs = offsets[i];
		u8 num_entries = save.data[ofs];
		if (num_entries > 30 || save.data[ofs + 1 + num_entries] != 0xFF)
			return false;
	}
	return true;
}

u16 swap_u16(u16 val) {
	return (val << 8) | (val >> 8);
}

SaveRGBY Gen1_SaveOpenWithBytes(u8* data) {
	SaveRGBY save;

	save.data = data;
	save.isJapanese = Gen1_SaveIsJapanese(save);

	initDictionnary(save.isJapanese);
	if (save.isJapanese) {
		save.info.OFS_TID              = OFFSET_G1_JP_TID;
		save.info.OFS_PARTY_DATA       = OFFSET_G1_JP_PARTY_DATA;
		save.info.OFS_CHECKSUM         = OFFSET_G1_JP_CHECKSUM;
		save.info.OFS_CURRENTBOX_INDEX = OFFSET_G1_JP_CURRENTBOX_INDEX;
		save.info.OFS_CURRENTBOX       = OFFSET_G1_JP_CURRENTBOX;

		save.info.OT_LENGTH            = INFOSAVE_G1_JP_OT_LENGTH;
		save.info.BOXCOUNT             = INFOSAVE_G1_JP_BOXCOUNT;
		save.info.BOXSLOTS             = INFOSAVE_G1_JP_BOXSLOTS;
		save.info.PKMN_DATA_STR_LENGTH = INFOSAVE_G1_JP_PKMN_DATA_STRING_LENGTH;

	} else {
		save.info.OFS_TID              = OFFSET_G1_US_TID;
		save.info.OFS_PARTY_DATA       = OFFSET_G1_US_PARTY_DATA;
		save.info.OFS_CHECKSUM         = OFFSET_G1_US_CHECKSUM;
		save.info.OFS_CURRENTBOX_INDEX = OFFSET_G1_US_CURRENTBOX_INDEX;
		save.info.OFS_CURRENTBOX       = OFFSET_G1_US_CURRENTBOX;

		save.info.OT_LENGTH            = INFOSAVE_G1_US_OT_LENGTH;
		save.info.BOXCOUNT             = INFOSAVE_G1_US_BOXCOUNT;
		save.info.BOXSLOTS             = INFOSAVE_G1_US_BOXSLOTS;
		save.info.PKMN_DATA_STR_LENGTH = INFOSAVE_G1_US_PKMN_DATA_STRING_LENGTH;
	}
	save.info.dictionnary = getDictionnary();

	// Box Data :
	//
	// Count               [1 byte]
	// ???                 [1 byte]
	// Pokemons Byte Data  [1 byte] * [BOXSLOTS]
	// Pokemon Data        [DATA_POKEMON_IN_BOX bytes] * [BOXSLOTS]
	// Pokemons OTs        [SIZE_STRING bytes] * [BOXSLOTS]
	// Pokemons Nikcnames  [SIZE_STRING bytes] * [BOXSLOTS]

	save.info.BOX_DATA_LENGTH = 2 + save.info.BOXSLOTS + (INFOSAVE_G1_PKMN_DATA_STORED_SIZE * save.info.BOXSLOTS) + (2*save.info.PKMN_DATA_STR_LENGTH*save.info.BOXSLOTS);

	return save;
}



void Gen1_DecodeString(SaveRGBY save, const u8* encoded, wchar_t* decoded, int length) {
	for (int i = 0; i < length; i++) {
		decoded[i] = (wchar_t)save.info.dictionnary[encoded[i]];
	}
	decoded[length] = 0;
}

u8 Gen1_GetTypeGen7(u8 typeGen1) {
	const int typesGen7[] = { 0, 1, 2, 3, 4, 5, 0, 6, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 10, 11, 12, 13, 14, 15 };
	return typesGen7[typeGen1];
}


u8 Gen1_SaveCurrentBoxIndex(SaveRGBY save) {
	return save.data[save.info.OFS_CURRENTBOX_INDEX]&0x7F;
}

bool Gen1_SaveIsCurrentBox(SaveRGBY save, int box) {
	return (Gen1_SaveCurrentBoxIndex(save) == box);
}


u16 Gen1_SaveGetBoxOffset(SaveRGBY save, int box) {
	u16 offsetBox;
	if (Gen1_SaveIsCurrentBox(save, box)) {
		offsetBox = save.info.OFS_CURRENTBOX;
	} else {
		offsetBox = OFFSET_G1_STORAGE1;
		if (box >= save.info.BOXCOUNT/2) {
			offsetBox = OFFSET_G1_STORAGE2;
			box -= save.info.BOXCOUNT/2;
		}
		offsetBox = offsetBox + (save.info.BOX_DATA_LENGTH*box);
	}
	return offsetBox;
}

void Gen1_SaveUpdateChecksum(SaveRGBY save) {
	u32 chksum = 0;
	for (int i = 0x2598; i < save.info.OFS_CHECKSUM; i++) {
		chksum += save.data[i];
	}

	chksum = ~chksum;
	chksum &= 0xFF;

	save.data[save.info.OFS_CHECKSUM] = (u8)chksum;
}

u16 Gen1_SaveGetTID(SaveRGBY save) {
	u16 buffer;
    memcpy(&buffer, &save.data[save.info.OFS_TID], 2);
    return swap_u16(buffer);
}

u8 Gen1_SaveTotalPkmnInParty(SaveRGBY save) {
    return save.data[save.info.OFS_PARTY_DATA];
}

u8 Gen1_SaveTotalBox(SaveRGBY save) {
	return save.info.BOXCOUNT;
}


void Gen1_SaveGetOT(SaveRGBY save, wchar_t* otname) {
	u8 encodedOT[save.info.OT_LENGTH];
	memcpy(encodedOT, &save.data[OFFSET_G1_OT], save.info.OT_LENGTH);

	Gen1_DecodeString(save, encodedOT, otname, save.info.OT_LENGTH);
}

u8 Gen1_PkmnDataSize(SaveRGBY save) {
	return INFOSAVE_G1_PKMN_DATA_STORED_SIZE + 2*save.info.PKMN_DATA_STR_LENGTH;
}


void Gen1_SaveGetPkmnInParty(SaveRGBY save, int index, u8* pkmn) {
	u8 nbPkmns = 6;
	u16 offsetBase = save.info.OFS_PARTY_DATA + 2 + nbPkmns; // PartyCount, ???, and 1 byte for ??? for each Pokemon in party
	u16 offsetData = offsetBase + index*INFOSAVE_G1_PKMN_DATA_PARTY_SIZE;
	u16 offsetOT   = offsetBase + (nbPkmns*INFOSAVE_G1_PKMN_DATA_PARTY_SIZE) + (index*save.info.PKMN_DATA_STR_LENGTH);
	u16 offsetNick = offsetBase + (nbPkmns*INFOSAVE_G1_PKMN_DATA_PARTY_SIZE) + (nbPkmns*save.info.PKMN_DATA_STR_LENGTH) + (index*save.info.PKMN_DATA_STR_LENGTH);

	memcpy(pkmn, &save.data[offsetData], INFOSAVE_G1_PKMN_DATA_STORED_SIZE);

	// We are putting OT and Nickname at the end of the data
	memcpy(pkmn+INFOSAVE_G1_PKMN_DATA_STORED_SIZE, &save.data[offsetOT], save.info.PKMN_DATA_STR_LENGTH);
	memcpy(pkmn+INFOSAVE_G1_PKMN_DATA_STORED_SIZE+save.info.PKMN_DATA_STR_LENGTH, &save.data[offsetNick], save.info.PKMN_DATA_STR_LENGTH);
}


void Gen1_SaveGetPkmnInBox(SaveRGBY save, int box, int index, u8* pkmn) {
	u8 nbPkmns = save.info.BOXSLOTS;

	u16 offsetBase = Gen1_SaveGetBoxOffset(save, box) + 2 + nbPkmns;
	u16 offsetData = offsetBase + (index*INFOSAVE_G1_PKMN_DATA_STORED_SIZE);
	u16 offsetOT   = offsetBase + (nbPkmns*INFOSAVE_G1_PKMN_DATA_STORED_SIZE) + (index*save.info.PKMN_DATA_STR_LENGTH);
	u16 offsetNick = offsetBase + (nbPkmns*INFOSAVE_G1_PKMN_DATA_STORED_SIZE) + (nbPkmns*save.info.PKMN_DATA_STR_LENGTH) + (index*save.info.PKMN_DATA_STR_LENGTH);

	memcpy(pkmn, &save.data[offsetData], INFOSAVE_G1_PKMN_DATA_STORED_SIZE);

	// We are putting OT and Nickname at the end of the data
	memcpy(pkmn+INFOSAVE_G1_PKMN_DATA_STORED_SIZE, &save.data[offsetOT], save.info.PKMN_DATA_STR_LENGTH);
	memcpy(pkmn+INFOSAVE_G1_PKMN_DATA_STORED_SIZE+save.info.PKMN_DATA_STR_LENGTH, &save.data[offsetNick], save.info.PKMN_DATA_STR_LENGTH);
}

u8 Gen1_SaveGetTotalPkmnInBox(SaveRGBY save, int box) {
	u16 offsetBox = Gen1_SaveGetBoxOffset(save, box);
	return save.data[offsetBox];
}

u8 Gen1_PkmnGetNationalDexID(u8 *pkmn) {
	u8 rawID = pkmn[OFFSET_G1_PKMN_RAW_SPECIES_ID];
	u8 speciesID = SpeciesID[rawID];
	return speciesID;
}

u8 Gen1_PkmnGetRawType1(u8 *pkmn) {
	return pkmn[OFFSET_G1_PKMN_TYPE1];
}

u8 Gen1_PkmnGetRawType2(u8 *pkmn) {
	return pkmn[OFFSET_G1_PKMN_TYPE2];
}

u8 Gen1_PkmnGetType1(u8* pkmn) {
	return Gen1_GetTypeGen7(Gen1_PkmnGetRawType1(pkmn));
}
u8 Gen1_PkmnGetType2(u8* pkmn) {
	return Gen1_GetTypeGen7(Gen1_PkmnGetRawType2(pkmn));
}

u8 Gen1_PkmnGetMove(u8 *pkmn, u8 move) {
	if (move < 1) {
		move = 1;
	}
	if (move > 4) {
		move = 4;
	}
	return pkmn[OFFSET_G1_PKMN_MOVE1+move-1];
}

u16 Gen1_PkmnGetTID(u8 *pkmn) {
	u16 buffer;
    memcpy(&buffer, &pkmn[OFFSET_G1_PKMN_TID], 2);
    return swap_u16(buffer);
}

u8 Gen1_PkmnGetEV(u8 *pkmn, EV_Gen1 typeEV) {
	const u8 offsets[] = { OFFSET_G1_PKMN_EV_HP, OFFSET_G1_PKMN_EV_ATK, OFFSET_G1_PKMN_EV_DEF, OFFSET_G1_PKMN_EV_SPE, OFFSET_G1_PKMN_EV_SPC };
	u8 offset = offsets[typeEV];

	u16 buffer;
    memcpy(&buffer, &pkmn[offset], 2);
    return swap_u16(buffer);
}

u8 Gen1_PkmnGetIV(u8 *pkmn, IV_Gen1 typeIV) {
	u16 buffer;
    memcpy(&buffer, &pkmn[OFFSET_G1_PKMN_IVS], 2);
    buffer = swap_u16(buffer);

	u8 value = 0;
	switch(typeIV) {
		case IV_HP:
			value = ((Gen1_PkmnGetIV(pkmn, IV_ATK) & 1) << 3) |
					  ((Gen1_PkmnGetIV(pkmn, IV_DEF) & 1) << 2) |
					  ((Gen1_PkmnGetIV(pkmn, IV_SPE) & 1) << 1) |
					  ((Gen1_PkmnGetIV(pkmn, IV_SPC) & 1) << 0);
			break;
		case IV_ATK:
			value = (buffer >> 12) & 0xF;
			break;
		case IV_DEF:
			value = (buffer >> 8) & 0xF;
			break;
		case IV_SPE:
			value = (buffer >> 4) & 0xF;
			break;
		case IV_SPC:
			value = (buffer >> 0) & 0xF;
			break;
	}
	return value;
}


void Gen1_PkmnGetOT(SaveRGBY save, u8* pkmn, wchar_t* otname) {
	u8 encodedOT[save.info.PKMN_DATA_STR_LENGTH];
	memcpy(encodedOT, &pkmn[INFOSAVE_G1_PKMN_DATA_STORED_SIZE], save.info.PKMN_DATA_STR_LENGTH);

	Gen1_DecodeString(save, encodedOT, otname, save.info.PKMN_DATA_STR_LENGTH);
}

void Gen1_PkmnGetNickname(SaveRGBY save, u8* pkmn, wchar_t* nickname) {
	u8 encodedNick[save.info.PKMN_DATA_STR_LENGTH];
	memcpy(encodedNick, &pkmn[INFOSAVE_G1_PKMN_DATA_STORED_SIZE+save.info.PKMN_DATA_STR_LENGTH], save.info.PKMN_DATA_STR_LENGTH);

	Gen1_DecodeString(save, encodedNick, nickname, save.info.PKMN_DATA_STR_LENGTH);
}

void Gen1_SaveSetPkmnInParty(SaveRGBY save, int index, u8* pkmn) {
	u8 nbPkmns = 6;

	u16 offsetBase = save.info.OFS_PARTY_DATA + 2 + nbPkmns;
	u16 offsetData = offsetBase + index*INFOSAVE_G1_PKMN_DATA_PARTY_SIZE;

	for (u16 offset = 0; offset < INFOSAVE_G1_PKMN_DATA_STORED_SIZE; offset++) {
		save.data[offsetData+offset] = pkmn[offset];
	}
	Gen1_SaveUpdateChecksum(save);
}
