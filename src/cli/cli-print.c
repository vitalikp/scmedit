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


static int cli_print(scm_map_t* map, int argc, char* argv[])
{
	map_print(map);

	return 0;
}

const cli_cmd clicmd_print =
{
	.name = "print",
	.cmd = cli_print,
	.help = "Print channel list"
};

