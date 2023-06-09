/* Copyright 2004,2007,2008,2010-2012,2014,2018-2021,2023 IPB, Universite de Bordeaux, INRIA & CNRS
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
/**   NAME       : gmap.c                                  **/
/**                                                        **/
/**   AUTHOR     : Francois PELLEGRINI                     **/
/**                Sebastien FOURESTIER (v6.0)             **/
/**                                                        **/
/**   FUNCTION   : Part of a graph mapping software.       **/
/**                This module contains the main function. **/
/**                                                        **/
/**   DATES      : # Version 0.0  : from : 05 jan 1993     **/
/**                                 to   : 12 may 1993     **/
/**                # Version 1.1  : from : 15 oct 1993     **/
/**                                 to   : 15 oct 1993     **/
/**                # Version 1.3  : from : 06 apr 1994     **/
/**                                 to   : 18 may 1994     **/
/**                # Version 2.0  : from : 06 jun 1994     **/
/**                                 to   : 17 nov 1994     **/
/**                # Version 2.1  : from : 07 apr 1995     **/
/**                                 to   : 18 jun 1995     **/
/**                # Version 3.0  : from : 01 jul 1995     **/
/**                                 to   : 02 oct 1995     **/
/**                # Version 3.1  : from : 07 nov 1995     **/
/**                                 to   : 25 apr 1996     **/
/**                # Version 3.2  : from : 24 sep 1996     **/
/**                                 to   : 26 may 1998     **/
/**                # Version 3.3  : from : 19 oct 1998     **/
/**                                 to   : 30 mar 1999     **/
/**                # Version 3.4  : from : 03 feb 2000     **/
/**                                 to   : 03 feb 2000     **/
/**                # Version 4.0  : from : 16 jan 2004     **/
/**                                 to   : 27 dec 2004     **/
/**                # Version 5.0  : from : 23 dec 2007     **/
/**                                 to   : 18 jun 2008     **/
/**                # Version 5.1  : from : 30 jun 2010     **/
/**                                 to   : 31 aug 2011     **/
/**                # Version 6.0  : from : 29 may 2010     **/
/**                                 to   : 26 aug 2020     **/
/**                # Version 7.0  : from : 10 oct 2021     **/
/**                                 to   : 21 jan 2023     **/
/**                                                        **/
/************************************************************/

/*
**  The defines and includes.
*/

#include "module.h"
#include "common.h"
#include "scotch.h"
#include "gmap.h"

/*
**  The static variables.
*/

static int                  C_partNbr = 1;        /* Default number of parts / cluster size */
static int                  C_paraNum = 0;        /* Number of parameters                   */
static int                  C_paraNbr = 0;        /* No parameters for mapping              */
static int                  C_fileNum = 0;        /* Number of file in arg list             */
static int                  C_fileNbr = 4;        /* Number of files for mapping            */
static File                 C_fileTab[C_FILENBR] = { /* File array                          */
                              { FILEMODER },
                              { FILEMODER },
                              { FILEMODEW },
                              { FILEMODEW },
                              { FILEMODER },
                              { FILEMODER },
                              { FILEMODEW } };

static const char *         C_usageList[] = {     /* Usage */
  "gmap [<input source file> [<input target file> [<output mapping file> [<output log file>]]]] <options>",
  "gpart [<nparts/pwght>] [<input source file> [<output mapping file> [<output log file>]]] <options>",
  "  -b<val>    : Load imbalance tolerance (default: 0.05)",
  "  -C<opt>    : Choose execution context options according to one or several of <opt>:",
  "                 d  : deterministic behavior (even across multiple threads; implies 'f')",
  "                 f  : fixed random seed",
  "                 r  : variable random seed",
  "                 u  : undeterministic behavior (may be faster with several threads)",
  "                 Default behavior depends on compilation flags",
  "  -c<opt>    : Choose default mapping strategy according to one or several of <opt>:",
  "                 b  : enforce load balance as much as possible",
  "                 q  : privilege quality over speed (default)",
  "                 r  : use only recursive bipartitioning",
  "                 s  : privilege speed over quality",
  "                 t  : enforce safety",
  "  -f<file>   : Fixed vertices input file",
  "  -h         : Display this help",
  "  -m<strat>  : Set mapping strategy (see user's manual)",
  "  -o         : Use partitioning with overlap (only for gpart)",
  "  -q         : Do graph clustering instead of graph partitioning (for gpart)",
  "  -q<pwght>  : Do graph clustering instead of static mapping (for gmap)",
  "  -ro<file>  : Old mapping input file (for remapping)",
  "  -rr<val>   : Edge migration ratio (for remapping, default: 1)",
  "  -rv<file>  : Vertex migration cost input file (for remapping)",
  "  -s<obj>    : Force unity weights on <obj>:",
  "                 e  : edges",
  "                 v  : vertices",
  "  -V         : Print program version and copyright",
  "  -v<verb>   : Set verbose mode to <verb>:",
  "                 m  : mapping information",
  "                 s  : strategy information",
  "                 t  : timing information",
  "",
  "See default strategy with option '-vs'",
  NULL };

