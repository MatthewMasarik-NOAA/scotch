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
/**   NAME       : library.h                               **/
/**                                                        **/
/**   AUTHOR     : Francois PELLEGRINI                     **/
/**                                                        **/
/**   FUNCTION   : Declaration file for the LibScotch      **/
/**                static mapping and sparse matrix block  **/
/**                ordering sequential library.            **/
/**                                                        **/
/**   DATES      : # Version 3.2  : from : 07 sep 1996     **/
/**                                 to     22 aug 1998     **/
/**                # Version 3.3  : from : 02 oct 1998     **/
/**                                 to     31 may 1999     **/
/**                # Version 3.4  : from : 10 oct 1999     **/
/**                                 to     15 nov 2001     **/
/**                # Version 4.0  : from : 11 dec 2001     **/
/**                                 to     20 dec 2005     **/
/**                # Version 5.0  : from : 26 apr 2006     **/
/**                                 to   : 05 aug 2007     **/
/**                                                        **/
/************************************************************/

/*
**  The type and structure definitions.
*/

/*+ Parallel processing flag. +*/

#ifndef SCOTCH_PTSCOTCH
#define SCOTCH_DUMMYPTFLAG
#endif /* SCOTCH_PTSCOTCH */

/*+ Integer type. +*/

typedef DUMMYINT SCOTCH_Num;

#define SCOTCH_NUMMAX               DUMMYMAXINT

/*+ Opaque objects. The dummy sizes of these objects,
computed at compile-time by program "dummysizes",
are given as double values for proper padding +*/

typedef struct {
  double                    dummy[DUMMYSIZEARCH];
} SCOTCH_Arch;

#ifdef SCOTCH_PTSCOTCH
typedef struct {
  double                    dummy[DUMMYSIZEDGRAPH];
} SCOTCH_Dgraph;

typedef struct {
  double                    dummy[DUMMYSIZEDORDER];
} SCOTCH_Dordering;
#endif /* SCOTCH_PTSCOTCH */

typedef struct {
  double                    dummy[DUMMYSIZEGEOM];
} SCOTCH_Geom;

typedef struct {
  double                    dummy[DUMMYSIZEGRAPH];
} SCOTCH_Graph;

typedef struct {
  double                    dummy[DUMMYSIZEMESH];
} SCOTCH_Mesh;

typedef struct {
  double                    dummy[DUMMYSIZEMAP];
} SCOTCH_Mapping;

typedef struct {
  double                    dummy[DUMMYSIZEORDER];
} SCOTCH_Ordering;

typedef struct {
  double                    dummy[DUMMYSIZESTRAT];
} SCOTCH_Strat;

/*
**  The function prototypes.
*/

