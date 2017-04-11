
#include <3ds.h>
#include <malloc.h>

typedef struct Config3DS {
	u8 region;
	u8 country;
	u8 consoleRegion;
	u8 language;
} Config3DS;


Config3DS getConfig3DS();
