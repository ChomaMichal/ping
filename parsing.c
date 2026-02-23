#include "ping.h"
#include <ctype.h>
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
  static char ip_buff[16];
  size_t i = 0;
  int num_len = 0;
  int amount_of_nums = 0;
  int amount_of_dots = 0;

  while (str[i]) {
    if (isdigit(str[i]) != 0) {
      for (num_len = 1; str[i + num_len]; num_len++) {
        if (isdigit(str[i + num_len]) == 0)
          break;
      }
      printf("%i == num_len\n", num_len);
      if (num_len > 3)
        return (-1);
      long check = atol(str + i);
      if (check > 255) {
        return (-1);
      }
      i += num_len;
      amount_of_nums++;
    } else if (str[i] == '.') {
      amount_of_dots++;
      i++;
    } else {
      return (-1);
    }
  }
  if (amount_of_nums != 4 || amount_of_dots != 3)
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

int parsing(int argc, char **argv, t_args *args) {
  memset(args, 0, sizeof(t_args));

  if (argc == 1) {
    return (1);
  }

  for (int i = 1; i < argc; i++) {
    // printf("%s argv[%i]\n", argv[i], i);
    if (simple_flags(argv[i], args) == 1) {
    } else if (validate_ip(argv[i]) == 1) {
      if (args->ip != NULL)
        return (-1);
      args->ip = argv[i];
    } else if (ttl_flag(argv[i], args) == 1) {
      // } else if (validate_adress(argv[i], args) == 1) {
    } else {
      args->fqdm = argv[i];
    }
  }
  return (1);
}
