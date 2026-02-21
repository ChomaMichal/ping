#include "ping.h"
#include <arpa/inet.h>
#include <bits/types/struct_timeval.h>
#include <netinet/in.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <time.h>

__pid_t mypid;

int send_echo_request(int sock_fd, struct timeval *time_stamp);
int main(int argc, char **argv) {
  mypid = getpid();

  t_args args;

  memset(&args, 0, sizeof(args));

  if (parsing(argc, argv, &args) == -1) {
    printf("Usage ft_ping <fqdm/ip adres> <flags>");
    return (1);
  }

  // print_t_args(&args);

  int sock_fd = set_up_socket(&args);

  struct timeval last_message_time;
  struct timeval current_time;

  last_message_time.tv_sec = 0;
  last_message_time.tv_usec = 0;
  // perror("before send_echo_request");

  gettimeofday(&current_time, NULL);
  send_echo_request(sock_fd, &current_time);
  // perror("after send_echo_request");
  sleep(1);
  receive_message(sock_fd, &current_time);
}

int receive_message(int sock_fd, struct timeval *current_time) {
  struct msghdr msg;
  int i = recvmsg(sock_fd, &msg, MSG_ERRQUEUE);
  struct cmsghdr *cmsg;

  for (cmsg = CMSG_FIRSTHDR(&msg); cmsg != NULL;
       cmsg = CMSG_NXTHDR(&msg, cmsg)) {
  }

  return (1);
}

uint16_t checksum_cal(uint16_t *buff, size_t len) {
  uint32_t rt;
  while (len > 1) {
    rt = (*buff);
    buff++;
    len -= 2;
  }
  if (len == 1) {
    rt += *(uint8_t *)buff;
  }
  while (rt >> 16) {
    rt = (rt & 0xFFFF) + (rt >> 16);
  }
  return (~(uint16_t)rt);
}

int send_echo_request(int sock_fd, struct timeval *time_stamp) {

  static uint16_t sequence_number = 0;
  char buff[100];
  struct icmphdr *header = (void *)buff;

  header->type = 8;     // type for echo
  header->checksum = 0; // calcucalet checksum later
  header->code = 0;     // should be 0
  header->un.echo.sequence = sequence_number++;
  header->un.echo.id = mypid;
  memcpy(buff + sizeof(struct icmphdr), time_stamp, sizeof(struct timeval));
  *((uint16_t *)buff + 2) = checksum_cal(
      (uint16_t *)buff, sizeof(struct icmphdr) + sizeof(struct timeval));
  return (write(sock_fd, buff, 8 + sizeof(struct timeval)));
}
