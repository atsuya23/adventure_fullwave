/********************************************************************
 Copyright (C) 2000-2019 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
*********************************************************************/

#include "fdname.h"


extern COM_WLD_ID COM_ID ;
extern COM_GRP_ID COM_GID ;


int check_directory( char *dirname )
{
  /*+ Function: check the status of directory +*/
  /*  return 0 when directory exist or is made
                              and can be written */
#ifdef __ADVFW_OS_UNIX__
  struct stat dirstat ;


  errno = 0 ;
  if( ( stat(dirname, &dirstat) != 0 ) && ( errno == ENOENT)) {
    Fprintf( stdout, "directory \"%s\" dose not exist\n", dirname ) ;
    mkdir( dirname, 0777 ) ;
    Fprintf( stdout, "make directory \"%s\"\n", dirname ) ;
  }


  if( stat(dirname, &dirstat) == 0 ) {
    if( S_ISDIR(dirstat.st_mode) ) {
      if( dirstat.st_mode & S_IWUSR ) {
	return 0 ;
      } else {
	Efprintf( stderr, "directory \"%s\" exists, but not writable",
		  dirname ) ;
	return 1 ;
      }
    } else {
      Efprintf( stderr, "cannot make directory, file \"%s\" exists\n",
	        dirname ) ;
      return 1 ;
    }
  } else {
    Efprintf( stderr, "cannot make directory \"%s\"\n", dirname ) ;
    return 1 ;
  }
#else /* __ADVFW_OS_UNIX__ */
  Fprintf( stdout, "cannot check directory \"%s\"\n", dirname ) ;
  return 0 ;
#endif /* __ADVFW_OS_UNIX__ */
}

/***********************************************************************/

int make_fwdata_directory( DataSW datasw )
{
  /*+ Function: make fwdata directory +*/
  /*  return 0 when directory is normally made +*/

  char dirname[1024];
  int no_ok;
  
  no_ok = 0;
  make_fwdata_filename(dirname, datasw, -1);
  no_ok += check_directory( dirname ) ;

  return(no_ok);
}

/************************************************************************/

int make_onedata_directory( DataSW datasw )
{
  /*+ Function: make onedata directory +*/
  /*  return 0 when directory is normally made +*/

  char dirname[1024];
  int no_ok;
  
  no_ok = 0;
  make_onedata_filename(dirname, datasw, -1);
  no_ok += check_directory( dirname ) ;

  return(no_ok);
}

/************************************************************************/

int make_modeldata_directory( DataSW datasw )
{
  /*+ Function: make onedata directory +*/
  /*  return 0 when directory is normally made +*/

  char dirname[1024];
  int no_ok;
  
  no_ok = 0;
  make_modeldata_filename(dirname, datasw, -1);
  no_ok += check_directory( dirname ) ;

  return(no_ok);
}

/************************************************************************/

int make_Jodata_directory( DataSW datasw, char *Jodata_file )
{
  /*+ Function: make Jodata directory +*/
  /*  return 0 when directory is normally made +*/

  char dirname[1024];
  int no_ok;
  
  no_ok = 0;
  make_Jodata_filename( dirname, datasw, Jodata_file, -3 ) ;
  no_ok += check_directory( dirname ) ;

  return(no_ok);
}

/************************************************************************/

int make_Jodefine_directory( DataSW datasw, char *Jodefine_file )
{
  /*+ Function: make Jodefine directory +*/
  /*  return 0 when directory is normally made +*/

  char dirname[1024];
  int no_ok;
  
  no_ok = 0;
  make_Jodefine_filename( dirname, datasw, Jodefine_file, -1 ) ;
  no_ok += check_directory( dirname ) ;

  return(no_ok);
}

/************************************************************************/

int make_Jometis_directory( DataSW datasw )
{
  /*+ Function: make Jometis directory +*/
  /*  return 0 when directory is normally made +*/

  char dirname[1024];
  int no_ok;
  
  no_ok = 0;
  make_Jometis_filename(dirname, datasw, 0, -3);
  no_ok += check_directory( dirname ) ;

  return(no_ok);
}

/***********************************************************************/

int make_Inidata_directory( DataSW datasw )
{
  /*+ Function: make Inidata directory +*/
  /*  return 0 when directory is normally made +*/

  char dirname[1024];
  int no_ok;
  
  no_ok = 0;
  make_Inidata_filename(dirname, datasw, -1);
  no_ok += check_directory( dirname ) ;

  return(no_ok);
}

/************************************************************************/

