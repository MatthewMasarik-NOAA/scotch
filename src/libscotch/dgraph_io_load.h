/* Copyright 2007,2018,2023 IPB, Universite de Bordeaux, INRIA & CNRS
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
/**   NAME       : dgraph_io_load.h                        **/
/**                                                        **/
/**   AUTHOR     : Francois PELLEGRINI                     **/
/**                                                        **/
/**   FUNCTION   : These lines are the data declaration    **/
/**                for the distributed graph loading       **/
/**                routines.                               **/
/**                                                        **/
/**   DATES      : # Version 5.0  : from : 12 may 2007     **/
/**                                 to   : 13 may 2007     **/
/**                # Version 6.0  : from : 07 jun 2018     **/
/**                                 to   : 07 jun 2018     **/
/**                # Version 7.0  : from : 17 jan 2023     **/
/**                                 to   : 17 jan 2023     **/
/**                                                        **/
/************************************************************/

/*
**  The function prototypes.
*/

#ifdef SCOTCH_DGRAPH_IO_LOAD
static int                  dgraphLoadCent      (Dgraph * restrict const, FILE * const, Gnum, const GraphFlag, const int);
static int                  dgraphLoadDist      (Dgraph * restrict const, FILE * const, Gnum, const GraphFlag);
static int                  dgraphLoadMulti     (Dgraph * restrict const, FILE * const, Gnum, const GraphFlag);
#endif /* SCOTCH_DGRAPH_IO_LOAD */
