#include "save.gen7.h"

u16 Gen7_PkmnGetSize() {
	return GEN7_PKMN_LENGTH;
}

u32 Gen7_SeedStep(const u32 seed) { return (seed * 0x41C64E6D + 0x00006073) & 0xFFFFFFFF; }

void Gen7_PkmnShuffleArray(u8* pkmn, const u32 encryptionkey) {
    const int BLOCKLENGTH = 56;

    u8 seed = (((encryptionkey & 0x3E000) >> 0xD) % 24);

    int aloc[24] = { 0, 0, 0, 0, 0, 0, 1, 1, 2, 3, 2, 3, 1, 1, 2, 3, 2, 3, 1, 1, 2, 3, 2, 3 };
    int bloc[24] = { 1, 1, 2, 3, 2, 3, 0, 0, 0, 0, 0, 0, 2, 3, 1, 1, 3, 2, 2, 3, 1, 1, 3, 2 };
    int cloc[24] = { 2, 3, 1, 1, 3, 2, 2, 3, 1, 1, 3, 2, 0, 0, 0, 0, 0, 0, 3, 2, 3, 2, 1, 1 };
    int dloc[24] = { 3, 2, 3, 2, 1, 1, 3, 2, 3, 2, 1, 1, 3, 2, 3, 2, 1, 1, 0, 0, 0, 0, 0, 0 };
    int ord[4] = {aloc[seed], bloc[seed], cloc[seed], dloc[seed]};

    char pkmncpy[GEN7_PKMN_LENGTH];
    char tmp[BLOCKLENGTH];

    memcpy(&pkmncpy, pkmn, GEN7_PKMN_LENGTH);

    for (int i = 0; i < 4; i++) {
        memcpy(tmp, pkmncpy + 8 + BLOCKLENGTH * ord[i], BLOCKLENGTH);
        memcpy(pkmn + 8 + BLOCKLENGTH * i, tmp, BLOCKLENGTH);
    }
}

void Gen7_PkmnEncrypt(u8* pkmn) {
    const int ENCRYPTIONKEYPOS = 0x0;
    const int ENCRYPTIONKEYLENGTH = 4;
    const int CRYPTEDAREAPOS = 0x08;

    u32 encryptionkey;
    memcpy(&encryptionkey, &pkmn[ENCRYPTIONKEYPOS], ENCRYPTIONKEYLENGTH);
    u32 seed = encryptionkey;

    for(int i = 0; i < 11; i++)
        Gen7_PkmnShuffleArray(pkmn, encryptionkey);

    u16 temp;
    for(int i = CRYPTEDAREAPOS; i < GEN7_PKMN_LENGTH; i += 2) {
        memcpy(&temp, &pkmn[i], 2);
        temp ^= (Gen7_SeedStep(seed) >> 16);
        seed = Gen7_SeedStep(seed);
        memcpy(&pkmn[i], &temp, 2);
    }
}

void Gen7_PkmnDecrypt(u8* pkmn) {
    const int ENCRYPTIONKEYPOS = 0x0;
    const int ENCRYPTIONKEYLENGTH = 4;
    const int CRYPTEDAREAPOS = 0x08;

    u32 encryptionkey;
    memcpy(&encryptionkey, &pkmn[ENCRYPTIONKEYPOS], ENCRYPTIONKEYLENGTH);
    u32 seed = encryptionkey;

    u16 temp;
    for (int i = CRYPTEDAREAPOS; i < GEN7_PKMN_LENGTH; i += 2) {
        memcpy(&temp, &pkmn[i], 2);
        temp ^= (Gen7_SeedStep(seed) >> 16);
        seed = Gen7_SeedStep(seed);
        memcpy(&pkmn[i], &temp, 2);
    }

    Gen7_PkmnShuffleArray(pkmn, encryptionkey);
}

void Gen7_PkmnRerollEncryptionKey(u8* pkmn) {
    const int ENCRYPTIONKEYPOS = 0x0;
    const int ENCRYPTIONKEYLENGTH = 4;

	srand(time(NULL));
	u32 encryptbuffer = rand();
	memcpy(&pkmn[ENCRYPTIONKEYPOS], &encryptbuffer, ENCRYPTIONKEYLENGTH);
}

