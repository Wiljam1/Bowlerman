#include <stdio.h>
#include "application.h"

int main()
{
    Application theApp = createApplication();
    applicationUpdate(theApp);
    destoryApplication(theApp);
    return 0;
}