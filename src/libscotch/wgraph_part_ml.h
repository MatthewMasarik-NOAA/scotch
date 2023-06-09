/* Copyright 2007-2011,2018,2023 IPB, Universite de Bordeaux, INRIA & CNRS
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
/**   NAME       : wgraph_part_ml.h                        **/
/**                                                        **/
/**   AUTHOR     : Jun-Ho HER (v6.0)                       **/
/**                Charles-Edmond BICHOT (v5.1b)           **/
/**                                                        **/
/**   FUNCTION   : These lines are the data declarations   **/
/**                multilevel framework for the vertex     **/
/**                overlapped graph partitioning.          **/
/**                                                        **/
/**   DATES      : # Version 5.1  : from : 01 dec 2007     **/
/**                                 to   : 01 jul 2008     **/
/**                # Version 6.0  : from : 05 nov 2009     **/
/**                                 to   : 31 may 2018     **/
/**                # Version 7.0  : from : 17 jan 2023     **/
/**                                 to   : 17 jan 2023     **/
/**                                                        **/
/************************************************************/

/*
**  The type and structure definitions.
*/

/*+ This structure holds the method parameters. +*/

typedef struct WgraphPartMlParam_ {
  INT                       coarnbr;              /*+ Minimum number of vertices per part +*/
  double                    coarval;              /*+ Coarsening ratio                    +*/
  Strat *                   stratlow;             /*+ Strategy at lowest level            +*/
  Strat *                   stratasc;             /*+ Strategy at ascending levels        +*/
} WgraphPartMlParam;

/*
**  The function prototypes.
*/

#ifdef SCOTCH_WGRAPH_PART_ML
static int                  wgraphPartMlCoarsen (const Wgraph * const, Wgraph * const, GraphCoarsenMulti * restrict * const, const WgraphPartMlParam * const);
static int                  wgraphPartMlUncoarsen (Wgraph * const, const Wgraph * const, const GraphCoarsenMulti * restrict const);
static int                  wgraphPartMl2       (Wgraph * const, const WgraphPartMlParam * const);
#endif /* SCOTCH_WGRAPH_PART_ML */

int                         wgraphPartMl        (Wgraph * const, const WgraphPartMlParam * const);
