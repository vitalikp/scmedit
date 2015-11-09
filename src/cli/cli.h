/*
 * Copyright © 2015 - Vitaliy Perevertun
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


typedef struct cli_cmd
{
	const char* name;
	int (*cmd)(scm_map_t* map, int argc, char* argv[]);
	const char *help;
} cli_cmd;
extern const cli_cmd clicmd_print;
extern const cli_cmd clicmd_mv;
extern const cli_cmd clicmd_write;
extern const cli_cmd clicmd_help;
extern const cli_cmd* cli_cmds[];


typedef struct buf_t
{
	char data[256];
	uint8_t len;
} buf_t;


int cli_tab(buf_t* in);
void cli_run(scm_map_t* map);

#endif
