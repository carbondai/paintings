/*
 * getsensors.c - A user-space program to read an I2C register.
 * Copyright (c) 2016 Dai Xin <daixin2015@icloud.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "i2c-dev.h"
#include <sys/ioctl.h>
#include "i2cbusses.h"
#include "getsensors.h"

int set_reg(int i2cbus, int addr, int daddr, int val)
{
    int file;
    int force = 1;
    char filename[20];

    file = open_i2c_dev(i2cbus, filename, sizeof(filename), 0);
    set_slave_addr(file, addr, force);
    //ioctl(file, I2C_SLAVE, addr);

    i2c_smbus_write_byte_data(file, daddr, val);

    close(file);
    return 0;
}
float get_sensors(int i2cbus, int addr, int daddr)
{
    int vrlsb, res, file;
    int force = 1;
    float val;
    int daddr_vrlsb = 0x3c;
    char filename[20];

    file = open_i2c_dev(i2cbus, filename, sizeof(filename), 0);
    set_slave_addr(file, addr, force);
   //ioctl(file, I2C_SLAVE, addr);


    res = i2c_smbus_read_byte_data(file, daddr);
    vrlsb = i2c_smbus_read_byte_data(file, daddr_vrlsb);

    switch(daddr){
        case 0x1f:
        case 0x20:
            val = res + (vrlsb >> 7) * 0.5 + ((vrlsb >> 6) & 0x1) * 0.25;
            break;
        case 0x1c:
        case 0x1d:
        case 0x1e:
            val = ((res << 2) + (vrlsb >> 6)) * 0.006;
            break;
        case 0x2e:
		case 0x2f:
            val = 1.35e6 / ((res << 4) + (vrlsb >> 4));
            break;
        default:
            val = ((res << 2) + (vrlsb >> 6)) * 0.002;
            break;
    }
    close(file);
    return val;



}

float read_gpu(const char *path, const char *name)
{
    char n[255];
    FILE *f;
    int res;
    double value;
    
    snprintf(n, 255, "%s/%s", path, name);

    f = fopen(n, "r");
    res = fscanf(f, "%lf", &value);
    fclose(f);

    value /= 1000;

    return value;
}

/*int main(void)
{
    float value,vol1,vol2,vol3,vol4,vol5,vol6,vol7,vol8,vol9,vol10,vol11,temp1, temp2, temp3;
    int fan;
    while(1)
	{
	    vol1 = get_sensors(1, 0x2c, 0x10); 
	    vol2 = get_sensors(1, 0x2c, 0x12); 
	    vol3 = get_sensors(1, 0x2c, 0x13); 
	    vol4 = get_sensors(1, 0x2c, 0x15); 
	    vol5 = get_sensors(1, 0x2c, 0x16) * 2; 
        value = get_sensors(1, 0x2c, 0x21) * 2;
	    vol6 = get_sensors(1, 0x2c, 0x22) * 3; 
	    vol7 = get_sensors(1, 0x2c, 0x23) * 12; 
	    vol8 = get_sensors(1, 0x2c, 0x24); 
	    vol9 = get_sensors(1, 0x2c, 0x1c); 
	    vol10 = get_sensors(1, 0x2c, 0x1d); 
	    vol11 = get_sensors(1, 0x2c, 0x1e); 
	    temp1 = get_sensors(1, 0x2c, 0x1f); 
	    temp2 = get_sensors(1, 0x2c, 0x20); 
	    fan = (int)get_sensors(1, 0x2c, 0x2f); 
	    temp3 = read_gpu("/sys/class/hwmon/hwmon0", "temp1_input");
	    
	    printf("vsen1=%.2f vsen3=%.2f vsen4=%.2f vsen6=%.2f vsen7=%.2f\nvdsen14=%.2f vdsen15=%.2f vdsen16=%.2f vdsen17=%.2f\n3vdd%.2f 3vsb%.2f vbat%.2f\n",vol1,vol2,vol3,vol4,vol5,value,vol6,vol7,vol8,vol9,vol10,vol11);
	    printf("cpu_temp %.1f°C switch_temp %.1f°C\n", temp1, temp2);
	    printf("fan %dRPM\n", fan);
	    printf("gpu_temp %.1f°C\n", temp3);

	    sleep(2);
	}
    //return 0;
}*/
