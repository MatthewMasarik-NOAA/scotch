/* Copyright 2004,2007,2016,2023 IPB, Universite de Bordeaux, INRIA & CNRS
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
/**   NAME       : vgraph_separate_zr.c                    **/
/**                                                        **/
/**   AUTHOR     : Francois PELLEGRINI                     **/
/**                                                        **/
/**   FUNCTION   : This module moves all of the vertices   **/
/**                to the first subdomain.                 **/
/**                                                        **/
/**   DATES      : # Version 3.3  : from : 31 may 1999     **/
/**                                 to   : 31 may 1999     **/
/**                # Version 4.0  : from : 19 dec 2001     **/
/**                                 to   : 29 may 2004     **/
/**                # Version 6.0  : from : 15 aug 2016     **/
/**                                 to   : 15 aug 2016     **/
/**                # Version 7.0  : from : 16 jan 2023     **/
/**                                 to   : 16 jan 2023     **/
/**                                                        **/
/************************************************************/

/*
**  The defines and includes.
*/

#include "module.h"
#include "common.h"
#include "graph.h"
#include "vgraph.h"
#include "vgraph_separate_zr.h"

/*****************************/
/*                           */
/* This is the main routine. */
/*                           */
/*****************************/

/* This routine moves all of the graph vertices
** to the first part of the partition.
** It returns:
** - 0   : if the bipartitioning could be computed.
** - !0  : on error.
*/

int
vgraphSeparateZr (
Vgraph * const              grafptr)              /*+ Active graph +*/
{
  if (grafptr->compsize[0] != grafptr->s.vertnbr) /* If not all vertices already in part zero */
    vgraphZero (grafptr);

  return (0);
}
