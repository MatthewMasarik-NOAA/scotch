/* Copyright 2008,2009 ENSEIRB, INRIA & CNRS
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
/**   NAME       : dgraph_match.c                          **/
/**                                                        **/
/**   AUTHOR     : Francois PELLEGRINI                     **/
/**                Cedric CHEVALIER (v5.0)                 **/
/**                                                        **/
/**   FUNCTION   : These lines are the data declarations   **/
/**                for the distributed graph matching      **/
/**                routines.                               **/
/**                                                        **/
/**    DATES     : # Version 5.1  : from : 01 dec 2008     **/
/**                                 to   : 10 feb 2009     **/
/**                                                        **/
/************************************************************/

/*
** The defines and includes.
*/

#define DGRAPH_MATCH

#include "module.h"
#include "common.h"
#include "dgraph.h"
#include "dgraph_coarsen.h"
#include "dgraph_match.h"

/*************************************/
/*                                   */
/* These routines handle distributed */
/* source graphs.                    */
/*                                   */
/*************************************/

/* This routine initializes a distributed graph
** structure. In order to avoid collective
** communication whenever possible, the allocation
** of send and receive index arrays is not performed
** in the routine itself, but rather delegated to
** subsequent routines such as dgraphBuild.
** However, these arrays will not be freed by
** dgraphFree, but by dgraphExit.
** It returns:
** - 0   : on success.
** - !0  : on error.
*/

int
dgraphMatchInit (
DgraphMatchData * restrict const    mateptr,
const float                         probval)
{
  Dgraph *            grafptr;
  int                 procngbnum;
  Gnum                vertlocnbr;
  Gnum                vertgstnbr;

  grafptr    = mateptr->c.finegrafptr;
  vertlocnbr = grafptr->vertlocnbr;
  vertgstnbr = grafptr->vertgstnbr;

  if (memAllocGroup ((void **) (void *)
                     &mateptr->procvgbtab, (size_t) ((grafptr->procngbnbr + 1) * sizeof (Gnum)),
                     &mateptr->queuloctab, (size_t) (vertlocnbr * sizeof (Gnum)), NULL) == NULL) {
    errorPrint ("dgraphMatchInit: out of memory");
    return     (1);
  }

  mateptr->c.multlocnbr = 0;
  mateptr->mategsttax = mateptr->c.coargsttax;    /* TRICK: re-use array               */
  mateptr->matelocnbr = 0;                        /* All vertices need to be processed */
  mateptr->queulocnbr = 0;
  mateptr->probval = (grafptr->procngbnbr == 0) ? 1.0F : probval;

  memSet (mateptr->mategsttax + grafptr->vertlocnnd, ~0, (vertgstnbr - vertlocnbr) * sizeof (Gnum)); /* No ghost vertices matched to date */

  for (procngbnum = 0; procngbnum < grafptr->procngbnbr; procngbnum ++)
    mateptr->procvgbtab[procngbnum] = (Gnum) grafptr->procvrttab[grafptr->procngbtab[procngbnum]];
  mateptr->procvgbtab[procngbnum] = (Gnum) grafptr->procvrttab[grafptr->procglbnbr]; /* Mark end */

  return (0);
}

/* This routine frees the contents of a matching
** data structure.
** It returns:
** - VOID  : in all cases.
*/

void
dgraphMatchExit (
DgraphMatchData * restrict const  mateptr)
{
  memFree (mateptr->procvgbtab);
}

/* This routine performs a round of communication
** to synchronize enqueued matching requests across
** processors.
** It returns:
** - 0   : on success.
** - !0  : on error.
*/

