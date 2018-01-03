/*
 * Copyright © 2015 - Vitaliy Perevertun
 *
 * This file is part of scmedit
 *
 * This file is licensed under the MIT license.
 * See the file LICENSE.
 */

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "cli.h"
#include "cli/buf_s.h"


static void cli_tab_cmds(const char* in)
{
	uint8_t i = 0;
	uint8_t cnt = 0;

	while (cli_cmds[i])
	{
		if (!in[0] || !strcmp(in, cli_cmds[i]->name))
		{
			if (i > 0)
				putchar(' ');
			else
				putchar('\n');
			printf("%s", cli_cmds[i]->name);
			cnt++;
		}
		i++;
	}
	if (cnt > 0)
		putchar('\n');
}

int cli_tab(buf_t* in)
{
	if (in->data[0])
	{
		uint8_t i = 0;
		uint8_t j = 0;
		uint8_t cnt = 0;
		size_t len = 0;
		char* p = NULL;

		while (cli_cmds[i])
		{
			if (!strncmp(in->data, cli_cmds[i]->name, in->len))
			{
				p = (char*)cli_cmds[i]->name + in->len;
				j = i;
				cnt++;
			}
			i++;
		}
		if (cnt == 1)
		{
			len = strlen(cli_cmds[j]->name)-in->len;
			printf("%s", p);
			memcpy(in->data + in->len, p, len);
			in->len += len;
			in->data[in->len] = '\0';
		}

		return 0;
	}

	cli_tab_cmds(in->data);

	return -1;
}
