#include "common.h"


// Petit commentaire des familles 6
void initServices() {
	aptInit();
	hidInit();

	gfxInitDefault();
	cfguInit();
	sdmcInit();
	romfsInit();
	fsStart();
	consoleInit(GFX_TOP, NULL);
	i18n_init();
}

void exitServices() {
	i18n_exit();
	fsEnd();
	romfsExit();
	sdmcExit();
	cfguExit();
	gfxExit();

	hidExit();
	aptExit();
}

void freezeMsg(wchar_t* message) {
	printf("= %ls\n", message);
}

void freezeMsgDetails(wchar_t* message) {
	printf("%ls\n", message);
}

const u64 VC_RGBY_IDS[] = { 0x0004000000170c00, 0x0004000000170d00, 0x0004000000170e00, 0x0004000000170f00, 0x0004000000171000, 0x0004000000171100, 0x0004000000171200, 0x0004000000171300, 0x0004000000171400, 0x0004000000171500, 0x0004000000171600, 0x0004000000171700, 0x0004000000171800, 0x0004000000171900, 0x0004000000171a00, 0x0004000000171b00, 0x0004000000171c00, 0x0004000000171d00, 0x0004000000171e00 };
const int VC_RGBY_TOTAL = 19; // EN:3 , FR :3, ES: 3, DE: 3, IT: 3, JP: 4
wchar_t* VC_RGBY_NAMES[] = { L"ポケットモンスター 赤 (日文版)", L"ポケットモンスター 綠 (日文版)", L"ポケットモンスター 青 (日文版)", L"ポケットモンスター ピカチュウ (日文版)", L"Pokémon™ Red Version", L"Pokémon™ Blue Version", L"Pokémon™ Yellow Version", L"Pokémon™ Rote Edition", L"Pokémon™ Blaue Edition", L"Pokémon™ Gelbe Edition", L"Pokémon™ Version Rouge", L"Pokémon™ Version Bleue", L"Pokémon™ Version Jaune", L"Pokémon™ Edición Roja", L"Pokémon™ Edición Azul", L"Pokémon™ Edición Amarilla", L"Pokémon™ Versione Rossa", L"Pokémon™ Versione Blu", L"Pokémon™ Versione Gialla" };

SaveRGBY save;

wchar_t* getNameGame(u64 id) {
	int i = 0;
	bool founded = false;
	for (i = 0; i < VC_RGBY_TOTAL; i++) {
		if (id == VC_RGBY_IDS[i]) {
			founded = true;
			break;
		}
	}
	wchar_t* name = L"";
	if (founded) {
		name = VC_RGBY_NAMES[i];
	}
	return name;
}

bool isVirtualConsoleRGBY(u64 id) {
	bool founded = false;
	for (int i = 0; i < VC_RGBY_TOTAL; i++) {
		if (id == VC_RGBY_IDS[i]) {
			founded = true;
			break;
		}
	}
	return founded;
}


u8* readSaveRGBYFile(char* path) {
	FILE *handle = fopen(path,"rb");
	u8* save = NULL;
	if (handle != NULL) {

		fseek(handle, 0, SEEK_END);
		int size = ftell(handle);

		if (size == SAVE_VC_RGBY_LENGTH) {
			save = malloc(sizeof(u8)*SAVE_VC_RGBY_LENGTH);
			rewind(handle);
			fread(save, sizeof(u8), SAVE_VC_RGBY_LENGTH, handle);
		} else {
			printf("Size Error ! Save file is %i, VC Save must be %i !\n", size, SAVE_VC_RGBY_LENGTH);
		}
		fclose(handle);
	} else {
			printf("File %s not found !\n", path);
	}
	return save;
}

bool writeSaveRGBYFile(u8* data, char* path) {
	FILE *handle = fopen(path, "wb");
	bool saved = false;
	if (handle != NULL) {
		fwrite(data, sizeof(u8), SAVE_VC_RGBY_LENGTH, handle);
		fclose(handle);
		saved = true;
	} else {
		printf("File %s can not be created !\n", path);
	}
	return saved;
}