int
dgraphMatchSync (
DgraphMatchData * restrict const  mateptr)
{
  Gnum                queulocnbr;
  Gnum                queulocnum;
  Gnum                matelocnbr;
  Gnum                multlocnbr;
  Gnum                vertlocadj;
  Gnum                edgekptnbr;
  int                 procngbnbr;
  int                 procngbnum;
  int                 vrcvreqnbr;

  Dgraph * restrict const             grafptr    = mateptr->c.finegrafptr;
  int * restrict const                procgsttax = mateptr->c.procgsttax;
  const Gnum * restrict const         procvgbtab = mateptr->procvgbtab;
  const Gnum * restrict const         vertloctax = grafptr->vertloctax;
  const Gnum * restrict const         vendloctax = grafptr->vendloctax;
  const Gnum * restrict const         edgeloctax = grafptr->edgeloctax;
  const Gnum * restrict const         edgegsttax = grafptr->edgegsttax;
  Gnum * restrict const               queuloctab = mateptr->queuloctab;
  Gnum * restrict const               mategsttax = mateptr->mategsttax;
  DgraphCoarsenMulti * restrict const multloctab = mateptr->c.multloctab;
  int * restrict const                vsndidxtab = mateptr->c.vsndidxtab;
  DgraphCoarsenVert * const           vsnddattab = mateptr->c.vsnddattab; /* [norestrict:async] */

  procngbnbr = grafptr->procngbnbr;

#ifdef SCOTCH_DEBUG_DGRAPH2
  if (procngbnbr <= 0) {                          /* This routine won't work if process has no neighbors */
    errorPrint ("dgraphMatchSync: invalid parameter");
    return     (1);
  }
  if (edgeloctax == NULL) {
    errorPrint ("dgraphMatchSync: not implemented");
    return     (1);
  }
  if (MPI_Barrier (grafptr->proccomm) != MPI_SUCCESS) {
    errorPrint ("dgraphMatchSync: communication error (1)");
    return     (1);
  }
#endif /* SCOTCH_DEBUG_DGRAPH2 */

  memCpy (vsndidxtab, mateptr->c.vsnddsptab, procngbnbr * sizeof (int)); /* Reset indices for sending messages */

  vertlocadj = grafptr->procvrttab[grafptr->proclocnum] - grafptr->baseval;
  for (queulocnum = 0, queulocnbr = mateptr->queulocnbr; queulocnum < queulocnbr; queulocnum ++) {
    Gnum                vertlocnum;
    Gnum                vertgstnum;
    Gnum                edgelocnum;
    Gnum                mategstnum;
    Gnum                mateglbnum;
    int                 procngbnum;
    int                 vsndidxnum;

    vertlocnum = queuloctab[queulocnum];          /* Get local vertex index  */
    mategstnum = mategsttax[vertlocnum];          /* Get mate (edge ?) index */

    if (mategstnum >= -1)                         /* If vertex not willing to mate or matched locally after being considered during matching phase */
      continue;

    edgelocnum = -2 - mategstnum;                 /* Get local edge to mate ghost vertex */
#ifdef SCOTCH_DEBUG_DGRAPH2
    if ((edgelocnum < grafptr->baseval) ||
        (edgelocnum >= (grafptr->edgelocsiz + grafptr->baseval)) ||
        (mategsttax[edgegsttax[edgelocnum]] != -1)) {
      errorPrint ("dgraphMatchSync: internal error (1)");
      return     (1);
    }
#endif /* SCOTCH_DEBUG_DGRAPH2 */
    mateglbnum = edgeloctax[edgelocnum];

    vertgstnum = edgegsttax[edgelocnum];
    procngbnum = procgsttax[vertgstnum];          /* Find neighbor owner process  */
    if (procngbnum < 0) {                         /* If neighbor not yet computed */
      int                 procngbmax;

      procngbnum = 0;
      procngbmax = procngbnbr;
      while ((procngbmax - procngbnum) > 1) {     /* Find owner process by dichotomy on procvgbtab */
        int                 procngbmed;

        procngbmed = (procngbmax + procngbnum) / 2;
        if (procvgbtab[procngbmed] > mateglbnum)
          procngbmax = procngbmed;
        else
          procngbnum = procngbmed;
      }
      procgsttax[vertgstnum] = procngbnum;
    }
#ifdef SCOTCH_DEBUG_DGRAPH2
    if ((grafptr->procvrttab[grafptr->procngbtab[procngbnum]]     >  mateglbnum) ||
        (grafptr->procvrttab[grafptr->procngbtab[procngbnum] + 1] <= mateglbnum)) {
      errorPrint ("dgraphMatchSync: internal error (2)");
      return     (1);
    }
#endif /* SCOTCH_DEBUG_DGRAPH2 */

    vsndidxnum = vsndidxtab[procngbnum] ++;       /* Get position of message in send array */
#ifdef SCOTCH_DEBUG_DGRAPH2
    if (vsndidxnum >= mateptr->c.vsnddsptab[procngbnum + 1]) {
      errorPrint ("dgraphMatchSync: internal error (3)");
      return     (1);
    }
#endif /* SCOTCH_DEBUG_DGRAPH2 */
    vsnddattab[vsndidxnum].datatab[0] = vertlocnum + vertlocadj;
    vsnddattab[vsndidxnum].datatab[1] = mateglbnum;
  }

  procngbnum = mateptr->c.procngbnxt;             /* Post receives of mating requests in descending order */
  do {
    int                 procglbnum;

    procngbnum = (procngbnum + (procngbnbr - 1)) % procngbnbr; /* Pre-decrement neighbor rank */
    procglbnum = grafptr->procngbtab[procngbnum];
    if (MPI_Irecv (mateptr->c.vrcvdattab + mateptr->c.vrcvdsptab[procngbnum],
                   2 * (mateptr->c.vrcvdsptab[procngbnum + 1] - mateptr->c.vrcvdsptab[procngbnum]), GNUM_MPI,
                   procglbnum, TAGMATCH, grafptr->proccomm, &mateptr->c.vrcvreqtab[procngbnum]) != MPI_SUCCESS) {
      errorPrint ("dgraphMatchSync: communication error (2)");
      return     (1);
    }
  } while (procngbnum != mateptr->c.procngbnxt);

  procngbnum = mateptr->c.procngbnxt;             /* Post sends of mating requests in ascending order */
  do {
    int                 procglbnum;

    procglbnum = grafptr->procngbtab[procngbnum];
    if (MPI_Isend (vsnddattab + mateptr->c.vsnddsptab[procngbnum],
                   2 * (vsndidxtab[procngbnum] - mateptr->c.vsnddsptab[procngbnum]), GNUM_MPI,
                   procglbnum, TAGMATCH, grafptr->proccomm, &mateptr->c.vsndreqtab[procngbnum]) != MPI_SUCCESS) {
      errorPrint ("dgraphMatchSync: communication error (3)");
      return     (1);
    }
    procngbnum = (procngbnum + 1) % procngbnbr;   /* Post-increment neighbor rank */
  } while (procngbnum != mateptr->c.procngbnxt);

  matelocnbr = mateptr->matelocnbr;
  multlocnbr = mateptr->c.multlocnbr;
  edgekptnbr = mateptr->c.edgekptnbr;

  for (vrcvreqnbr = procngbnbr; vrcvreqnbr > 0; vrcvreqnbr --) { /* For all pending receive requests */
    int                 vrcvidxnnd;
    int                 requrcvnum;
    int                 requnxtnum;               /* Index of location where to pack requests to process when all messages arrive */
    int                 procngbnum;
    MPI_Status          statdat;
    int                 statsiz;
    int                 o;

#ifdef SCOTCH_DETERMINISTIC
    procngbnum = vrcvreqnbr - 1;
    o = MPI_Wait (&mateptr->c.vrcvreqtab[procngbnum], &statdat);
#else /* SCOTCH_DETERMINISTIC */
    o = MPI_Waitany (procngbnbr, mateptr->c.vrcvreqtab, &procngbnum, &statdat);
#endif /* SCOTCH_DETERMINISTIC */
    if ((o != MPI_SUCCESS) ||
        (MPI_Get_count (&statdat, GNUM_MPI, &statsiz) != MPI_SUCCESS)) {
      errorPrint ("dgraphMatchSync: communication error (4)");
      return     (1);
    }
#ifdef SCOTCH_DEBUG_DGRAPH2
    if (statdat.MPI_SOURCE != grafptr->procngbtab[procngbnum]) {
      errorPrint ("dgraphMatchSync: internal error (4)");
      return     (1);
    }
#endif /* SCOTCH_DEBUG_DGRAPH2 */

    vrcvidxnnd = mateptr->c.vrcvdsptab[procngbnum];
    if (statsiz <= 0) {                           /* If query message is empty */
      mateptr->c.vrcvidxtab[procngbnum] = -1;     /* No need to send a reply   */
      continue;                                   /* Skip message processing   */
    }
    else {
      Gnum                vertsndnbr;               /* Number of vertices to be sent to requesting neighbor */
      Gnum                edgesndnbr;               /* Number of edges to be sent to requesting neighbor    */

      DgraphCoarsenVert * restrict const  vrcvdattab = mateptr->c.vrcvdattab; /* Local restrict pointer only when data available (has been received) */

      vertsndnbr =
      edgesndnbr = 0;
      for (requrcvnum = requnxtnum = vrcvidxnnd, vrcvidxnnd += (statsiz / 2); /* TRICK: each message item costs 2 Gnum's */
           requrcvnum < vrcvidxnnd; requrcvnum ++) {
        Gnum                vertglbnum;           /* Our global number (the one seen as mate by sender)   */
        Gnum                vertlocnum;           /* Our local number (the one seen as mate by sender)    */
        Gnum                vmatglbnum;           /* Global number of requesting mate (sender of message) */
        Gnum                mategstnum;           /* The mate we wanted to ask for                        */

        vmatglbnum = vrcvdattab[requrcvnum].datatab[0]; /* Names are opposite because receiving side */
        vertglbnum = vrcvdattab[requrcvnum].datatab[1];
        vertlocnum = vertglbnum - vertlocadj;
#ifdef SCOTCH_DEBUG_DGRAPH2
        if ((vertlocnum <  grafptr->baseval) ||   /* If matching request is not directed towards our process */
            (vertlocnum >= grafptr->vertlocnnd)) {
          errorPrint ("dgraphMatchSync: internal error (5)");
          return     (1);
        }
#endif /* SCOTCH_DEBUG_DGRAPH2 */

        mategstnum = mategsttax[vertlocnum];      /* Get our local mating decision data        */
        if (mategstnum == -1) {                   /* If local vertex wanted for mating is free */
          Gnum                edgelocnum;

          for (edgelocnum = vertloctax[vertlocnum]; edgeloctax[edgelocnum] != vmatglbnum; edgelocnum ++) {
#ifdef SCOTCH_DEBUG_DGRAPH2
            if (edgelocnum >= vendloctax[vertlocnum]) {
              errorPrint ("dgraphMatchSync: internal error (6)");
              return     (1);
            }
#endif /* SCOTCH_DEBUG_DGRAPH2 */
          }
          mategsttax[edgegsttax[edgelocnum]] = vertglbnum; /* We are no longer free        */
          mategsttax[vertlocnum] = vmatglbnum;    /* Leave message as is to acknowledge it */
          matelocnbr ++;
          vertsndnbr ++;
          edgesndnbr += vendloctax[vertlocnum] - vertloctax[vertlocnum];
        }
        else if (mategstnum < -1) {               /* If local vertex is also asking for mating */
          Gnum                edgelocnum;
          Gnum                mateglbnum;

          edgelocnum = -2 - mategstnum;
          mateglbnum = edgeloctax[edgelocnum];    /* Get global number of our remote mate       */
          if (mateglbnum == vmatglbnum) {         /* If it is with the sender                   */
            Gnum                flagval;          /* Flag for choosing side to create multinode */

            mategsttax[vertlocnum] = mateglbnum;  /* Say we are mated to inform future requesting processes in same pass */
            mategsttax[edgegsttax[edgelocnum]] = vertglbnum;
            flagval = (mateglbnum > vertglbnum) ? 1 : 0; /* Compute pseudo-random flag always opposite for both ends */
            flagval = ((mateglbnum + (mateglbnum - vertglbnum) * flagval) & 1) ^ flagval;
            if (flagval == 0) {                   /* If flag is even, create multinode */
              multloctab[multlocnbr].vertglbnum[0] = vertglbnum;
              multloctab[multlocnbr].vertglbnum[1] = mategstnum; /* Remote mate: negative value */
              multlocnbr ++;                      /* One more coarse vertex created             */
            edgekptnbr += vendloctax[vertlocnum] - vertloctax[vertlocnum];
            }
            else {                                /* If flag is odd, prepare to send vertex data at build time */
              vertsndnbr ++;
              edgesndnbr += vendloctax[vertlocnum] - vertloctax[vertlocnum];
            }                                     /* Go on by destroying message in all cases since both ends know what it is about */

            vrcvdattab[requrcvnum --] = vrcvdattab[-- vrcvidxnnd]; /* Replace current message with another one and process it */
            matelocnbr ++;                        /* One more local vertex mated on each side; no messages will tell it       */
          }
          else {                                  /* If willing to mate but not with the sender, tell later with whom */
            DgraphCoarsenVert   vertdat;          /* Temporary storage data for swapping vertices                     */

            vertdat = vrcvdattab[requnxtnum];     /* Pack requests to process later at beginning of message */
            vrcvdattab[requnxtnum].datatab[0] = vmatglbnum;
            vrcvdattab[requnxtnum].datatab[1] = -2 - vertlocnum; /* Build appropriate answer to mating request later, when all messages arrived */
            if (requnxtnum ++ != requrcvnum)
              vrcvdattab[requrcvnum] = vertdat;   /* Swap vertices if not already at the right place */
          }
        }
        else                                      /* If already matched, inform sender */
          vrcvdattab[requrcvnum].datatab[1] = mategstnum;
      }
      mateptr->c.dcntloctab[grafptr->procngbtab[procngbnum]].vertsndnbr += vertsndnbr;
      mateptr->c.dcntloctab[grafptr->procngbtab[procngbnum]].edgesndnbr += edgesndnbr;
    }
    mateptr->c.vrcvidxtab[procngbnum] = vrcvidxnnd;
  }

  if (MPI_Waitall (procngbnbr, mateptr->c.vsndreqtab, MPI_STATUSES_IGNORE) != MPI_SUCCESS) { /* Wait for send requests of mating requests to complete */
    errorPrint ("dgraphMatchSync: communication error (5)");
    return     (1);
  }

#ifdef SCOTCH_DEBUG_DGRAPH2
  if (MPI_Barrier (grafptr->proccomm) != MPI_SUCCESS) {
    errorPrint ("dgraphMatchSync: communication error (6)");
    return     (1);
  }
#endif /* SCOTCH_DEBUG_DGRAPH2 */

  vrcvreqnbr = procngbnbr;                        /* Count number of receive requests we will have to consider */
  procngbnum = mateptr->c.procngbnxt;             /* Post receives of mating replies in descending order       */
  do {
    int                 procglbnum;

    procngbnum = (procngbnum + (procngbnbr - 1)) % procngbnbr; /* Pre-decrement neighbor rank */

    if (vsndidxtab[procngbnum] <= mateptr->c.vsnddsptab[procngbnum]) { /* If we had sent an empty query message, do not expect any reply */
      mateptr->c.vrcvreqtab[procngbnum] = MPI_REQUEST_NULL;
      vrcvreqnbr --;                                 /* One less receive request to wait for */
      continue;
    }
    procglbnum = grafptr->procngbtab[procngbnum];
    if (MPI_Irecv (vsnddattab + mateptr->c.vsnddsptab[procngbnum],
                   2 * (mateptr->c.vsnddsptab[procngbnum + 1] - mateptr->c.vsnddsptab[procngbnum]), GNUM_MPI,
                   procglbnum, TAGMATCH + 1, grafptr->proccomm, &mateptr->c.vrcvreqtab[procngbnum]) != MPI_SUCCESS) {
      errorPrint ("dgraphMatchSync: communication error (7)");
      return     (1);
    }
  } while (procngbnum != mateptr->c.procngbnxt);

  procngbnum = mateptr->c.procngbnxt;             /* Post sends of mating replies in ascending order */
  do {
    int                 procglbnum;
    int                 vsndidxnnd;

    procglbnum = grafptr->procngbtab[procngbnum];
    vsndidxnnd = mateptr->c.vrcvidxtab[procngbnum]; /* Re-send (or not) the messages we have received to acknowledge             */
    if (vsndidxnnd >= 0) {                        /* If we had received a non-empty message (but reply might be empty)           */
      DgraphCoarsenVert * restrict const  vrcvdattab = mateptr->c.vrcvdattab; /* Local restrict pointer only when data available */
      int                                 vsndidxnum;

      for (vsndidxnum = mateptr->c.vrcvdsptab[procngbnum]; /* Finalize unfinished messages */
           vsndidxnum < vsndidxnnd; vsndidxnum ++) {
        Gnum                vertlocnum;
        Gnum                mateglbnum;

        vertlocnum = vrcvdattab[vsndidxnum].datatab[1];
        if (vertlocnum >= 0)                      /* If no more unfinished messages to process, quit scanning */
          break;
        vertlocnum = -2 - vertlocnum;
        mateglbnum = mategsttax[vertlocnum];
        if (mateglbnum >= 0)                      /* If vertex we wanted to mate with has been mated in this round */
          vrcvdattab[vsndidxnum].datatab[1] = mateglbnum; /* Propagate this information back to the requester      */
        else {                                    /* Vertex mating data not yet available (maybe in answer)        */
          vrcvdattab[vsndidxnum] = vrcvdattab[-- vsndidxnnd]; /* Remove message as no reply means not willing      */
          if (vrcvdattab[vsndidxnum].datatab[1] < 0) /* If replacing message is also to be processed               */
            vsndidxnum --;                        /* Do not skip replaced message in next iteration                */
        }
      }

      if (MPI_Isend (vrcvdattab + mateptr->c.vrcvdsptab[procngbnum], 2 * (vsndidxnnd - mateptr->c.vrcvdsptab[procngbnum]), GNUM_MPI,
                     procglbnum, TAGMATCH + 1, grafptr->proccomm, &mateptr->c.vsndreqtab[procngbnum]) != MPI_SUCCESS) {
        errorPrint ("dgraphMatchSync: communication error (8)");
        return     (1);
      }
    }
#ifdef SCOTCH_DEBUG_DGRAPH2
    else {
      if (mateptr->c.vsndreqtab[procngbnum] != MPI_REQUEST_NULL) { /* Should have been set by previous MPI_Waitall() */
        errorPrint ("dgraphMatchSync: internal error (7)");
        return     (1);
      }
    }
#endif /* SCOTCH_DEBUG_DGRAPH2 */

    procngbnum = (procngbnum + 1) % procngbnbr;   /* Post-increment neighbor rank */
  } while (procngbnum != mateptr->c.procngbnxt);

#ifdef SCOTCH_DETERMINISTIC
  vrcvreqnbr = procngbnbr;                           /* For deterministic behavior, consider all neighbors in order, whether communicating or not */
#endif /* SCOTCH_DETERMINISTIC */
  for ( ; vrcvreqnbr > 0; vrcvreqnbr --) {              /* For all pending receive requests */
    int                 vrcvidxnnd;
    int                 vrcvidxnum;
    int                 procngbnum;
    MPI_Status          statdat;
    int                 statsiz;
    int                 o;

#ifdef SCOTCH_DETERMINISTIC
    procngbnum = vrcvreqnbr - 1;
    if (mateptr->c.vrcvreqtab[procngbnum] == MPI_REQUEST_NULL) /* If we do not expect this message, skip it */
      continue;
    o = MPI_Wait (&mateptr->c.vrcvreqtab[procngbnum], &statdat);
#else /* SCOTCH_DETERMINISTIC */
    o = MPI_Waitany (procngbnbr, mateptr->c.vrcvreqtab, &procngbnum, &statdat);
#endif /* SCOTCH_DETERMINISTIC */
    if ((o != MPI_SUCCESS) ||
        (MPI_Get_count (&statdat, GNUM_MPI, &statsiz) != MPI_SUCCESS)) {
      errorPrint ("dgraphMatchSync: communication error (9)");
      return     (1);
    }

    for (vrcvidxnum = mateptr->c.vsnddsptab[procngbnum], vrcvidxnnd = vrcvidxnum + (statsiz / 2); /* TRICK: each message item costs 2 Gnum's */
         vrcvidxnum < vrcvidxnnd; vrcvidxnum ++) {
      Gnum                edgelocnum;
      Gnum                vertglbnum;             /* Our global number (the one seen as mate by sender) */
      Gnum                vertlocnum;             /* Our local number (the one seen as mate by sender)  */
      Gnum                vmatglbnum;             /* Global number of vertex to which the mate is mated */
      Gnum                mategstnum;             /* The mate we wanted to ask for                      */

      vertglbnum = vsnddattab[vrcvidxnum].datatab[0];
      vmatglbnum = vsnddattab[vrcvidxnum].datatab[1];
      vertlocnum = vertglbnum - vertlocadj;
#ifdef SCOTCH_DEBUG_DGRAPH2
      if ((vertlocnum <  grafptr->baseval) ||     /* If matching reply is not directed towards our process */
          (vertlocnum >= grafptr->vertlocnnd)) {
        errorPrint ("dgraphMatchSync: internal error (8)");
        return     (1);
      }
#endif /* SCOTCH_DEBUG_DGRAPH2 */

      mategstnum = mategsttax[vertlocnum];        /* Get our local mating decision data */
      edgelocnum = -2 - mategstnum;
#ifdef SCOTCH_DEBUG_DGRAPH2
      if ((mategstnum >= -1) ||                   /* If we did not ask anything or if we were already matched, no reply message should come to us */
          ((mategsttax[edgegsttax[edgelocnum]] >= 0) && /* Also, if our prospective mate was itself already set as matched by a previous reply    */
           (mategsttax[edgegsttax[edgelocnum]] != vertglbnum) && /* And this message is not the positive reply which acknowledges this mating     */
           (mategsttax[edgegsttax[edgelocnum]] != vmatglbnum))) { /* Or an informative negative reply which gives again the mate of the ghost     */
        errorPrint ("dgraphMatchSync: internal error (9)");
        return     (1);
      }
#endif /* SCOTCH_DEBUG_DGRAPH2 */
      if (edgeloctax[edgelocnum] == vmatglbnum) { /* If positive answer from the mate we wanted */
        mategsttax[vertlocnum] = vmatglbnum;      /* Set local vertex as matched with the mate  */
        mategsttax[edgegsttax[edgelocnum]] = vertglbnum; /* Update state of ghost mate          */
        multloctab[multlocnbr].vertglbnum[0] = vertglbnum;
        multloctab[multlocnbr].vertglbnum[1] = mategstnum; /* Remote mate: negative value */
        multlocnbr ++;                            /* One more coarse vertex created       */
        matelocnbr ++;
        edgekptnbr += vendloctax[vertlocnum] - vertloctax[vertlocnum];
      }
      else {                                      /* If negative answer from the mate we wanted  */
        mategsttax[vertlocnum] = -1;              /* Reset local vertex as free for mating       */
        mategsttax[edgegsttax[edgelocnum]] = vmatglbnum; /* Update state of unwilling ghost mate */
      }
    }
  }

  mateptr->matelocnbr   = matelocnbr;
  mateptr->c.multlocnbr = multlocnbr;
  mateptr->c.edgekptnbr = edgekptnbr;

  if (MPI_Waitall (procngbnbr, mateptr->c.vsndreqtab, MPI_STATUSES_IGNORE) != MPI_SUCCESS) { /* Wait for send requests of mating requests to complete */
    errorPrint ("dgraphMatchSync: communication error (10)");
    return     (1);
  }

#ifdef SCOTCH_DEBUG_DGRAPH2
  if (MPI_Barrier (grafptr->proccomm) != MPI_SUCCESS) {
    errorPrint ("dgraphMatchSync: communication error (11)");
    return     (1);
  }
#endif /* SCOTCH_DEBUG_DGRAPH2 */

  return (0);
}