/******************************/
/*                            */
/* This is the main function. */
/*                            */
/******************************/

int
main (
int                         argc,
char *                      argv[])
{
  SCOTCH_Context        contdat;                  /* Execution context              */
  SCOTCH_Graph          cogrdat;                  /* Context graph binding          */
  SCOTCH_Graph          grafdat;                  /* Source graph                   */
  SCOTCH_Num            grafflag;                 /* Source graph properties        */
  SCOTCH_Arch           archdat;                  /* Target architecture            */
  SCOTCH_Strat          stradat;                  /* Mapping strategy               */
  char *                straptr;                  /* Strategy string to use         */
  SCOTCH_Num            straval;
  SCOTCH_Mapping        mappdat;                  /* Mapping data                   */
  SCOTCH_Mapping        mapodat;                  /* Old mapping data               */
  SCOTCH_Num * restrict parttab;                  /* Partition array                */
  SCOTCH_Num * restrict vmlotab;                  /* Vertex migration cost array    */
  SCOTCH_Num            vertnbr;                  /* Number of graph vertices       */
  Clock                 runtime[2];               /* Timing variables               */
  int                   flagval;
  double                kbalval;                  /* Imbalance tolerance value      */
  double                emraval;                  /* Edge migration ratio           */
  int                   i, j;

  flagval = C_FLAGNONE;                           /* Default behavior               */
  kbalval = 0.01;                                 /* Default imbalance              */
  emraval = 1;                                    /* Default edge migration ratio   */
  straval = 0;                                    /* No strategy flags              */
  straptr = NULL;

  vmlotab = NULL;

#ifdef SCOTCH_COMPILE_PART
  flagval |= C_FLAGPART;
  C_paraNbr = 1;                                  /* One more parameter       */
  C_fileNbr = 3;                                  /* One less file to provide */
  errorProg ("gpart");
#else
  errorProg ("gmap");
#endif /* SCOTCH_COMPILE_PART */

  if ((argc >= 2) && (argv[1][0] == '?')) {       /* If need for help */
    usagePrint (stdout, C_usageList);
    return     (EXIT_SUCCESS);
  }

  grafflag = 0;                                   /* Use vertex and edge weights  */
  SCOTCH_contextInit (&contdat);                  /* Set default context          */
  SCOTCH_stratInit   (&stradat);                  /* Set default mapping strategy */

  fileBlockInit (C_fileTab, C_FILENBR);           /* Set default stream pointers */

  for (i = 1; i < argc; i ++) {                   /* Loop for all option codes                        */
    if ((argv[i][0] != '-') || (argv[i][1] == '\0') || (argv[i][1] == '.')) { /* If found a file name */
      if (C_paraNum < C_paraNbr) {                /* If number of parameters not reached              */
        if ((C_partNbr = atoi (argv[i])) < 1)     /* Get the number of parts                          */
          errorPrint ("main: invalid number of parts '%s'", argv[i]);
        C_paraNum ++;
        continue;                                 /* Process the other parameters */
      }
      if (C_fileNum < C_fileNbr)                  /* A file name has been given */
        fileBlockName (C_fileTab, C_fileNum ++) = argv[i];
      else
        errorPrint ("main: too many file names given");
    }
    else {                                        /* If found an option name */
      switch (argv[i][1]) {
        case 'B' :
        case 'b' :
          flagval |= C_FLAGKBALVAL;
          kbalval = atof (&argv[i][2]);
          if ((kbalval < 0.0) ||
              (kbalval > 1.0) ||
              ((kbalval == 0.0) &&
               ((argv[i][2] != '0') && (argv[i][2] != '.'))))
            errorPrint ("main: invalid load imbalance ratio");
          break;
        case 'C' :
          if (SCOTCH_contextOptionParse (&contdat, &argv[i][2]) != 0)
            errorPrint ("main: invalid context option string");
          break;
        case 'c' :                                /* Strategy selection parameters */
          for (j = 2; argv[i][j] != '\0'; j ++) {
            switch (argv[i][j]) {
              case 'B' :
              case 'b' :
                straval |= SCOTCH_STRATBALANCE;
                break;
              case 'Q' :
              case 'q' :
                straval |= SCOTCH_STRATQUALITY;
                break;
              case 'R' :
              case 'r' :
                straval |= SCOTCH_STRATRECURSIVE;
                break;
              case 'S' :
              case 's' :
                straval |= SCOTCH_STRATSPEED;
                break;
              case 'T' :
              case 't' :
                straval |= SCOTCH_STRATSAFETY;
                break;
              default :
                errorPrint ("main: invalid strategy selection option '%c' after '-C'", argv[i][j]);
            }
          }
          break;
        case 'F' :
        case 'f' :                                /* Fixed vertex file */
          flagval |= C_FLAGFIXED;
          C_filenamevfxinp = &argv[i][2];
          break;
        case 'H' :                                /* Give the usage message */
        case 'h' :
          usagePrint (stdout, C_usageList);
          return     (EXIT_SUCCESS);
        case 'M' :
        case 'm' :
          straptr = &argv[i][2];
          break;
        case 'O' :
        case 'o' :
          flagval |= C_FLAGPARTOVL;
          break;
        case 'Q' :
        case 'q' :
          flagval |= C_FLAGCLUSTER;
          if ((flagval & C_FLAGPART) != 0) {      /* If partitioning program */
            if (argv[i][2] != '\0')
              errorPrint ("main: invalid parameter '%s' after '-q' for gpart", argv[i] + 2);
          }
          else {
            if (argv[i][1] == '\0')
              errorPrint ("main: missing parameter after '-q' for gmap");
            if ((C_partNbr = atoi (argv[i] + 2)) < 1) /* Get maximum cluster load */
              errorPrint ("main: invalid cluster load '%s'", argv[i] + 2);
          }
          break;
        case 'R' :
        case 'r' :                                /* Remapping parameters */
          switch (argv[i][2]) {
            case 'O' :
            case 'o' :                            /* Old mapping input file */
              flagval |= C_FLAGRMAPOLD;
              C_filenamemaoinp = &argv[i][3];
              break;
            case 'R' :
            case 'r' :                            /* Edge migration ratio */
              flagval |= C_FLAGRMAPRAT;
              emraval = atof (&argv[i][3]);
              if (emraval <= 0.0)
                errorPrint ("main: invalid edge migration ratio");
              break;
            case 'V' :
            case 'v' :                            /* Vertex migration cost */
              flagval |= C_FLAGRMAPCST;
              C_filenamevmlinp = &argv[i][3];
              break;
            default :
              errorPrint ("main: invalid remapping option '%c'", argv[i][2]);
          }
          break;
        case 'S' :
        case 's' :                                /* Source graph parameters */
          for (j = 2; argv[i][j] != '\0'; j ++) {
            switch (argv[i][j]) {
              case 'E' :
              case 'e' :
                grafflag |= 2;                    /* Do not load edge weights */
                break;
              case 'V' :
              case 'v' :
                grafflag |= 1;                    /* Do not load vertex weights */
                break;
              default :
                errorPrint ("main: invalid source graph option '%c'", argv[i][j]);
            }
          }
          break;
        case 'V' :
          fprintf (stderr, "gmap/gpart, version " SCOTCH_VERSION_STRING "\n");
          fprintf (stderr, SCOTCH_COPYRIGHT_STRING "\n");
          fprintf (stderr, SCOTCH_LICENSE_STRING "\n");
          return  (EXIT_SUCCESS);
        case 'v' :                                /* Output control info */
          for (j = 2; argv[i][j] != '\0'; j ++) {
            switch (argv[i][j]) {
              case 'M' :
              case 'm' :
                flagval |= C_FLAGVERBMAP;
                break;
              case 'S' :
              case 's' :
                flagval |= C_FLAGVERBSTR;
                break;
              case 'T' :
              case 't' :
                flagval |= C_FLAGVERBTIM;
                break;
              default :
                errorPrint ("main: unprocessed parameter '%c' in '%s'", argv[i][j], argv[i]);
            }
          }
          break;
        default :
          errorPrint ("main: unprocessed option '%s'", argv[i]);
      }
    }
  }

  if ((flagval & C_FLAGPART) != 0) {              /* If program run as the partitioner                           */
    fileBlockName (C_fileTab, 3) = fileBlockName (C_fileTab, 2); /* Put provided file names at their right place */
    fileBlockName (C_fileTab, 2) = fileBlockName (C_fileTab, 1);
    fileBlockName (C_fileTab, 1) = "-";
  }

  fileBlockOpen (C_fileTab, C_FILENBR);           /* Open all files */

  clockInit  (&runtime[0]);
  clockStart (&runtime[0]);

  SCOTCH_graphInit (&grafdat);                    /* Create graph structure         */
  SCOTCH_graphLoad (&grafdat, C_filepntrsrcinp, -1, grafflag); /* Read source graph */

  SCOTCH_archInit (&archdat);                     /* Create architecture structure             */
  if ((flagval & C_FLAGPART) != 0) {              /* If program run as the partitioner         */
    if ((flagval & C_FLAGCLUSTER) != 0)           /* If program run as graph clustering        */
      SCOTCH_archVcmplt (&archdat);               /* Create a variable-sized complete graph    */
    else                                          /* Program is run as plain graph partitioner */
      SCOTCH_archCmplt (&archdat, C_partNbr);     /* Create a complete graph of proper size    */
  }
  else {
    SCOTCH_archLoad (&archdat, C_filepntrtgtinp); /* Read target architecture         */
    if ((flagval & C_FLAGCLUSTER) == 0)           /* If part size not to be preserved */
      C_partNbr = SCOTCH_archSize (&archdat);
    else {
      if (SCOTCH_archVar (&archdat) == 0)
        errorPrint ("main: non variable-sized architecture provided while '-q' flag set");
    }
  }

  if (((flagval & (C_FLAGPART | C_FLAGPARTOVL)) == C_FLAGPARTOVL) && /* If the mapper was asked to compute an overlap   */
      (strcmp (SCOTCH_archName (&archdat), "cmplt") != 0)) /* And the given target architecture is not a complete graph */
    errorPrint ("main: option '-o' only valid for graph partitioning");

  if ((flagval & (C_FLAGPARTOVL | C_FLAGCLUSTER)) == (C_FLAGPARTOVL | C_FLAGCLUSTER))
    errorPrint ("main: options '-o' and '-q' are exclusive");

  if ((flagval & (C_FLAGPARTOVL | C_FLAGFIXED)) == (C_FLAGPARTOVL | C_FLAGFIXED))
    errorPrint ("main: options '-o' and '-f' are exclusive");

  if ((flagval & (C_FLAGPARTOVL | C_FLAGRMAPOLD)) == (C_FLAGPARTOVL | C_FLAGRMAPOLD))
    errorPrint ("main: options '-o' and '-ro' are exclusive");

  if (((flagval & C_FLAGRMAPOLD) == 0) &&
      (((flagval & C_FLAGRMAPRAT) != 0) || ((flagval & C_FLAGRMAPCST) != 0)))
        errorPrint ("main: an old mapping file must be provided ('-ro' flag) when '-rr' or '-rv' flags are set");

  if ((straval != 0) || ((flagval & C_FLAGKBALVAL) != 0)) {
    if (straptr != NULL)
      errorPrint ("main: options '-b' / '-c' and '-m' are exclusive");

    if ((flagval & C_FLAGPARTOVL) != 0)           /* If overlap partitioning wanted */
      SCOTCH_stratGraphPartOvlBuild (&stradat, straval, (SCOTCH_Num) C_partNbr, kbalval);
    else if ((flagval & C_FLAGCLUSTER) != 0)      /* If clustering wanted */
      SCOTCH_stratGraphClusterBuild (&stradat, straval, (SCOTCH_Num) C_partNbr, 1.0, kbalval);
    else
      SCOTCH_stratGraphMapBuild (&stradat, straval, (SCOTCH_Num) C_partNbr, kbalval);
  }

  SCOTCH_graphSize (&grafdat, &vertnbr, NULL);
  if ((parttab = memAlloc (vertnbr * sizeof (SCOTCH_Num))) == NULL) /* Allocate by hand in case of overlap partitioning */
    errorPrint ("main: out of memory");

  if ((flagval & C_FLAGPARTOVL) == 0) {
    if ((flagval & C_FLAGFIXED) != 0)
      SCOTCH_graphTabLoad (&grafdat, parttab, C_filepntrvfxinp);

    if ((flagval & C_FLAGRMAPOLD) != 0) {
      SCOTCH_graphMapInit (&grafdat, &mapodat, &archdat, NULL);
      SCOTCH_graphMapLoad (&grafdat, &mapodat, C_filepntrmaoinp);
      if ((flagval & C_FLAGRMAPCST) != 0) {
        if ((vmlotab = memAlloc (vertnbr * sizeof (SCOTCH_Num))) == NULL)
          errorPrint ("main: out of memory (2)");
        SCOTCH_graphTabLoad (&grafdat, vmlotab, C_filepntrvmlinp);
      }
    }
  }

  SCOTCH_contextBindGraph (&contdat, &grafdat, &cogrdat);

  clockStop  (&runtime[0]);                       /* Get input time */
  clockInit  (&runtime[1]);
  clockStart (&runtime[1]);

  if ((flagval & C_FLAGPARTOVL) != 0) {           /* If overlap partitioning wanted              */
    if (straptr != NULL)                          /* Set overlap partitioning strategy if needed */
      SCOTCH_stratGraphPartOvl (&stradat, straptr);

    SCOTCH_graphPartOvl (&cogrdat, C_partNbr, &stradat, parttab); /* Perform overlap partitioning */

    clockStop  (&runtime[1]);                     /* Get computation time */
    clockStart (&runtime[0]);

    SCOTCH_graphTabSave (&grafdat, parttab, C_filepntrmapout); /* Write partitioning */
  }
  else {                                          /* Regular partitioning / mapping / clustering wanted */
    if (straptr != NULL)                          /* Set static mapping strategy if needed              */
      SCOTCH_stratGraphMap (&stradat, straptr);

    SCOTCH_graphMapInit (&grafdat, &mappdat, &archdat, parttab);

    if ((flagval & C_FLAGRMAPOLD) != 0) {
      if ((flagval & C_FLAGFIXED) != 0)
        SCOTCH_graphRemapFixedCompute (&cogrdat, &mappdat, &mapodat, emraval, vmlotab, &stradat); /* Perform remapping */
      else
        SCOTCH_graphRemapCompute (&cogrdat, &mappdat, &mapodat, emraval, vmlotab, &stradat);
    }
    else {
      if ((flagval & C_FLAGFIXED) != 0)
        SCOTCH_graphMapFixedCompute (&cogrdat, &mappdat, &stradat); /* Perform mapping */
      else
        SCOTCH_graphMapCompute (&cogrdat, &mappdat, &stradat);
    }

    clockStop  (&runtime[1]);                     /* Get computation time */
    clockStart (&runtime[0]);

    SCOTCH_graphMapSave (&grafdat, &mappdat, C_filepntrmapout); /* Write mapping */
  }

  clockStop (&runtime[0]);                        /* Get output time */

  if (flagval & C_FLAGVERBSTR) {
    fprintf (C_filepntrlogout, "S\tStrat=");
    SCOTCH_stratSave (&stradat, C_filepntrlogout);
    putc ('\n', C_filepntrlogout);
  }
  if (flagval & C_FLAGVERBTIM) {
    fprintf (C_filepntrlogout, "T\tMapping\t\t%g\nT\tI/O\t\t%g\nT\tTotal\t\t%g\n",
             (double) clockVal (&runtime[1]),
             (double) clockVal (&runtime[0]),
             (double) clockVal (&runtime[0]) +
             (double) clockVal (&runtime[1]));
  }
  if ((flagval & C_FLAGPARTOVL) != 0) {           /* If overlap partitioning wanted */
    if (flagval & C_FLAGVERBMAP)
      SCOTCH_graphPartOvlView (&grafdat, C_partNbr, parttab, C_filepntrlogout);
  }
  else {                                          /* Regular partitioning / mapping wanted */
    if (flagval & C_FLAGVERBMAP) {
      if ((flagval & C_FLAGRMAPOLD) != 0)
        SCOTCH_graphRemapView (&grafdat, &mappdat, &mapodat, emraval, vmlotab, C_filepntrlogout);
      else
        SCOTCH_graphMapView (&grafdat, &mappdat, C_filepntrlogout);
    }

    SCOTCH_graphMapExit (&grafdat, &mappdat);     /* Free mapping structure only when used, that is, not for overlay */
    if ((flagval & C_FLAGRMAPOLD) != 0) {
      SCOTCH_graphMapExit (&grafdat, &mapodat);
      if ((flagval & C_FLAGRMAPCST) != 0)
        memFree (vmlotab);
    }
  }

  fileBlockClose (C_fileTab, C_FILENBR);          /* Always close explicitely to end eventual (un)compression tasks */

  SCOTCH_graphExit   (&cogrdat);                  /* Destroy context binding first */
  SCOTCH_graphExit   (&grafdat);
  SCOTCH_stratExit   (&stradat);
  SCOTCH_archExit    (&archdat);
  SCOTCH_contextExit (&contdat);

  memFree (parttab);                              /* Free hand-made partition array */

  return (EXIT_SUCCESS);
}
