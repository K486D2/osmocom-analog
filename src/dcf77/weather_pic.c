#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "weather_pic.h"

static const char sonne[] =
	"13"
	"             \\__/  "
	"           __/  \\__"
	"             \\__/  "
	"             /  \\  "
	"                   "
	"                   "
	"                   "
	;

static const char mond[] =
	"17"
	"  x   +  .   +  |\\ "
	"+    . x    x   ) )"
	"   .      .     |/ "
	"                   "
	"                   "
	"                   "
	"                   "
	;

static const char wolke[] =
	"07"
	"                   "
	"     ________      "
	"    /FFFFFFFF\\     "
	"    \\________/     "
	"                   "
	"                   "
	"                   "
	;

static const char wolken[] =
	"07"
	"                   "
	"    _________      "
	"   /FFFF_____\\__   "
	"   \\___/FFFFFFFF\\  "
	"       \\________/  "
	"                   "
	"                   "
	;

static const char nebel[] =
	"07"
	"                   "
	"                   "
	"                   "
	"                   "
	"    __ __ __   __  "
	"    ___ __ __ __ _ "
	"                   "
	;

static const char regen[] =
	"14"
	"                   "
	"                   "
	"                   "
	"                   "
	"    /              "
	"   /  /  /  /  /   "
	"     /  /  /  /    "
	;

static const char niesel[] =
	"14"
	"                   "
	"                   "
	"                   "
	"                   "
	"      '            "
	"    ,   '  '  '    "
	"      ,  ,  ,      "
	;

static const char schnee[] =
	"17"
	"                   "
	"                   "
	"                   "
	"                   "
	"      *            "
	"    *   *  *  *    "
	"      *  *  *      "
	;

static const char graupel[] =
	"07"
	"                   "
	"                   "
	"                   "
	"                   "
	"    / *            "
	"   * / * / * / *   "
	"      / * / * /    "
	;

static const char blitz[] =
	"13"
	"                   "
	"                   "
	"|                  "
	"|                  "
	"|/|                "
	"  |                "
	"  V                "
	;

static const char *weather_pic_day[] = {
	NULL,
	sonne, NULL,
	sonne, wolke, NULL,
	sonne, wolken, NULL,
	wolken, NULL,
	sonne, wolken, blitz, NULL,
	wolken, regen, niesel, NULL,
	wolken, schnee, NULL,
	sonne, nebel, NULL,
	wolken, regen, schnee, NULL,
	sonne, wolken, regen, NULL,
	wolken, niesel, NULL,
	sonne, wolken, schnee, NULL,
	wolken, regen, niesel, blitz, NULL,
	nebel, NULL,
	sonne, wolken, graupel, NULL,
};

static const char *weather_pic_night[] = {
	NULL,
	mond, NULL,
	mond, wolke, NULL,
	mond, wolken, NULL,
	wolken, NULL,
	mond, wolken, blitz, NULL,
	wolken, regen, niesel, NULL,
	wolken, schnee, NULL,
	mond, nebel, NULL,
	wolken, regen, schnee, NULL,
	mond, wolken, regen, NULL,
	wolken, niesel, NULL,
	mond, wolken, schnee, NULL,
	wolken, regen, niesel, blitz, NULL,
	nebel, NULL,
	mond, wolken, graupel, NULL,
};

#define BUF_WIDTH	(19 + 10 + 19)

void print_weather_pic(int weather_day, int weather_night)
{
	char pic_character[7*BUF_WIDTH], pic_color[7*BUF_WIDTH];
	const char **pic_set;
	uint8_t color;
	unsigned int l, c, xoffset;
	int weather;

	printf("\n");

	if (weather_day == 0 && weather_night == 0)
		return;

	/* Init buffers */
	memset(pic_character, ' ', sizeof(pic_character));
	memset(pic_color, 0xff, sizeof(pic_color));

	for (pic_set = weather_pic_day, weather = weather_day, xoffset = 0; xoffset <= 29; pic_set = weather_pic_night, weather = weather_night, xoffset += 29) {
		/* Fast forward to given weather in picture set. */
		while (weather) {
			if (*(pic_set++))
				continue;
			weather--;
		}

		/* Render each image into image buffer. */
		while (*pic_set) {
			color = (((*pic_set)[0] - '0') << 4) | ((*pic_set)[1] - '0');
			for (l = 0; l < 7; l++) {
				for (c = 0; c < 19; c++) {
					if ((*pic_set)[2 + l * 19 + c] != ' ') {
						pic_character[l * BUF_WIDTH + xoffset + c] = (*pic_set)[2 + l * 19 + c];
						if (pic_character[l * BUF_WIDTH + xoffset + c] == 'F')
							pic_character[l * BUF_WIDTH + xoffset + c] = ' ';
						pic_color[l * BUF_WIDTH + xoffset + c] = color;
					}
				}
			}
			pic_set++;
		}
	}

	color = 0xff;
	for (c = 0; c < sizeof(pic_character); c++) {
		if (pic_character[c] != ' ' &&  pic_color[c] != color) {
			color = pic_color[c];
			printf("\033[%d;3%dm", color >> 4, color & 0xf);
		}
		putchar(pic_character[c]);
		if (c % BUF_WIDTH == BUF_WIDTH - 1)
			printf("\n");
	}

	printf("\033[0;39m\n");
}

