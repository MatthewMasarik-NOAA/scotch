/* Copyright 2004,2007,2010-2012,2018,2023 IPB, Universite de Bordeaux, INRIA & CNRS
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
/**   NAME       : library_graph_map_f.c                   **/
/**                                                        **/
/**   AUTHOR     : Francois PELLEGRINI                     **/
/**                Sebastien FOURESTIER (v6.0)             **/
/**                                                        **/
/**   FUNCTION   : This module is the Fortran API for the  **/
/**                mapping routines of the libSCOTCH       **/
/**                library.                                **/
/**                                                        **/
/**   DATES      : # Version 3.4  : from : 02 dec 1999     **/
/**                                 to   : 15 nov 2001     **/
/**                # Version 4.0  : from : 12 jan 2004     **/
/**                                 to   : 12 dec 2005     **/
/**                # Version 5.1  : from : 27 mar 2010     **/
/**                                 to   : 31 aug 2011     **/
/**                # Version 6.0  : from : 17 apr 2011     **/
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

/*
**
*/

/* TODO update fortran interface... */
SCOTCH_FORTRAN (                      \
GRAPHMAPINIT, graphmapinit, (         \
const SCOTCH_Graph * const  grafptr,  \
SCOTCH_Mapping * const      mappptr,  \
const SCOTCH_Arch * const   archptr,  \
SCOTCH_Num * const          mapptab,  \
int * const                 revaptr), \
(grafptr, mappptr, archptr, mapptab, revaptr))
{
  *revaptr = SCOTCH_graphMapInit (grafptr, mappptr, archptr, mapptab);
}

/*
**
*/

SCOTCH_FORTRAN (                      \
GRAPHMAPEXIT, graphmapexit, (         \
const SCOTCH_Graph * const  grafptr,  \
SCOTCH_Mapping * const      mappptr), \
(grafptr, mappptr))
{
  SCOTCH_graphMapExit (grafptr, mappptr);
}

/*
**
*/

SCOTCH_FORTRAN (                      \
GRAPHMAPCOMPUTE, graphmapcompute, (   \
SCOTCH_Graph * const        grafptr,  \
SCOTCH_Mapping * const      mappptr,  \
SCOTCH_Strat * const        straptr,  \
int * const                 revaptr), \
(grafptr, mappptr, straptr, revaptr))
{
  *revaptr = SCOTCH_graphMapCompute (grafptr, mappptr, straptr);
}

/*
**
*/

SCOTCH_FORTRAN (                              \
GRAPHMAPFIXEDCOMPUTE, graphmapfixedcompute, ( \
SCOTCH_Graph * const        grafptr,          \
SCOTCH_Mapping * const      mappptr,          \
SCOTCH_Strat * const        straptr,          \
int * const                 revaptr),         \
(grafptr, mappptr, straptr, revaptr))
{
  *revaptr = SCOTCH_graphMapFixedCompute (grafptr, mappptr, straptr);
}

/*
**
*/

SCOTCH_FORTRAN (                        \
GRAPHREMAPCOMPUTE, graphremapcompute, ( \
SCOTCH_Graph * const        grafptr,    \
SCOTCH_Mapping * const      mappptr,    \
SCOTCH_Mapping * const      mapoptr,    \
const double * const        emraptr,    \
const SCOTCH_Num * const    vmlotab,    \
SCOTCH_Strat * const        straptr,    \
int * const                 revaptr),   \
(grafptr, mappptr, mapoptr, emraptr, vmlotab, straptr, revaptr))
{
  *revaptr = SCOTCH_graphRemapCompute (grafptr, mappptr, mapoptr, *emraptr, vmlotab, straptr);
}

/*
**
*/