int make_Inimetis_directory( DataSW datasw )
{
  /*+ Function: make Inimetis directory +*/
  /*  return 0 when directory is normally made +*/

  char dirname[1024];
  int no_ok;
  
  no_ok = 0;
  make_Inimetis_filename(dirname, datasw, -1);
  no_ok += check_directory( dirname ) ;

  return(no_ok);
}

/************************************************************************/

int make_outdata_directory( DataSW datasw )
{
  /*+ Function: make outdata directory +*/
  /*  return 0 when directory is normally made +*/

  char dirname[1024];
  int no_ok;

  no_ok = 0;
  make_outdata_filename(dirname, datasw, 0, -2 ) ;
  no_ok += check_directory( dirname ) ;

  return(no_ok);
}

/************************************************************************/

int make_avsfile_directory( DataSW datasw )
{
  /*+ Function: make avsfile directory +*/
  /*  return 0 when directory is normally made +*/

  char dirname[1024];
  int no_ok;

  no_ok = 0;
  make_avsfile_filename(dirname, datasw, 0, -2, NULL ) ;
  no_ok += check_directory( dirname ) ;

  return(no_ok);
}

/************************************************************************/

int make_vtkfile_directory( DataSW datasw )
{
  /*+ Function: make vtkfile directory +*/
  /*  return 0 when directory is normally made +*/

  char dirname[1024];
  int no_ok;

  no_ok = 0;
  make_vtkfile_filename(dirname, datasw, 0, -2, NULL ) ;
  no_ok += check_directory( dirname ) ;

  return(no_ok);
}

/************************************************************************/

int make_calclog_directory( DataSW datasw )
{
  /*+ Function: make calclog directory +*/
  /*  return 0 when directory is normally made +*/

  char dirname[1024];
  int no_ok;
  
  no_ok = 0;
  make_calclog_filename(dirname, datasw, 0, -1);
  no_ok += check_directory( dirname ) ;

  return(no_ok);
}

/***********************************************************************/

int make_mtrldat_directory( DataSW datasw )
{
  /*+ Function: make mtrldat directory +*/
  /*  return 0 when directory is normally made +*/

  char dirname[1024];
  int no_ok;
  
  no_ok = 0;
  make_mtrldat_filename(dirname, datasw, -1);
  no_ok += check_directory( dirname ) ;

  return(no_ok);
}

/********************************************************************/

void make_fwdata_filename( char *fname, DataSW datasw, int i )
{
  /*+ Funciton: make file name of fwdata +*/
  /* if i < 0 , return only directory name */
  if ( i < 0 ) {
    sprintf(fname, "%s/%s", datasw.data_dir, datasw.fwdata_dir );
  } else {
    sprintf( fname, "%s/%s/%s", datasw.data_dir, datasw.fwdata_dir,
             datasw.fwdata_file );
  }
}

/********************************************************************/

void make_onedata_filename( char *fname, DataSW datasw, int i )
{
  /*+ Funciton: make file name of onedata +*/
  /* if i < 0 , return only directory name */
  if ( i < 0 ) {
    sprintf(fname, "%s/%s", datasw.data_dir, datasw.onedata_dir );
  } else {
    sprintf( fname, "%s/%s/%s%s", datasw.data_dir, datasw.onedata_dir,
             datasw.onedata_file, ADVFILE_EXT );
  }
}

/********************************************************************/

void make_modeldata_filename( char *fname, DataSW datasw, int ipart )
{
  /*+ Funciton: make file name of onedata +*/
  /* if ipart < 0 , return only directory name */
  if ( ipart < 0 ) {
    sprintf(fname, "%s/%s", datasw.data_dir, datasw.modeldata_dir );
  } else {
    sprintf( fname, "%s/%s/%s_%d%s", datasw.data_dir,
             datasw.modeldata_dir, datasw.modeldata_file, ipart,
             ADVFILE_EXT );
  }
}

/********************************************************************/

void make_Jodata_filename( char *fname, DataSW datasw,
                           char *Jodata_file, int time_step )
{
  /*+ Funciton: make file name of Jodata +*/
  /* if time_step < -2 , return only directory name */
  /* if time_step == -2 , return file name for Full Wave i */
  /* if time_step == -1 , return file name for Full Wave r */
  if ( time_step < -2 ) {
    sprintf(fname, "%s", datasw.data_dir ) ;
  } else if( time_step == -2 ) {
    sprintf( fname, "%s/%si", datasw.data_dir, Jodata_file );
  } else if( time_step == -1 ) {
    sprintf( fname, "%s/%sr", datasw.data_dir, Jodata_file );
  } else {
    sprintf( fname, "%s/%s%d", datasw.data_dir, Jodata_file, time_step );
  }
}