void Gen7_PkmnCalculateChecksum(u8* data) {
    u16 chk = 0;

    for (int i = 8; i < GEN7_PKMN_LENGTH; i += 2)
        chk += *(u16*)(data + i);

    memcpy(data + 6, &chk, 2);
}

u16 Gen7_PkmnGetOTID(u8* pkmn) {
    u16 otidbuffer;
    memcpy(&otidbuffer, &pkmn[0x0C], GEN7_OTIDLENGTH);
    return otidbuffer;
}

u16 Gen7_PkmnGetSOTID(u8* pkmn) {
    u16 sotidbuffer;
    memcpy(&sotidbuffer, &pkmn[0x0E], GEN7_SOTIDLENGTH);
    return sotidbuffer;
}

u32 Gen7_PkmnGetPID(u8* pkmn) {
    u32 pidbuffer;
    memcpy(&pidbuffer, &pkmn[0x18], GEN7_PIDLENGTH);
    return pidbuffer;
}

u16 Gen7_PkmnGetTSV(u8* pkmn) {
	u16 TID = Gen7_PkmnGetOTID(pkmn);
	u16 SID = Gen7_PkmnGetSOTID(pkmn);
	return (TID ^ SID) >> 4;
}

u16 Gen7_PkmnGetPSV(u8* pkmn) {
	u32 PID = Gen7_PkmnGetPID(pkmn);
	return ((PID >> 16) ^ (PID & 0xFFFF)) >> 4;
}

void Gen7_PkmnSetTID(u8* pkmn, u16 tid) {
    memcpy(&pkmn[0x0C], &tid, 2);
}

void Gen7_PkmnSetSID(u8* pkmn, u16 sid) {
    memcpy(&pkmn[0x0E], &sid, 2);
}

u16 Gen7_PkmnGetPokedexNumber(u8* pkmn) {
    u16 pokedexnumber;
    memcpy(&pokedexnumber, &pkmn[0x08], GEN7_POKEDEXNUMBERLENGTH);
    return pokedexnumber;
}

void Gen7_PkmnSetPokedexNumber(u8* pkmn, u16 pokedexnumber) {
    memcpy(&pkmn[0x08], &pokedexnumber, GEN7_POKEDEXNUMBERLENGTH);
}

void Gen7_PkmnSetAbility(u8* pkmn, const u8 ability) {

	u8 abilitynum = 0;
	if (ability == 0)      abilitynum = 1;
	else if (ability == 1) abilitynum = 2;
	else                   abilitynum = 4;

	memcpy(&pkmn[0x15], &abilitynum, GEN7_ABILITYNUMLENGTH);
	// Need to load PKMData
    // u16 tempspecies = Gen7_PkmnGetPokedexNumber(pkmn);
	//memcpy(&pkmn[0x14], &personal.pkmData[tempspecies][0x09 + ability], ABILITYLENGTH);
}

void Gen7_PkmnSetMove(u8* pkmn, const u16 move, const int nmove) {
    memcpy(&pkmn[0x5A + (GEN7_MOVELENGTH * nmove)], &move, GEN7_MOVELENGTH);
}

/*
bool Gen7_PkmnSetIsShiny(u8* pkmn) {
    u16 trainersv = (getOTID(pkmn) ^ getSOTID(pkmn)) >> 4;
    u16 pkmnv = ((getPID(pkmn) >> 16) ^ (getPID(pkmn) & 0xFFFF)) >> 4;

    if (trainersv == pkmnv) return true;
    else return false;
}
*/

void Gen7_PkmnRerollPID(u8* pkmn) {
    srand(Gen7_PkmnGetPID(pkmn));
    u32 pidbuffer = rand();
    memcpy(&pkmn[0x18], &pidbuffer, GEN7_PIDLENGTH);
}

