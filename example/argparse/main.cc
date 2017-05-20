

#include <stdio.h>
#include <slankdev/argparse.h>


int main (int argc, char **argv)
{
  slankdev::argparse parser;
  parser.add_opt("add" , no_argument      , 0, 'a');
  parser.add_opt("file", required_argument, 0, 'f');
  parser.add_opt(0     , 0               , 0,  0 );

  parser.parse(argc, argv);
  if (parser.is_set('a')) printf("a\n");
  if (parser.is_set('f')) printf("f %s \n", parser.get_optarg('f').c_str());
}


