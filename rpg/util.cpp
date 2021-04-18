#include <iostream>

#include "main.h"
#ifndef WIN32
#include <unistd.h>     /* usleep */
#include <termios.h>
#include <fcntl.h>
#endif

using namespace std;

#include "util.h"

#ifndef WIN32
int _kbhit(void)
{
    struct termios oldt, newt;
    int ch;
    int oldf;
    
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    
    ch = getchar();
    
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    
    if(ch != EOF)
    {
        ungetc(ch, stdin);
        return 1;
    }
    
    return 0;
}
#endif

char toUpperCase(char lowerCaseChar) {
    return ((lowerCaseChar-'a')+'A');
}