/* These routines perform a round of communication
** to synchronize enqueued matching requests across
** processors.
** They return:
** - 0   : on success.
** - !0  : on error.
*/

#define DGRAPHMATCHSCANNAME         dgraphMatchSc /* Scan matching (no edge weights) */
#define DGRAPHMATCHSCANINIT
#define DGRAPHMATCHSCANCOUNTINIT                                          \
      probval = intRandVal (32768);               /* Get proba for this vertex */
#define DGRAPHMATCHSCANCOUNTSELECT                                        \
          edgefrenbr ++;
#define DGRAPHMATCHSCANFINDSELECT                                         \
          (edgefrenbr -- == 0)
#include "dgraph_match_scan.c"
#undef DGRAPHMATCHSCANFINDSELECT
#undef DGRAPHMATCHSCANCOUNTSELECT
#undef DGRAPHMATCHSCANCOUNTINIT
#undef DGRAPHMATCHSCANINIT
#undef DGRAPHMATCHSCANNAME

#define DGRAPHMATCHSCANNAME         dgraphMatchHy /* Heavy edge matching */
#define DGRAPHMATCHSCANINIT                                                     \
  const Gnum * restrict const edloloctax = mateptr->c.finegrafptr->edloloctax;  \
  if (edloloctax == NULL) {                                                     \
    dgraphMatchSc (mateptr);                                                    \
    return;                                                                     \
  }