void printInfo(u8* pkmn) {
	wchar_t otname[12];
	wchar_t nickname[12];
	Gen1_PkmnGetOT(save, pkmn, otname);
	Gen1_PkmnGetNickname(save, pkmn, nickname);

	int type1 = Gen1_PkmnGetType1(pkmn);
	int type2 = Gen1_PkmnGetType2(pkmn);

	const char* types[] = { "Normal", "Fighting", "Flying", "Poison", "Ground", "Rock", "Bug", "Ghost", "Steel", "Fire", "Water", "Grass", "Electric", "Psychic", "Ice", "Dragon", "Dark", "Fairy" };

	printf("Nickname: %ls\n", nickname);
	printf("OT: %i / %ls\n",     Gen1_PkmnGetTID(pkmn), otname);
	printf("Species: %i\n", Gen1_PkmnGetNationalDexID(pkmn));
	if (type1 != type2) {
		printf("Type: %s / %s\n", types[type1], types[type2]);
	} else {
		printf("Type: %s\n", types[type1]);
	}
	printf("Moves: %i / %i / %i / %i\n", Gen1_PkmnGetMove(pkmn, 1), Gen1_PkmnGetMove(pkmn, 2), Gen1_PkmnGetMove(pkmn, 3), Gen1_PkmnGetMove(pkmn, 4));
	printf("\n");
	printf("               HP     ATK     DEF     SPE     SPC\n");
	printf("IVs            %02i      %02i      %02i      %02i      %02i\n", Gen1_PkmnGetIV(pkmn, IV_HP), Gen1_PkmnGetIV(pkmn, IV_ATK), Gen1_PkmnGetIV(pkmn, IV_DEF), Gen1_PkmnGetIV(pkmn, IV_SPE), Gen1_PkmnGetIV(pkmn, IV_SPC));
	printf("EVs           %03i     %03i     %03i     %03i     %03i\n", Gen1_PkmnGetEV(pkmn, EV_HP), Gen1_PkmnGetEV(pkmn, EV_ATK), Gen1_PkmnGetEV(pkmn, EV_DEF), Gen1_PkmnGetEV(pkmn, EV_SPE), Gen1_PkmnGetEV(pkmn, EV_SPC));
	printf("\n");
}

void mainLoop(bool fromCIA, u64 id) {
	char *message = "Press START to reboot to Homebrew Menu\n      A to print details\n      B to print Pokemon Party slot %i\n      X to print Pokemon in Boxes\n\n";
	int indexParty = 0;
	int indexInBox = 0;
	int indexBox = 0;
	printf(message, indexParty + 1);
	while (aptMainLoop()) {
		hidScanInput();
		u32 key = hidKeysDown();
		if (key & KEY_START) {
			printf("Exiting...\n");
			break;
		}
		if (key & KEY_A) {
			consoleClear();
			wchar_t nickname[10];
			Gen1_SaveGetOT(save, nickname);

			printf("Save TID: %u\n", Gen1_SaveGetTID(save));
			printf("Save OT: %ls\n", nickname);
			printf("Pokemon in Party: %u\n", Gen1_SaveTotalPkmnInParty(save));
			printf("Pokemon in Box1: %u\n", Gen1_SaveGetTotalPkmnInBox(save, 0));
			printf("Pokemon in Box2: %u\n", Gen1_SaveGetTotalPkmnInBox(save, 1));
			printf("\n");
		}

		if (key & KEY_B) {

			consoleClear();
			u8 pkmn[Gen1_PkmnDataSize(save)];
			Gen1_SaveGetPkmnInParty(save, indexParty, pkmn);
			printf("Pokemon %i in Party : \n", indexParty + 1);
			printInfo(pkmn);
			indexParty = (indexParty + 1)%(Gen1_SaveTotalPkmnInParty(save));
		}
		if (key & KEY_X) {
			consoleClear();
			printf("(X) Pokemon %i in Box %i: \n", indexInBox+1, indexBox+1);
			u8 pkmn[Gen1_PkmnDataSize(save)];
			Gen1_SaveGetPkmnInBox(save, indexBox, indexInBox, pkmn);
			printInfo(pkmn);

			// Finding next Pokemon to display
			indexInBox++;
			if (indexInBox >= Gen1_SaveGetTotalPkmnInBox(save, indexBox)) {
				indexInBox = 0;
				int totalBox = Gen1_SaveTotalBox(save);
				int i;
				for (i = indexBox+1; i < totalBox; i++) {
					if (Gen1_SaveGetTotalPkmnInBox(save, i) > 0) {
						indexBox = i;
						break;
					}
				}
				if (i == totalBox) { // Found no new box with a Pokemon returning to Box 1
					indexBox = 0;
				}
			}
		}
		if (key) {
			printf(message, indexParty+1);
		}
		gfxFlushBuffers();
		gfxSwapBuffers();
		gspWaitForVBlank();
	}
}


