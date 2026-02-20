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

int main(int argc, char **argv) {
  t_args args;

  memset(&args, 0, sizeof(args));

  if (parsing(argc, argv, &args) == -1) {
    printf("Usage ft_ping <fqdm/ip adres> <flags>");
    return (1);
  }

  print_t_args(&args);

  int sock_fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
  if (sock_fd == -1) {
    perror("socket has failed");
    return (1);
  }

  struct sockaddr_in s_addr;
  memset(&s_addr, 0, sizeof(s_addr));

  s_addr.sin_family = AF_INET;
  s_addr.sin_port = htons(0);

  if (inet_aton(args.ip, &s_addr.sin_addr) == -1) {
    perror("invalid ip adres");
    return (1);
  }

  int hehe = connect(sock_fd, &s_addr, sizeof(s_addr));
  struct pollfd pl;

  pl.fd = hehe;
  pl.events = POLLIN | POLLOUT;
  pl.revents = 0;

  struct timeval last_message_time;
  struct timeval current_time;

  last_message_time.tv_sec = 0;
  last_message_time.tv_usec = 0;
  perror("before send_echo_request");

  gettimeofday(&current_time, NULL);
  send_echo_request(sock_fd, &current_time);
  perror("after send_echo_request");
  sleep(1);
  recieve_message(sock_fd, &current_time);
  /*
while (1) {
  poll(&pl, 1, -1);
  gettimeofday(&current_time, NULL);
  if (current_time.tv_sec + 1 < last_message_time.tv_sec) {
    if (pl.revents & POLLOUT) {
      send_echo_request(sock_fd, &current_time);
    }
  }
  if (pl.revents & POLLIN) {
    recieve_message(sock_fd, &current_time);
  }
}
  */
}

void print_recieved_message(char *buff) {
  printf("Recieved message\n");
  printf("    type = %i\n", buff[0]);
  printf("    code = %i\n", buff[1]);
  printf("    checksum = %i\n", ((uint16_t *)buff)[1]);
  printf("    identifier = %i\n", ((uint16_t *)buff)[2]);
  printf("    sequence = %i\n", ((uint16_t *)buff)[3]);
  printf("    sequence = %i\n", ((int *)buff)[2]);
}

int recieve_message(int sock_fd, struct timeval *current_time) {
  char buff[1024];
  int i = read(sock_fd, buff, 1024);

  int type = buff[0];
  int code = buff[1];
  int checksum = 0;
  memcpy(&checksum, buff + 2, 2);
  int identifier = 0;
  memcpy(&identifier, buff + 4, 2);
  int sequence = 0;
  memcpy(&sequence, buff + 6, 2);
  struct timeval time_stamp;
  memcpy(&time_stamp, buff + 8, sizeof(struct timeval));

  print_recieved_message(buff);
  return (1);
}

uint16_t checksum_cal(uint16_t *buff) {
  uint16_t rt = buff[0] + buff[2] + buff[3];
  rt = ~rt;
  return (rt);
}

int send_echo_request(int sock_fd, struct timeval *time_stamp) {

  static uint16_t sequence_number = 0;
  const uint16_t identifier = 69;
  char buff[1024];

  buff[0] = 8; // type
  buff[1] = 0; // code
  buff[2] = 0; // checksum
  buff[3] = 0; // checksum
  buff[4] = 0; // identifier
  buff[5] = 0; // identifier
  *((uint16_t *)buff + 4) = identifier;
  buff[6] = 0; // sequence number
  buff[7] = 0; // sequence number
  *((uint16_t *)buff + 6) = sequence_number++;

  *((uint16_t *)buff + 2) = checksum_cal((uint16_t *)buff);
  memcpy(buff + 8, time_stamp, sizeof(struct timeval));
  return (write(sock_fd, buff, 8 + sizeof(struct timeval)));
}
