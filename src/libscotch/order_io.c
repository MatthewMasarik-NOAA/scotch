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
/**   NAME       : order_io.c                              **/
/**                                                        **/
/**   AUTHOR     : Francois PELLEGRINI                     **/
/**                                                        **/
/**   FUNCTION   : This module handles generic orderings.  **/
/**                                                        **/
/**   DATES      : # Version 3.2  : from : 19 oct 1996     **/
/**                                 to     27 aug 1998     **/
/**                # Version 4.0  : from : 19 dec 2001     **/
/**                                 to     28 jun 2004     **/
/**                # Version 5.0  : from : 12 sep 2007     **/
/**                                 to     12 sep 2007     **/
/**                                                        **/
/************************************************************/

/*
**  The defines and includes.
*/

#define ORDER_IO

#include "module.h"
#include "common.h"
#include "graph.h"
#include "order.h"

/************************************/
/*                                  */
/* These routines handle orderings. */
/*                                  */
/************************************/

/* This routine loads an ordering.
** It returns:
** - 0   : on success.
** - !0  : on error.
*/

int
orderLoad (
Order * restrict const      ordeptr,
const Gnum * restrict const vlbltab,
FILE * restrict const       stream)
{
  Gnum * restrict     permtab;
  Gnum                vertnum;

  if (vlbltab != NULL) {
    errorPrint ("orderLoad: vertex labels not yet supported");
    return     (1);
  }

  if ((permtab = memAlloc (ordeptr->vnodnbr * sizeof (Gnum))) == NULL) {
    errorPrint ("orderLoad: out of memory");
    return     (1);
  }

  if (intLoad (stream, &ordeptr->vnodnbr) != 1) {
    errorPrint ("orderLoad: bad input (1)");
    memFree    (permtab);
    return     (1);
  }

  if (vlbltab == NULL) {                          /* If ordering does not have label array */
    for (vertnum = 0; vertnum < ordeptr->vnodnbr; vertnum ++) {
      Gnum                vertval;

      if (intLoad (stream, &vertval) +            /* Read item data */
          intLoad (stream, &permtab[vertnum]) != 2) {
        errorPrint ("orderLoad: bad input (2)");
        memFree    (permtab);
        return     (1);
      }
      if (vertval != (vertnum + ordeptr->baseval)) { /* Read item data */
        errorPrint ("orderLoad: bad input (3)");
        memFree    (permtab);
        return     (1);
      }
    }
  }

  orderPeri (permtab, ordeptr->baseval, ordeptr->vnodnbr, ordeptr->peritab, ordeptr->baseval); /* Compute inverse permutation */

  memFree (permtab);
  return  (0);
}

/* This routine saves an ordering.
** It returns:
** - 0   : on success.
** - !0  : on error.
*/

int
orderSave (
const Order * restrict const  ordeptr,
const Gnum * restrict const   vlbltab,
FILE * restrict const         stream)
{
  const Gnum * restrict vlbltax;
  Gnum * restrict       permtab;
  Gnum                  vertnum;

  vlbltax = (vlbltab != NULL) ? (vlbltab - ordeptr->baseval) : NULL;

  if ((permtab = memAlloc (ordeptr->vnodnbr * sizeof (Gnum))) == NULL) {
    errorPrint ("orderSave: out of memory");
    return     (1);
  }

  if (fprintf (stream, "%ld\n", (long) ordeptr->vnodnbr) == EOF) {
    errorPrint ("orderSave: bad output (1)");
    memFree    (permtab);
    return     (1);
  }

  orderPeri (ordeptr->peritab, ordeptr->baseval, ordeptr->vnodnbr, permtab, ordeptr->baseval); /* Compute direct permutation */

  if (vlbltax != NULL) {                          /* If ordering has label array */
    for (vertnum = 0; vertnum < ordeptr->vnodnbr; vertnum ++) {
      if (fprintf (stream, "%ld\t%ld\n",
                   (long) vlbltax[vertnum + ordeptr->baseval],
                   (long) vlbltax[permtab[vertnum]]) == EOF) {
        errorPrint ("orderSave: bad output (2)");
        memFree    (permtab);
        return     (1);
      }
    }
  }
  else {
    for (vertnum = 0; vertnum < ordeptr->vnodnbr; vertnum ++) {
      if (fprintf (stream, "%ld\t%ld\n",
                   (long) (vertnum + ordeptr->baseval),
                   (long) permtab[vertnum]) == EOF) {
        errorPrint ("orderSave: bad output (3)");
        memFree    (permtab);
        return     (1);
      }
    }
  }

  memFree (permtab);
  return  (0);
}

