/*********************************************************
**                                                      **
**  WARNING: THIS IS NOT THE ORIGINAL INCLUDE FILE OF   **
**  THE MeTiS SOFTWARE PACKAGE.                         **
**  This file is a compatibility include file provided  **
**  as part of the Scotch software distribution.        **
**  Preferably use the original MeTiS include file to   **
**  keep definitions of routines not overloaded by      **
**  the libScotchMeTiS library.                         **
**                                                      **
*********************************************************/
/* Copyright 2007,2010,2012,2018,2019,2021 IPB, Universite de Bordeaux, INRIA & CNRS
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
/**   NAME       : library_metis.h                         **/
/**                                                        **/
/**   AUTHOR     : Francois PELLEGRINI                     **/
/**                Amaury JACQUES (v6.0)                   **/
/**                Marc FUENTES (v6.1)                     **/
/**                                                        **/
/**   FUNCTION   : Compatibility declaration file for the  **/
/**                MeTiS interface routines provided by    **/
/**                the Scotch project.                     **/
/**                                                        **/
/**   DATES      : # Version 5.0  : from : 08 sep 2006     **/
/**                                 to   : 07 jun 2007     **/
/**                # Version 5.1  : from : 30 jun 2010     **/
/**                                 to   : 30 jun 2010     **/
/**                # Version 6.0  : from : 13 sep 2012     **/
/**                                 to   : 17 jun 2019     **/
/**                # Version 6.1  : from : 09 feb 2021     **/
/**                                 to   : 15 jun 2021     **/
/**                                                        **/
/************************************************************/

/*
**  The defines.
*/

#ifdef SCOTCH_METIS_PREFIX
#define SCOTCH_METIS_PREFIXL        scotch_
#define SCOTCH_METIS_PREFIXU        SCOTCH_
#endif /* SCOTCH_METIS_PREFIX */

#ifndef SCOTCH_METIS_PREFIXL
#define SCOTCH_METIS_PREFIXL
#endif /* SCOTCH_METIS_PREFIXL */

#ifndef SCOTCH_METIS_PREFIXU
#define SCOTCH_METIS_PREFIXU
#endif /* SCOTCH_METIS_PREFIXU */

#ifndef METISNAMEL
#define METISNAMEL(s)               METISNAME2(METISNAME3(SCOTCH_METIS_PREFIXL),s)
#define METISNAMEU(s)               METISNAME2(METISNAME3(SCOTCH_METIS_PREFIXU),s)
#define METISNAME2(p,s)             METISNAME4(p,s)
#define METISNAME3(s)               s
#define METISNAME4(p,s)             p##s
#endif /* METISNAMEL */

#ifndef SCOTCH_METIS_DATATYPES
#define SCOTCH_METIS_DATATYPES
typedef SCOTCH_Num          idx_t;
typedef double              real_t;
#endif /* SCOTCH_METIS_DATATYPES */

#ifndef SCOTCH_METIS_OPTIONS
#define SCOTCH_METIS_OPTIONS
#define METIS_NOPTIONS              40

typedef enum {
  METIS_OPTION_PTYPE,
  METIS_OPTION_OBJTYPE,
  METIS_OPTION_CTYPE,
  METIS_OPTION_IPTYPE,
  METIS_OPTION_RTYPE,
  METIS_OPTION_DBGLVL,
  METIS_OPTION_NITER,
  METIS_OPTION_NCUTS,
  METIS_OPTION_SEED,
  METIS_OPTION_NO2HOP,
  METIS_OPTION_MINCONN,
  METIS_OPTION_CONTIG,
  METIS_OPTION_COMPRESS,
  METIS_OPTION_CCORDER,
  METIS_OPTION_PFACTOR,
  METIS_OPTION_NSEPS,
  METIS_OPTION_UFACTOR,
  METIS_OPTION_NUMBERING,
  METIS_OPTION_HELP,
  METIS_OPTION_TPWGTS,
  METIS_OPTION_NCOMMON,
  METIS_OPTION_NOOUTPUT,
  METIS_OPTION_BALANCE,
  METIS_OPTION_GTYPE,
  METIS_OPTION_UBVEC
} moptions_et;
#endif /* SCOTCH_METIS_OPTIONS */

