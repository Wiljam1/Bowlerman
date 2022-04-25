
#ifndef application_h
#define application_h

typedef struct Application_type *Application;

Application createApplication();
void applicationUpdate(Application theApp);
void destoryApplication(Application theApp);

#endif