/* This routine saves the column block
** mapping data of the given ordering.
** It returns:
** - 0   : on success.
** - !0  : on error.
*/

int
orderSaveMap (
const Order * restrict const  ordeptr,
const Gnum * restrict const   vlbltab,
FILE * const                  stream)
{
  const Gnum * restrict vlbltax;
  Gnum * restrict       rangtab;
  Gnum * restrict       cblktab;
  Gnum                  vertnum;
  Gnum                  cblknum;

  vlbltax = (vlbltab != NULL) ? (vlbltab - ordeptr->baseval) : NULL;

  if (memAllocGroup ((void **) (void *)
        &rangtab, (size_t) ((ordeptr->vnodnbr + 1) * sizeof (Gnum)),
        &cblktab, (size_t) ( ordeptr->vnodnbr      * sizeof (Gnum)), NULL) == NULL) {
    errorPrint ("orderSaveMap: out of memory");
    return     (1);
  }

  orderRang (ordeptr, rangtab);
  for (vertnum = 0, cblknum = 0; vertnum < ordeptr->vnodnbr; vertnum ++) {
    if (vertnum >= rangtab[cblknum + 1])
      cblknum ++;
    cblktab[ordeptr->peritab[vertnum] - ordeptr->baseval] = cblknum;
  }

  if (fprintf (stream, "%ld\n", (long) ordeptr->vnodnbr) == EOF) {
    errorPrint ("orderSaveMap: bad output (1)");
    memFree    (rangtab);                         /* Free memory group leader */
    return     (1);
  }
  for (vertnum = 0; vertnum < ordeptr->vnodnbr; vertnum ++) {
    if (fprintf (stream, "%ld\t%ld\n",
                 (long) ((vlbltax != NULL) ? vlbltax[vertnum + ordeptr->baseval] : (vertnum + ordeptr->baseval)),
                 (long) cblktab[vertnum]) == EOF) {
      errorPrint ("orderSaveMap: bad output (2)");
      memFree    (rangtab);                       /* Free memory group leader */
      return     (1);
    }
  }

  memFree (rangtab);                              /* Free memory group leader */
  return  (0);
}

/* This routine saves the separator
** tree data of the given ordering.
** It returns:
** - 0   : on success.
** - !0  : on error.
*/

int
orderSaveTree (
const Order * restrict const  ordeptr,
const Gnum * restrict const   vlbltab,
FILE * const                  stream)
{
  const Gnum * restrict vlbltax;
  Gnum * restrict       rangtab;
  Gnum * restrict       treetab;
  Gnum * restrict       cblktab;
  Gnum                  vertnum;
  Gnum                  cblknum;

  vlbltax = (vlbltab != NULL) ? (vlbltab - ordeptr->baseval) : NULL;

  if (memAllocGroup ((void **) (void *)
        &rangtab, (size_t) ((ordeptr->vnodnbr + 1) * sizeof (Gnum)),
        &treetab, (size_t) ( ordeptr->vnodnbr      * sizeof (Gnum)),
        &cblktab, (size_t) ( ordeptr->vnodnbr      * sizeof (Gnum)), NULL) == NULL) {
    errorPrint ("orderSaveTree: out of memory");
    return     (1);
  }

  orderRang (ordeptr, rangtab);
  orderTree (ordeptr, treetab);
  for (vertnum = 0, cblknum = 0; vertnum < ordeptr->vnodnbr; vertnum ++) {
    if (vertnum >= rangtab[cblknum + 1])
      cblknum ++;
    cblktab[ordeptr->peritab[vertnum] - ordeptr->baseval] = treetab[cblknum];
  }

  if (fprintf (stream, "%ld\n", (long) ordeptr->vnodnbr) == EOF) {
    errorPrint ("orderSaveTree: bad output (1)");
    memFree    (rangtab);                         /* Free memory group leader */
    return     (1);
  }
  for (vertnum = 0; vertnum < ordeptr->vnodnbr; vertnum ++) {
    if (fprintf (stream, "%ld\t%ld\n",
                 (long) ((vlbltax != NULL) ? vlbltax[vertnum + ordeptr->baseval] : (vertnum + ordeptr->baseval)),
                 (long) cblktab[vertnum]) == EOF) {
      errorPrint ("orderSaveTree: bad output (2)");
      memFree    (rangtab);                       /* Free memory group leader */
      return     (1);
    }
  }

  memFree (rangtab);                              /* Free memory group leader */
  return  (0);
}
