
#include <stdbool.h>
#include <fcntl.h>
#include <arpa/inet.h>

int srv_sock;
int clt_sock;
bool alive;
void split_proc(int n) { while (--n != 0) fork(); }
bool work();

int main(int c, char **v) {
  srv_sock = socket(AF_UNIX, SOCK_STREAM, 0);
  unsigned int type = 1;
  setsockopt(srv_sock, SOL_SOCKET, SO_REUSEADDR, &type, sizeof(type));
  struct sockaddr_in sockaddr;
  sockaddr.sin_family = AF_INET;
  sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  sockaddr.sin_port = htons(1234);
  bind(srv_sock, (struct sockaddr*)&sockaddr, (socklen_t)sizeof(sockaddr));
  split_proc(c > 1 ? atoi(v[1]) : 1);
  listen(srv_sock,2);
  do {
    clt_sock = accept(srv_sock, (struct sockaddr*)&sockaddr, (socklen_t*)&type);
    alive = work();
    close(clt_sock);
  } while (alive);
  close(srv_sock);
  return (2);
}

