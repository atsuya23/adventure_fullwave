/********************************************************************
 Copyright (C) 2000-2019 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
*********************************************************************/

#ifndef  __OPTIONS_H__
#define  __OPTIONS_H__

#include "advfullwave_lib.h"


void HelpOptions( int argc, char **argv, char *mname, char *version,
                  char *usage ) ;
void SetOptions( int argc, char **argv, OPTIONS *options,
                 char *mname, char *version,
                 int work, int field, int analysis,
                 int n_argument, char *usage ) ;
void DataSW_init( DataSW* datasw);
void SolvOpt_init( SolvOpt *solvopt ) ;
void HDDM_Opt_init( HDDM_Opt *hddm_opt, int field, int analysis ) ;
void UNIT_init( UNIT *unit ) ;
void OPTIONS_init( OPTIONS *options, int work, int field, int analysis ) ;
void PrintUsage( char *commandname, char *mname, char *version,
                 char *usage ) ;

#endif /* __OPTIONS_H__ */
