/* Copyright 2004,2007,2008 ENSEIRB, INRIA & CNRS
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
/**   NAME       : bgraph_bipart_ml.c                      **/
/**                                                        **/
/**   AUTHOR     : Francois PELLEGRINI                     **/
/**                Luca SCARANO (v3.1)                     **/
/**                                                        **/
/**   FUNCTION   : This module bipartitions an active      **/
/**                graph using a multi-level scheme.       **/
/**                                                        **/
/**   DATES      : # Version 3.1  : from : 24 oct 1995     **/
/**                                 to     19 sep 1996     **/
/**                # Version 3.2  : from : 20 sep 1996     **/
/**                                 to     13 sep 1998     **/
/**                # Version 3.3  : from : 01 oct 1998     **/
/**                                 to     12 mar 1999     **/
/**                # Version 3.4  : from : 01 jun 2001     **/
/**                                 to     01 jun 2001     **/
/**                # Version 4.0  : from : 12 dec 2003     **/
/**                                 to     20 mar 2005     **/
/**                # Version 5.1  : from : 28 sep 2008     **/
/**                                 to     28 sep 2008     **/
/**                                                        **/
/************************************************************/

/*
**  The defines and includes.
*/

#define BGRAPH_BIPART_ML

#include "module.h"
#include "common.h"
#include "parser.h"
#include "graph.h"
#include "graph_coarsen.h"
#include "arch.h"
#include "bgraph.h"
#include "bgraph_bipart_ml.h"
#include "bgraph_bipart_st.h"

/*********************************************/
/*                                           */
/* The coarsening and uncoarsening routines. */
/*                                           */
/*********************************************/

/* This routine builds a coarser graph from the
** graph that is given on input. The coarser
** graphs differ at this stage from classical
** active graphs as their internal gains are not
** yet computed.
** It returns:
** - 0  : if the coarse graph has been built.
** - 1  : if threshold reached or on error.
*/

static
int
bgraphBipartMlCoarsen (
const Bgraph * const                  finegrafptr, /*+ Finer graph                         +*/
Bgraph * restrict const               coargrafptr, /*+ Coarser graph to build              +*/
GraphCoarsenMulti * restrict * const  coarmultptr, /*+ Pointer to multinode table to build +*/
const BgraphBipartMlParam * const     paraptr)    /*+ Method parameters                    +*/
{
  if (graphCoarsen (&finegrafptr->s, &coargrafptr->s, coarmultptr, paraptr->coarnbr, paraptr->coarrat, paraptr->matchtype) != 0)
    return (1);                                   /* Return if coarsening failed */

  if (finegrafptr->veextax != NULL) {             /* Merge external gains for coarsened vertices */
    GraphCoarsenMulti * restrict  coarmulttax;
    Gnum * restrict               coarveextax;
    Gnum                          coarvertnum;

    if ((coarveextax = (Gnum *) memAlloc (coargrafptr->s.vertnbr * sizeof (Gnum))) == NULL) {
      errorPrint ("bgraphBipartMlCoarsen: out of memory");
      graphExit  (&coargrafptr->s);               /* Only free Graph since veextab not allocated */
      return     (1);
    }
    coarveextax -= coargrafptr->s.baseval;
    coarmulttax  = *coarmultptr;
    coargrafptr->s.flagval |= BGRAPHFREEVEEX;
    coargrafptr->veextax    = coarveextax;

    for (coarvertnum = coargrafptr->s.baseval; coarvertnum < coargrafptr->s.vertnnd; coarvertnum ++) {
      Gnum                finevertnum0;           /* First multinode vertex  */
      Gnum                finevertnum1;           /* Second multinode vertex */

      finevertnum0 = coarmulttax[coarvertnum].vertnum[0];
      finevertnum1 = coarmulttax[coarvertnum].vertnum[1];
      coarveextax[coarvertnum] = (finevertnum0 != finevertnum1)
                                 ? finegrafptr->veextax[finevertnum0] + finegrafptr->veextax[finevertnum1]
                                 : finegrafptr->veextax[finevertnum0];
    }
  }
  else                                            /* If fine graph does not have external gains */
    coargrafptr->veextax = NULL;                  /* Coarse graph does not have external gains  */

  coargrafptr->s.flagval |= BGRAPHFREEPART;       /* Only part array will have to be freed, as frontier is shared */
  coargrafptr->parttax    = NULL;                 /* Do not allocate partition data yet                           */
  coargrafptr->frontab    = finegrafptr->frontab; /* Use frontier array of finer graph as coarse frontier array   */

  coargrafptr->compload0avg  = finegrafptr->compload0avg; /* Only set constant partition parameters as others will be set on uncoarsening */
  coargrafptr->commloadextn0 = finegrafptr->commloadextn0;
  coargrafptr->commgainextn0 = finegrafptr->commgainextn0;
  coargrafptr->domdist       = finegrafptr->domdist;
  coargrafptr->domwght[0]    = finegrafptr->domwght[0];
  coargrafptr->domwght[1]    = finegrafptr->domwght[1];
  coargrafptr->levlnum       = finegrafptr->levlnum + 1;

  return (0);
}

