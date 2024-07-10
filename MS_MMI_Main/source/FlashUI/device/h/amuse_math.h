/*
*  Copyright (C) 2011 Microrapid, Inc. All Rights Reserved.
*  Authors: Microrapid AMUSE GROUP
*  This file is part of Amuse(TM).
*  Date:
*  $Id$
*/
#ifndef AMUSE_MATH_H
#define AMUSE_MATH_H
#if 1//ENABLE_FOR_RITU
typedef double as2_double;
#else
#include "amuse_inttypes.h"
#endif

as2_double amuse_fmod(as2_double x, as2_double y);

#endif