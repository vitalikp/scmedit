/*
 * Copyright © 2015 - Vitaliy Perevertun
 *
 * This file is part of scmedit
 *
 * This file is licensed under the MIT license.
 * See the file LICENSE.
 */

#include <stdio.h>

#include "cli.h"


static encode_func encode = &ch_encode;

static int scm_write(const char* name, scm_map_t* map)
{
	size_t total = 0;

	int sz;

	FILE *out;

	out = fopen(name, "wb");
	if (!out)
	{
		printf("cannot open file '%s': %m\n", name);

		return -1;
	}

	sz = map_write(out, map, encode);
	if (sz < 0)
	{
		fclose(out);
		return -1;
	}
	total += sz;

	fclose(out);

	printf("written %d bytes in file '%s' \n", sz, name);

	return total;
}

static int cli_write(scm_map_t* map, int argc, char* argv[])
{
	if (argc < 2 || !argv[1])
	{
		fprintf(stderr, "filename expected!\n");
		return -1;
	}

	if (scm_write(argv[1], map) < 0)
	{
		fprintf(stderr, "error write file: %m\n");

		return -1;
	}

	return 0;
}

const cli_cmd clicmd_write =
{
	.name = "write",
	.cmd = cli_write,
	.help = "Write channel list to file"
};

