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

  struct sockaddr_in s_addr;
  memset(&s_addr, 0, sizeof(s_addr));
  s_addr.sin_family = AF_INET;
  s_addr.sin_port = htons(0);

  if (inet_aton(args->ip, &s_addr.sin_addr) == -1) {
    perror("invalid ip address");
    return (1);
  }
  if (connect(socket_fd, (struct sockaddr *)&s_addr, sizeof(s_addr)) == -1) {
    perror("connection failed");
    return (-1);
  }
  return (socket_fd);
}
