/*
 * Copyright © 2015 - Vitaliy Perevertun
 *
 * This file is part of scmedit
 *
 * This file is licensed under the MIT license.
 * See the file LICENSE.
 */

#include <stdlib.h>
#include <stdio.h>

#include "cli/cli_s.h"



static int cli_mv(cli_t *cli, int argc, char* argv[])
{
	if (argc < 3)
	{
		fprintf(stderr, "mv command require two arguments!\n");

		return -1;
	}

	uint16_t num1;
	uint16_t num2;

	num1 = strtol(argv[1], NULL, 10);
	num2 = strtol(argv[2], NULL, 10);

	if (num1 < 1 || num1 > 1000 || num2 < 1 || num2 > 1000)
	{
		fprintf(stderr, "channel number must be from 1 to 1000\n");

		return -1;
	}

	if (num1 == num2)
	{
		fprintf(stderr, "channel numbers must be different\n");

		return -1;
	}

	channel_t* ch1;
	channel_t* ch2;

	ch1 = map_remove(cli->map, num1);
	ch2 = map_remove(cli->map, num2);

	if (ch1)
	{
		map_put(cli->map, num2, ch1);
		printf("channel '%s' moved from [%d] to [%d] position\n", ch1->name, num1, num2);
	}

	if (ch2)
	{
		map_put(cli->map, num1, ch2);
		printf("channel '%s' moved from [%d] to [%d] position\n", ch2->name, num2, num1);
	}

	return 0;
}

const cli_cmd clicmd_mv =
{
	.name = "mv",
	.cmd = cli_mv,
	.help = "Move the channel to a new position"
};

