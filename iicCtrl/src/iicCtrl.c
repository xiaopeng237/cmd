/*
 * iicCtrl.c
 *
 *  Created on: 2021-8-22
 *      Author: vmuser
 */
#include "iicCtrl.h"
#include <stdarg.h>
int fdIic[3];

int iic_init(void)
{
	fdIic[0] = open(IIC_DEVICE0,O_RDWR);
	if(fdIic[0] == -1)
    	perror("open i2c-0 \n");
	fdIic[1] = open(IIC_DEVICE1,O_RDWR);
	if(fdIic[1] == -1)
    	perror("open i2c-1 \n");
	fdIic[2] = open(IIC_DEVICE2,O_RDWR);
	if(fdIic[2] == -1)
    	perror("open i2c-2 \n");
	return 0;
}
int iic_closed(void)
{
	close(fdIic[0]);
	close(fdIic[1]);
	close(fdIic[2]);
	return 0;
}
/*
 * 函数说明：Iic 写
 * 入参说明：
 * busId ：IIC总线选择 0/1/2
 * deviceAddress：设备地址
 * Register： 寄存器地址
 * len：寄存器长度
 * ... ：要写入的值
 * */
int iic_write(int busId, int deviceAddress, int Register, int len, int data1, int data2)
{
	unsigned char address;
	unsigned char reg;
	unsigned char dataBuf[20];
	int ret = 0;
	int i;

	address = (unsigned char)deviceAddress;
	reg = (unsigned char)Register;

//	printf("iic write len: %d \n", len);

	dataBuf[0] = reg;
	dataBuf[1] = (unsigned char)data1;
	dataBuf[2] = (unsigned char)data2;
//	printf("address = 0x%x reg = 0x%x \n", address, reg);
//	printf("dataBuf[1] = 0x%x dataBuf[2] = 0x%x \n", dataBuf[1], dataBuf[2]);
	ret = ioctl(fdIic[busId], I2C_SLAVE_FORCE, address >> 1);
	if (ret < 0) {
		printf("setenv address faile ret: %d \n", ret);
		return ret;
	}

	ret = write(fdIic[busId], dataBuf, len + 1);
	if (ret < 0)
	{
		printf("IIC write data faile ret: %d \n", ret);
	}

	return 0;
}
/*
 * 函数说明：Iic 读
 * 入参说明：
 * busId ：IIC总线选择 0/1/2
 * deviceAddress：设备地址
 * Register： 寄存器地址
 * len：寄存器长度
 * */
int iic_read_m(int busId, int deviceAddress, int Register, int len, unsigned short int *iicDatas)
{
	unsigned char address;
	unsigned char reg;
	unsigned char dataBuf[20];
	int ret = 0;
	int i;

	address = (unsigned char)deviceAddress;
	reg = (unsigned char)Register;
	unsigned short int data = 0;

	ret = ioctl(fdIic[busId], I2C_SLAVE_FORCE, address >> 1);
	if (ret < 0) {
		printf("setenv address faile ret: %d \n", ret);
		return ret;
	}

	ret = write(fdIic[busId], &reg, 1);
	if (ret < 0)
	{
		printf("IIC write data faile ret: %d \n", ret);
	}

	ret = read(fdIic[busId], dataBuf, len);
	if (ret < 0)
	{
		printf("IIC Read data faile ret: %d \n", ret);
	}

	// for (i = 0; i < len; ++i)
	// {
	// 	printf("iic read data[%d]: 0x%x \n", i, dataBuf[i]);
	// }
	data = dataBuf[0] *256 + dataBuf[1];
	//printf("iic read data: 0x%x \n", data);	
	*iicDatas = data;
	return 0;
}

int iic_read (int busId, int deviceAddress, int Register, int len)
{
	int i;
	unsigned short int readData = 0;
	iic_read_m(busId, deviceAddress, Register, len, &readData);
	printf("iic read data: 0x%x \n", readData);
}

int iic_read_pressure (void)
{
	int i;
	unsigned short int readData = 0;
	iic_write(2, 0x92, 1, 2, 0xb0, 0xe3);
	iic_read_m(2, 0x92, 0, 2, &readData);
	printf("iic read pressure: %d \n", readData);
}

int iic_read_delta_pressure (void)
{
	int i;
	unsigned short int readData = 0;
	iic_write(2, 0x94, 1, 2, 0xb0, 0xe3);
	iic_read_m(2, 0x94, 0, 2, &readData);
	printf("iic read delta pressure: %d \n", readData);
}

int iic_read_pressure_temperature (void)
{
	int i;
	unsigned short int readData = 0;
	iic_write(2, 0x90, 1, 2, 0xaa, 0x83);
	iic_read_m(2, 0x90, 0, 2, &readData);
	readData = readData | ~0x8000;
	printf("iic read pressure temperature: %d \n", readData);
}

int iic_read_delta_pressure_temperature (void)
{
	int i;
	unsigned short int readData = 0;
	iic_write(2, 0x90, 1, 2, 0x9a, 0x83);
	iic_read_m(2, 0x90, 0, 2, &readData);
	printf("iic read delta pressure temperature: %d \n", readData);
}

int iic_read_board_temperature (void)
{
	int i;
	unsigned short int readData = 0;
	iic_write(2, 0x90, 1, 2, 0xba, 0x83);
	iic_read_m(2, 0x90, 0, 2, &readData);
	printf("iic read board temperature: %d \n", readData);
}

void __attribute__ ((constructor)) my_init(void)
{
	iic_init();
}
void __attribute__ ((destructor)) my_fnit(void)
{
	iic_closed();
}
