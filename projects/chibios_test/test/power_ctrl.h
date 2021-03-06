
#ifndef __POWER_CTRL_H_
#define __POWER_CTRL_H_

#include "ch.h"
#include "shell.h"

void initPower( void );

void powerConfig( int onFirst, int oonRegular, int off );
void powerOffReset( void );
void cmd_pwr_cfg( BaseChannel *chp, int argc, char * argv [] );
void cmd_pwr_rst( BaseChannel *chp, int argc, char * argv [] );
// This command is only for debug purpose when CMU is powered separately.
void cmd_pwr_en( BaseChannel *chp, int argc, char * argv [] );

#endif





