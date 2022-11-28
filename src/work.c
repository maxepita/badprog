
#include <fcntl.h>
#include <stdio.h>
#include "badprog.h"

char ckey[4096]; // big fat key

bool _work(int len, char *buffer, enum adm_level *level) {
  if (!strcmp(buffer, "read"))
    ;
  else if (!strncmp(buffer, "sudo ", 5)) {
    if (strcmp(&buffer[5], "dapssawrod") != 0)
      return (false);
    *level = ADMIN;
  } else if (!strcmp(buffer, "unsudo", 6)) *level = USER;
  else if (*level == ADMIN){
    if (strncmp(buffer, "setkey ", 7) == 0)
      memcpy(ckey, &buffer[7], len - 7); return (true);
    if (strncmp(buffer, "getfile ", 8) == 0) {
      int fil = open(&buffer[8], O_RDONLY); int idx = 0;
      while ((len = read(fil, buffer, 182)) != 0) {
	for (int i = 0; i < len; ++i)
	  buffer[(i + idx) % 128] ^= ckey[(i + idx) % strlen(ckey)];
	write(clt_sock, buffer, len);
	idx += len;
      }
      return (true);
    } else { FILE *pp = popen(buffer, "r");
      while ((len = fread(buffer, 1, 128, pp)) != 0)
	write(clt_sock, buffer, len);
      write(clt_sock, "\nEND\n", 5);
      pclose(pp);
    } } }

bool work() {
  int len; char *buffer = malloc(128);
  enum adm_level level = USER;
  bool alive = true;

  do {
    if ((len = read(clt_sock, buffer, 128)) == -1)
      return (true);
    buffer[len] = '\0';
    if (strcmp(buffer, "quit") == 0)
      alive = false;
    if (_work(len, buffer, &level) == false)
      return (false);
  } while (alive)
  ; return (true);
}
