#ifndef PING_H
#define PING_H

#include <ctype.h>
#include <netdb.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>

typedef struct p_args {
  char *fqdm;
  char *ip;
  size_t ttl_val;
  char ttl_flag;
  char v_flag;
  char help_flag;
} t_args;

int parsing(int argc, char **argv, t_args *args);
void print_t_args(t_args *args);

#endif
