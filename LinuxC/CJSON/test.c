#include<stdio.h>
#include<cjson/cJSON.h>
int main()
{
    cJSON * json=cJSON_CreateObject();

    cJSON * province = cJSON_CreateArray();

    cJSON * heilongjiang = cJSON_CreateObject();
    cJSON * hljcities = cJSON_CreateObject();
    cJSON * hljArray = cJSON_CreateArray();

    cJSON * haherbing = cJSON_CreateObject();
    cJSON * daqing = cJSON_CreateObject();

    cJSON * guangdong = cJSON_CreateObject();
    cJSON * gdcities = cJSON_CreateObject(); 
    cJSON * gdArray =cJSON_CreateArray();

    cJSON * guangzhou = cJSON_CreateObject();
    cJSON * shengzheng = cJSON_CreateObject();
    cJSON * zhuhai = cJSON_CreateObject();

    cJSON * taiwan = cJSON_CreateObject();
    cJSON * twcites = cJSON_CreateObject(); 
    cJSON * twArray =cJSON_CreateArray();

    cJSON * taibei = cJSON_CreateObject();
    cJSON * gaoxiong = cJSON_CreateObject();

    cJSON * xinjiang = cJSON_CreateObject();
    cJSON * xjcities = cJSON_CreateObject();
    cJSON * xjArray = cJSON_CreateArray();

    cJSON * wulumuqi = cJSON_CreateObject();
    

    cJSON_AddStringToObject(json,"name","中国");
    cJSON_AddStringToObject(heilongjiang,"name","黑龙江");
    cJSON_AddItemToArray(hljArray,cJSON_CreateString("哈尔滨"));
    cJSON_AddItemToArray(hljArray, cJSON_CreateString("大庆"));
    cJSON_AddItemToObject(hljcities, "city", hljArray);
    cJSON_AddItemToObject(heilongjiang,"cities",hljcities);

    cJSON_AddStringToObject(guangdong,"name","广东");
    cJSON_AddItemToArray(gdArray,cJSON_CreateString("广州"));
    cJSON_AddItemToArray(gdArray,cJSON_CreateString("深圳"));
    cJSON_AddItemToArray(gdArray,cJSON_CreateString("珠海"));
    cJSON_AddItemToObject(gdcities,"city",gdArray);
    cJSON_AddItemToObject(guangdong,"cities",gdcities);

    cJSON_AddItemToArray(province,guangdong);
    cJSON_AddItemToArray(province,heilongjiang);

    cJSON_AddItemToObject(json,"province",province);
    


    printf("%s\n", cJSON_Print(json));
    

}