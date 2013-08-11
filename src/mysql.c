#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <mysql/mysql.h>

#define STRING_SIZE 256

MYSQL *db_init(char *host, char *user, char *passwd, char *dbname)
{
  MYSQL *mysql;
  mysql = mysql_init(NULL);
  
  /* Here we make the connection to MySQL */
  if (mysql_real_connect(mysql, host, user, passwd, dbname, 0, NULL, 0) == NULL) {
    fprintf(stderr, "No connection could be made to the database\n");
    return NULL;
  }
  return mysql;
}
void db_destroy(MYSQL *con){
  if (con != NULL){
    mysql_close(con);
  }
}



long *db_can_user_access_topic(MYSQL *mysql, const char *username, const char *topic, int *io) {

  MYSQL_STMT *stmt;
  MYSQL_BIND param[2], result[1];
  char *sql;

  sql = "select count(id)  from user, user_group_association where login = ? and user_id = id and group_id = ? ";

  char          str_data[STRING_SIZE];
  unsigned long str_length;
  unsigned long data_length;


  /* Initialize our statement */
  stmt = mysql_stmt_init(mysql);
  if (!stmt) {
    fprintf(stderr, " mysql_stmt_init(), out of memory\n");
    return NULL;
  }

  if (mysql_stmt_prepare(stmt, sql, strlen(sql))) {
    fprintf(stderr, " mysql_stmt_prepare(), INSERT failed\n");
    fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
    return NULL;
  }

  /* Zero out both the param and result data structures */
  memset(param, 0, sizeof(param));
  memset(result, 0, sizeof(result));

  /* STRING PARAM */
  param[0].buffer_type = MYSQL_TYPE_STRING;
  param[0].buffer = (char *)str_data;
  param[0].buffer_length = STRING_SIZE;
  param[0].is_null = 0;
  param[0].length = &str_length;

  long topic_id = atol(topic);
  /* LONG PARAM */
  param[1].buffer_type = MYSQL_TYPE_LONG;
  param[1].buffer = (char *)&topic_id;
  //param[1].buffer_length = STRING_SIZE;
  param[1].is_null = 0;
  param[1].length = 0;



  long found_count = 0;
  result[0].buffer_type= MYSQL_TYPE_LONG;
  result[0].buffer = &found_count;
  result[0].buffer_length = 0;
  result[0].is_null = 0;
  result[0].length = &data_length;

  /* Bind the parameters buffer */
  if (mysql_stmt_bind_param(stmt, param)) {
    fprintf(stderr, " mysql_stmt_bind_param() failed\n");
    fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
    return NULL;
  }

  /* Bind the results buffer */
  if (mysql_stmt_bind_result(stmt, result) != 0) {
    fprintf(stderr, " mysql_stmt_bind_result() failed\n");
    fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
    return NULL;
  }

  /* Specify the parameter that we send to the query */
  strncpy(str_data, username, STRING_SIZE);
  str_length= strlen(str_data);

  /* Execute the statement */
  if (mysql_stmt_execute(stmt)) {
    fprintf(stderr, " mysql_stmt_execute(), failed\n");
    fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
    return NULL;
  }

  /* Print our results */
  if(mysql_stmt_fetch (stmt) != 0) {
    printf("No results found!\n");
    return NULL;
  }

  /* Close the statement */
  if (mysql_stmt_close(stmt)) {
    fprintf(stderr, " failed while closing the statement\n");
    fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
    return NULL;
  }
  if (found_count >0){
  	long * ret =  (long*)malloc(sizeof(long));
  	*ret = found_count;
  	return ret;
  }else{
	return NULL;
  }
} 

char *db_getuser_passwd(MYSQL *mysql, char *username, int *io) {
  MYSQL_STMT *stmt;
  MYSQL_BIND param[1], result[1];
  char *sql;

  sql = "SELECT `passwd` FROM `client` WHERE `login` = ? AND `activation` is NULL";

  char          str_data[STRING_SIZE];
  char          result_data[STRING_SIZE];
  unsigned long str_length;
  unsigned long data_length;


  /* Initialize our statement */
  stmt = mysql_stmt_init(mysql);
  if (!stmt) {
    fprintf(stderr, " mysql_stmt_init(), out of memory\n");
    return NULL;
  }

  if (mysql_stmt_prepare(stmt, sql, strlen(sql))) {
    fprintf(stderr, " mysql_stmt_prepare(), INSERT failed\n");
    fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
    return NULL;
  }

  /* Zero out both the param and result data structures */
  memset(param, 0, sizeof(param));
  memset(result, 0, sizeof(result));

  /* STRING PARAM */
  param[0].buffer_type = MYSQL_TYPE_STRING;
  param[0].buffer = (char *)str_data;
  param[0].buffer_length = STRING_SIZE;
  param[0].is_null = 0;
  param[0].length = &str_length;


  result[0].buffer_type= MYSQL_TYPE_VAR_STRING;
  result[0].buffer = result_data;
  result[0].buffer_length = STRING_SIZE;
  result[0].is_null = 0;
  result[0].length = &data_length;

  /* Bind the parameters buffer */
  if (mysql_stmt_bind_param(stmt, param)) {
    fprintf(stderr, " mysql_stmt_bind_param() failed\n");
    fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
    return NULL;
  }

  /* Bind the results buffer */
  if (mysql_stmt_bind_result(stmt, result) != 0) {
    fprintf(stderr, " mysql_stmt_bind_result() failed\n");
    fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
    return NULL;
  }

  /* Specify the parameter that we send to the query */
  strncpy(str_data, username, STRING_SIZE);
  str_length= strlen(str_data);

  /* Execute the statement */
  if (mysql_stmt_execute(stmt)) {
    fprintf(stderr, " mysql_stmt_execute(), failed\n");
    fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
    return NULL;
  }

  /* Print our results */
  if(mysql_stmt_fetch (stmt) != 0) {
    printf("No results found!\n");
    return NULL;
  }

  /* Close the statement */
  if (mysql_stmt_close(stmt)) {
    fprintf(stderr, " failed while closing the statement\n");
    fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
    return NULL;
  }
  char * ret =  (char*)malloc(sizeof(char)*strlen(result_data));
  strncpy(ret, result_data, strlen(result_data));
  return ret;

}

#ifdef TEST
int main(){
  MYSQL *con = db_init("localhost", "root", "root", "dev");
  int i = 0;
  /*char * a = db_getuser_passwd(con, "ab", &i);
  if (a != NULL){
    printf("GOT PASSWD: %s\n", a);
  }else{
    printf("GOT PASSWD: %s\n", a);
  }*/

  long * l = db_can_user_access_topic(con, "a", "20", &i);
  if (l != NULL){
    printf("GOT count: %ld\n", *l);
  }else{
    printf("Didn't get count.\n");
  }
}
#endif

