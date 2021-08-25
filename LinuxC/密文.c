#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <assert.h>
#include <string.h>
#include <stdio_ext.h>
#include<stdlib.h>
int getch1(void) //自己实现的getch函数
{
    struct termios tm, tm_old;
    int fd = STDIN_FILENO, c, d;
    if (tcgetattr(fd, &tm) < 0)
    {
        return -1;
    }
    tm_old = tm;
    cfmakeraw(&tm);
    if (tcsetattr(fd, TCSANOW, &tm) < 0)
    {
        return -1;
    }
    c = fgetc(stdin);
    if (tcsetattr(fd, TCSANOW, &tm_old) < 0)
    {
        return -1;
    }
    return c;
}

int inputkey(char k[]) //密文密码输入函数，所传参数是要保存的字符串地址
{
    int i;
    for (i = 0; i < 10; i++)
    {
        __fpurge(stdin);
        k[i] = getch1();
        if ((k[i] == 127) && (i == 0)) //当遇到backspace并且已经退格到最前面时不再退格
        {
            i--;
            continue;
        }
        if (k[i] == 13)
        {
            k[i] = 0;
            break;
        }
        else if ((k[i] == 127) && (i > 0)) //按下backspace键时，后退显示输出
        {
            printf("\b \b");
            i -= 2;
        }
        else if (k[i] == 3)
        {
            printf("\n");
            exit(0);
        }
        else
        {
            printf("*");
        }
    }
}