#ifndef PING_H
#define PING_H

#include <ctype.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

extern sig_atomic_t g_sig;
extern struct s_stats g_stats;

struct s_stats {
  float max;
  float min;
  float stddev;
  size_t pck_send;
  size_t pck_recieved;
};

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
// sends icmp echo request to ip spesified in args, if ttl_flag is ture it also
// spesifies ttl from ttl_val
// body of mesage is timeval struct from get time of the day,
// id is pid number retured from getpid()
// returns number of bites set, -1 on error, it also
// uses perror when returning -1
int send_echo_request(int sock_fd, t_args *args);
// recieves all icmp messages if message id doesn't match the pid it doesn't
// print anything if pid matches it prints relevant information return 0 if
// nothing was recieved and signal interupted the reading, after this checg
// g_sig, returns -1 on failiure of recv that wasn't caused by interupt
int receive_message(int sock_fd, t_args *args);
void print_received_message(char *buff);
int set_up_socket(t_args *args);
#endif
