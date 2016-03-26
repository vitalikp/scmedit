/*
 * Copyright © 2016 - Vitaliy Perevertun
 *
 * This file is part of scmedit
 *
 * This file is licensed under the MIT license.
 * See the file LICENSE.
 */

#ifndef _FAV_H_
#define _FAV_H_

#include <stdint.h>


#define FAV_LEN 5

typedef uint32_t fav_t[FAV_LEN];


int fav_encode(fav_t fav, uint8_t* out);
int fav_decode(fav_t fav, uint8_t* in);

void fav_fill(fav_t fav, char* out);

#endif
