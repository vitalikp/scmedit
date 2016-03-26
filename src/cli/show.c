/*
 * Copyright © 2016 - Vitaliy Perevertun
 *
 * This file is part of scmedit
 *
 * This file is licensed under the MIT license.
 * See the file LICENSE.
 */

#include <stdlib.h>
#include <stdio.h>

#include "cli.h"
#include "fav.h"


static int cli_show(scm_map_t* map, int argc, char* argv[])
{
	if (argc < 2)
	{
		fprintf(stderr, "show command require one argument!\n");

		return -1;
	}

	uint16_t num;

	num = strtol(argv[1], NULL, 10);

	if (num < 1 || num > 1000)
	{
		fprintf(stderr, "channel number must be from 1 to 1000\n");

		return -1;
	}

	channel_t* ch;

	ch = map_get(map, num);

	if (!ch)
	{
		fprintf(stderr, "no channel at [%d] position\n", num);

		return -1;
	}

	static char fav[FAV_LEN];
	fav_fill(ch->fav, fav);

	printf("position:\t[%3d]\n", num);
	printf("name:\t\t'%s'\n", ch->name);

	printf("favorite:\t[%s]\n", fav);

	printf("vpid:\t\t%d\n", ch->vpid);
	printf("pcr_pid:\t%d\n", ch->pcr_pid);

	return 0;
}

const cli_cmd clicmd_show =
{
	.name = "show",
	.cmd = cli_show,
	.help = "Show channel info"
};