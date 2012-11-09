/*
 * Print all open file descriptors (files, pipes, sockets, etc...)
 * Usage example:
 *  php -r '$s = socket_create( AF_INET, SOCK_STREAM, SOL_TCP); socket_bind( $s, "127.0.0.1", 1948); exec( "./list-open-fd", $out); print( join( $out, "\n")); print( "\n");'
 */


#include <stdio.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/un.h>


int main() {
  struct stat st;
  int fd;
  struct sockaddr sa;
  struct sockaddr_in sin;
  struct sockaddr_un sun;
  socklen_t len = sizeof(sa);
  socklen_t len_in = sizeof(sin);
  socklen_t len_un = sizeof(sun);
  char sockaddrstr[INET_ADDRSTRLEN];

  //FIXME: maximum fd number can be > 65535
  for( fd=0; fd<65535; fd++) {
    if( fstat( fd, &st) == -1) {
      continue;
    }

    switch (st.st_mode & S_IFMT) {
      case S_IFBLK:  printf("fd %d is block device\n", fd);            break;
      case S_IFCHR:  printf("fd %d is character device\n", fd);        break;
      case S_IFDIR:  printf("fd %d is directory\n", fd);               break;
      case S_IFIFO:  printf("fd %d is FIFO/pipe\n", fd);               break;
      case S_IFLNK:  printf("fd %d is symlink\n", fd);                 break;
      case S_IFREG:  printf("fd %d is regular file\n", fd);            break;
      case S_IFSOCK: printf("fd %d is socket\n", fd);                  break;
      default:       printf("fd %d is unknown?\n", fd);                break;
    }

    if( (st.st_mode & S_IFMT) == S_IFSOCK) {
      getsockname( fd, &sa, &len);
      if( sa.sa_family == 2) {
        getsockname( fd, (struct sockaddr *)&sin, &len_in);
        printf("socket local address: %d.%d.%d.%d:%d\n", 
          sin.sin_addr.s_addr % 256,
          (sin.sin_addr.s_addr / 256) % 256,
          (sin.sin_addr.s_addr / 256 / 256) % 256,
          sin.sin_addr.s_addr / 256 / 256 / 256,
          ntohs(sin.sin_port));

        getpeername( fd, (struct sockaddr *)&sin, &len_in);
        printf("socket peer address: %d.%d.%d.%d:%d\n", 
          sin.sin_addr.s_addr % 256,
          (sin.sin_addr.s_addr / 256) % 256,
          (sin.sin_addr.s_addr / 256 / 256) % 256,
          sin.sin_addr.s_addr / 256 / 256 / 256,
          ntohs(sin.sin_port));
      } else {
        getsockname( fd, (struct sockaddr *)&sun, &len_un);
        printf("unix socket %s\n", sun.sun_path);
        getpeername( fd, (struct sockaddr *)&sun, &len_un);
        printf("unix socket peer %d\n", *((int *)sun.sun_path));
      }
    }
  }
}
