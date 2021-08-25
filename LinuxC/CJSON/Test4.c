#include <stdio.h>
#include<cjson/cJSON.h>


int main(void)
{
    const char *jsonStr = "{						\
 		\"name\": \"中国\",						\
 		\"province\": [{						\
 			\"name\": \"黑龙江\",					\
 			\"cities\": {						\
 				\"city\": [\"哈尔滨\", \"大庆\"]		\
 			}							\
 		}, {								\
 			\"name\": \"广东\",					\
 			\"cities\": {						\
 				\"city\": [\"广州\", \"深圳\", \"珠海\"]	\
 			}							\
 		}, {								\
 			\"name\": \"台湾\",					\
 			\"cities\": {						\
 				\"city\": [\"台北\", \"高雄\"]			\
 			}							\
 		}, {								\
 			\"name\": \"新疆\",					\
 			\"cities\": {						\
 			\"city\": [\"乌鲁木齐\"]				\
 			}							\
 		}]								\
 	}";

    cJSON *json = cJSON_Parse(jsonStr);

    if(json!=NULL){

        cJSON * temp;
        temp = cJSON_GetObjectItem(json,"name");

        printf("%s:%s\n",temp->string,temp->valuestring);
        temp =cJSON_GetObjectItem(json,"province");
        int n=cJSON_GetArraySize(temp);
        for(int i=0;i<n;i++){
            cJSON *province =cJSON_GetArrayItem(temp,i);
            printf("\t%s:%s\n", cJSON_GetObjectItem(province, "name")->string, cJSON_GetObjectItem(province, "name")->valuestring);
            cJSON *Cities = cJSON_GetObjectItem(province, "cities");
            printf("\t\t%s:\n",Cities->string);
            cJSON *CityArray = cJSON_GetObjectItem(Cities,"city");
            printf("\t\t\t%s:",CityArray->string);
            int m= cJSON_GetArraySize(CityArray);
            for(int j=0;j<m;j++){
                cJSON * cityname=cJSON_GetArrayItem(CityArray,j);
                printf("[%s]",cityname->valuestring);
            }
            printf("\n");
        }

    }


    return 0;
}