/* Copyright 2011,2015,2018,2023 IPB, Universite de Bordeaux, INRIA & CNRS
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
/**   NAME       : library_graph_coarsen_f.c               **/
/**                                                        **/
/**   AUTHOR     : Francois PELLEGRINI                     **/
/**                                                        **/
/**   FUNCTION   : This module is the Fortran API for the  **/
/**                graph coarsening routine of the         **/
/**                libSCOTCH library.                      **/
/**                                                        **/
/**   DATES      : # Version 5.1  : from : 07 aug 2011     **/
/**                                 to   : 07 aug 2011     **/
/**                # Version 6.0  : from : 28 feb 2015     **/
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
/* for the coarsening routine.        */
/*                                    */
/**************************************/

/*
**
*/

SCOTCH_FORTRAN (                         \
GRAPHCOARSEN, graphcoarsen, (            \
SCOTCH_Graph * const        finegrafptr, \
const SCOTCH_Num * const    coarvertptr, \
const double * const        coarnbr,     \
const SCOTCH_Num * const    flagval,     \
SCOTCH_Graph * const        coargrafptr, \
SCOTCH_Num * const          coarmulttab, \
int * const                 revaptr),    \
(finegrafptr, coarvertptr, coarnbr, flagval, coargrafptr, coarmulttab, revaptr))
{
  *revaptr = SCOTCH_graphCoarsen (finegrafptr, *coarvertptr, *coarnbr, *flagval, coargrafptr, coarmulttab);
}

/*
**
*/

SCOTCH_FORTRAN (                         \
GRAPHCOARSENMATCH, graphcoarsenmatch, (  \
SCOTCH_Graph * const        finegrafptr, \
SCOTCH_Num * const          coarvertptr, \
const double * const        coarnbr,     \
const SCOTCH_Num * const    flagval,     \
SCOTCH_Num * const          finematetab, \
int * const                 revaptr),    \
(finegrafptr, coarvertptr, coarnbr, flagval, finematetab, revaptr))
{
  *revaptr = SCOTCH_graphCoarsenMatch (finegrafptr, coarvertptr, *coarnbr, *flagval, finematetab);
}

/*
**
*/

SCOTCH_FORTRAN (                         \
GRAPHCOARSENBUILD, graphcoarsenbuild, (  \
SCOTCH_Graph * const        finegrafptr, \
SCOTCH_Num * const          coarvertptr, \
SCOTCH_Num * const          finematetab, \
SCOTCH_Graph * const        coargrafptr, \
SCOTCH_Num * const          coarmulttab, \
int * const                 revaptr),    \
(finegrafptr, coarvertptr, finematetab, coargrafptr, coarmulttab, revaptr))
{
  *revaptr = SCOTCH_graphCoarsenBuild (finegrafptr, *coarvertptr, finematetab, coargrafptr, coarmulttab);
}
