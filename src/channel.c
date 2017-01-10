/*
 * Copyright © 2015-2017 - Vitaliy Perevertun
 *
 * This file is part of scmedit
 *
 * This file is licensed under the MIT license.
 * See the file LICENSE.
 */

#include <stdlib.h>
#include <string.h>

#include "channel.h"
#include "fav.h"

#include "utf8.h"

channel_t* ch_new(void)
{
	channel_t* ch = calloc(1, sizeof(channel_t));
	if (!ch)
		return NULL;

	return ch;
}

void ch_free(channel_t** ch)
{
	if (!ch || !*ch)
		return;

	channel_t* p = *ch;

	if (p->name)
		free(p->name);

	free(p);
	*ch = NULL;
}

static int read_name(uint8_t* in, char** name, size_t len)
{
	size_t total = 0;

	uint8_t buf[len];
	uint8_t* p = &buf[0];

	uint16_t val;

	unsigned i = 0;
	unsigned j;

	while (i < len)
	{
		val = 0;
		val |= (in[i++] & 0xFF)<<8;
		val |= (in[i++]) & 0xFF;

		j = utf8_enc(val, p);
		total += j;

		p += j;
	}

	*name = malloc(total);
	if (!*name)
		return -1;

	memcpy(*name, buf, total);

	return len;
}

static int write_name(const char* name, uint8_t* out, size_t len)
{
	uint8_t* p = (uint8_t*)name;

	uint32_t val;

	uint8_t i = 0;

	while (i < len)
	{
		val = 0;
		p += utf8_dec(p, &val);
		out[i++] = (val>>8) & 0xFF;
		out[i++] = val & 0xFF;
	}

	return len;
}

int ch_encode(channel_t* ch, uint8_t* out)
{
	uint8_t* p = out;

	htole16(ch->vpid, p);
	p += 2;

	htole16(ch->pcr_pid, p);
	p += 2;

	memcpy(p, ch->raw1, 3);
	p += 3;

	p[0] = ch->srv_type;
	p++;

	htole16(ch->sid, p);
	p += 2;

	htole16(ch->onid, p);
	p += 2;

	htole16(ch->nid, p);
	p += 2;

	memcpy(p, ch->raw2, 12);
	p += 12;

	htole16(ch->symrate, p);
	p += 2;

	memcpy(p, ch->raw3, 3*16);
	p += 2*17;

	size_t sz = write_name(ch->name, p, 200);
	if (sz < 0)
		return -1;
	p += sz;

	memcpy(p, ch->raw4, 28);
	p += 28;

	p += fav_encode(ch->fav, p);

	memcpy(p, ch->raw5, 7);
	p += 7;

	p[0] = ch->cksum;
	p++;

	return p - out;
}

int ch_decode(channel_t* ch, uint8_t* in)
{
	uint8_t* p = in;

	ch->vpid = le16toh(p);
	p += 2;

	ch->pcr_pid = p[0] | p[1]<<8;
	p += 2;

	memcpy(ch->raw1, p, 3);
	p += 3;

	ch->srv_type = (uint8_t)p[0];
	p++;

	ch->sid = p[0] | p[1]<<8;
	p += 2;

	ch->onid = p[0] | p[1]<<8;
	p += 2;

	ch->nid = p[0] | p[1]<<8;
	p += 2;

	memcpy(ch->raw2, p, 3*16);
	p += 12;

	ch->symrate = p[0] | p[1]<<8;
	p += 2;

	memcpy(ch->raw3, p, 3*16);
	p += 2*17;

	size_t sz = read_name(p, &ch->name, 200);
	if (sz < 0)
		return -1;
	p += sz;

	memcpy(ch->raw4, p, 28);
	p += 28;

	p += fav_decode(ch->fav, p);

	memcpy(ch->raw5, p, 7);
	p += 7;

	ch->cksum = (uint8_t)p[0];
	p++;

	return p - in;
}
