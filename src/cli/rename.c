/*
 * Copyright © 2018 - Vitaliy Perevertun
 *
 * This file is part of scmedit
 *
 * This file is licensed under the MIT license.
 * See the file LICENSE.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "cli/cli_s.h"


static int cli_rename(cli_t *cli, int argc, char* argv[])
{
	char name[255] = {};
	uint16_t num;

	if (argc > 3)
	{
		fprintf(stderr, "rename command require two arguments!\n");
		return -1;
	}

	if (argc < 2)
	{
		if (cli_read(cli, "enter channel number", "%d", &num) != 1)
			return -1;
	}
	else
		num = strtol(argv[1], NULL, 10);

	if (num < 1 || num > 1000)
	{
		fprintf(stderr, "channel number must be from 1 to 1000\n");
		return -1;
	}

	if (argc < 3)
	{
		if (cli_read(cli, "enter channel name", "%s", &name) != 1)
			return -1;
	}
	else
		strcpy(name, argv[2]);

	if (!name || !name[0])
	{
		fprintf(stderr, "channel name must be not empty\n");
		return -1;
	}

	channel_t* ch;

	ch = map_get(cli->map, num);

	if (ch)
	{
		char *oldname;
		size_t len;

		oldname = ch->name;
		len = strlen(name);
		ch->name = malloc(len + 1);
		if (!ch->name)
		{
			ch->name = oldname;

			fprintf(stderr, "out of memory\n");

			return -1;
		}

		memcpy(ch->name, name, len);
		ch->name[len] = '\0';

		printf("channel '%s' renamed to '%s' in [%d] position\n", oldname, ch->name, num);

		free(oldname);
	}

	return 0;
}

const cli_cmd clicmd_rename =
{
	.name = "rename",
	.cmd = cli_rename,
	.help = "Rename the channel"
};