/* This routine propagates the bipartition of the
** coarser graph back to the finer graph, according
** to the multinode table of collapsed vertices.
** After the bipartition is propagated, it finishes
** to compute the parameters of the finer graph that
** were not computed at the coarsening stage.
** It returns:
** - 0   : if coarse graph data has been propagated to fine graph.
** - !0  : on error.
*/

static
int
bgraphBipartMlUncoarsen (
Bgraph * restrict const         finegrafptr,      /*+ Finer graph                +*/
const Bgraph * const            coargrafptr,      /*+ Coarser graph              +*/
const GraphCoarsenMulti * const coarmulttax)      /*+ Pointer to multinode array +*/
{
  if (finegrafptr->parttax == NULL) {             /* If partition array not yet allocated */
    if ((finegrafptr->parttax = (GraphPart *) memAlloc (finegrafptr->s.vertnbr * sizeof (GraphPart))) == NULL) {
      errorPrint ("bgraphBipartMlUncoarsen: out of memory");
      return     (1);                             /* Allocated data will be freed along with graph structure */
    }
    finegrafptr->parttax -= finegrafptr->s.baseval;
  }

  if (coargrafptr != NULL) {                      /* If coarser graph provided */
    Gnum                coarvertnum;
    Gnum                coarfronnum;
    Gnum                finefronnum;
    Gnum                finecompsize1;

    finecompsize1 = coargrafptr->s.vertnbr - coargrafptr->compsize0; /* Pre-allocate sizes */

    for (coarvertnum = coargrafptr->s.baseval; coarvertnum < coargrafptr->s.vertnnd; coarvertnum ++) {
      Gnum                finevertnum0;           /* First multinode vertex  */
      Gnum                finevertnum1;           /* Second multinode vertex */
      GraphPart           partval;

      finevertnum0 = coarmulttax[coarvertnum].vertnum[0];
      finevertnum1 = coarmulttax[coarvertnum].vertnum[1];
      partval      = coargrafptr->parttax[coarvertnum];

      finegrafptr->parttax[finevertnum0] = partval;
      if (finevertnum0 != finevertnum1) {
        finegrafptr->parttax[finevertnum1] = partval;
        finecompsize1 += partval;                 /* Account for extra vertices created in part 1 */
      }
    }

    finegrafptr->compload0    = coargrafptr->compload0;
    finegrafptr->compload0dlt = coargrafptr->compload0dlt;
    finegrafptr->compsize0    = finegrafptr->s.vertnbr - finecompsize1;
    finegrafptr->commload     = coargrafptr->commload;
    finegrafptr->commgainextn = coargrafptr->commgainextn;

    for (coarfronnum = 0, finefronnum = coargrafptr->fronnbr; /* Re-cycle frontier array from coarse to fine graph */
         coarfronnum < coargrafptr->fronnbr; coarfronnum ++) {
      Gnum                coarvertnum;
      Gnum                finevertnum0;           /* First multinode vertex  */
      Gnum                finevertnum1;           /* Second multinode vertex */

      coarvertnum  = coargrafptr->frontab[coarfronnum];
      finevertnum0 = coarmulttax[coarvertnum].vertnum[0];
      finevertnum1 = coarmulttax[coarvertnum].vertnum[1];
      
      if (finevertnum0 != finevertnum1) {         /* If multinode si made of two distinct vertices */
        Gnum                coarpartval;
        Gnum                fineedgenum;

        coarpartval = coargrafptr->parttax[coarvertnum];

#ifdef SCOTCH_DEBUG_BGRAPH2
        coargrafptr->frontab[coarfronnum] = ~0;
#endif /* SCOTCH_DEBUG_BGRAPH2 */

        for (fineedgenum = finegrafptr->s.verttax[finevertnum0];
             fineedgenum < finegrafptr->s.vendtax[finevertnum0]; fineedgenum ++) {
          if (finegrafptr->parttax[finegrafptr->s.edgetax[fineedgenum]] != coarpartval) { /* If first vertex belongs to frontier */
            coargrafptr->frontab[coarfronnum] = finevertnum0; /* Record it in lieu of the coarse frontier vertex                 */
            break;
	  }
        }
        if (fineedgenum >= finegrafptr->s.vendtax[finevertnum0]) { /* If first vertex not in frontier */
          coargrafptr->frontab[coarfronnum] = finevertnum1; /* Then second vertex must be in frontier */
          continue;                               /* Skip to next multinode                           */
        }

        for (fineedgenum = finegrafptr->s.verttax[finevertnum1]; /* Check if second vertex belong to frontier too */
             fineedgenum < finegrafptr->s.vendtax[finevertnum1]; fineedgenum ++) {
          if (finegrafptr->parttax[finegrafptr->s.edgetax[fineedgenum]] != coarpartval) { /* If second vertex belongs to frontier */
            coargrafptr->frontab[finefronnum ++] = finevertnum1; /* Record it at the end of the recycled frontier array           */
            break;
          }
        }

#ifdef SCOTCH_DEBUG_BGRAPH2
        if (coargrafptr->frontab[coarfronnum] == ~0) {
          errorPrint ("bgraphBipartMlUncoarsen: internal error");
          return     (1);
        }
#endif /* SCOTCH_DEBUG_BGRAPH2 */
      }
      else                                        /* If coarse vertex is single node         */
        coargrafptr->frontab[coarfronnum] = finevertnum0; /* Then it belongs to the frontier */
    }
    finegrafptr->fronnbr = finefronnum;

#ifdef SCOTCH_DEBUG_BGRAPH2
    if (bgraphCheck (finegrafptr) != 0) {
      errorPrint ("bgraphBipartMlUncoarsen: inconsistent graph data");
      return     (1);
    }
#endif /* SCOTCH_DEBUG_BGRAPH2 */
  }
  else                                            /* No coarse graph provided      */
    bgraphZero (finegrafptr);                     /* Assign all vertices to part 0 */

  return (0);
}

