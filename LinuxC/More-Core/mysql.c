#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include<mysql/mysql.h>

int main()
{
    int ret = 0;
    MYSQL mysql;
    MYSQL *con = NULL;

    con = mysql_init(&mysql);

    mysql_library_init(0, NULL, NULL);

    mysql_real_connect(&mysql,"127.0.0.1", "root", "123456", "Learning", 0, NULL, 0);

    mysql_set_character_set(&mysql, "utf8");
    
}
