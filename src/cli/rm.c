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


static int cli_rm(scm_map_t* map, int argc, char* argv[])
{
	if (argc < 2)
	{
		fprintf(stderr, "rm command require one argument!\n");

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

	ch = map_remove(map, num);

	if (!ch)
	{
		fprintf(stderr, "no channel at [%d] position\n", num);

		return -1;
	}

	printf("channel '%s' removed from [%d] position\n", ch->name, num);

	return 0;
}

const cli_cmd clicmd_rm =
{
	.name = "rm",
	.cmd = cli_rm,
	.help = "Remove a channel"
};