/* This routine recursively performs the
** bipartitioning recursion.
** It returns:
** - 0 : if bipartitioning could be computed.
** - 1 : on error.
*/

static
int
bgraphBipartMl2 (
Bgraph * restrict const           grafptr,        /*+ Active graph      +*/
const BgraphBipartMlParam * const paraptr)        /*+ Method parameters +*/
{
  Bgraph              coargrafdat;
  GraphCoarsenMulti * coarmultptr;
  int                 o;

  if (bgraphBipartMlCoarsen (grafptr, &coargrafdat, &coarmultptr, paraptr) == 0) {
    if (((o = bgraphBipartMl2         (&coargrafdat, paraptr))              == 0) &&
        ((o = bgraphBipartMlUncoarsen (grafptr, &coargrafdat, coarmultptr)) == 0) &&
        ((o = bgraphBipartSt          (grafptr, paraptr->stratasc))         != 0)) /* Apply ascending strategy */
      errorPrint ("bgraphBipartMl2: cannot apply ascending strategy");
    bgraphExit (&coargrafdat);
  }
  else {
    if (((o = bgraphBipartMlUncoarsen (grafptr, NULL, NULL))        == 0) && /* Finalize graph   */
        ((o = bgraphBipartSt          (grafptr, paraptr->stratlow)) != 0)) /* Apply low strategy */
      errorPrint ("bgraphBipartMl2: cannot apply low strategy");
  }

  return (o);
}

/*****************************/
/*                           */
/* This is the main routine. */
/*                           */
/*****************************/

/* This routine performs the multi-level bipartitioning.
** It returns:
** - 0 : if bipartitioning could be computed.
** - 1 : on error.
*/

int
bgraphBipartMl (
Bgraph * restrict const           grafptr,        /*+ Active graph      +*/
const BgraphBipartMlParam * const paraptr)        /*+ Method parameters +*/
{
  Gnum                levlnum;                    /* Save value for graph level */
  int                 o;

  levlnum = grafptr->levlnum;                     /* Save graph level                   */
  grafptr->levlnum = 0;                           /* Initialize coarsening level        */
  o = bgraphBipartMl2 (grafptr, paraptr);         /* Perform multi-level bipartitioning */
  grafptr->levlnum = levlnum;                     /* Restore graph level                */

  return (o);
}
