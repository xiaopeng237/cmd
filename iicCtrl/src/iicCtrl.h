/*
 * iicCtrl.h
 *
 *  Created on: 2021-8-22
 *      Author: vmuser
 */

#ifndef IICCTRL_H_
#define IICCTRL_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>

#define I2C_SLAVE_FORCE	0x0706	/* Use this slave address, even if it is already in use by a driver! */
#define IIC_DEVICE0	"/dev/i2c-0"
#define IIC_DEVICE1	"/dev/i2c-1"
#define IIC_DEVICE2	"/dev/i2c-2"

#endif /* IICCTRL_H_ */