void Gen7_PkmnSetShiny(u8* pkmn, const bool shiny) {
	if (!shiny)
		Gen7_PkmnRerollPID(pkmn);
	else {
		u16 tsv = (Gen7_PkmnGetOTID(pkmn) ^ Gen7_PkmnGetSOTID(pkmn)) >> 4;
		u16 buffer = (Gen7_PkmnGetPID(pkmn) >> 16) ^ (tsv << 4);
		memcpy(&pkmn[0x18], &buffer, 2);
	}
}
void Gen7_PkmnSetNature(u8* pkmn, const u8 nature) {
    memcpy(&pkmn[0x1C], &nature, GEN7_NATURELENGTH);
}

void Gen7_PkmnSetFlag(u8* pkmn, int flgaddr, int flgshift, bool value) {
	if (flgaddr < 0 || GEN7_PKMN_LENGTH <= flgaddr || flgshift < 0 || 8 <= flgshift)return;
	u8 tmp;
	memcpy(&tmp, &pkmn[flgaddr], 1);
	tmp = (u8)((tmp & ~(1 << flgshift)) | (value ? 1 << flgshift : 0));
	memcpy(&pkmn[flgaddr], &tmp, 1);
}

// dst 0x40(Nickname) 0xB0(OT) 0x78(HT)
void Gen7_PkmnSetName(u8* pkmn, char* nick, int dst) {

	// !!! UTF8 to UTF16, possibility to compress with CTRULIB util function

	// dst 0x40(Nickname) 0xB0(OT) 0x78(HT)
	u8 toinsert[GEN7_NICKNAMELENGTH];
	memset(toinsert, 0, GEN7_NICKNAMELENGTH);

	if (!memcmp(nick, toinsert, GEN7_NICKNAMELENGTH))
		return;

	char buf;
	int nicklen = strlen(nick);
	int r = 0, w = 0, i = 0;
	while (r < nicklen) {
		buf = *(nick + r);
		r++;
		if ((buf & 0x80) == 0) {
			toinsert[w] = buf & 0x7f;
			i = 0;
		//	w += 2;
		}
		else if ((buf & 0xe0) == 0xc0) {
			toinsert[w] = buf & 0x1f;
			i = 1;
		}
		else if ((buf & 0xf0) == 0xe0) {
			toinsert[w] = buf & 0x0f;
			i = 2;
		}
		else break;
		for (int j = 0; j < i; j++) {
			buf = *(nick + r);
			r++;
			if (toinsert[w] > 0x04) {
				toinsert[w + 1] = (toinsert[w + 1] << 6) | (((toinsert[w] & 0xfc) >> 2) & 0x3f);
				toinsert[w] &= 0x03;
			}
			toinsert[w] = (toinsert[w] << 6) | (buf & 0x3f);
		}
		// r++;
		w += 2;
		if (w > GEN7_NICKNAMELENGTH)
			break;
	}

	if (dst == 0x40) {
		u8 isnicknamed;
		memcpy(&isnicknamed, &pkmn[0x77], 1);
		isnicknamed |= 0x80;
		memcpy(&pkmn[0x77], &isnicknamed, 1);
	}

	memcpy(&pkmn[dst], toinsert, GEN7_NICKNAMELENGTH);
}


void Gen7_PkmnSetIV(u8* pkmn, u8 val, const int stat) {
	u32 nval = val;
	u32 mask = 0xFFFFFFFF;
	mask ^= 0x1F << (5 * stat);

	u32 buffer;
	memcpy(&buffer, &pkmn[0x74], GEN7_IVLENGTH);

	buffer &= mask;
	buffer ^= ((nval & 0x1F) << (5 * stat));
	memcpy(&pkmn[0x74], &buffer, GEN7_IVLENGTH);
}



void Gen7_PkmnSetHTGender(u8* pkmn, const u8 gender) {
	memcpy(&pkmn[0x92], &gender, 1);
}

void Gen7_PkmnSetMetLocation(u8* pkmn, u16 locationID) {
	u16 val = (locationID << 8) | (locationID >> 8);
	memcpy(&pkmn[0xDA], &val, 2);
}

