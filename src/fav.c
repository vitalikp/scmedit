/*
 * Copyright © 2016 - Vitaliy Perevertun
 *
 * This file is part of scmedit
 *
 * This file is licensed under the MIT license.
 * See the file LICENSE.
 */

#include <stdlib.h>
#include <stdio.h>

#include "fav.h"
#include "channel.h"


int fav_encode(fav_t fav, uint8_t* out)
{
	uint8_t* p = out;

	uint8_t i = 0;
	while (i < FAV_LEN)
	{
		htole32(fav[i++], p);
		p += 4;
	}

	return p - out;
}

int fav_decode(fav_t fav, uint8_t* in)
{
	uint8_t* p = in;

	uint8_t i = 0;
	while (i < FAV_LEN)
	{
		fav[i++] = le32toh(p);
		p += 4;
	}

	return p - in;
}

void fav_fill(fav_t fav, char* out)
{
	static char fill[] = {' ', '*'};

	uint8_t i = 0;
	while (i < FAV_LEN)
	{
		out[i] = fill[fav[i] == 1];
		i++;
	}
}