/* Copyright 2007-2009,2014,2018,2023 IPB, Universite de Bordeaux, INRIA & CNRS
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
/**   NAME       : vdgraph_separate_ml.h                   **/
/**                                                        **/
/**   AUTHOR     : Francois PELLEGRINI                     **/
/**                Cedric CHEVALIER (v5.0)                 **/
/**                                                        **/
/**   FUNCTION   : These lines are the data declaration    **/
/**                for the sequential vertex separation    **/
/**                routine for distributed graphs.         **/
/**                                                        **/
/**   DATES      : # Version 5.0  : from : 07 feb 2006     **/
/**                                 to   : 03 aug 2007     **/
/**                # Version 5.1  : from : 14 dec 2008     **/
/**                                 to   : 29 may 2009     **/
/**                # Version 6.0  : from : 28 sep 2014     **/
/**                                 to   : 06 jun 2018     **/
/**                # Version 7.0  : from : 20 jan 2023     **/
/**                                 to   : 20 jan 2023     **/
/**                                                        **/
/************************************************************/

/*
**  The type and structure definitions.
*/

/*+ This structure holds the method parameters. +*/

typedef struct VdgraphSeparateMlParam_ {
  INT                       passnbr;              /*+ Number of coarsening passes to go                      +*/
  INT                       foldmax;              /*+ Maximum number of vertices per processor to do folding +*/
  int                       foldval;              /*+ Type of folding                                        +*/
  INT                       coarnbr;              /*+ Minimum number of vertices                             +*/
  double                    coarrat;              /*+ Coarsening ratio                                       +*/
  Strat *                   stratlow;             /*+ Strategy at lowest level                               +*/
  Strat *                   stratasc;             /*+ Strategy at ascending levels                           +*/
  Strat *                   stratseq;             /*+ Strategy when running on a single processor            +*/
} VdgraphSeparateMlParam;

/*
**  The function prototypes.
*/

#ifdef SCOTCH_VDGRAPH_SEPARATE_ML
static int                  vdgraphSeparateMlCoarsen (Vdgraph * const, Vdgraph * const, DgraphCoarsenMulti * restrict * const, const VdgraphSeparateMlParam * const);
static int                  vdgraphSeparateMlUncoarsen (Vdgraph *, const Vdgraph * const, const DgraphCoarsenMulti * restrict const);
static void                 vdgraphSeparateMlOpBest (const Gnum * const, Gnum * const, const int * const, const MPI_Datatype * const);
static int                  vdgraphSeparateMl2  (Vdgraph * const, const VdgraphSeparateMlParam * const);
#endif /* SCOTCH_VDGRAPH_SEPARATE_ML */

int                         vdgraphSeparateMl   (Vdgraph * const, const VdgraphSeparateMlParam * const);
