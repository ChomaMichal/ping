#include "ping.h"
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <sys/socket.h>
#include <sys/time.h>

uint16_t checksum_cal(const void *buf, size_t len) {
  const uint16_t *data = buf;
  uint32_t sum = 0;
  while (len > 1) {
    sum += *data++;
    len -= 2;
  }
  while (sum >> 16) {
    sum = (sum & 0xFFFF) + (sum >> 16);
  }
  return ((uint16_t)(~sum));
}

int send_echo_request(int sock_fd, t_args *args) {

  static uint16_t sequence_number = 0;
  static struct sockaddr_in sockaddr;

  if (sockaddr.sin_family == 0) {
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port = 0;
    sockaddr.sin_addr.s_addr = args->ip_bits;
    memset(&sockaddr.sin_zero, 0, sizeof(sockaddr.sin_zero));
  }

  char buff[100];
  struct icmphdr *header = (void *)buff;

  header->type = ICMP_ECHO; // type for echo
  header->checksum = 0;     // calcucalet checksum later
  header->code = 0;         // should be 0
  header->un.echo.sequence = sequence_number++;
  header->un.echo.id = getpid(); // mypid;
  gettimeofday((struct timeval *)(buff + sizeof(struct icmphdr)), NULL);
  header->checksum =
      checksum_cal(buff, sizeof(struct icmphdr) + sizeof(struct timeval));

  int rt =
      sendto(sock_fd, buff, sizeof(struct icmphdr) + sizeof(struct timeval), 0,
             (struct sockaddr *)&sockaddr, sizeof(struct sockaddr_in));
  if (rt != -1) {
    g_stats.pck_send++;
  }
  return (rt);
}

void print_formated_report(void *ptr, ssize_t len, t_args *args) {
  struct iphdr *iphdr = ptr;
  struct icmphdr *icmphdr = (ptr + iphdr->ihl * 4);
  struct timeval *send_time = ((void *)icmphdr) + sizeof(struct icmphdr);
  struct timeval current_time;

  gettimeofday(&current_time, NULL);

  float timediff = (current_time.tv_sec - send_time->tv_sec) * 1000 +
                   (float)(current_time.tv_usec - send_time->tv_usec) / 1000;
  printf("%lu bytes from %s icmp_seq=%i ttl=%i time=%f ms\n", len, args->ip,
         icmphdr->un.echo.sequence, iphdr->ttl, timediff);
}

int receive_message(int sock_fd, t_args *args) {
  char buff[IP_MAXPACKET] = {0};
  struct sockaddr_in sockaddr = {0};
  socklen_t sock_len = 0;
  struct icmphdr *icmphdr;
  struct iphdr *iphdr;

  while (1) {
    memset(&sockaddr, 0, sizeof(struct sockaddr_in));
    memset(buff, 0, IP_MAXPACKET);
    ssize_t i = recvfrom(sock_fd, buff, IP_MAXPACKET, 0,
                         (struct sockaddr *)&sockaddr, &sock_len);
    if (i == -1) {
      if (errno == EINTR) {
        return (0);
      }
      perror("Recvfrom failed");
      return (-1);
    }
    iphdr = (void *)buff;
    icmphdr = (void *)(buff + iphdr->ihl * 4);
    if (icmphdr->code == ICMP_ECHOREPLY) {
      if (icmphdr->un.echo.id == getpid()) {
        print_formated_report(buff, i, args);
        g_stats.pck_recieved++;
        continue;
      } else {
        printf("not a correct client\n");
      }
    }
    if (g_sig == SIGINT || g_sig == SIGALRM) {
      return (0);
    }
  }
  return (0);
}
