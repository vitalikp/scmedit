/*
 * Copyright © 2018 - Vitaliy Perevertun
 *
 * This file is part of scmedit
 *
 * This file is licensed under the MIT license.
 * See the file LICENSE.
 */

#ifndef _BUF_S_H_
#define _BUF_S_H_

#include <stdint.h>


struct buf
{
	char data[256];
	uint8_t len;
};

#endif /* _BUF_S_H_ */