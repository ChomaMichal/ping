#include "ping.h"
#include <arpa/inet.h>
#include <ctype.h>
#include <filesystem>
#include <netdb.h>
#include <stdio.h>

int simple_flags(char *flags, t_args *args) {
  for (size_t i = 1; flags[i]; i++) {
    if (flags[i] == 'v') {
      args->v_flag = 1;
    } else if (flags[i] == '?') {
      args->help_flag = 1;
    } else {
      return (-1);
    }
  }
  return (1);
}

int validate_ip(char *str) {
  struct in_addr addr;
  if (inet_aton(str, &addr) == -1)
    return (-1);
  return (1);
}

int ttl_flag(char *str, t_args *args) {
  if (strncmp(str, "--ttl=", 6) != 0) {
    return (-1);
  }
  if (isdigit(str[6]) == 0) {
    return (-1);
  }
  size_t num = atoll(str + 6);
  args->ttl_flag = 1;
  args->ttl_val = num;
  return (1);
}

int string_conversion(char *str, t_args *args) {
  int is_digits = 1;
  for (size_t i = 0; str[i]; i++) {
    if (isdigit(str[i]) == 0)
      is_digits = 0;
  }
  if (is_digits == 1) {
    uint32_t num = atoi(str);
    args->ip_bits = num;
    return 1;
  }
  if (inet_aton(str, (void *)&args->ip_bits) == 1) {
    return (1);
  }
  struct addrinfo addrinfo;
  memset(&addrinfo, 0, sizeof(addrinfo));
  struct addrinfo *ret;
  struct addrinfo *p;
  addrinfo.ai_family = AF_INET;

  int errcode = getaddrinfo(str, NULL, &addrinfo, &ret);
  if (errcode != 0) {
    gai_strerror(errcode);
    return (0);
  } else {
    for (p = ret; p != NULL; p = p->ai_next) {
      struct sockaddr_in *tmp = (void *)p->ai_addr;
      args->ip_bits = tmp->sin_addr.s_addr;
      freeaddrinfo(ret);
      return (1);
    }
  }
  return (0);
}

int parsing(int argc, char **argv, t_args *args) {
  memset(args, 0, sizeof(t_args));

  if (argc == 1) {
    return (1);
  }

  for (int i = 1; i < argc; i++) {
    // printf("%s argv[%i]\n", argv[i], i);
    if (simple_flags(argv[i], args) == 1) {
    } else if (ttl_flag(argv[i], args) == 1) {
      args->fqdm = argv[i];
    } else {
      string_conversion(argv[i], args);
    }
  }
  return (1);
}
