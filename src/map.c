/*
 * Copyright © 2015 - Vitaliy Perevertun
 *
 * This file is part of scmedit
 *
 * This file is licensed under the MIT license.
 * See the file LICENSE.
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "map.h"


static const char* service_types[] =
{
	"unknown",
	"TV",
	"Radio",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"Data",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"HD",
};


scm_map_t* map_new(void)
{
	scm_map_t* map = calloc(1, sizeof(scm_map_t));
	if (!map)
		return NULL;

	return map;
}

void map_free(scm_map_t** map)
{
	if (!map || !*map)
		return;

	unsigned i = 0;
	channel_t** p = (*map)->chlist;

	while (i < 1000)
		ch_free(&p[i++]);

	free(*map);
	*map = NULL;
}

int map_put(scm_map_t* map, uint16_t num, channel_t* ch)
{
	if (!map || !ch || num > 1000)
		return -1;

	if (map->chlist[num-1])
		return -1;

	map->chlist[num-1] = ch;

	return 0;
}

channel_t* map_get(scm_map_t* map, uint16_t num)
{
	if (!map || num > 1000)
		return NULL;

	return map->chlist[num-1];
}

channel_t* map_remove(scm_map_t* map, uint16_t num)
{
	if (!map || num > 1000)
		return NULL;

	channel_t* ch = map->chlist[num-1];

	map->chlist[num-1] = NULL;

	return ch;
}

void map_print(scm_map_t* map)
{
	channel_t* ch;

	uint16_t num = 1;
	while (num <= 1000)
	{
		ch = map_get(map, num);
		if (ch)
		{
			printf("[%3d] %d %d\t{0x%.2x - %s} '%s'\n", num, ch->vpid, ch->pcr_pid, ch->srv_type, service_types[ch->srv_type], ch->name);
//			printf("favorite: 0x%.4x 0x%.4x 0x%.4x 0x%.4x 0x%.4x\n", ch->fav1, ch->fav2, ch->fav3, ch->fav4, ch->fav5);
//			printf("checksum: 0x%.2x\n", ch->cksum);
//			printf("\n");
		}

		num++;
	}
}

static uint8_t crc(uint8_t* in, uint32_t len)
{
	uint8_t crc = 0;

	while (len--)
	{
		crc += *in;
		in++;
	}

	return crc;
}

int map_write(FILE *out, scm_map_t* map, encode_func encode)
{
	int sz;
	size_t total = 0;

	uint8_t buf[320];

	uint16_t num = 1;

	channel_t* ch;

	while (num <= 1000)
	{
		ch = map_get(map, num);

		if (!ch)
			memset(buf, 0, 320);
		else
		{
			sz = encode(ch, buf+2);
			if (sz < 0)
			{
				printf("error encode channel: %d\n", num);
				return -1;
			}
			htole16(num, buf);
			sz += 2;

			buf[319] = crc(buf, 319);

		}

		sz = fwrite(buf, 1, sz, out);
		if (sz < 0)
			return -1;
		total += sz;
//		printf("[%d] write %d bytes\n", num, sz);
		num++;
	}

	return total;
}

int map_read(FILE *in, scm_map_t* map, decode_func decode)
{
	size_t total = 0;

	int sz;

	_cleanup_channel_ channel_t* ch;

	uint8_t cksum;

	uint8_t buf[320];
	uint16_t num;

	while ((sz = fread(buf, 1, 320, in)) > 1)
	{
		total += sz;

		cksum = crc(buf, 319);
		if (!cksum)
			continue;

		num = le16toh(buf);

		if (buf[319] != cksum)
		{
			fprintf(stderr, "[%d] checksum incorrect: 0x%.2x 0x%.2x\n", num, cksum, buf[319]);

			continue;
		}

		ch = ch_new();
		if (!ch)
		{
			printf("error create channel: %d\n", num);
			return -1;
		}

		sz = decode(ch, buf+2);
		if (sz < 0)
		{
			printf("error decode channel: %d\n", num);
			return -1;
		}

		if (map_put(map, num, ch) < 0)
		{
			printf("error put channel in map: %d '%s'\n", num, ch->name);
			continue;
		}

		ch = NULL;
	}

	return total;
}
