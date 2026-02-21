#include "ping.h"

void print_t_args(t_args *args) {
  printf("Args:\n");
  printf("  help_flag == %i\n", args->help_flag);
  printf("  v_flag == %i\n", args->v_flag);
  printf("  ttl_flag == %i\n", args->ttl_flag);
  printf("  ttl_val == %lu\n", args->ttl_val);
  printf("  ip == %s\n", args->ip);
  printf("  fqdm == %s\n", args->fqdm);
}

void print_received_message(char *buff) {
  struct icmphdr *header = (void *)buff;
  printf("Received message\n");
  printf("    type = %i\n", header->type);
  printf("    code = %i\n", header->code);
  printf("    checksum = %i\n", header->checksum);
  printf("    identifier = %i\n", header->un.echo.id);
  printf("    sequence = %i\n", header->un.echo.sequence);
}
