#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>

void clErrCheck(cl_int err, char* msg) {
    if (err != CL_SUCCESS) {
        printf_s("Fatal CL Error %d when trying to execute %s", err, msg);
        exit(1);
    }
}