/********************************************************************/

void make_Jodefine_filename( char *fname, DataSW datasw,
                             char *Jodefine_file, int i )
{
  /*+ Funciton: make file name of Jodefine +*/
  /* if i < 0 , return only directory name */
  if ( i < 0 ) {
    sprintf( fname, "%s", datasw.data_dir ) ;
  } else {
    sprintf( fname, "%s/%s", datasw.data_dir, Jodefine_file );
  }
}

/********************************************************************/

void make_Jometis_filename( char *fname, DataSW datasw, int ipart,
                            int time_step )
{
  /*+ Funciton: make file name of Jometis +*/
  /* if time_step < -2 , return only directory name */
  /* if time_step == -2 , return file name for Full Wave i */
  /* if time_step == -1 , return file name for Full Wave r */

  if ( time_step < -2 ) {
    sprintf(fname, "%s/%s", datasw.data_dir, datasw.Jometis_dir );
  } else if( time_step == -2 ) {
    sprintf( fname, "%s/%s/%si_%d%s", datasw.data_dir, datasw.Jometis_dir,
             datasw.Jometis_file, ipart, ADVFILE_EXT );
  } else if( time_step == -1 ) {
    sprintf( fname, "%s/%s/%sr_%d%s", datasw.data_dir, datasw.Jometis_dir,
             datasw.Jometis_file, ipart, ADVFILE_EXT );
  } else {
    sprintf( fname, "%s/%s/%s_%d_%d%s", datasw.data_dir,
             datasw.Jometis_dir, datasw.Jometis_file, time_step, ipart,
             ADVFILE_EXT );
  }
}

/********************************************************************/

void make_Inidata_filename( char *fname, DataSW datasw, int i )
{
  /*+ Funciton: make file name of Inidata +*/
  /* if i < 0 , return only directory name */
  if ( i < 0 ) {
    sprintf(fname, "%s/%s", datasw.data_dir, datasw.Inidata_dir );
  } else {
    sprintf( fname, "%s/%s/%s", datasw.data_dir, datasw.Inidata_dir,
             datasw.Inidata_file );
  }
}

/********************************************************************/

void make_Inimetis_filename( char *fname, DataSW datasw, int ipart )
{
  /*+ Funciton: make file name of Inimetis +*/
  /* if ipart < 0 , return only directory name */
  if ( ipart < 0 ) {
    sprintf(fname, "%s/%s", datasw.data_dir, datasw.Inimetis_dir );
  } else {
    sprintf( fname, "%s/%s/%s_%d%s", datasw.data_dir,
             datasw.Inimetis_dir, datasw.Inimetis_file, ipart,
             ADVFILE_EXT );
  }
}

/********************************************************************/

void make_outdata_filename( char *fname, DataSW datasw, int ipart,
                            int time_step )
{
  /*+ Funciton: make file name of outdata +*/
  /* if ipart < 0 , return filename for one domain */
  /* if time_step < -1 , return only directory name */
  /* if time_step == -1 , return file name for Full Wave */
  if ( time_step < -1 ) {
    sprintf(fname, "%s/%s", datasw.data_dir, datasw.outdata_dir );
  } else if( time_step <= 0 ) {
    if( ipart < 0 ) {
      sprintf( fname, "%s/%s/%s%s", datasw.data_dir, datasw.outdata_dir,
               datasw.outdata_file, ADVFILE_EXT );
    } else {
      sprintf( fname, "%s/%s/%s_%d%s", datasw.data_dir, datasw.outdata_dir,
               datasw.outdata_file, ipart, ADVFILE_EXT );
    }
  } else {
    if( ipart < 0 ) {
      sprintf( fname, "%s/%s/%s_%d%s", datasw.data_dir,
               datasw.outdata_dir, datasw.outdata_file, time_step,
               ADVFILE_EXT );
    } else {
      sprintf( fname, "%s/%s/%s_%d_%d%s", datasw.data_dir,
               datasw.outdata_dir, datasw.outdata_file, time_step, ipart,
               ADVFILE_EXT );
    }
  }
}

/********************************************************************/

