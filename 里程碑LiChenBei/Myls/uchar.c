#include <stdio.h>
#include <uchardet/uchardet.h>

/* 样本数量 */
#define NUMBER_OF_SAMPLES (2048)

int main(int argc, char *argv[])
{
    FILE *file;
    char buf[NUMBER_OF_SAMPLES];
    int len;
    uchardet_t ud;

    /* 打开被检测文本文件，并读取一定数量的样本字符 */
    file = fopen("gb18030.txt", "rt");
    len = fread(buf, sizeof(char), NUMBER_OF_SAMPLES, file);
    fclose(file);

    /* 通过样本字符分析文本编码 */
    ud = uchardet_new();
    if (uchardet_handle_data(ud, buf, len) != 0) /* 如果样本字符不够，那么有可能导致分析失败 */
    {
        printf("分析编码失败！\n");
        return -1;
    }
    uchardet_data_end(ud);
    printf("文本的编码方式是%s。\n", uchardet_get_charset(ud)); /* 获取并打印文本编码 */
    uchardet_delete(ud);

    return 0;
}