SCOTCH_FORTRAN (                                  \
GRAPHREMAPFIXEDCOMPUTE, graphremapfixedcompute, ( \
SCOTCH_Graph * const        grafptr,              \
SCOTCH_Mapping * const      mappptr,              \
SCOTCH_Mapping * const      mapoptr,              \
const double * const        emraptr,              \
const SCOTCH_Num * const    vmlotab,              \
SCOTCH_Strat * const        straptr,              \
int * const                 revaptr),             \
(grafptr, mappptr, mapoptr, emraptr, vmlotab, straptr, revaptr))
{
  *revaptr = SCOTCH_graphRemapFixedCompute (grafptr, mappptr, mapoptr, *emraptr, vmlotab, straptr);
}

/*
**
*/

SCOTCH_FORTRAN (                      \
GRAPHMAP, graphmap, (                 \
SCOTCH_Graph * const        grafptr,  \
const SCOTCH_Arch * const   archptr,  \
SCOTCH_Strat * const        straptr,  \
SCOTCH_Num * const          parttab,  \
int * const                 revaptr), \
(grafptr, archptr, straptr, parttab, revaptr))
{
  *revaptr = SCOTCH_graphMap (grafptr, archptr, straptr, parttab);
}

/*
**
*/

SCOTCH_FORTRAN (                      \
GRAPHMAPFIXED, graphmapfixed, (       \
SCOTCH_Graph * const        grafptr,  \
const SCOTCH_Arch * const   archptr,  \
SCOTCH_Strat * const        straptr,  \
SCOTCH_Num * const          parttab,  \
int * const                 revaptr), \
(grafptr, archptr, straptr, parttab, revaptr))
{
  *revaptr = SCOTCH_graphMapFixed (grafptr, archptr, straptr, parttab);
}

/*
**
*/

SCOTCH_FORTRAN (                      \
GRAPHREMAP, graphremap, (             \
SCOTCH_Graph * const        grafptr,  \
const SCOTCH_Arch * const   archptr,  \
SCOTCH_Num * const          parotab,  \
const double * const        emraptr,  \
const SCOTCH_Num * const    vmlotab,  \
SCOTCH_Strat * const        straptr,  \
SCOTCH_Num * const          parttab,  \
int * const                 revaptr), \
(grafptr, archptr, parotab, emraptr, vmlotab, straptr, parttab, revaptr))
{
  *revaptr = SCOTCH_graphRemap (grafptr, archptr, parotab, *emraptr, vmlotab, straptr, parttab);
}

/*
**
*/

SCOTCH_FORTRAN (                      \
GRAPHREMAPFIXED, graphremapfixed, (   \
SCOTCH_Graph * const        grafptr,  \
const SCOTCH_Arch * const   archptr,  \
SCOTCH_Num * const          parotab,  \
const double * const        emraptr,  \
const SCOTCH_Num * const    vmlotab,  \
SCOTCH_Strat * const        straptr,  \
SCOTCH_Num * const          parttab,  \
int * const                 revaptr), \
(grafptr, archptr, parotab, emraptr, vmlotab, straptr, parttab, revaptr))
{
  *revaptr = SCOTCH_graphRemapFixed (grafptr, archptr, parotab, *emraptr, vmlotab, straptr, parttab);
}

/*
**
*/

SCOTCH_FORTRAN (                      \
GRAPHPART, graphpart, (               \
SCOTCH_Graph * const        grafptr,  \
const SCOTCH_Num * const    partptr,  \
SCOTCH_Strat * const        straptr,  \
SCOTCH_Num * const          parttab,  \
int * const                 revaptr), \
(grafptr, partptr, straptr, parttab, revaptr))
{
  *revaptr = SCOTCH_graphPart (grafptr, *partptr, straptr, parttab);
}

/*
**
*/

SCOTCH_FORTRAN (                      \
GRAPHPARTFIXED, graphpartfixed, (     \
SCOTCH_Graph * const        grafptr,  \
const SCOTCH_Num * const    partptr,  \
SCOTCH_Strat * const        straptr,  \
SCOTCH_Num * const          parttab,  \
int * const                 revaptr), \
(grafptr, partptr, straptr, parttab, revaptr))
{
  *revaptr = SCOTCH_graphPartFixed (grafptr, *partptr, straptr, parttab);
}

