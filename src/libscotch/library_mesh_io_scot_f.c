/* Copyright 2004,2007,2010,2018,2023 IPB, Universite de Bordeaux, INRIA & CNRS
**
** This file is part of the Scotch software package for static mapping,
** graph partitioning and sparse matrix ordering.
**
** This software is governed by the CeCILL-C license under French law
** and abiding by the rules of distribution of free software. You can
** use, modify and/or redistribute the software under the terms of the
** CeCILL-C license as circulated by CEA, CNRS and INRIA at the following
** URL: "http://www.cecill.info".
**
** As a counterpart to the access to the source code and rights to copy,
** modify and redistribute granted by the license, users are provided
** only with a limited warranty and the software's author, the holder of
** the economic rights, and the successive licensors have only limited
** liability.
**
** In this respect, the user's attention is drawn to the risks associated
** with loading, using, modifying and/or developing or reproducing the
** software by the user in light of its specific status of free software,
** that may mean that it is complicated to manipulate, and that also
** therefore means that it is reserved for developers and experienced
** professionals having in-depth computer knowledge. Users are therefore
** encouraged to load and test the software's suitability as regards
** their requirements in conditions enabling the security of their
** systems and/or data to be ensured and, more generally, to use and
** operate it in the same conditions as regards security.
**
** The fact that you are presently reading this means that you have had
** knowledge of the CeCILL-C license and that you accept its terms.
*/
/************************************************************/
/**                                                        **/
/**   NAME       : library_mesh_io_scot_f.c                **/
/**                                                        **/
/**   AUTHOR     : Francois PELLEGRINI                     **/
/**                                                        **/
/**   FUNCTION   : This module is the Fortran API for the  **/
/**                mesh i/o routines of the libSCOTCH      **/
/**                library.                                **/
/**                                                        **/
/**   DATES      : # Version 4.0  : from : 24 nov 2005     **/
/**                                 to   : 24 nov 2005     **/
/**                # Version 5.1  : from : 27 mar 2010     **/
/**                                 to   : 27 mar 2010     **/
/**                # Version 6.0  : from : 20 apr 2018     **/
/**                                 to   : 25 apr 2018     **/
/**                # Version 7.0  : from : 21 jan 2023     **/
/**                                 to   : 21 jan 2023     **/
/**                                                        **/
/************************************************************/

/*
**  The defines and includes.
*/

#include "module.h"
#include "common.h"
#include "scotch.h"

/**************************************/
/*                                    */
/* These routines are the Fortran API */
/* for the mapping routines.          */
/*                                    */
/**************************************/

/* String lengths are passed at the very
** end of the argument list.
*/

SCOTCH_FORTRAN (                                  \
MESHGEOMLOADSCOT, meshgeomloadscot, (             \
SCOTCH_Mesh * const         meshptr,              \
SCOTCH_Geom * const         geomptr,              \
const int * const           filegrfptr,           \
const int * const           filegeoptr,           \
const char * const          dataptr, /* No use */ \
int * const                 revaptr,              \
const int                   datanbr),             \
(meshptr, geomptr, filegrfptr, filegeoptr, dataptr, revaptr, datanbr))
{
  FILE *              filegrfstream;              /* Streams to build from handles */
  FILE *              filegeostream;
  int                 filegrfnum;                 /* Duplicated handle */
  int                 filegeonum;
  int                 o;

  if ((filegrfnum = dup (*filegrfptr)) < 0) {     /* If cannot duplicate file descriptor */
    errorPrint (STRINGIFY (SCOTCH_NAME_PUBLICFU (MESHGEOMLOADSCOT)) ": cannot duplicate handle (1)");
    *revaptr = 1;                                 /* Indicate error */
    return;
  }
  if ((filegeonum = dup (*filegeoptr)) < 0) {     /* If cannot duplicate file descriptor */
    errorPrint (STRINGIFY (SCOTCH_NAME_PUBLICFU (MESHGEOMLOADSCOT)) ": cannot duplicate handle (2)");
    close      (filegrfnum);
    *revaptr = 1;                                 /* Indicate error */
    return;
  }
  if ((filegrfstream = fdopen (filegrfnum, "r")) == NULL) { /* Build stream from handle */
    errorPrint (STRINGIFY (SCOTCH_NAME_PUBLICFU (MESHGEOMLOADSCOT)) ": cannot open input stream (1)");
    close      (filegrfnum);
    close      (filegeonum);
    *revaptr = 1;
    return;
  }
  if ((filegeostream = fdopen (filegeonum, "r")) == NULL) { /* Build stream from handle */
    errorPrint (STRINGIFY (SCOTCH_NAME_PUBLICFU (MESHGEOMLOADSCOT)) ": cannot open input stream (2)");
    fclose     (filegrfstream);
    close      (filegeonum);
    *revaptr = 1;
    return;
  }

  o = SCOTCH_meshGeomLoadScot (meshptr, geomptr, filegrfstream, filegeostream, NULL);

  fclose (filegrfstream);                         /* This closes file descriptors too */
  fclose (filegeostream);

  *revaptr = o;
}

/* String lengths are passed at the very
** end of the argument list.
*/

SCOTCH_FORTRAN (                                  \
MESHGEOMSAVESCOT, meshgeomsavescot, (             \
const SCOTCH_Mesh * const   meshptr,              \
const SCOTCH_Geom * const   geomptr,              \
const int * const           filegrfptr,           \
const int * const           filegeoptr,           \
const char * const          dataptr, /* No use */ \
int * const                 revaptr,              \
const int                   datanbr),             \
(meshptr, geomptr, filegrfptr, filegeoptr, dataptr, revaptr, datanbr))
{
  FILE *              filegrfstream;              /* Streams to build from handles */
  FILE *              filegeostream;
  int                 filegrfnum;                 /* Duplicated handle */
  int                 filegeonum;
  int                 o;

  if ((filegrfnum = dup (*filegrfptr)) < 0) {     /* If cannot duplicate file descriptor */
    errorPrint (STRINGIFY (SCOTCH_NAME_PUBLICFU (MESHGEOMSAVESCOT)) ": cannot duplicate handle (1)");
    *revaptr = 1;                                 /* Indicate error */
    return;
  }
  if ((filegeonum = dup (*filegeoptr)) < 0) {     /* If cannot duplicate file descriptor */
    errorPrint (STRINGIFY (SCOTCH_NAME_PUBLICFU (MESHGEOMSAVESCOT)) ": cannot duplicate handle (2)");
    close      (filegrfnum);
    *revaptr = 1;                                 /* Indicate error */
    return;
  }
  if ((filegrfstream = fdopen (filegrfnum, "w")) == NULL) { /* Build stream from handle */
    errorPrint (STRINGIFY (SCOTCH_NAME_PUBLICFU (MESHGEOMSAVESCOT)) ": cannot open output stream (1)");
    close      (filegrfnum);
    close      (filegeonum);
    *revaptr = 1;
    return;
  }
  if ((filegeostream = fdopen (filegeonum, "w")) == NULL) { /* Build stream from handle */
    errorPrint (STRINGIFY (SCOTCH_NAME_PUBLICFU (MESHGEOMSAVESCOT)) ": cannot open output stream (2)");
    fclose     (filegrfstream);
    close      (filegeonum);
    *revaptr = 1;
    return;
  }

  o = SCOTCH_meshGeomSaveScot (meshptr, geomptr, filegrfstream, filegeostream, NULL);

  fclose (filegrfstream);                         /* This closes file descriptors too */
  fclose (filegeostream);

  *revaptr = o;
}