void Gen7_PkmnSetGender(u8* pkmn, u8 val)    { pkmn[0x1D] = (u8)((pkmn[0x1D] & ~0x06) | (val << 1)); }
void Gen7_PkmnSetBall(u8* pkmn, u8 val)      { pkmn[0xDC] = val; }
void Gen7_PkmnSetOTGender(u8* pkmn, u8 val)  { Gen7_PkmnSetFlag(pkmn, 0xDD, 7, (val == 1) ? true : false); }

void Gen7_PkmnSetIsNicknamed(u8* pkmn, bool value) { Gen7_PkmnSetFlag(pkmn, 0x77, 7, value); }
void Gen7_PkmnSetNickname(u8* pkmn, char* nick) { Gen7_PkmnSetName(pkmn, nick, 0x40); }
void Gen7_PkmnSetOTName(u8* pkmn, char* nick)   { Gen7_PkmnSetName(pkmn, nick, 0xB0); }
void Gen7_PkmnSetHTName(u8* pkmn, char* nick)   { Gen7_PkmnSetName(pkmn, nick, 0x78); }

void Gen7_PkmnFromGen1(u8* gen1pkmn, u8* gen7pkmn) {
	// Get country from gameid
	// Ability is always hidden ability, must checked with all Pokémon
	// Nature is random
	// EV ?
	// IV : 3IV at 31
	// IsNicknamed
	// Pokeball ?
	// Add Gameboy Flag = Location : 30013
	// Region, Console ?
	// Mewtwo Jackpot removed
	// Mew fateful encounter + Renaming + OT
	// Check if Mew legit or not ?


	// Encryption constante: random
	// Sanity: 0
	// Checksum: à faire
	// Species : ID de l'espèce
	// Held Item : 0
	// TID: TID Pokémon RGB
	// SID: 0
	// Expérience: check à faire
	// Ability: hidden ability
	// Ability Number : ? (4?)
	// PID: Random (no shiny)
	// Nature: Random
	// Gender: Random (si Pokémon assexué)
	// EV: 0
	// Nickname: à convertir
	// Move: on prends les même
	// Move PP: on prends les PP actuels
	// Move PPUp: On prends les PPUp dans le jeu
	// Aucun relearn move
	// IV: 3 random
	// isNicknamed : false si nom d'origine dans version
	// isEgg : false
	// HT Name: Dresseur de la save qui importe
	// HT Gender: Sexe du dresseur de la save qui importe
	// HT Friendship: 0 (pour le moment, base de données de friendship à avoir par la suite)
	// HT Affection: 0
	// HT Intensity: 1
	// HT Memory: 4
	// HT TextVar: 0
	// HT Feeling: rand(0,9)
	// Current Handler : true
	// Latest Country & Region 1: Console 3DS
	// OT Name: Dresseur de la save importée
	// OT Frienship: 0 (identique à HT Friendship)
	// OT Affection: 0
	// OT Intensity: 0
	// OT Memory: 0
	// OT TextVar: 0
	// Egg: 0
	// Met date: date courante
	// Met location: 0x753d
	// Ball: 4 toujours même Mew
	// Met level: Niveau dans la save importée
	// OT Gender: Male
	// Version: Version du jeu (à récupérer quelque part)
	// Country: Pays préférence console
	// Region: Refion préférence console
	// Console Region: Dans CTRULIB
	// Language: Selon version du jeu
	// Shiny: IV_ATK == 10 && IV_SPE == 10 && IV_SPC == 10 && (IV_DEF & 2) == 2

	//HT - Save courante
	//HT_Feeling = Util.rand.Next(0, 9); // 0-9 Bank


	// Mew Event
	// IV: ATK 10, DEF 1, SpC 5, VIT 12
	// TID: 22796
	// OT: GF / ゲーフリ
	// Nickname: Mew / ミュウ

	// Excelangue : Pokémon Jaune uniquement, sinon OT Trainer
	// M. Mime: OT Trainer / トレーナ uniquement
}
