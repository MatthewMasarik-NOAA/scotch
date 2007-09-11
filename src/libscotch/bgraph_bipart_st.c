/* Copyright 2004,2007 ENSEIRB, INRIA & CNRS
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
/**   NAME       : bgraph_bipart_st.c                      **/
/**                                                        **/
/**   AUTHOR     : Francois PELLEGRINI                     **/
/**                                                        **/
/**   FUNCTION   : This module contains the strategy and   **/
/**                method tables for graph bipartitioning  **/
/**                methods.                                **/
/**                                                        **/
/**   DATES      : # Version 3.2  : from : 08 oct 1996     **/
/**                                 to     13 sep 1998     **/
/**                # Version 3.3  : from : 01 oct 1998     **/
/**                                 to     12 mar 1999     **/
/**                # Version 3.4  : from : 01 jun 2001     **/
/**                                 to     01 jun 2001     **/
/**                # Version 4.0  : from : 12 jan 2004     **/
/**                                 to     20 aug 2004     **/
/**                # Version 5.0  : from : 27 nov 2006     **/
/**                                 to     29 may 2007     **/
/**                                                        **/
/************************************************************/

/*
**  The defines and includes.
*/

#define BGRAPH_BIPART_ST

#include "module.h"
#include "common.h"
#include "gain.h"
#include "parser.h"
#include "graph.h"
#include "graph_coarsen.h"
#include "arch.h"
#include "bgraph.h"
#include "bgraph_bipart_bd.h"
#include "bgraph_bipart_df.h"
#include "bgraph_bipart_ex.h"
#include "bgraph_bipart_fm.h"
#include "bgraph_bipart_gg.h"
#include "bgraph_bipart_gp.h"
#include "bgraph_bipart_ml.h"
#include "bgraph_bipart_zr.h"
#include "bgraph_bipart_st.h"

/*
**  The static and global variables.
*/

static Bgraph               bgraphdummy;      /*+ Dummy active graph for offset computations +*/

static union {
  BgraphBipartBdParam       param;
  StratNodeMethodData       padding;
} bgraphbipartdefaultbd = { { 3, &stratdummy } };

static union {
  BgraphBipartDfParam       param;
  StratNodeMethodData       padding;
} bgraphbipartdefaultdf = { { 40, 1.0F, 1.0F } };

static union {                                /* Default parameters for bipartitioning methods */
  BgraphBipartFmParam       param;            /* Parameter zone                                */
  StratNodeMethodData       padding;          /* To avoid reading out of structure             */
} bgraphbipartdefaultfm = { { 80, ~0, 0.01L } };

static union {
  BgraphBipartGgParam       param;
  StratNodeMethodData       padding;
} bgraphbipartdefaultgg = { { 5 } };

static union {
  BgraphBipartGpParam       param;
  StratNodeMethodData       padding;
} bgraphbipartdefaultgp = { { 5 } };

static union {
  BgraphBipartMlParam       param;
  StratNodeMethodData       padding;
} bgraphbipartdefaultml = { { 100, 0.8L, GRAPHCOARHEM, &stratdummy, &stratdummy } };

static StratMethodTab       bgraphbipartstmethtab[] = { /* Bipartitioning methods array */
                              { BGRAPHBIPARTMETHBD, "b",  bgraphBipartBd, &bgraphbipartdefaultbd },
                              { BGRAPHBIPARTMETHDF, "d",  bgraphBipartDf, &bgraphbipartdefaultdf },
                              { BGRAPHBIPARTMETHEX, "x",  bgraphBipartEx, NULL },
                              { BGRAPHBIPARTMETHFM, "f",  bgraphBipartFm, &bgraphbipartdefaultfm },
                              { BGRAPHBIPARTMETHGG, "h",  bgraphBipartGg, &bgraphbipartdefaultgg },
                              { BGRAPHBIPARTMETHGP, "g",  bgraphBipartGp, &bgraphbipartdefaultgp },
                              { BGRAPHBIPARTMETHML, "m",  bgraphBipartMl, &bgraphbipartdefaultml },
                              { BGRAPHBIPARTMETHZR, "z",  bgraphBipartZr, NULL },
                              { -1,                 NULL, NULL,           NULL } };