#define DGRAPHMATCHSCANCOUNTINIT                                                \
      Gnum                edlolocmax;                                           \
      edlolocmax = 0;                                                           \
      probval = intRandVal (32768);               /* Get proba for this vertex */
#define DGRAPHMATCHSCANCOUNTSELECT                                              \
          Gnum                edlolocval;                                       \
          edlolocval = edloloctax[edgelocnum];                                  \
          if (edlolocval > edlolocmax) {                                        \
            edlolocmax = edlolocval;                                            \
            edgefrenbr = 1;                                                     \
          }                                                                     \
          else if (edlolocval == edlolocmax)                                    \
            edgefrenbr ++;
#define DGRAPHMATCHSCANFINDSELECT                                               \
          ((edloloctax[edgelocnum] == edlolocmax) &&                            \
           (edgefrenbr -- == 0))
#include "dgraph_match_scan.c"
#undef DGRAPHMATCHSCANFINDSELECT
#undef DGRAPHMATCHSCANCOUNTSELECT
#undef DGRAPHMATCHSCANCOUNTINIT
#undef DGRAPHMATCHSCANINIT
#undef DGRAPHMATCHSCANNAME

#define DGRAPHMATCHSCANNAME         dgraphMatchLc /* Local scan matching */
#define DGRAPHMATCHSCANINIT
#define DGRAPHMATCHSCANCOUNTINIT                                                \
      probval = 0;                                /* Vertices will always be active */
