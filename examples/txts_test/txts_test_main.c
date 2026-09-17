/* Throwaway probe: does a PKT socket's SO_TIMESTAMPING TX flag ever
 * deliver a timestamp via MSG_ERRQUEUE on real Ethernet hardware
 * (stm32f4discovery-ext, DP83848C), after reconciling the self-echo
 * suppression filter with apache/nuttx PR #20161?
 */

#include <nuttx/config.h>

#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <poll.h>
#include <net/if.h>
#include <netpacket/packet.h>
#include <sys/socket.h>
#include <sys/time.h>

int main(int argc, char *argv[])
{
  int fd;
  int val;
  unsigned int ifindex;
  struct sockaddr_ll addr;
  char buf[64];
  char cmsgbuf[256];
  struct msghdr msg;
  struct iovec iov;
  struct cmsghdr *cmsg;
  struct pollfd pfd;
  ssize_t n;

  fd = socket(AF_PACKET, SOCK_DGRAM, htons(0x0800));
  if (fd < 0)
    {
      printf("FAIL: socket: %d\n", errno);
      return 1;
    }

  val = SOF_TIMESTAMPING_TX_SOFTWARE;
  if (setsockopt(fd, SOL_SOCKET, SO_TIMESTAMPING, &val, sizeof(val)) < 0)
    {
      printf("FAIL: setsockopt SO_TIMESTAMPING: %d\n", errno);
      close(fd);
      return 1;
    }

  ifindex = if_nametoindex("eth0");
  if (ifindex == 0)
    {
      printf("FAIL: eth0 not found\n");
      close(fd);
      return 1;
    }

  memset(&addr, 0, sizeof(addr));
  addr.sll_family   = AF_PACKET;
  addr.sll_protocol = htons(0x0800);
  addr.sll_ifindex  = ifindex;
  addr.sll_halen    = 6;
  memset(addr.sll_addr, 0xff, 6);

  memset(buf, 0xaa, sizeof(buf));
  n = sendto(fd, buf, 46, 0, (struct sockaddr *)&addr, sizeof(addr));
  if (n < 0)
    {
      printf("FAIL: sendto: %d\n", errno);
      close(fd);
      return 1;
    }

  printf("sent %zd bytes on eth0, waiting for POLLPRI...\n", n);

  pfd.fd      = fd;
  pfd.events  = POLLPRI;
  pfd.revents = 0;

  n = poll(&pfd, 1, 2000);
  if (n <= 0)
    {
      printf("RESULT: no POLLPRI within 2s (n=%zd, errno=%d) - "
             "TX timestamp NOT delivered\n", n, errno);
      close(fd);
      return 1;
    }

  printf("poll: revents=0x%lx\n", (unsigned long)pfd.revents);

  memset(&msg, 0, sizeof(msg));
  iov.iov_base       = buf;
  iov.iov_len        = sizeof(buf);
  msg.msg_iov        = &iov;
  msg.msg_iovlen     = 1;
  msg.msg_control    = cmsgbuf;
  msg.msg_controllen = sizeof(cmsgbuf);

  n = recvmsg(fd, &msg, MSG_ERRQUEUE);
  if (n < 0)
    {
      printf("FAIL: recvmsg MSG_ERRQUEUE: %d\n", errno);
      close(fd);
      return 1;
    }

  printf("recvmsg: %zd bytes, flags=0x%x\n", n, msg.msg_flags);

  for (cmsg = CMSG_FIRSTHDR(&msg); cmsg; cmsg = CMSG_NXTHDR(&msg, cmsg))
    {
      if (cmsg->cmsg_level == SOL_SOCKET &&
          cmsg->cmsg_type == SO_TIMESTAMPING)
        {
          struct timespec *ts = (struct timespec *)CMSG_DATA(cmsg);
          printf("RESULT: TX timestamp delivered: %lld.%09ld - PASS\n",
                 (long long)ts[0].tv_sec, ts[0].tv_nsec);
          close(fd);
          return 0;
        }
    }

  printf("RESULT: no SO_TIMESTAMPING cmsg in errqueue message\n");
  close(fd);
  return 1;
}
