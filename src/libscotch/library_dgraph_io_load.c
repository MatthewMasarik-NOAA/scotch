/* Copyright 2007,2012,2018,2019,2023 IPB, Universite de Bordeaux, INRIA & CNRS
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
/**   NAME       : library_dgraph_io_load.c                **/
/**                                                        **/
/**   AUTHOR     : Francois PELLEGRINI                     **/
/**                                                        **/
/**   FUNCTION   : This module is the API for the distri-  **/
/**                buted source graph loading routine of   **/
/**                the libSCOTCH library.                  **/
/**                                                        **/
/**   DATES      : # Version 5.0  : from : 13 may 2007     **/
/**                                 to   : 13 may 2007     **/
/**                # Version 6.0  : from : 29 nov 2012     **/
/**                                 to   : 25 apr 2018     **/
/**                # Version 7.0  : from : 20 sep 2019     **/
/**                                 to   : 21 jan 2023     **/
/**                                                        **/
/************************************************************/

/*
**  The defines and includes.
*/

#include "module.h"
#include "common.h"
#include "context.h"
#include "graph.h"
#include "dgraph.h"
#include "ptscotch.h"

/****************************************/
/*                                      */
/* These routines are the C API for the */
/* distributed graph handling routines. */
/*                                      */
/****************************************/

/*+ This routine loads the given opaque graph
*** structure with the data of the given stream.
*** The base value allows the user to set the
*** graph base to 0 or 1, or to the base value
*** of the stream if the base value is equal
*** to -1. On input, vertex loads are discarded if
*** flagval is 1, edge loads are discarded if flagval
*** is 2, and both if flagval is set to 3.
*** It returns:
*** - 0   : if the loading succeeded.
*** - !0  : on error.
+*/

int
SCOTCH_dgraphLoad (
SCOTCH_Dgraph * const       grafptr,
FILE * const                stream,
const SCOTCH_Num            baseval,
const SCOTCH_Num            flagval)
{
  GraphFlag           srcgrafflag;                /* Graph flags */

  if ((baseval < -1) || (baseval > 1)) {
    errorPrint (STRINGIFY (SCOTCH_dgraphLoad) ": invalid base parameter");
    return     (1);
  }
  if ((flagval < 0) || (flagval > 3)) {
    errorPrint (STRINGIFY (SCOTCH_dgraphLoad) ": invalid flag parameter");
    return     (1);
  }

  srcgrafflag = (((flagval & 1) != 0) ? GRAPHIONOLOADVERT : 0) +
                (((flagval & 2) != 0) ? GRAPHIONOLOADEDGE : 0);

  return (dgraphLoad ((Dgraph * const) CONTEXTOBJECT (grafptr), stream, (Gnum) baseval, srcgrafflag));
}