static StratParamTab        bgraphbipartstparatab[] = { /* Method parameter list */
                              { BGRAPHBIPARTMETHBD,  STRATPARAMSTRAT,  "bnd",
                                (byte *) &bgraphbipartdefaultbd.param,
                                (byte *) &bgraphbipartdefaultbd.param.stratbnd,
                                (void *) &bgraphbipartststratab },
                              { BGRAPHBIPARTMETHBD,  STRATPARAMSTRAT,  "org",
                                (byte *) &bgraphbipartdefaultbd.param,
                                (byte *) &bgraphbipartdefaultbd.param.stratorg,
                                (void *) &bgraphbipartststratab },
                              { BGRAPHBIPARTMETHBD,  STRATPARAMINT,    "width",
                                (byte *) &bgraphbipartdefaultbd.param,
                                (byte *) &bgraphbipartdefaultbd.param.distmax,
                                NULL },
                              { BGRAPHBIPARTMETHDF,  STRATPARAMINT,    "pass",
                                (byte *) &bgraphbipartdefaultdf.param,
                                (byte *) &bgraphbipartdefaultdf.param.passnbr,
                                NULL },
                              { BGRAPHBIPARTMETHDF,  STRATPARAMDOUBLE, "dif",
                                (byte *) &bgraphbipartdefaultdf.param,
                                (byte *) &bgraphbipartdefaultdf.param.cdifval,
                                NULL },
                              { BGRAPHBIPARTMETHDF,  STRATPARAMDOUBLE, "rem",
                                (byte *) &bgraphbipartdefaultdf.param,
                                (byte *) &bgraphbipartdefaultdf.param.cremval,
                                NULL },
                              { BGRAPHBIPARTMETHFM,  STRATPARAMINT,    "move",
                                (byte *) &bgraphbipartdefaultfm.param,
                                (byte *) &bgraphbipartdefaultfm.param.movenbr,
                                NULL },
                              { BGRAPHBIPARTMETHFM,  STRATPARAMINT,    "pass",
                                (byte *) &bgraphbipartdefaultfm.param,
                                (byte *) &bgraphbipartdefaultfm.param.passnbr,
                                NULL },
                              { BGRAPHBIPARTMETHFM,  STRATPARAMDOUBLE, "bal",
                                (byte *) &bgraphbipartdefaultfm.param,
                                (byte *) &bgraphbipartdefaultfm.param.deltrat,
                                NULL },
                              { BGRAPHBIPARTMETHGG,  STRATPARAMINT,    "pass",
                                (byte *) &bgraphbipartdefaultgg.param,
                                (byte *) &bgraphbipartdefaultgg.param.passnbr,
                                NULL },
                              { BGRAPHBIPARTMETHML,  STRATPARAMSTRAT,  "asc",
                                (byte *) &bgraphbipartdefaultml.param,
                                (byte *) &bgraphbipartdefaultml.param.stratasc,
                                (void *) &bgraphbipartststratab },
                              { BGRAPHBIPARTMETHML,  STRATPARAMSTRAT,  "low",
                                (byte *) &bgraphbipartdefaultml.param,
                                (byte *) &bgraphbipartdefaultml.param.stratlow,
                                (void *) &bgraphbipartststratab },
                              { BGRAPHBIPARTMETHML,  STRATPARAMCASE,   "type",
                                (byte *) &bgraphbipartdefaultml.param,
                                (byte *) &bgraphbipartdefaultml.param.matchtype,
                                (void *) "hscd" },
                              { BGRAPHBIPARTMETHML,  STRATPARAMINT,    "vert",
                                (byte *) &bgraphbipartdefaultml.param,
                                (byte *) &bgraphbipartdefaultml.param.coarnbr,
                                NULL },
                              { BGRAPHBIPARTMETHML,  STRATPARAMDOUBLE, "rat",
                                (byte *) &bgraphbipartdefaultml.param,
                                (byte *) &bgraphbipartdefaultml.param.coarrat,
                                NULL },
                              { BGRAPHBIPARTMETHNBR, STRATPARAMINT,    NULL,
                                NULL, NULL, NULL } };

static StratParamTab        bgraphbipartstcondtab[] = { /* Active graph condition parameter table */
                              { STRATNODECOND,       STRATPARAMINT,    "load",
                                (byte *) &bgraphdummy,
                                (byte *) &bgraphdummy.s.velosum,
                                NULL },
                              { STRATNODECOND,       STRATPARAMINT,    "load0",
                                (byte *) &bgraphdummy,
                                (byte *) &bgraphdummy.compload0,
                                NULL },
                              { STRATNODECOND,       STRATPARAMINT,    "edge",
                                (byte *) &bgraphdummy,
                                (byte *) &bgraphdummy.s.edgenbr,
                                NULL },
                              { STRATNODECOND,       STRATPARAMINT,    "vert",
                                (byte *) &bgraphdummy,
                                (byte *) &bgraphdummy.s.vertnbr,
                                NULL },
                              { STRATNODECOND,       STRATPARAMDOUBLE, "levl",
                                (byte *) &bgraphdummy,
                                (byte *) &bgraphdummy.levlnum,
                                NULL },
                              { STRATNODENBR,        STRATPARAMINT,    NULL,
                                NULL, NULL, NULL } };

StratTab                    bgraphbipartststratab = { /* Strategy tables for graph bipartitioning methods */
                              bgraphbipartstmethtab,
                              bgraphbipartstparatab,
                              bgraphbipartstcondtab };

/***********************************************/
/*                                             */
/* This is the generic bipartitioning routine. */
/*                                             */
/***********************************************/

/* This routine performs the bipartitioning of
** the given active graph according to the
** given strategy.
** It returns:
** - 0 : if bipartitioning could be computed.
** - 1 : on error.
*/