void exitMenu() {
	printf("Press a key to reboot to Homebrew Menu.\n");
	while (aptMainLoop()) {
		hidScanInput();
		if (hidKeysDown()) {
			printf("Exiting...\n");
			break;
		}
		gfxFlushBuffers();
		gfxSwapBuffers();
		gspWaitForVBlank();
	}
}

int main(int argc, char **argv) {

	initServices();

	u64 id = getProgramID();
	u8* mainbuf;
	u64 mainSize = 0;
	Handle mainHandle;
	FS_Archive saveArch;

	// Code for getting user country and region, console region, and language
	//
	// printf("Trying to get CFG...\n");
	// Config3DS config3DS = getConfig3DS();
	// printf("ConfigInfo : \n - Country : %i\n - Region : %i\n - Lang : %i\n - Console Region : %i\n", config3DS.country, config3DS.region, config3DS.language, config3DS.consoleRegion);
	// exitMenu();

	// Code for rebooting to a real title
	//
	// exitServices();
	// nsInit();
	// NS_RebootToTitle(MEDIATYPE_SD, 0x0004000000171000);
	// nsExit();

	// Code for relaunching Homebrew App ? Not available

	if (isVirtualConsoleRGBY(id)) {
		printf("Virtual Pokemon RGB is selected !\n");
		printf("Game selected: %ls\n", getNameGame(id));
		printf("Loading save file game 0x%016llx...\n", id);
		if (!(openSaveArch(&saveArch, id))) {
			printf("Can't load save in game.\n");
			exitMenu();
		} else {
			FSUSER_OpenFile(&mainHandle, saveArch, fsMakePath(PATH_ASCII, "/sav.dat"), FS_OPEN_READ, 0);
			FSFILE_GetSize(mainHandle, &mainSize);
			printf("Size of the Pokemon save: %llu\n", mainSize);

			if (mainSize == SAVE_VC_RGBY_LENGTH) {
				printf("Correct size !\n");
			}

			printf("Reading save... !\n");
			mainbuf = malloc(mainSize);
			FSFILE_Read(mainHandle, NULL, 0, mainbuf, mainSize);
			printf("Save read !");
			save = Gen1_SaveOpenWithBytes(mainbuf);

			mainLoop(false, id);
			FSFILE_Close(mainHandle);
			free(mainbuf);
		}
		FSUSER_CloseArchive(saveArch);

	} else {

		printf("It seems you haven't selected a Virtual Pokemon game.\n");
		#ifdef USE_DEBUG_SAVE
		printf("Attempting to load debug save file...\n");
		mainbuf = readSaveRGBYFile("sdmc:/3ds/data/sav.dat");
		if (mainbuf == NULL) {
			printf("Can't load debug save file.\n");
			exitMenu();
		} else {
			printf("Loaded debug save file !\n");
			save = Gen1_SaveOpenWithBytes(mainbuf);
			mainLoop(false, 0);
			free(mainbuf);
		}
		#else
		exitMenu();
		#endif
	}
	exitServices();
	return 0;
}
