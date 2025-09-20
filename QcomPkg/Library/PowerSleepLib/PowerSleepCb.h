#ifndef POWERSLEEPCB_H
#define POWERSLEEPCB_H

/******************************************************************//**
 * @file PowerSleepCb.h
 *
 * @brief PowerSleep callback support header
 *
 * Copyright (c) 2017-2018 by Qualcomm Technologies, Inc.
 * All Rights Reserved.
 *
 *********************************************************************/


#include "PowerSleepLib.h"

void psleep_invoke_cbs(psleep_cb_mode_enum mode);

void PsleepCbInit (void);


#endif //POWERSLEEPCB_H
