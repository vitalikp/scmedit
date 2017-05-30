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
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <termios.h>

#include "cli.h"


const cli_cmd* cli_cmds[] =
{
	&clicmd_print,
	&clicmd_show,
	&clicmd_mv,
	&clicmd_rm,
	&clicmd_write,
	&clicmd_help,
	NULL
};

static int cli_help(scm_map_t* map, int argc, char* argv[])
{
	printf("Commands:\n");

	uint8_t i = 0;

	while (cli_cmds[i])
	{
		printf("  ");
		printf("%s", cli_cmds[i]->name);
		printf("\t%*s", 5, " ");
		printf("%s\n", cli_cmds[i]->help);

		i++;
	}

	return 0;
}

const cli_cmd clicmd_help =
{
	.name = "help",
	.cmd = cli_help,
	.help = "Display a helpful usage message"
};

static struct termios oldt;

static void cli_restore(void)
{
	tcsetattr(0, TCSANOW, &oldt);
}

static void cli_init(void)
{
	struct termios newt;

	tcgetattr(0, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON|ECHO);
	tcsetattr(0, TCSANOW, &newt);
	atexit(cli_restore);
}

static int getchars(buf_t* buf)
{
	int ch;

	while ((ch = getc(stdin)) != EOF)
	{
		if (ch == '\t')
			return ch;

		if (ch == 0x7F)
		{
			if (buf->len > 0)
			{
				buf->len--;
				buf->data[buf->len] = '\0';
				printf("\b\033[K");
				fflush(stdout);
			}
			continue;
		}

		if (ch == '\e')
		{
			printf("\r\033[2K");
			fflush(stdout);

			return ch;
		}

		if (ch == 4)
		{
			if (buf->len > 0)
				continue;

			putchar('\n');
			break;
		}

		putchar(ch);
		if (ch == '\n')
			return 0;

		buf->data[buf->len++] = ch;
		buf->data[buf->len] = '\0';
	}

	return -1;
}

static void cli_parse(buf_t* buf, int* argc, char** argv)
{
	uint8_t i = 0;
	int cnt = 0;
	char* p = buf->data;
	argv[cnt++] = p;

	while (i < buf->len)
	{
		if (buf->data[i] == ' ')
		{
			buf->data[i++] = '\0';
			p = buf->data + i;
			argv[cnt++] = p;
		}

		i++;
	}

	*argc = cnt;
}

void cli_run(scm_map_t* map)
{
	cli_init();

	buf_t in;

	cli_cmd* pcmd;
	uint8_t i;
	int res;

	int argc = 0;
	char* argv[10];

	printf("Welcome to scmedit(version %s).\n", VERSION);
	printf("(press q for quit)\n");
	while (true)
	{
		in.len = 0;
		in.data[0] = '\0';
		printf("scmedit>");

		read: res = getchars(&in);
		if (res < 0)
			break;

		if (in.data[0] == 'q')
			break;

		if (!res && in.data[0] == '\0')
			continue;

		if (res == '\t')
		{
			if (cli_tab(&in) < 0)
				continue;

			goto read;
		}

		if (res == '\e')
			continue;

		cli_parse(&in, &argc, argv);

		i = 0;
		while((pcmd=(cli_cmd*)cli_cmds[i++]))
		{
			if (!strcmp(pcmd->name, argv[0]))
				break;
		}

		if (!pcmd)
		{
			printf("Unknown command '%s'\n", argv[0]);
			cli_help(NULL, argc, argv);
			continue;
		}

		pcmd->cmd(map, argc, argv);
	}

	printf("exiting...\n");
}
