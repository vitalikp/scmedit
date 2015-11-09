/*
 * Copyright © 2015 - Vitaliy Perevertun
 *
 * This file is part of scmedit
 *
 * This file is licensed under the MIT license.
 * See the file LICENSE.
 */

#ifndef _UTF8_H_
#define _UTF8_H_

#include <stdint.h>

int utf8_enc(uint32_t code, uint8_t* out);
int utf8_dec(uint8_t* in, uint32_t *code);

#endif
