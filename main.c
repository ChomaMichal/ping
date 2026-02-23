#include "ping.h"
#include <arpa/inet.h>
#include <bits/types/struct_timeval.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <time.h>

sig_atomic_t g_sig;
struct s_stats g_stats;

void signal_handler(int sig) {
  if (sig == SIGINT) {
    g_sig = sig;
  }
}

int set_up_signal(void) {
  struct sigaction sig_struct = {0};
  sig_struct.sa_handler = signal_handler;
  sig_struct.sa_flags = 0;
  if (sigaction(SIGALRM, &sig_struct, NULL) == -1) {
    perror("Sig action failed");
    return (-1);
  }
  if (sigaction(SIGINT, &sig_struct, NULL) == -1) {
    perror("Sig action failed");
    return (-1);
  }
  return (1);
}

void print_stats(t_args *args) {
  if (args->fqdm != NULL) {
    printf("--- %s ping statistics ---\n", args->fqdm);
  } else {
    printf("--- %s ping statistics ---\n", args->ip);
  }
  printf("%lu packtes transmitted, %lu packets recieved, %.f%% packet loss\n",
         g_stats.pck_send, g_stats.pck_recieved,
         100.f - ((float)g_stats.pck_recieved / g_stats.pck_send) * 100);

  if (g_stats.pck_recieved != 0) {
    printf("round-trip min/max/stddev = %f/%f/%f ms", g_stats.min, g_stats.max,
           g_stats.stddev);
  }
}

int main(int argc, char **argv) {

  t_args args;

  if (parsing(argc, argv, &args) == -1) {
    printf("Usage ft_ping <fqdm/ip adres> <flags>");
    return (1);
  }

  if (set_up_signal() == -1) {
    return (1);
  }
  int sock_fd = set_up_socket(&args);
  if (sock_fd == -1) {
    return (1);
  }
  if (args.fqdm == NULL) {
    printf("PING %s (%s): 56 data bytes\n", args.ip,
           args.ip); // figure out if the amount of data bytes changes
  } else if (args.fqdm != NULL) {
    printf("PING %s (%s): 56 data bytes\n", args.fqdm,
           args.ip); // figure out if the amount of data bytes changes
  }
  while (1) {
    alarm(1);
    if (send_echo_request(sock_fd, &args) == -1) {
      return (-1);
    }
    int rt = receive_message(sock_fd, &args);
    if (rt == -1) {
      return (-1);
    }
    if (rt == 0) {
      if (g_sig == SIGINT) {
        g_sig = 0;
        break;
      }
    }
  }
  print_stats(&args);
}
