/*
 * Copyright © 2015 - Vitaliy Perevertun
 *
 * This file is part of scmedit
 *
 * This file is licensed under the MIT license.
 * See the file LICENSE.
 */

#ifndef _MAP_H_
#define _MAP_H_

#include <stdint.h>

#include "channel.h"


typedef struct scm_map_t
{
	channel_t*	chlist[1000];
} scm_map_t;


scm_map_t* map_new(void);
void map_free(scm_map_t** map);
#define _cleanup_map_ __attribute__((cleanup(map_free)))

int map_put(scm_map_t* map, uint16_t num, channel_t* ch);
channel_t* map_get(scm_map_t* map, uint16_t num);
channel_t* map_remove(scm_map_t* map, uint16_t num);
void map_print(scm_map_t* map);

int map_write(FILE *out, scm_map_t* map, encode_func encode);
int map_read(FILE *in, scm_map_t* map, decode_func decode);

#endif