void make_avsfile_filename( char *fname, DataSW datasw, int ipart,
                            int time_step, char *str )
{
  /*+ Funciton: make file name of avsfile +*/
  /* if ipart < 0 , return filename for one domain */
  /* if time_step < -1 , return only directory name */
  /* if time_step == -1 , return file name for Full Wave */
  if ( time_step < -1 ) {
    sprintf(fname, "%s/%s", datasw.data_dir, datasw.avsfile_dir );
  } else if( time_step == -1 ) {
    if( ipart < 0 ) {
      sprintf( fname, "%s/%s/%s_%s.inp", datasw.data_dir,
               datasw.avsfile_dir, datasw.avsfile_file, str );
    } else {
      sprintf( fname, "%s/%s/%s_%s_%d.inp", datasw.data_dir,
               datasw.avsfile_dir, datasw.avsfile_file, str, ipart );
    }
  } else {
    if( ipart < 0 ) {
      sprintf( fname, "%s/%s/%s_%s_%d.inp", datasw.data_dir,
               datasw.avsfile_dir, datasw.avsfile_file, str,
               time_step );
    } else {
      sprintf( fname, "%s/%s/%s_%s_%d_%d.inp", datasw.data_dir,
               datasw.avsfile_dir, datasw.avsfile_file, str, 
               time_step, ipart );
    }
  }
}

/********************************************************************/

void make_vtkfile_filename( char *fname, DataSW datasw, int ipart,
                            int time_step, char *str )
{
  /*+ Funciton: make file name of vtkfile +*/
  /* if ipart < 0 , return filename for one domain */
  /* if time_step < -1 , return only directory name */
  /* if time_step == -1 , return file name for Full Wave */
  if ( time_step < -1 ) {
    sprintf(fname, "%s/%s", datasw.data_dir, datasw.vtkfile_dir );
  } else if( time_step == -1 ) {
    if( ipart < 0 ) {
      sprintf( fname, "%s/%s/%s_%s.vtk", datasw.data_dir,
               datasw.vtkfile_dir, datasw.vtkfile_file, str );
    } else {
      sprintf( fname, "%s/%s/%s_%s_%d.vtk", datasw.data_dir,
               datasw.vtkfile_dir, datasw.vtkfile_file, str, ipart );
    }
  } else {
    if( ipart < 0 ) {
      sprintf( fname, "%s/%s/%s_%s_%d.vtk", datasw.data_dir,
               datasw.vtkfile_dir, datasw.vtkfile_file, str,
               time_step );
    } else {
      sprintf( fname, "%s/%s/%s_%s_%d_%d.vtk", datasw.data_dir,
               datasw.vtkfile_dir, datasw.vtkfile_file, str, 
               time_step, ipart );
    }
  }
}

/********************************************************************/

void make_calclog_filename( char *fname, DataSW datasw, int ipart,
                            int time_step )
{
  /*+ Funciton: make file name of calclog +*/
  /* if ipart == -1 , return file name for FEM */
  /* if ipart == -2 , return file name for HDDM */
  /* if time_step == -1 , return only directory name */
  /* if time_step == -2 , return file name for Full Wave */

  if( ipart == -1 ) {
    if ( time_step == -1 ) {
      sprintf(fname, "%s/%s", datasw.data_dir, datasw.calclog_dir );
    } else if( time_step == -2 ) {
      sprintf( fname, "%s/%s/%s_Full_Wave", datasw.data_dir, datasw.calclog_dir,
               datasw.calclog_file );
    } 

  } else if( ipart == -2 ) {
    char* str = "_HDDM" ;
    if ( time_step == -1 ) {
      sprintf(fname, "%s/%s", datasw.data_dir, datasw.calclog_dir );
    } else if( time_step == -2 ) {
      sprintf( fname, "%s/%s/%s%s_Full_Wave", datasw.data_dir,
               datasw.calclog_dir, datasw.calclog_file, str );
    } 

  } else {
    char* str = "_HDDM_DOM" ;
    if ( time_step == -1 ) {
      sprintf(fname, "%s/%s", datasw.data_dir, datasw.calclog_dir );
    } else if( time_step == -2 ) {
      sprintf( fname, "%s/%s/%s%s_Full_Wave_%d", datasw.data_dir,
               datasw.calclog_dir, datasw.calclog_file, str, ipart );
    } 

  }
}

/********************************************************************/

void make_mtrldat_filename( char *fname, DataSW datasw, int i )
{
  /*+ Funciton: make file name of mtrldat +*/
  /* if i < 0 , return only directory name */
  if( strlen(datasw.mtrldat_dir) == 0 ) {
    if ( i < 0 ) {
      sprintf(fname, "%s", datasw.data_dir );
    } else {
      sprintf( fname, "%s/%s", datasw.data_dir, datasw.mtrldat_file );
    }
  } else {
    if ( i < 0 ) {
      sprintf(fname, "%s/%s", datasw.data_dir, datasw.mtrldat_dir );
    } else {
      sprintf( fname, "%s/%s/%s", datasw.data_dir, datasw.mtrldat_dir,
               datasw.mtrldat_file );
    }
  }
}
