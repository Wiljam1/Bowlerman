#include <stdio.h>
#include "application.h"

int main()
{
    Application theApp = createApplication();
    applicationUpdate(theApp);
    destoryApplication(theApp);
    printf("Niklas har strul med SDL_net\n");
    return 0;
}