/********************************************************************
 Copyright (C) 2000-2019 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
*********************************************************************/

#ifndef  __FDNAME_H__
#define  __FDNAME_H__

#include "advfullwave_lib.h"


/*** file or directory ***/
/** check or make directory **/
int check_directory( char *dirname ) ;
int make_fwdata_directory( DataSW datasw ) ;
int make_onedata_directory( DataSW datasw ) ;
int make_modeldata_directory( DataSW datasw ) ;
int make_Jodata_directory( DataSW datasw, char *Jodata_file ) ;
int make_Jodefine_directory( DataSW datasw, char *Jodefine_file ) ;
int make_Jometis_directory( DataSW datasw ) ;
int make_Inidata_directory( DataSW datasw ) ;
int make_Inimetis_directory( DataSW datasw ) ;
int make_outdata_directory( DataSW datasw ) ;
int make_avsfile_directory( DataSW datasw ) ;
int make_vtkfile_directory( DataSW datasw ) ;
int make_calclog_directory( DataSW datasw ) ;
int make_mtrldat_directory( DataSW datasw ) ;


/** make file or directory name **/
void make_fwdata_filename( char *fname, DataSW datasw, int i ) ;
void make_onedata_filename( char *fname, DataSW datasw, int i ) ;
void make_modeldata_filename( char *fname, DataSW datasw, int ipart ) ;
void make_Jodata_filename( char *fname, DataSW datasw,
                           char *Jodata_file, int time_step ) ;
void make_Jodefine_filename( char *fname, DataSW datasw,
                             char *Jodefine_file, int i ) ;
void make_Jometis_filename( char *fname, DataSW datasw, int ipart,
                            int time_step ) ;
void make_Inidata_filename( char *fname, DataSW datasw, int i ) ;
void make_Inimetis_filename( char *fname, DataSW datasw, int ipart ) ;
void make_outdata_filename( char *fname, DataSW datasw, int ipart,
                            int time_step ) ;
void make_avsfile_filename( char *fname, DataSW datasw, int ipart,
                            int time_step, char *str ) ;
void make_vtkfile_filename( char *fname, DataSW datasw, int ipart,
                            int time_step, char *str ) ;
void make_calclog_filename( char *fname, DataSW datasw, int ipart,
                            int time_step ) ;
void make_mtrldat_filename( char *fname, DataSW datasw, int i ) ;

#endif /* __FDNAME_H__ */
