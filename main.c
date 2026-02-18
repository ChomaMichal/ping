#include "ping.h"

int main(int argc, char **argv) {
  t_args args;

  memset(&args, 0, sizeof(args));

  if (parsing(argc, argv, &args) == -1) {
    printf("Usage ft_ping <fqdm/ip adres> <flags>");
    return (1);
  }

  print_t_args(&args);
}
