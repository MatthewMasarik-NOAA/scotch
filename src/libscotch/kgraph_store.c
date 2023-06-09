/* Copyright 2011,2021,2023 IPB, Universite de Bordeaux, INRIA & CNRS
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
/**   NAME       : kgraph_store.c                          **/
/**                                                        **/
/**   AUTHOR     : Francois PELLEGRINI                     **/
/**                Sebastien FOURESTIER (v6.0)             **/
/**                                                        **/
/**   FUNCTION   : This module contains the save data      **/
/**                structure handling routines for k-par-  **/
/**                tition graphs.                          **/
/**                                                        **/
/**   DATES      : # Version 6.0  : from : 16 aug 2011     **/
/**                                 to   : 16 aug 2011     **/
/**                # Version 6.1  : from : 18 jul 2021     **/
/**                                 to   : 18 jul 2021     **/
/**                # Version 7.0  : from : 20 jan 2023     **/
/**                                 to   : 20 jan 2023     **/
/**                                                        **/
/************************************************************/

/*
**  The defines and includes.
*/

#include "module.h"
#include "common.h"
#include "graph.h"
#include "arch.h"
#include "mapping.h"
#include "kgraph.h"

/**********************************/
/*                                */
/* Store graph handling routines. */
/*                                */
/**********************************/

/* This routine builds a save structure
** for the given active graph.
** It returns:
** - 0   : if allocation succeeded.
** - !0  : on error.
*/

int
kgraphStoreInit (
const Kgraph * const        grafptr,
KgraphStore * const         storptr)
{
  storptr->domnmax = (Gnum) grafptr->m.domnmax;   /* Get maximum number of domains to save */

  if (memAllocGroup ((void **) (void *)           /* Allocate save structure */
                     &storptr->parttab,     (size_t) (grafptr->s.vertnbr * sizeof (Anum)),
                     &storptr->domntab,     (size_t) (grafptr->m.domnmax * sizeof (ArchDom)),
                     &storptr->comploadavg, (size_t) (grafptr->m.domnmax * sizeof (Gnum)),
                     &storptr->comploaddlt, (size_t) (grafptr->m.domnmax * sizeof (Gnum)),
                     &storptr->frontab,     (size_t) (grafptr->s.vertnbr * sizeof (Gnum)), NULL) == NULL) { /* TRICK: frontab last as partially used */
    errorPrint ("kgraphStoreInit: out of memory");
    return (1);
  }

  return (0);
}

/* This routine frees a save structure.
** It returns:
** - VOID  : in all cases.
*/

void
kgraphStoreExit (
KgraphStore * const         storptr)
{
  memFree (storptr->parttab);                     /* Free group leader */
#ifdef SCOTCH_DEBUG_KGRAPH2
  storptr->parttab     = NULL;
  storptr->domntab     = NULL;
  storptr->frontab     = NULL;
  storptr->comploadavg = NULL;
  storptr->comploaddlt = NULL;
#endif /* SCOTCH_DEBUG_KGRAPH2 */
}

/* This routine saves partition data from the
** given active graph to the given save structure.
** It returns:
** - VOID  : in all cases.
*/

void
kgraphStoreSave (
const Kgraph * const        grafptr,
KgraphStore * const         storptr)
{
#ifdef SCOTCH_DEBUG_KGRAPH2
  if (grafptr->m.domnnbr > storptr->domnmax) {
    errorPrint ("kgraphStoreSave: cannot save state");
    return;
  }
#endif /* SCOTCH_DEBUG_KGRAPH2 */

  storptr->domnnbr  = grafptr->m.domnnbr;
  storptr->fronnbr  = grafptr->fronnbr;
  storptr->commload = grafptr->commload;
  storptr->kbalval  = grafptr->kbalval;

  if (grafptr->m.domnnbr > 0) {                   /* If valid mapping */
    memCpy (storptr->parttab,     grafptr->m.parttax + grafptr->s.baseval, grafptr->s.vertnbr * sizeof (Anum));
    memCpy (storptr->domntab,     grafptr->m.domntab,                      grafptr->m.domnnbr * sizeof (ArchDom));
    memCpy (storptr->comploadavg, grafptr->comploadavg,                    grafptr->m.domnnbr * sizeof (Gnum));
    memCpy (storptr->comploaddlt, grafptr->comploaddlt,                    grafptr->m.domnnbr * sizeof (Gnum));
    memCpy (storptr->frontab,     grafptr->frontab,                        grafptr->fronnbr   * sizeof (Gnum));
  }
}

/* This routine updates partition data of the
** given active graph, using the given save graph.
** It returns:
** - VOID  : in all cases.
*/

void
kgraphStoreUpdt (
Kgraph * const              grafptr,
const KgraphStore * const   storptr)
{
#ifdef SCOTCH_DEBUG_KGRAPH2
  if (grafptr->m.domnmax < storptr->domnmax) {
    errorPrint ("kgraphStoreUpdt: cannot update state");
    return;
  }
#endif /* SCOTCH_DEBUG_KGRAPH2 */

  grafptr->m.domnnbr = storptr->domnnbr;
  grafptr->fronnbr   = storptr->fronnbr;
  grafptr->commload  = storptr->commload;
  grafptr->kbalval   = storptr->kbalval;

  if (grafptr->m.domnnbr > 0) {                   /* If valid mapping */
    memCpy (grafptr->m.parttax + grafptr->s.baseval, storptr->parttab,     grafptr->s.vertnbr * sizeof (Anum));
    memCpy (grafptr->m.domntab,                      storptr->domntab,     grafptr->m.domnnbr * sizeof (ArchDom));
    memCpy (grafptr->comploadavg,                    storptr->comploadavg, grafptr->m.domnnbr * sizeof (Gnum));
    memCpy (grafptr->comploaddlt,                    storptr->comploaddlt, grafptr->m.domnnbr * sizeof (Gnum));
    memCpy (grafptr->frontab,                        storptr->frontab,     grafptr->fronnbr   * sizeof (Gnum));

#ifdef SCOTCH_DEBUG_KGRAPH2
    if (kgraphCheck (grafptr) != 0)
      errorPrint ("kgraphStoreUpdt: inconsistent graph data");
#endif /* SCOTCH_DEBUG_KGRAPH2 */
  }
}
