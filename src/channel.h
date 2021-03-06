/*
 * Copyright © 2015-2017 - Vitaliy Perevertun
 *
 * This file is part of scmedit
 *
 * This file is licensed under the MIT license.
 * See the file LICENSE.
 */

#ifndef _CHANNEL_H_
#define _CHANNEL_H_

#include <stdint.h>

#include "fav.h"


typedef struct channel_t
{
	uint16_t	vpid;		/* video pid */
	uint16_t	pcr_pid;	/* program clock reference pid */
	uint8_t		raw1[3];	/* raw data */
	uint8_t		srv_type;	/* service type: 0x01 - TV, 0x02 - Radio, 0x0C - data, 0x19 - HD */
	uint16_t	sid;		/* service id */
	uint16_t	onid;		/* original network id */
	uint16_t	nid;		/* network id */
	uint8_t		raw2[12];	/* raw data */
	uint16_t	symrate;	/* symbol rate */
	uint8_t		raw3[2*17];	/* raw data */
	char*		name;		/* name */
	uint8_t		raw4[28];	/* raw data */
	fav_t		fav;		/* favorite bitmask 1-5 */
	uint8_t		raw5[7];	/* raw data */
	uint8_t		cksum;		/* *checksum */
} channel_t;

typedef int(*encode_func)(channel_t* map, uint8_t* out);
typedef int(*decode_func)(channel_t* ch, uint8_t* in);


#undef le16toh
static inline uint16_t le16toh(uint8_t* in)
{
	return in[0]|(in[1]<<8);
}

#undef le32toh
static inline uint32_t le32toh(uint8_t* in)
{
	return in[0]|(in[1]<<8)|(in[2]<<16)|(in[3]<<24);
}

#undef htole16
static inline void htole16(uint16_t num, uint8_t* out)
{
	out[0] = num & 0xFF;
	out[1] = (num>>8) & 0xFF;
}

#undef htole32
static inline void htole32(uint32_t num, uint8_t* out)
{
	out[0] = num & 0xFF;
	out[1] = (num>>8) & 0xFF;
	out[2] = (num>>16) & 0xFF;
	out[3] = (num>>24) & 0xFF;
}


channel_t* ch_new(void);
void ch_free(channel_t** ch);
#define _cleanup_channel_ __attribute__((cleanup(ch_free)))

int ch_encode(channel_t* ch, uint8_t* out);
int ch_decode(channel_t* ch, uint8_t* in);

#endif
