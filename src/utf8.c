/*
 * Copyright © 2015 - Vitaliy Perevertun
 *
 * This file is part of scmedit
 *
 * This file is licensed under the MIT license.
 * See the file LICENSE.
 */

#include "utf8.h"


int utf8_enc(uint32_t code, uint8_t* out)
{
	unsigned i = 0;

	if (code < 0x80)
	{
		out[i++] = code;

		return i;
	}

	if (code < 0x800)
	{
		out[i++] = 0xC0 | (code>>6);
		out[i++] = 0x80 | (code & 0x3F);

		return i;
	}

	if (code < 0x10000)
	{
		out[i++] = 0xE0 | (code>>12);
		out[i++] = 0x80 | (code>>6);
		out[i++] = 0x80 | (code & 0x3F);

		return i;
	}

	return 0;
}

int utf8_dec(uint8_t* in, uint32_t *code)
{
	int i = 0;

	if (in[i] < 0x80)
	{
		*code = in[i++];

		return i;
	}

	if (in[i] >> 5 == 0x06 && in[i+1] >> 6 == 2)
	{
		*code = (in[i++] & 0x1F) << 6;
		*code |= in[i++] & 0x3F;

		return i;
	}

	if (in[i] >> 5 == 0x07 && in[i+1] >> 6 == 2 && in[i+2] >> 6 == 2)
	{
		*code = (in[i++] & 0x1F) << 12;
		*code |= (in[i++] & 0x3F) << 6;
		*code |= in[i++] & 0x3F;

		return i;
	}

	return 0;
}