int
bgraphBipartSt (
Bgraph * restrict const       grafptr,            /*+ Active graph to bipartition +*/
const Strat * restrict const  strat)              /*+ Bipartitioning strategy     +*/
{
  StratTest           val;                        /* Result of condition evaluation */
  BgraphStore         savetab[2];                 /* Results of the two strategies  */
  int                 o;
  int                 o2;

#ifdef SCOTCH_DEBUG_BGRAPH2
  if (sizeof (Gnum) != sizeof (INT)) {
    errorPrint ("bgraphBipartSt: invalid type specification for parser variables");
    return     (1);
  }
  if ((sizeof (BgraphBipartFmParam) > sizeof (StratNodeMethodData)) ||
      (sizeof (BgraphBipartGgParam) > sizeof (StratNodeMethodData)) ||
      (sizeof (BgraphBipartMlParam) > sizeof (StratNodeMethodData))) {
    errorPrint ("bgraphBipartSt: invalid type specification");
    return     (1);
  }
#endif /* SCOTCH_DEBUG_BGRAPH2 */
#ifdef SCOTCH_DEBUG_BGRAPH1
  if (strat->tabl != &bgraphbipartststratab) {
    errorPrint ("bgraphBipartSt: invalid parameter (1)");
    return     (1);
  }
#endif /* SCOTCH_DEBUG_BGRAPH1 */

  o = 0;
  switch (strat->type) {
    case STRATNODECONCAT :
      o = bgraphBipartSt (grafptr, strat->data.concat.strat[0]); /* Apply the first strategy      */
      if (o == 0)                                 /* If it worked all right                         */
        o |= bgraphBipartSt (grafptr, strat->data.concat.strat[1]); /* Then apply second strategy */
      break;
    case STRATNODECOND :
      o = stratTestEval (strat->data.cond.test, &val, (void *) grafptr); /* Evaluate expression */
      if (o == 0) {                               /* If evaluation was correct                              */
#ifdef SCOTCH_DEBUG_VGRAPH2
        if ((val.typetest != STRATTESTVAL) ||
            (val.typenode != STRATPARAMLOG)) {
          errorPrint ("bgraphBipartSt: invalid test result");
          o = 1;
          break;
        }
#endif /* SCOTCH_DEBUG_VGRAPH2 */
        if (val.data.val.vallog == 1)             /* If expression is true                    */
          o = bgraphBipartSt (grafptr, strat->data.cond.strat[0]); /* Apply first strategy    */
        else {                                    /* Else if expression is false              */
          if (strat->data.cond.strat[1] != NULL)  /* And if there is an else statement        */
            o = bgraphBipartSt (grafptr, strat->data.cond.strat[1]); /* Apply second strategy */
        }
      }
      break;
    case STRATNODEEMPTY :
      break;
    case STRATNODESELECT :
      if (((bgraphStoreInit (grafptr, &savetab[0])) != 0) || /* Allocate save areas */
          ((bgraphStoreInit (grafptr, &savetab[1])) != 0)) {
        errorPrint ("bgraphBipartSt: out of memory");
        bgraphStoreExit (&savetab[0]);
        return          (1);
      }

      bgraphStoreSave     (grafptr, &savetab[1]); /* Save initial bipartition              */
      o = bgraphBipartSt  (grafptr, strat->data.select.strat[0]); /* Apply first strategy  */
      bgraphStoreSave     (grafptr, &savetab[0]); /* Save its result                       */
      bgraphStoreUpdt     (grafptr, &savetab[1]); /* Restore initial bipartition           */
      o2 = bgraphBipartSt (grafptr, strat->data.select.strat[1]); /* Apply second strategy */

      if ((o == 0) || (o2 == 0)) {                /* If at least one method did bipartition */
        if ( (savetab[0].commload <  grafptr->commload) || /* If first strategy is better   */
            ((savetab[0].commload == grafptr->commload) &&
             (abs (savetab[0].compload0dlt) < abs (grafptr->compload0dlt))))
          bgraphStoreUpdt (grafptr, &savetab[0]); /* Restore its result */
      }
      if (o2 < o)                                 /* o = min(o,o2): if one biparts, then bipart */
        o = o2;                                   /* Else if one stops, then stop, else error   */

      bgraphStoreExit (&savetab[0]);              /* Free both save areas */
      bgraphStoreExit (&savetab[1]);
      break;
#ifdef SCOTCH_DEBUG_BGRAPH2
    case STRATNODEMETHOD :
#else /* SCOTCH_DEBUG_BGRAPH2 */
    default :
#endif /* SCOTCH_DEBUG_BGRAPH2 */
      return (strat->tabl->methtab[strat->data.method.meth].func (grafptr, (void *) &strat->data.method.data));
#ifdef SCOTCH_DEBUG_BGRAPH2
    default :
      errorPrint ("bgraphBipartSt: invalid parameter (2)");
      return     (1);
#endif /* SCOTCH_DEBUG_BGRAPH2 */
  }
  return (o);
}