#ifdef X_ARCHalpha_compaq_osf1
#ifndef USE_CXX
extern "C" {
#endif /* USE_CXX */
#endif /* X_ARCHalpha_compaq_osf1 */

void                        SCOTCH_errorProg    (const char * const);
void                        SCOTCH_errorPrint   (const char * const, ...);
void                        SCOTCH_errorPrintW  (const char * const, ...);

int                         SCOTCH_archInit     (SCOTCH_Arch * const);
void                        SCOTCH_archExit     (SCOTCH_Arch * const);
int                         SCOTCH_archLoad     (SCOTCH_Arch * const, FILE * const);
int                         SCOTCH_archSave     (const SCOTCH_Arch * const, FILE * const);
int                         SCOTCH_archBuild    (SCOTCH_Arch * const, const SCOTCH_Graph * const, const SCOTCH_Num, const SCOTCH_Num * const, const SCOTCH_Strat * const);
int                         SCOTCH_archCmplt    (SCOTCH_Arch * const, const SCOTCH_Num);
char *                      SCOTCH_archName     (const SCOTCH_Arch * const);
SCOTCH_Num                  SCOTCH_archSize     (const SCOTCH_Arch * const);

#ifdef SCOTCH_PTSCOTCH
int                         SCOTCH_dgraphInit   (SCOTCH_Dgraph * const, MPI_Comm);
void                        SCOTCH_dgraphExit   (SCOTCH_Dgraph * const);
void                        SCOTCH_dgraphFree   (SCOTCH_Dgraph * const);
int                         SCOTCH_dgraphLoad   (SCOTCH_Dgraph * const, FILE * const, const SCOTCH_Num, const SCOTCH_Num);
int                         SCOTCH_dgraphSave   (SCOTCH_Dgraph * const, FILE * const);
int                         SCOTCH_dgraphCheck  (const SCOTCH_Dgraph * const);
int                         SCOTCH_dgraphBuild  (SCOTCH_Dgraph * const, const SCOTCH_Num, const SCOTCH_Num, const SCOTCH_Num, SCOTCH_Num * const, SCOTCH_Num * const, SCOTCH_Num * const, SCOTCH_Num * const, const SCOTCH_Num, const SCOTCH_Num, SCOTCH_Num * const, SCOTCH_Num * const, SCOTCH_Num * const);
int                         SCOTCH_dgraphBuildGrid3D (SCOTCH_Dgraph * const, const SCOTCH_Num, const SCOTCH_Num, const SCOTCH_Num, const SCOTCH_Num);
int                         SCOTCH_dgraphGather (const SCOTCH_Dgraph * const, SCOTCH_Graph * const);
int                         SCOTCH_dgraphScatter (SCOTCH_Dgraph * const, const SCOTCH_Graph * const);
void                        SCOTCH_dgraphSize   (const SCOTCH_Dgraph * const, SCOTCH_Num * const, SCOTCH_Num * const, SCOTCH_Num * const, SCOTCH_Num * const);
void                        SCOTCH_dgraphData   (const SCOTCH_Dgraph * const, SCOTCH_Num * const, SCOTCH_Num * const, SCOTCH_Num * const, SCOTCH_Num * const, SCOTCH_Num * const, SCOTCH_Num ** const, SCOTCH_Num ** const, SCOTCH_Num ** const, SCOTCH_Num ** const, SCOTCH_Num * const, SCOTCH_Num * const, SCOTCH_Num * const, SCOTCH_Num ** const, SCOTCH_Num ** const, SCOTCH_Num ** const, MPI_Comm * const);
int                         SCOTCH_dgraphStat   (const SCOTCH_Dgraph * const, SCOTCH_Num * const, SCOTCH_Num * const, SCOTCH_Num * const, double * const, double * const, SCOTCH_Num * const, SCOTCH_Num * const, double * const, double * const, SCOTCH_Num * const, SCOTCH_Num * const, SCOTCH_Num * const, double * const, double * const);
int                         SCOTCH_dgraphHalo   (SCOTCH_Dgraph * const, void * const, const MPI_Datatype);
int                         SCOTCH_dgraphCorderInit (const SCOTCH_Dgraph * const, SCOTCH_Ordering * const, SCOTCH_Num * const, SCOTCH_Num * const, SCOTCH_Num * const, SCOTCH_Num * const, SCOTCH_Num * const);
void                        SCOTCH_dgraphCorderExit (const SCOTCH_Dgraph * const, SCOTCH_Ordering * const);
int                         SCOTCH_dgraphOrderInit (const SCOTCH_Dgraph * const, SCOTCH_Dordering * const);
void                        SCOTCH_dgraphOrderExit (const SCOTCH_Dgraph * const, SCOTCH_Dordering * const);
int                         SCOTCH_dgraphOrderSave (const SCOTCH_Dgraph * const, const SCOTCH_Dordering * const, FILE * const);
int                         SCOTCH_dgraphOrderSaveMap (const SCOTCH_Dgraph * const, const SCOTCH_Dordering * const, FILE * const);
int                         SCOTCH_dgraphOrderSaveTree (const SCOTCH_Dgraph * const, const SCOTCH_Dordering * const, FILE * const);
int                         SCOTCH_dgraphOrderCompute (const SCOTCH_Dgraph * const, SCOTCH_Dordering * const, const SCOTCH_Strat * const);
int                         SCOTCH_dgraphOrderComputeList (const SCOTCH_Dgraph * const, SCOTCH_Dordering * const, const SCOTCH_Num, const SCOTCH_Num * const, const SCOTCH_Strat * const);
int                         SCOTCH_dgraphOrderGather (const SCOTCH_Dgraph * const, const SCOTCH_Dordering * const, SCOTCH_Ordering * const);
#endif /* SCOTCH_PTSCOTCH */

int                         SCOTCH_geomInit     (SCOTCH_Geom * const);
void                        SCOTCH_geomExit     (SCOTCH_Geom * const);
void                        SCOTCH_geomData     (const SCOTCH_Geom * const, SCOTCH_Num * const, double ** const);

int                         SCOTCH_graphInit    (SCOTCH_Graph * const);
void                        SCOTCH_graphExit    (SCOTCH_Graph * const);
void                        SCOTCH_graphFree    (SCOTCH_Graph * const);
int                         SCOTCH_graphLoad    (SCOTCH_Graph * const, FILE * const, const SCOTCH_Num, const SCOTCH_Num);
int                         SCOTCH_graphSave    (const SCOTCH_Graph * const, FILE * const);
int                         SCOTCH_graphBuild   (SCOTCH_Graph * const, const SCOTCH_Num, const SCOTCH_Num, const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num, const SCOTCH_Num * const, const SCOTCH_Num * const);
SCOTCH_Num                  SCOTCH_graphBase    (SCOTCH_Graph * const, const SCOTCH_Num baseval);
int                         SCOTCH_graphCheck   (const SCOTCH_Graph * const);
void                        SCOTCH_graphSize    (const SCOTCH_Graph * const, SCOTCH_Num * const, SCOTCH_Num * const);
void                        SCOTCH_graphData    (const SCOTCH_Graph * const, SCOTCH_Num * const, SCOTCH_Num * const, SCOTCH_Num ** const, SCOTCH_Num ** const, SCOTCH_Num ** const, SCOTCH_Num ** const, SCOTCH_Num * const, SCOTCH_Num ** const, SCOTCH_Num ** const);
void                        SCOTCH_graphStat    (const SCOTCH_Graph * const, SCOTCH_Num * const, SCOTCH_Num * const, SCOTCH_Num * const, double * const, double * const, SCOTCH_Num * const, SCOTCH_Num * const, double * const, double * const, SCOTCH_Num * const, SCOTCH_Num * const, SCOTCH_Num * const, double * const, double * const);
int                         SCOTCH_graphGeomLoadChac (SCOTCH_Graph * const, SCOTCH_Geom * const, FILE * const, FILE * const, const char * const);
int                         SCOTCH_graphGeomSaveChac (const SCOTCH_Graph * const, const SCOTCH_Geom * const, FILE * const, FILE * const, const char * const);
int                         SCOTCH_graphGeomLoadHabo (SCOTCH_Graph * const, SCOTCH_Geom * const, FILE * const, FILE * const, const char * const);
int                         SCOTCH_graphGeomLoadScot (SCOTCH_Graph * const, SCOTCH_Geom * const, FILE * const, FILE * const, const char * const);
int                         SCOTCH_graphGeomSaveScot (const SCOTCH_Graph * const, const SCOTCH_Geom * const, FILE * const, FILE * const, const char * const);

int                         SCOTCH_graphMapInit (const SCOTCH_Graph * const, SCOTCH_Mapping * const, const SCOTCH_Arch * const, SCOTCH_Num * const);
void                        SCOTCH_graphMapExit (const SCOTCH_Graph * const, SCOTCH_Mapping * const);
int                         SCOTCH_graphMapLoad (const SCOTCH_Graph * const, const SCOTCH_Mapping * const, FILE * const);
int                         SCOTCH_graphMapSave (const SCOTCH_Graph * const, const SCOTCH_Mapping * const, FILE * const);
int                         SCOTCH_graphMapView (const SCOTCH_Graph * const, const SCOTCH_Mapping * const, FILE * const);
int                         SCOTCH_graphMapCompute (const SCOTCH_Graph * const, SCOTCH_Mapping * const, const SCOTCH_Strat * const);
int                         SCOTCH_graphMap     (const SCOTCH_Graph * const, const SCOTCH_Arch * const, const SCOTCH_Strat * const, SCOTCH_Num * const);
int                         SCOTCH_graphPart    (const SCOTCH_Graph * const, const SCOTCH_Num, const SCOTCH_Strat * const, SCOTCH_Num * const);

int                         SCOTCH_graphOrderInit (const SCOTCH_Graph * const, SCOTCH_Ordering * const, SCOTCH_Num * const, SCOTCH_Num * const, SCOTCH_Num * const, SCOTCH_Num * const, SCOTCH_Num * const);
void                        SCOTCH_graphOrderExit (const SCOTCH_Graph * const, SCOTCH_Ordering * const);
int                         SCOTCH_graphOrderLoad (const SCOTCH_Graph * const, SCOTCH_Ordering * const, FILE * const);
int                         SCOTCH_graphOrderSave (const SCOTCH_Graph * const, const SCOTCH_Ordering * const, FILE * const);
int                         SCOTCH_graphOrderSaveMap (const SCOTCH_Graph * const, const SCOTCH_Ordering * const, FILE * const);
int                         SCOTCH_graphOrderSaveTree (const SCOTCH_Graph * const, const SCOTCH_Ordering * const, FILE * const);
int                         SCOTCH_graphOrderCompute (const SCOTCH_Graph * const, SCOTCH_Ordering * const, const SCOTCH_Strat * const);
int                         SCOTCH_graphOrderComputeList (const SCOTCH_Graph * const, SCOTCH_Ordering * const, const SCOTCH_Num, const SCOTCH_Num * const, const SCOTCH_Strat * const);
int                         SCOTCH_graphOrderFactor (const SCOTCH_Graph * const, const SCOTCH_Ordering * const, SCOTCH_Graph * const);
int                         SCOTCH_graphOrderView (const SCOTCH_Graph * const, const SCOTCH_Ordering * const, FILE * const);
int                         SCOTCH_graphOrder   (const SCOTCH_Graph * const, const SCOTCH_Strat * const, SCOTCH_Num * const, SCOTCH_Num * const, SCOTCH_Num * const, SCOTCH_Num * const, SCOTCH_Num * const);
int                         SCOTCH_graphOrderList (const SCOTCH_Graph * const, const SCOTCH_Num, const SCOTCH_Num * const, const SCOTCH_Strat * const, SCOTCH_Num * const, SCOTCH_Num * const, SCOTCH_Num * const, SCOTCH_Num * const, SCOTCH_Num * const);
int                         SCOTCH_graphOrderCheck (const SCOTCH_Graph * const, const SCOTCH_Ordering * const);

int                         SCOTCH_meshInit     (SCOTCH_Mesh * const);
void                        SCOTCH_meshExit     (SCOTCH_Mesh * const);
int                         SCOTCH_meshLoad     (SCOTCH_Mesh * const, FILE * const, const SCOTCH_Num);
int                         SCOTCH_meshSave     (const SCOTCH_Mesh * const, FILE * const);
int                         SCOTCH_meshBuild    (SCOTCH_Mesh * const, const SCOTCH_Num, const SCOTCH_Num, const SCOTCH_Num, const SCOTCH_Num, const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num, const SCOTCH_Num * const);
int                         SCOTCH_meshCheck    (const SCOTCH_Mesh * const);
void                        SCOTCH_meshSize     (const SCOTCH_Mesh * const, SCOTCH_Num * const, SCOTCH_Num * const, SCOTCH_Num * const);
void                        SCOTCH_meshData     (const SCOTCH_Mesh * const, SCOTCH_Num * const, SCOTCH_Num * const, SCOTCH_Num * const, SCOTCH_Num * const, SCOTCH_Num ** const, SCOTCH_Num ** const, SCOTCH_Num ** const, SCOTCH_Num ** const, SCOTCH_Num ** const, SCOTCH_Num * const, SCOTCH_Num ** const, SCOTCH_Num * const);
void                        SCOTCH_meshStat     (const SCOTCH_Mesh * const, SCOTCH_Num * const, SCOTCH_Num * const, SCOTCH_Num * const, double * const, double * const, SCOTCH_Num * const, SCOTCH_Num * const, double * const, double * const, SCOTCH_Num * const, SCOTCH_Num * const, double * const, double * const);
int                         SCOTCH_meshGraph    (const SCOTCH_Mesh * const, SCOTCH_Graph * const);
int                         SCOTCH_meshGeomLoadHabo (SCOTCH_Mesh * const, SCOTCH_Geom * const, FILE * const, FILE * const, const char * const);
int                         SCOTCH_meshGeomLoadScot (SCOTCH_Mesh * const, SCOTCH_Geom * const, FILE * const, FILE * const, const char * const);
int                         SCOTCH_meshGeomSaveScot (const SCOTCH_Mesh * const, const SCOTCH_Geom * const, FILE * const, FILE * const, const char * const);

int                         SCOTCH_meshOrderInit (const SCOTCH_Mesh * const, SCOTCH_Ordering * const, SCOTCH_Num * const, SCOTCH_Num * const, SCOTCH_Num * const, SCOTCH_Num * const, SCOTCH_Num * const);
void                        SCOTCH_meshOrderExit (const SCOTCH_Mesh * const, SCOTCH_Ordering * const);
int                         SCOTCH_meshOrderSave (const SCOTCH_Mesh * const, const SCOTCH_Ordering * const, FILE * const);
int                         SCOTCH_meshOrderSaveMap (const SCOTCH_Mesh * const, const SCOTCH_Ordering * const, FILE * const);
int                         SCOTCH_meshOrderSaveTree (const SCOTCH_Mesh * const, const SCOTCH_Ordering * const, FILE * const);
int                         SCOTCH_meshOrderCompute (const SCOTCH_Mesh * const, SCOTCH_Ordering * const, const SCOTCH_Strat * const);
int                         SCOTCH_meshOrderComputeList (const SCOTCH_Mesh * const, SCOTCH_Ordering * const, const SCOTCH_Num, const SCOTCH_Num * const, const SCOTCH_Strat * const);
int                         SCOTCH_meshOrder    (const SCOTCH_Mesh * const, const SCOTCH_Strat * const, SCOTCH_Num * const, SCOTCH_Num * const, SCOTCH_Num * const, SCOTCH_Num * const, SCOTCH_Num * const);
int                         SCOTCH_meshOrderList (const SCOTCH_Mesh * const, const SCOTCH_Num, const SCOTCH_Num * const, const SCOTCH_Strat * const, SCOTCH_Num * const, SCOTCH_Num * const, SCOTCH_Num * const, SCOTCH_Num * const, SCOTCH_Num * const);
int                         SCOTCH_meshOrderCheck (const SCOTCH_Mesh * const, const SCOTCH_Ordering * const);

void                        SCOTCH_randomReset  (void);

int                         SCOTCH_stratInit    (SCOTCH_Strat * const);
void                        SCOTCH_stratExit    (SCOTCH_Strat * const);
void                        SCOTCH_stratFree    (SCOTCH_Strat * const);
int                         SCOTCH_stratSave    (const SCOTCH_Strat * const, FILE * const);
#ifdef SCOTCH_PTSCOTCH
int                         SCOTCH_stratDgraphOrder (SCOTCH_Strat * const, const char * const);
#endif /* SCOTCH_PTSCOTCH */
int                         SCOTCH_stratGraphBipart (SCOTCH_Strat * const, const char * const);
int                         SCOTCH_stratGraphMap (SCOTCH_Strat * const, const char * const);
int                         SCOTCH_stratGraphOrder (SCOTCH_Strat * const, const char * const);
int                         SCOTCH_stratMeshOrder (SCOTCH_Strat * const, const char * const);

void                        SCOTCH_memoryTrace  (void);
void                        SCOTCH_memoryUntrace (void);
void                        SCOTCH_memoryTraceReset (void);
unsigned long               SCOTCH_memoryTraceGet (void);

#ifdef X_ARCHalpha_compaq_osf1
#ifndef USE_CXX
}
#endif /* USE_CXX */
#endif /* X_ARCHalpha_compaq_osf1 */
