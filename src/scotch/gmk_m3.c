/* Copyright 2004,2007,2008,2010-2012,2014,2018,2019,2023 IPB, Universite de Bordeaux, INRIA & CNRS
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
/**   NAME       : gmk_m3.c                                **/
/**                                                        **/
/**   AUTHOR     : Francois PELLEGRINI                     **/
/**                                                        **/
/**   FUNCTION   : Creates the source graph for            **/
/**                tridimensional mesh source graphs.      **/
/**                                                        **/
/**   DATES      : # Version 4.0  : from : 11 feb 2002     **/
/**                                 to   : 18 may 2004     **/
/**                # Version 5.0  : from : 13 dec 2007     **/
/**                                 to   : 16 mar 2008     **/
/**                # Version 5.1  : from : 01 jul 2010     **/
/**                                 to   : 14 feb 2011     **/
/**                # Version 6.0  : from : 01 jan 2012     **/
/**                                 to   : 24 sep 2019     **/
/**                # Version 7.0  : from : 02 apr 2021     **/
/**                                 to   : 21 jan 2023     **/
/**                                                        **/
/**   NOTES      : # The vertices of the (dX,dY,dZ) mesh   **/
/**                  are numbered as terminals so that     **/
/**                  t(0,0,0) = 0, t(1,0,0) = 1,           **/
/**                  t(dX - 1, 0, 0) = dX - 1, t(0,1,0) =  **/
/**                  dX, t (0, 0, 1) = dX * dY - 1,        **/
/**                  and t(x,y,z) = (z * dY + y) * dX + x. **/
/**                                                        **/
/************************************************************/

/*
**  The defines and includes.
*/

#include "module.h"
#include "common.h"
#include "scotch.h"
#include "gmk_m3.h"

/*
**  The static definitions.
*/

static int                  C_paraNum = 0;        /* Number of parameters       */
static int                  C_fileNum = 0;        /* Number of file in arg list */
static File                 C_fileTab[C_FILENBR] = { /* The file array          */
                              { FILEMODEW },
                              { FILEMODEW } };

static const char *         C_usageList[] = {
  "gmk_m3 <dimX> [<dimY> [<dimZ> [<output source file>]]] <options>",
  "  -b<val>   : Set base value for output (0 or 1)",
  "  -g<file>  : Output the geometry to <file>",
  "  -h        : Display this help",
  "  -t        : Build a torus rather than a grid",
  "  -V        : Print program version and copyright",
  "  -y        : Invert y coordinate in geometry",
  NULL };

/****************************************/
/*                                      */
/* The main routine, which computes the */
/* source graph description.            */
/*                                      */
/****************************************/