#ifndef SCOTCH_METIS_OBJTYPE
#define SCOTCH_METIS_OBJTYPE
typedef enum {
  METIS_OBJTYPE_CUT,
  METIS_OBJTYPE_VOL,
  METIS_OBJTYPE_NODE
} mobjtype_et;
#endif /* SCOTCH_METIS_OBJTYPE */

#ifndef SCOTCH_METIS_RETURN
#define SCOTCH_METIS_RETURN
typedef enum {
  METIS_OK           = 1,
  METIS_ERROR_INPUT  = -2,
  METIS_ERROR_MEMORY = -3,
  METIS_ERROR        = -4
} rstatus_et;
#endif /* SCOTCH_METIS_RETURN */

/*
**  The type and structure definitions.
*/

#ifndef SCOTCH_H                                  /* In case "scotch.h" not included before */
typedef DUMMYINT SCOTCH_Num;
#endif /* SCOTCH_H */

/*
**  The function prototypes.
*/

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

int                         SCOTCH_METIS_V3_EdgeND (const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, SCOTCH_Num * const, SCOTCH_Num * const);
int                         SCOTCH_METIS_V3_NodeND (const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, SCOTCH_Num * const, SCOTCH_Num * const);
int                         SCOTCH_METIS_V3_NodeWND (const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, SCOTCH_Num * const, SCOTCH_Num * const);
int                         SCOTCH_METIS_V5_NodeND (const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, SCOTCH_Num * const, SCOTCH_Num * const);

int                         SCOTCH_METIS_V3_PartGraphKway (const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, SCOTCH_Num * const, SCOTCH_Num * const);
int                         SCOTCH_METIS_V3_PartGraphRecursive (const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, SCOTCH_Num * const, SCOTCH_Num * const);
int                         SCOTCH_METIS_V3_PartGraphVKway (const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, SCOTCH_Num * const, SCOTCH_Num * const);
int                         SCOTCH_METIS_V5_PartGraphKway (const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, const double * const, const SCOTCH_Num * const, SCOTCH_Num * const, SCOTCH_Num * const);
int                         SCOTCH_METIS_V5_PartGraphRecursive (const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, const double * const, const SCOTCH_Num * const, SCOTCH_Num * const, SCOTCH_Num * const);

#ifndef SCOTCH_METIS_VERSION
#define SCOTCH_METIS_VERSION        3             /* MeTiS API version is 3 by default */
#endif /* SCOTCH_METIS_VERSION */

#if (SCOTCH_METIS_VERSION == 3)
int                         METISNAMEU (METIS_EdgeND) (const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, SCOTCH_Num * const, SCOTCH_Num * const);
int                         METISNAMEU (METIS_NodeND) (const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, SCOTCH_Num * const, SCOTCH_Num * const);
int                         METISNAMEU (METIS_NodeWND) (const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, SCOTCH_Num * const, SCOTCH_Num * const);

int                         METISNAMEU (METIS_PartGraphKway) (const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, SCOTCH_Num * const, SCOTCH_Num * const);
int                         METISNAMEU (METIS_PartGraphRecursive) (const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, SCOTCH_Num * const, SCOTCH_Num * const);
int                         METISNAMEU (METIS_PartGraphVKway) (const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, SCOTCH_Num * const, SCOTCH_Num * const);
#endif /* (SCOTCH_METIS_VERSION == 3) */

#if (SCOTCH_METIS_VERSION == 5)
int                         METISNAMEU (METIS_NodeND) (const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, SCOTCH_Num * const, SCOTCH_Num * const);
int                         METISNAMEU (METIS_PartGraphKway) (const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, const double * const, const SCOTCH_Num * const, SCOTCH_Num * const, SCOTCH_Num * const);
int                         METISNAMEU (METIS_PartGraphRecursive) (const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, const double * const, const SCOTCH_Num * const, SCOTCH_Num * const, SCOTCH_Num * const);
#endif /* (SCOTCH_METIS_VERSION == 5) */

int                         METISNAMEU (METIS_MeshToDual) (const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, SCOTCH_Num ** const, SCOTCH_Num ** const);
int                         METISNAMEU (METIS_PartMeshDual) (const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, const SCOTCH_Num * const, const double * const, const SCOTCH_Num * const, SCOTCH_Num * const, SCOTCH_Num * const, SCOTCH_Num * const);

#ifdef __cplusplus
}
#endif /* __cplusplus */
