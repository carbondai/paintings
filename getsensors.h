#ifndef GETSENSORS_H
#define GETSENSORS_H

#endif // GETSENSORS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "i2c-dev.h"
#include <sys/ioctl.h>
#include "i2cbusses.h"

#ifdef __cplusplus
extern "C" {
#endif
int set_reg(int, int, int, int);
float get_sensors(int, int, int);
float read_gpu(const char *, const char *);
#ifdef __cplusplus
}
#endif
