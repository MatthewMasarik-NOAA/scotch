/* Copyright 2007,2021,2023 IPB, Universite de Bordeaux, INRIA & CNRS
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
/**   NAME       : hdgraph.c                               **/
/**                                                        **/
/**   AUTHOR     : Francois PELLEGRINI                     **/
/**                                                        **/
/**   FUNCTION   : These lines are the data declarations   **/
/**                for the distributed graph general       **/
/**                purpose routines.                       **/
/**                                                        **/
/**    DATES     : # Version 5.0  : from : 21 apr 2006     **/
/**                                 to   : 21 apr 2006     **/
/**                # Version 6.1  : from : 24 sep 2021     **/
/**                                 to   : 24 sep 2021     **/
/**                # Version 7.0  : from : 19 jan 2023     **/
/**                                 to   : 19 jan 2023     **/
/**                                                        **/
/************************************************************/

/*
** The defines and includes.
*/

#include "module.h"
#include "common.h"
#include "dgraph.h"
#include "hdgraph.h"

/******************************/
/*                            */
/* These routines handle halo */
/* distributed source graphs. */
/*                            */
/******************************/

/* This routine destroys a distributed halo graph
** structure. It is not a collective routine, as no
** communication is needed to perform the freeing of
** memory structures.
** It returns:
** - 0   : on success.
** - !0  : on error.
*/

void
hdgraphExit (
Hdgraph * restrict const    grafptr)
{
  if ((grafptr->vhndloctax != grafptr->s.vertloctax + 1) && /* If graph has a halo, with a separate end vertex array */
      ((grafptr->s.flagval & HDGRAPHFREEVHND) != 0))
    memFree (grafptr->vhndloctax);

  dgraphExit (&grafptr->s);                       /* Free distributed graph data */

#ifdef SCOTCH_DEBUG_HDGRAPH1
  memSet (grafptr, ~0, sizeof (Hdgraph));
#endif /* SCOTCH_DEBUG_HDGRAPH1 */
  grafptr->s.flagval = DGRAPHNONE;                /* A subsequent hdgraphExit() will have no effect */
}
