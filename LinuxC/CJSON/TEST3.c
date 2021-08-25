#include <stdio.h>

#include <cjson/cJSON.h>


int main(void)
{
    cJSON *json = cJSON_CreateObject();

    cJSON *provinceArray = cJSON_CreateArray();

    cJSON *heilongjiang = cJSON_CreateObject();
    cJSON *hljcities = cJSON_CreateObject();
    cJSON *hljcityArray = cJSON_CreateArray();

    cJSON *guangdong = cJSON_CreateObject();
    cJSON *gdcities = cJSON_CreateObject();
    cJSON *gdcityArray = cJSON_CreateArray();

    cJSON *taiwan = cJSON_CreateObject();
    cJSON *twcities = cJSON_CreateObject();
    cJSON *twcityArray = cJSON_CreateArray();

    cJSON *xinjiang = cJSON_CreateObject();
    cJSON *xjcities = cJSON_CreateObject();
    cJSON *xjcityArray = cJSON_CreateArray();

    cJSON_AddStringToObject(json, "name", "中国");

    cJSON_AddStringToObject(heilongjiang, "name", "黑龙江");
    cJSON_AddItemToArray(hljcityArray, cJSON_CreateString("哈尔滨"));
    cJSON_AddItemToArray(hljcityArray, cJSON_CreateString("大庆"));
    cJSON_AddItemToObject(hljcities, "city", hljcityArray);
    cJSON_AddItemToObject(heilongjiang, "cities", hljcities);

    cJSON_AddStringToObject(guangdong, "name", "广东");
    cJSON_AddItemToArray(gdcityArray, cJSON_CreateString("广州"));
    cJSON_AddItemToArray(gdcityArray, cJSON_CreateString("深圳"));
    cJSON_AddItemToArray(gdcityArray, cJSON_CreateString("珠海"));
    cJSON_AddItemToObject(gdcities, "city", gdcityArray);
    cJSON_AddItemToObject(guangdong, "cities", gdcities);

    cJSON_AddStringToObject(taiwan, "name", "台湾");
    cJSON_AddItemToArray(twcityArray, cJSON_CreateString("台北"));
    cJSON_AddItemToArray(twcityArray, cJSON_CreateString("高雄"));
    cJSON_AddItemToObject(twcities, "city", twcityArray);
    cJSON_AddItemToObject(taiwan, "cities", twcities);

    cJSON_AddStringToObject(xinjiang, "name", "新疆");
    cJSON_AddItemToArray(xjcityArray, cJSON_CreateString("乌鲁木齐"));
    cJSON_AddItemToObject(xjcities, "city", xjcityArray);
    cJSON_AddItemToObject(xinjiang, "cities", xjcities);

    cJSON_AddItemToArray(provinceArray, heilongjiang);
    cJSON_AddItemToArray(provinceArray, guangdong);
    cJSON_AddItemToArray(provinceArray, taiwan);
    cJSON_AddItemToArray(provinceArray, xinjiang);

    cJSON_AddItemToObject(json, "province", provinceArray);

    printf("%s\n", cJSON_Print(json));

    if (NULL != json)
    {
        cJSON_Delete(json);
        json = NULL;
    }

    return 0;
}