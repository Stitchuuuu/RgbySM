/*  This file is part of PKSM
>	Copyright (C) 2016/2017 Bernardo Giordano
>
>   Multi-Language support added by Naxann
>
>   This program is free software: you can redistribute it and/or modify
>   it under the terms of the GNU General Public License as published by
>   the Free Software Foundation, either version 3 of the License, or
>   (at your option) any later version.
>
>   This program is distributed in the hope that it will be useful,
>   but WITHOUT ANY WARRANTY; without even the implied warranty of
>   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
>   GNU General Public License for more details.
>
>   You should have received a copy of the GNU General Public License
>   along with this program.  If not, see <http://www.gnu.org/licenses/>.
>   See LICENSE for information.
*/

#pragma once

#include <3ds.h>
#include <fcntl.h>
#include <malloc.h>
#include <wchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <stdarg.h>

// debuglogf displays message on the app if defined to 1
#define DEBUG_I18N 1
// Force the lang to display in PKSM. Useful when debugging another lang than the 3DS lang
// Langs ID are defined in ctrulib : https://smealum.github.io/ctrulib/cfgu_8h.html
// CFG_LANGUAGE_JP = 0
// CFG_LANGUAGE_EN = 1
// CFG_LANGUAGE_FR = 2
// CFG_LANGUAGE_DE = 3
// CFG_LANGUAGE_IT = 4
// CFG_LANGUAGE_ES = 5
// CFG_LANGUAGE_ZH = 6
// CFG_LANGUAGE_KO = 7
// CFG_LANGUAGE_NL = 8
// CFG_LANGUAGE_PT = 9
// CFG_LANGUAGE_RU = 10
// CFG_LANGUAGE_TW = 11
#define DEBUG_I18N_LANG 6


#define MAX_LANGUAGE 12

/**
 * Localization files
 */
struct i18n_files {
	char *abilities;
	char *species;
	char *natures;
	char *moves;
	char *items;
	char *hp;
	char *forms;
	char *balls;
	char *types;
	char *app;
};

/**
 * Array of strings in UTF32
 */
typedef struct ArrayUTF32 {
	int length;
	wchar_t** items;
	wchar_t** sortedItems;
	int* sortedItemsID;
	bool sorted;
} ArrayUTF32;


/**
 * Labels of localization strings used in the application
 */
typedef enum {
    S_DOWNLOADING_ASSETS = 0,
} AppTextCode;


struct ArrayUTF32 i18n_FileToArrayUTF32(char* filepath);
void i18n_free_ArrayUTF32(ArrayUTF32 *arr);

struct i18n_files i18n_getFilesPath();

// struct ArrayUTF32 ArrayUTF32_copy(struct ArrayUTF32 from);
void ArrayUTF32_sort_starting_index(ArrayUTF32 *arr, int index);
void ArrayUTF32_sort_starting_index_with_sort_func(struct ArrayUTF32 *arr, int index, int (*f)(const wchar_t *a,const wchar_t *b));
void ArrayUTF32_sort(ArrayUTF32 *arr);
void debuglogf(const char* format, ...);
void getCharactersUsedIn(ArrayUTF32 *file, wchar_t *chars);
void getCharactersUsedInApp(wchar_t *chars);

wchar_t* i18n(AppTextCode code);
void i18n_init();
void i18n_initTextSwkbd(SwkbdState* swkbd, AppTextCode leftButtonTextCode, AppTextCode rightButtonTextCode, AppTextCode hintTextCode);
void i18n_exit();

