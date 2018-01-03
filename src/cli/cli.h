/*
 * Copyright © 2015-2018 - Vitaliy Perevertun
 *
 * This file is part of scmedit
 *
 * This file is licensed under the MIT license.
 * See the file LICENSE.
 */

#ifndef _CLI_H_
#define _CLI_H_

#include <stdint.h>

#include "map.h"


typedef struct cli cli_t;

typedef struct cli_cmd
{
	const char* name;
	int (*cmd)(cli_t *cli, int argc, char* argv[]);
	const char *help;
} cli_cmd;
extern const cli_cmd clicmd_print;
extern const cli_cmd clicmd_show;
extern const cli_cmd clicmd_mv;
extern const cli_cmd clicmd_rm;
extern const cli_cmd clicmd_write;
extern const cli_cmd clicmd_help;
extern const cli_cmd* cli_cmds[];


int cli_tab(cli_t* cli);
void cli_run(scm_map_t* map);

#endif
