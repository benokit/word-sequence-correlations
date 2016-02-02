#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <stdexcept>
#include "optionsParser.h"

Parameters GetParameters (int argc, char **argv)
{
  int aflag = 0;
  int bflag = 0;
  char *cvalue = NULL;
  int index;
  int c;
  Parameters params;
  std::string s;

  opterr = 0;
  while ((c = getopt (argc, argv, "w:c:")) != -1)
    switch (c)
      {
      case 'w':
        s = std::string(optarg);
        params.firstWord = s;    
        break;
      case 'c':
        s = std::string(optarg);
        params.minWordsCount = std::stoi(s);
        break;
      case '?':
        if (optopt == 'c')
          fprintf (stderr, "Option -%c requires an argument.\n", optopt);
        else if (optopt == 'w')
          fprintf (stderr, "Option -%c requires an argument.\n", optopt);
        else if (isprint (optopt))
          fprintf (stderr, "Unknown option `-%c'.\n", optopt);
        else
          fprintf (stderr,
                   "Unknown option character `\\x%x'.\n",
                   optopt);
        throw std::invalid_argument("Can not parse input parameters.");
      default:
        abort ();
      }

  params.pathToFileWithWords = argv[optind];

  return params;
}
