/*
 * Copyright © 2018 - Vitaliy Perevertun
 *
 * This file is part of scmedit
 *
 * This file is licensed under the MIT license.
 * See the file LICENSE.
 */

#ifndef _CLI_S_H_
#define _CLI_S_H_

#include "cli.h"
#include "buf/buf.h"
#include "buf/buf_s.h"


struct cli
{
	buf_t in;
	scm_map_t* map;
};

#endif /* _CLI_S_H_ */