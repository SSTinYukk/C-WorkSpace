#include<stdio.h>
#include<stdlib.h>
#include<mysql/mysql.h>

int main()
{
    

    //mysql_query(mysql,"insert into 用户列表 values(\"0000001\",\"Xiaoming\",\"XXXXXXX\")");
    
    char name[] = "Xiaohu";
    char buff[100];
    sprintf(buff, "select *from 用户列表 where 用户名 = \"%s\"", name);
    mysql_query(mysql, buff);
    result = mysql_store_result(mysql);
    unsigned short numFields;
    numFields = mysql_num_fields(result);
    printf("%d\n",numFields);

    while((row=mysql_fetch_row(result))!=NULL)
    {
        for(int j=0;j<numFields;j++){
            printf("%s\t",row[j]);
        }
    }

    mysql_close(mysql);
    mysql_library_end();
    free(mysql);
}