int
main (
int                         argc,
char *                      argv[])
{
  int                 flagval;                    /* Process flags      */
  SCOTCH_Num          baseval;                    /* Base value         */
  SCOTCH_Num          d[3] = { 1, 1, 1 };         /* Mesh dimensions    */
  SCOTCH_Num          c[3];                       /* Vertex coordinates */
  int                 i;

  errorProg ("gmk_m3");

  flagval = C_FLAGDEFAULT;                        /* Set default flags */
  baseval = 0;

  if ((argc >= 2) && (argv[1][0] == '?')) {       /* If need for help */
    usagePrint (stdout, C_usageList);
    return     (EXIT_SUCCESS);
  }

  fileBlockInit (C_fileTab, C_FILENBR);           /* Set default stream pointers */

  for (i = 1; i < argc; i ++) {                   /* Loop for all option codes                        */
    if ((argv[i][0] != '-') || (argv[i][1] == '\0') || (argv[i][1] == '.')) { /* If found a file name */
      if (C_paraNum < 3) {                        /* If number of parameters not reached              */
        if ((d[C_paraNum ++] = atoi (argv[i])) < 1) /* Get the dimension                              */
          errorPrint ("main: invalid dimension '%s'", argv[i]);
        continue;                                 /* Process the other parameters */
      }
      if (C_fileNum < C_FILEARGNBR)               /* A file name has been given */
        fileBlockName (C_fileTab, C_fileNum ++) = argv[i];
      else
        errorPrint ("main: too many file names given");
    }
    else {                                        /* If found an option name */
      switch (argv[i][1]) {
        case 'B' :                                /* Set base value */
        case 'b' :
          baseval = (SCOTCH_Num) atol (&argv[i][2]);
          if ((baseval < 0) || (baseval > 1)) {
            errorPrint ("main: invalid base value '" SCOTCH_NUMSTRING "'", (SCOTCH_Num) baseval);
          }
          break;
        case 'G' :                                /* Output the geometry */
        case 'g' :
          flagval |= C_FLAGGEOOUT;
          if (argv[i][2] != '\0')
            C_filenamegeoout = &argv[i][2];
          break;
        case 'H' :                                /* Give the usage message */
        case 'h' :
          usagePrint (stdout, C_usageList);
          return     (EXIT_SUCCESS);
        case 'T' :                                /* Build a torus */
        case 't' :
          flagval |= C_FLAGTORUS;
          break;
        case 'V' :
          fprintf (stderr, "gmk_m3, version " SCOTCH_VERSION_STRING "\n");
          fprintf (stderr, SCOTCH_COPYRIGHT_STRING "\n");
          fprintf (stderr, SCOTCH_LICENSE_STRING "\n");
          return  (EXIT_SUCCESS);
        case 'Y' :                                /* Invert the y coordinate in geometry file */
        case 'y' :
          flagval |= C_FLAGGEOINVY;
          break;
        default :
          errorPrint ("main: unprocessed option '%s'", argv[i]);
      }
    }
  }

  fileBlockOpen (C_fileTab, C_FILENBR);           /* Open all files */

  if (flagval & C_FLAGTORUS) {                    /* Build a torus */
    fprintf (C_filepntrsrcout, "0\n" SCOTCH_NUMSTRING "\t" SCOTCH_NUMSTRING "\n" SCOTCH_NUMSTRING "\t000\n",
             (SCOTCH_Num) (d[0] * d[1] * d[2]),   /* Print number of vertices              */
             (SCOTCH_Num) ((6 * d[0] * d[1] * d[2])      - /* Print number of edges (arcs) */
                           ((d[0] < 3) ? (2 * d[1] * d[2]) : 0) -
                           ((d[1] < 3) ? (2 * d[0] * d[2]) : 0) -
                           ((d[2] < 3) ? (2 * d[0] * d[1]) : 0)),
             (SCOTCH_Num) baseval);

    for (c[2] = 0; c[2] < d[2]; c[2] ++) {        /* Output neighbor list */
      for (c[1] = 0; c[1] < d[1]; c[1] ++) {
        for (c[0] = 0; c[0] < d[0]; c[0] ++) {
          fprintf (C_filepntrsrcout, SCOTCH_NUMSTRING,
                   (SCOTCH_Num) (((d[0] > 2) ? 3 : d[0]) + /* Output number of neighbors */
                                 ((d[1] > 2) ? 3 : d[1]) +
                                 ((d[2] > 2) ? 3 : d[2]) - 3));
          if (d[2] > 2)
            fprintf (C_filepntrsrcout, "\t" SCOTCH_NUMSTRING, /* Output the neighbors */
                     (SCOTCH_Num) ((((c[2] + d[2] - 1) % d[2]) * d[1] + c[1]) * d[0] + c[0] + baseval));
          if (d[1] > 2)
            fprintf (C_filepntrsrcout, "\t" SCOTCH_NUMSTRING,
                     (SCOTCH_Num) ((c[2] * d[1] + ((c[1] + d[1] - 1) % d[1])) * d[0] + c[0] + baseval));
          if (d[0] > 2)
            fprintf (C_filepntrsrcout, "\t" SCOTCH_NUMSTRING,
                     (SCOTCH_Num) (((c[2] * d[1] + c[1]) * d[0] + (c[0] + d[0] - 1) % d[0])) + baseval);
          if (d[0] > 1)
            fprintf (C_filepntrsrcout, "\t" SCOTCH_NUMSTRING,
                     (SCOTCH_Num) ((c[2] * d[1] + c[1]) * d[0] + ((c[0] + 1) % d[0]) + baseval));
          if (d[1] > 1)
            fprintf (C_filepntrsrcout, "\t" SCOTCH_NUMSTRING,
                     (SCOTCH_Num) ((c[2] * d[1] + ((c[1] + 1) % d[1])) * d[0] + c[0] + baseval));
          if (d[2] > 1)
            fprintf (C_filepntrsrcout, "\t" SCOTCH_NUMSTRING,
                     (SCOTCH_Num) ((((c[2] + 1) % d[2]) * d[1] + c[1]) * d[0] + c[0] + baseval));
          fprintf (C_filepntrsrcout, "\n");
        }
      }
    }
  }
  else {                                          /* Build a mesh */
    fprintf (C_filepntrsrcout, "0\n" SCOTCH_NUMSTRING "\t" SCOTCH_NUMSTRING "\n" SCOTCH_NUMSTRING "\t000\n",
             (SCOTCH_Num) (d[0] * d[1] * d[2]),
             (SCOTCH_Num) ((d[0] * d[1] * d[2] * 3 - (d[0] * d[1] + d[0] * d[2] + d[1] * d[2])) * 2),
             (SCOTCH_Num) baseval);

    for (c[2] = 0; c[2] < d[2]; c[2] ++) {        /* Output neighbor list */
      for (c[1] = 0; c[1] < d[1]; c[1] ++) {
        for (c[0] = 0; c[0] < d[0]; c[0] ++) {
          fprintf (C_filepntrsrcout, "%d",
                   ((c[0] == 0)          ? 0 : 1) + /* Output number of neighbors */
                   ((c[0] == (d[0] - 1)) ? 0 : 1) +
                   ((c[1] == 0)          ? 0 : 1) +
                   ((c[1] == (d[1] - 1)) ? 0 : 1) +
                   ((c[2] == 0)          ? 0 : 1) +
                   ((c[2] == (d[2] - 1)) ? 0 : 1));
          if (c[2] != 0)                          /* Output the neighbors */
            fprintf (C_filepntrsrcout, "\t" SCOTCH_NUMSTRING,
                     (SCOTCH_Num) (((c[2] - 1) * d[1] + c[1]) * d[0] + c[0] + baseval));
          if (c[1] != 0)
            fprintf (C_filepntrsrcout, "\t" SCOTCH_NUMSTRING,
                     (SCOTCH_Num) ((c[2] * d[1] + c[1] - 1) * d[0] + c[0] + baseval));
          if (c[0] != 0)
            fprintf (C_filepntrsrcout, "\t" SCOTCH_NUMSTRING,
                     (SCOTCH_Num) ((c[2] * d[1] + c[1]) * d[0] + c[0] - 1 + baseval));
          if (c[0] != (d[0] - 1))
            fprintf (C_filepntrsrcout, "\t" SCOTCH_NUMSTRING,
                     (SCOTCH_Num) ((c[2] * d[1] + c[1]) * d[0] + c[0] + 1 + baseval));
          if (c[1] != (d[1] - 1))
            fprintf (C_filepntrsrcout, "\t" SCOTCH_NUMSTRING,
                     (SCOTCH_Num) ((c[2] * d[1] + c[1] + 1) * d[0] + c[0] + baseval));
          if (c[2] != (d[2] - 1))
            fprintf (C_filepntrsrcout, "\t" SCOTCH_NUMSTRING,
                     (SCOTCH_Num) (((c[2] + 1) * d[1] + c[1]) * d[0] + c[0] + baseval));
          fprintf (C_filepntrsrcout, "\n");
        }
      }
    }
  }

  if (flagval & C_FLAGGEOOUT) {                   /* If geometry is wanted                */
   fprintf (C_filepntrgeoout, "3\n" SCOTCH_NUMSTRING "\n", /* Output geometry file header */
            (SCOTCH_Num) (d[0] * d[1] * d[2]));

    for (c[2] = 0; c[2] < d[2]; c[2] ++) {        /* Output mesh coordinates */
      for (c[1] = 0; c[1] < d[1]; c[1] ++) {
        for (c[0] = 0; c[0] < d[0]; c[0] ++)
          fprintf (C_filepntrgeoout, SCOTCH_NUMSTRING "\t" SCOTCH_NUMSTRING "\t" SCOTCH_NUMSTRING "\t" SCOTCH_NUMSTRING "\n",
                   (SCOTCH_Num) ((c[2] * d[1] + c[1]) * d[0] + c[0] + baseval),
                   (SCOTCH_Num) c[0],
                   (SCOTCH_Num) ((flagval & C_FLAGGEOINVY) ? (d[1] - 1 - c[1]) : c[1]),
                   (SCOTCH_Num) c[2]);
      }
    }
  }

  fileBlockClose (C_fileTab, C_FILENBR);          /* Always close explicitely to end eventual (un)compression tasks */

  return (EXIT_SUCCESS);
}