#define DGRAPHMATCHSCANCOUNTSELECT                                              \
          if (vertgstend < vertlocnnd)                                          \
            edgefrenbr ++;                                                      \
          else                                                                  \
            edgeendnbr --;
#define DGRAPHMATCHSCANFINDSELECT                                               \
          ((vertgstend < vertlocnnd) &&                                         \
           (edgefrenbr -- == 0))
#include "dgraph_match_scan.c"
#undef DGRAPHMATCHSCANFINDSELECT
#undef DGRAPHMATCHSCANCOUNTSELECT
#undef DGRAPHMATCHSCANCOUNTINIT
#undef DGRAPHMATCHSCANINIT
#undef DGRAPHMATCHSCANNAME

#define DGRAPHMATCHSCANNAME         dgraphMatchLy /* Local heavy edge matching */
#define DGRAPHMATCHSCANINIT                                                     \
  const Gnum * restrict const edloloctax = mateptr->c.finegrafptr->edloloctax;  \
  if (edloloctax == NULL) {                                                     \
    dgraphMatchLc (mateptr);                                                    \
    return;                                                                     \
  }
#define DGRAPHMATCHSCANCOUNTINIT                                                \
      Gnum                edlolocmax;                                           \
      edlolocmax = 0;                                                           \
      probval = 0;                                /* Vertices will always be active */
#define DGRAPHMATCHSCANCOUNTSELECT                                              \
          if (vertgstend < vertlocnnd) {                                        \
            Gnum                edlolocval;                                     \
            edlolocval = edloloctax[edgelocnum];                                \
            if (edlolocval > edlolocmax) {                                      \
              edlolocmax = edlolocval;                                          \
              edgefrenbr = 1;                                                   \
            }                                                                   \
            else if (edlolocval == edlolocmax)                                  \
              edgefrenbr ++;                                                    \
          }                                                                     \
          else                                                                  \
            edgeendnbr --;
#define DGRAPHMATCHSCANFINDSELECT                                               \
          ((vertgstend < vertlocnnd) &&                                         \
           (edloloctax[edgelocnum] == edlolocmax) &&                            \
           (edgefrenbr -- == 0))
#include "dgraph_match_scan.c"
#undef DGRAPHMATCHSCANFINDSELECT
#undef DGRAPHMATCHSCANCOUNTSELECT
#undef DGRAPHMATCHSCANCOUNTINIT
#undef DGRAPHMATCHSCANINIT
#undef DGRAPHMATCHSCANNAME
