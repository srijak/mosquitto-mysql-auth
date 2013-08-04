#ifndef _NORDBE_MYSQL_H
#define _NORDBE_MYSQL_H   1

#include <mysql/mysql.h>
MYSQL *db_init(char *host, char *user, char *passwd, char *dbname);
char *db_getuser_passwd(MYSQL *mysql, const char *username, int *io);
void db_destroy(MYSQL *con);
long *db_can_user_access_topic(MYSQL *mysql, const char *username, const char *topic, int *io);

#endif
