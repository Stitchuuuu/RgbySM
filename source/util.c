
#include "util.h"

void getCountryCodeAndRegion(u8* country, u8* region) {
	u8* data = malloc(sizeof(u8)*4);
	CFGU_GetConfigInfoBlk2(0x4, 0x000B0000, data);
	*country = data[2];
	*region = data[3];
}


Config3DS getConfig3DS() {
	Config3DS config;
	CFGU_GetSystemLanguage(&config.language);
	CFGU_SecureInfoGetRegion(&config.consoleRegion);
	getCountryCodeAndRegion(&config.country, &config.region);
	return config;
}

