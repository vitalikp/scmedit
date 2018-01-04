/*
 * Copyright © 2018 - Vitaliy Perevertun
 *
 * This file is part of scmedit
 *
 * This file is licensed under the MIT license.
 * See the file LICENSE.
 */

#include <stdio.h>
#include <stdarg.h>

#include "cli/cli_s.h"
#include "buf/buf.h"


int cli_read(cli_t* cli, const char *title, const char *fmt, ...)
{
	va_list ap;
	int res;

	if (!title)
		return -1;

	fprintf(stderr, "%s: ", title);

	cli->in.len = 0;
	cli->in.data[0] = '\0';

	if (buf_read(&cli->in) < 0)
		return -1;

	va_start(ap, fmt);
	res = vsscanf(cli->in.data, fmt, ap);
	va_end(ap);

	return res;
}