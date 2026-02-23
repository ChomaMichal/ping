#include "ping.h"
#include <arpa/inet.h>
#include <netinet/in.h>
int set_up_socket(t_args *args) {

  int socket_fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
  if (socket_fd == -1) {
    perror("socket has failed");
    return (-1);
  }

  int val = 1;
  setsockopt(socket_fd, SOL_IP, IP_RECVERR, &val, sizeof(val));
  if (args->ttl_flag) {
    setsockopt(socket_fd, IPPROTO_IP, IP_TTL, &args->ttl_val,
               sizeof(args->ttl_val));
  }
}
