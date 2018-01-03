/*
 * Copyright © 2018 - Vitaliy Perevertun
 *
 * This file is part of scmedit
 *
 * This file is licensed under the MIT license.
 * See the file LICENSE.
 */

#include <stdio.h>

#include "cli/buf.h"
#include "cli/buf/buf_s.h"


int buf_read(buf_t *buf)
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