/*
**
*/

SCOTCH_FORTRAN (                      \
GRAPHREPART, graphrepart, (           \
SCOTCH_Graph * const        grafptr,  \
const SCOTCH_Num * const    partptr,  \
SCOTCH_Num * const          parotab,  \
const double * const        emraptr,  \
const SCOTCH_Num * const    vmlotab,  \
SCOTCH_Strat * const        straptr,  \
SCOTCH_Num * const          parttab,  \
int * const                 revaptr), \
(grafptr, partptr, parotab, emraptr, vmlotab, straptr, parttab, revaptr))
{
  *revaptr = SCOTCH_graphRepart (grafptr, *partptr, parotab, *emraptr, vmlotab, straptr, parttab);
}

/*
**
*/

SCOTCH_FORTRAN (                      \
GRAPHREPARTFIXED, graphrepartfixed, ( \
SCOTCH_Graph * const        grafptr,  \
const SCOTCH_Num * const    partptr,  \
SCOTCH_Num * const          parotab,  \
const double * const        emraptr,  \
const SCOTCH_Num * const    vmlotab,  \
SCOTCH_Strat * const        straptr,  \
SCOTCH_Num * const          parttab,  \
int * const                 revaptr), \
(grafptr, partptr, parotab, emraptr, vmlotab, straptr, parttab, revaptr))
{
  *revaptr = SCOTCH_graphRepartFixed (grafptr, *partptr, parotab, *emraptr, vmlotab, straptr, parttab);
}

/* String lengths are passed at the very
** end of the argument list.
*/

SCOTCH_FORTRAN (                     \
STRATGRAPHMAP, stratgraphmap, (      \
SCOTCH_Strat * const        straptr, \
const char * const          string,  \
int * const                 revaptr, \
const int                   strnbr), \
(straptr, string, revaptr, strnbr))
{
  char * restrict     strtab;                     /* Pointer to null-terminated string */

  if ((strtab = (char *) memAlloc (strnbr + 1)) == NULL) { /* Allocate temporary space */
    errorPrint (STRINGIFY (SCOTCH_NAME_PUBLICFU (STRATGRAPHMAP)) ": out of memory");
    *revaptr = 1;
    return;
  }
  memCpy (strtab, string, strnbr);                /* Copy string contents */
  strtab[strnbr] = '\0';                          /* Terminate string     */

  *revaptr = SCOTCH_stratGraphMap (straptr, strtab); /* Call original routine */

  memFree (strtab);
}

/*
**
*/

SCOTCH_FORTRAN (                          \
STRATGRAPHMAPBUILD, stratgraphmapbuild, ( \
SCOTCH_Strat * const        straptr,      \
const SCOTCH_Num * const    flagval,      \
const SCOTCH_Num * const    partnbr,      \
const double * const        kbalval,      \
int * const                 revaptr),     \
(straptr, flagval, partnbr, kbalval, revaptr))
{
  *revaptr = SCOTCH_stratGraphMapBuild (straptr, *flagval, *partnbr, *kbalval);
}

/*
**
*/

SCOTCH_FORTRAN (                                  \
STRATGRAPHCLUSTERBUILD, stratgraphclusterbuild, ( \
SCOTCH_Strat * const        straptr,              \
const SCOTCH_Num * const    flagval,              \
const SCOTCH_Num * const    pwgtval,              \
const double * const        densval,              \
const double * const        bbalval,              \
int * const                 revaptr),             \
(straptr, flagval, pwgtval, densval, bbalval, revaptr))
{
  *revaptr = SCOTCH_stratGraphClusterBuild (straptr, *flagval, *pwgtval, *densval, *bbalval);
}
