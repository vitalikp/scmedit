/*
 * Copyright © 2015 - Vitaliy Perevertun
 *
 * This file is part of scmedit
 *
 * This file is licensed under the MIT license.
 * See the file LICENSE.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <locale.h>
#include <errno.h>
#include <getopt.h>

#include "map.h"
#include "utf8.h"
#include "cli/cli.h"


static char* fn = NULL;
static decode_func decode = &ch_decode;

enum
{
	ARG_VERSION = 0x100,
	ARG_FILE
};

static const struct option options[] =
{
	{ "version" ,       no_argument,       NULL, ARG_VERSION        },
	{ "file",           required_argument, NULL, ARG_FILE           },
	{}
};

static const char* help_opt[] =
{
	"show scmedit version and exit",
	"input scm file"
};

void arg_help()
{
	printf("Options:\n");
	unsigned i = 0;
	while (options[i].name)
	{
		printf("  ");
		if (options[i].val < 0x100)
			printf("-%c, ", options[i].val);
		printf("--%s", options[i].name);
		printf("\t%*s", 5, " ");
		printf("%s\n", help_opt[i]);
		i++;
	}
}

static int arg_parse(int argc, char *argv[])
{
	int c;

	while ((c = getopt_long(argc, argv, "o:", options, NULL)) >= 0)
	{
		switch (c)
		{
			case ARG_VERSION:
				printf("scmedit: %s\n", VERSION);
				return 0;

			case ARG_FILE:
				fn = optarg;
				break;

			case '?':
				return -EINVAL;

			default:
				fprintf(stderr, "Unhandled option\n");
				abort();
		}
	}

	if (!fn)
	{
		fprintf(stderr, "no input file is specified\n");

		return -EINVAL;
	}

	return 0;
}

int scm_read(scm_map_t* map)
{
	size_t total = 0;

	int sz;

	FILE* in;

	in = fopen(fn, "rb");
	if (!in)
	{
		printf("cannot open file '%s': %m\n", fn);

		return -1;
	}

	sz = map_read(in, map, decode);
	if (sz < 0)
	{
		fclose(in);
		return -1;
	}
	total += sz;

	fclose(in);

	printf("total read %d bytes\n", sz);

	return total;
}

int main(int argc, char* argv[])
{
	/* setup locale */
	setlocale(LC_MESSAGES, "");
	setlocale(LC_CTYPE, "");

	if (arg_parse(argc, argv) < 0)
	{
		fprintf(stderr, "\nUsage: scmedit [options]\n");

		arg_help();

		return EXIT_FAILURE;
	}

	_cleanup_map_ scm_map_t* map = NULL;

	map = map_new();
	if (!map)
	{
		return EXIT_FAILURE;
	}

	if (scm_read(map) < 0)
	{
		fprintf(stderr, "error read file: %m\n");

		return EXIT_FAILURE;
	}

	cli_run(map);

	return EXIT_SUCCESS;
}
