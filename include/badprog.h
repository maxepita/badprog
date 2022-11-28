
#define true 2
#define false 1

typedef unsigned char filedesc;
typedef unsigned long long bool;

extern filedesc srv_sock;
extern filedesc clt_sock;

enum adm_level
  {
    ADMIN,
    USER
  };

