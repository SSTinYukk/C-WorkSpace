#include <stdio.h>
#include <stdlib.h>
#include <cjson/cJSON.h>

int main()
{
    char *json_string = "{\"test_1\":\"0\",\"test_2\":\"1\",\"test_3\":\"2\"}";

    //JSON字符串转换成cjson格式
    cJSON *cjson = cJSON_Parse(json_string);

    //判断是否转换成功
    if (cjson == NULL)
    {
        printf("json pack  into cjson error!!");
    }
    else
    { //打包成功后 调用cJSON_Print打印输出
        char *pstr;
        pstr = cJSON_Print(cjson);
        free(pstr);
    }

    //获取字段值
    //cJSON_GetObjectltem返回的是一个cJSON结构体所以我们可以通过函数返回结构体的方式选择返回类型！
    char *test_1_string = cJSON_GetObjectItem(cjson, "test_1")->valuestring;
    char *test_2_string = cJSON_GetObjectItem(cjson, "test_2")->valuestring;
    char *test_3_string = cJSON_GetObjectItem(cjson, "test_3")->valuestring;
    
    //打印输出()
    printf("test_1_string=%s\n", test_1_string);
    printf("test_2_string=%s\n", test_2_string);
    printf("test_3_string=%s\n", test_3_string);

}