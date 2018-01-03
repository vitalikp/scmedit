/*
 * Copyright © 2016-2017 - Vitaliy Perevertun
 *
 * This file is part of scmedit
 *
 * This file is licensed under the MIT license.
 * See the file LICENSE.
 */

#include <stdlib.h>
#include <stdio.h>

#include "cli/cli_s.h"
#include "fav.h"


static int cli_show(cli_t *cli, int argc, char* argv[])
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

	ch = map_get(cli->map, num);

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

	printf("sid:\t\t0x%.4x\n", ch->sid);
	printf("onid:\t\t0x%.4x\n", ch->onid);
	printf("nid:\t\t0x%.4x\n", ch->nid);

	printf("symrate:\t%d ksymb/s\n", ch->symrate);

	return 0;
}

const cli_cmd clicmd_show =
{
	.name = "show",
	.cmd = cli_show,
	.help = "Show channel info"
};