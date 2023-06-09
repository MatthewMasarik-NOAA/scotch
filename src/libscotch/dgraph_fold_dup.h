/* Copyright 2007,2010,2014,2018,2021,2023 IPB, Universite de Bordeaux, INRIA & CNRS
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
/**   NAME       : dgraph_fold_dup.h                       **/
/**                                                        **/
/**   AUTHOR     : Francois PELLEGRINI                     **/
/**                                                        **/
/**   FUNCTION   : These lines are the data declaration    **/
/**                for the distributed graph duplicate     **/
/**                folding routine.                        **/
/**                                                        **/
/**   DATES      : # Version 5.0  : from : 13 aug 2006     **/
/**                                 to   : 13 aug 2006     **/
/**                # Version 5.1  : from : 04 nov 2010     **/
/**                                 to   : 04 nov 2010     **/
/**                # Version 6.0  : from : 28 sep 2014     **/
/**                                 to   : 07 jun 2018     **/
/**                # Version 7.0  : from : 26 sep 2021     **/
/**                                 to   : 17 jan 2023     **/
/**                                                        **/
/************************************************************/

/*
**  The type and structure definitions.
*/

/*+ This structure holds the splitting parameters. +*/

typedef struct DgraphFoldDupSplit2_ {
  const Dgraph *            orggrafptr;           /*+ Pointer to original graph               +*/
  MPI_Comm                  fldproccomm;          /*+ Communicator to be used in folded graph +*/
} DgraphFoldDupSplit2;

typedef struct DgraphFoldDupSplit_ {
  DgraphFoldDupSplit2       splttab[2];           /*+ Array of folded graph data                    +*/
  void *                    orgdataptr;           /*+ Data associated to vertices, e.g. coarmulttab +*/
  Dgraph *                  fldgrafptr;           /*+ Pointer to folded graph                       +*/
  void *                    flddataptr;           /*+ Data associated to vertices, e.g. coarmulttab +*/
  MPI_Datatype              datatype;             /*+ MPI type of associated information            +*/
  int *                     revaptr;              /*+ Pointer to return value                       +*/
} DgraphFoldDupSplit;

/*
**  The function prototypes.
*/

#ifdef SCOTCH_DGRAPH_FOLD_DUP
#ifdef SCOTCH_PTHREAD_MPI
static void                 dgraphFoldDup2      (Context * restrict const, const int, const DgraphFoldDupSplit * const);
#endif /* SCOTCH_PTHREAD_MPI */
#endif /* SCOTCH_DGRAPH_FOLD_DUP */
