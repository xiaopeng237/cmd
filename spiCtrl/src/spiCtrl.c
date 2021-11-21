/*
 * spiCtrl.c
 *
 *  Created on: 2021-8-22
 *      Author: vmuser
 */
#include "spiCtrl.h"
#include <stdarg.h>
int fdSpi[3];
static uint8_t mode = 0;
static uint8_t bits = 8;
static uint32_t speed = 2000000;
static uint16_t delay;

/*
 * 函数说明：SPI 初始化
 * 入参说明：
 * */
int spi_init(void)
{

	static const char *device = "/dev/spidev1.0";
	//static const char *device = "/dev/spidev1.1";
	int ret1 = 0;

	fdSpi[0] = open(device, O_RDWR);
	if (fdSpi[0] < 0)
		printf("can't open device");

	ret1 = ioctl(fdSpi[0], SPI_IOC_WR_MODE, &mode);//数据输入 模式0
	if (ret1 == -1)
		printf("can't set wr spi mode");

	ret1 = ioctl(fdSpi[0], SPI_IOC_RD_MODE, &mode);//数据发送 模式0
	if (ret1 == -1)
		printf("can't get spi mode");

	ret1 = ioctl(fdSpi[0], SPI_IOC_WR_BITS_PER_WORD, &bits);//设置每字长度8字节
	if (ret1 == -1)
		printf("can't set bits per word");

	ret1 = ioctl(fdSpi[0], SPI_IOC_RD_BITS_PER_WORD, &bits);
	if (ret1 == -1)
		printf("can't get bits per word");

	ret1 = ioctl(fdSpi[0], SPI_IOC_WR_MAX_SPEED_HZ, &speed);//输入命令
	if (ret1 == -1)
		printf("can't set max speed hz");

	ret1 = ioctl(fdSpi[0], SPI_IOC_RD_MAX_SPEED_HZ, &speed);//输出命令
	if (ret1 == -1)
		printf("can't get max speed hz");

	printf("spi mode: %d\n", mode);
	printf("bits per word: %d\n", bits);
	printf("max speed: %d Hz (%d KHz)\n", speed, speed/1000);
	return ret1;
}
/*
 * 函数说明：SPI 关闭
 * 入参说明：
 * */
int spi_closed(void)
{
	close(fdSpi[0]);
	return 0;
}
/*
 * 函数说明：SPI write register address
 * 入参说明：
 * busId ：SPI总线选择 0
 * cs :
 * Register： 寄存器地址
 * */
//int spi_write(int busId, int cs, int Register, int len, unsigned short int Datas)
int spi_write(int busId, int cs, int Register)
{
	int ret = 0;
	int i;
	uint8_t tx[2] = {0};

	tx[0] = (unsigned char)Register;
	tx[1] = (unsigned char)(Register >> 8);
	//printf("SPI write register =%x  data = %x  %x \n",Register, tx[0],tx[1]);
	struct spi_ioc_transfer tr_txrxw = {
	                .tx_buf = (unsigned long)tx,
	                .rx_buf = 0,
	                .len = 2,
	                .delay_usecs = delay,
	                .speed_hz = speed,
	                .bits_per_word = bits,
	};
	ret = ioctl(fdSpi[0], SPI_IOC_MESSAGE(1), &tr_txrxw);
	if (ret < 0)
	{
		printf("SPI write data faile ret: %d \n", ret);
	}

	return ret;
}

/*
 * 函数说明：SPI 读
 * 入参说明：
 * busId ：SPI总线选择 ：0
 * cs: 片选 ：0
 * Register： 寄存器地址
 * len：寄存器长度 ：2
 * */
int spi_read(int busId, int cs, int Register, int len, unsigned short int *spiDatas)
{
    unsigned char rx[2] = {0};
    int ret;
	unsigned short int data = 0;

	spi_write(busId, cs, Register);

    struct spi_ioc_transfer tr_txrx2 = {
            .tx_buf = 0,
            .rx_buf = (unsigned long)rx,
            .len = len,
            .delay_usecs = delay,
            .speed_hz = speed,
            .bits_per_word = bits,
    };
    ret = ioctl(fdSpi[0], SPI_IOC_MESSAGE(1), &tr_txrx2);
    if (ret == 1) {
        printf("can't revieve spi message");
    }
	
	data = (rx[0] * 256) + rx[1];
	*spiDatas = data;
	
	//printf("SPI read data(%x) = %x  %x \n",data, rx[0],rx[1]);

	return 0;
}
/*
 * 函数说明：SPI 读取脉冲数据
 * 入参说明：
 * */
void read_pluse(void)
{
	int i = 0;
    unsigned short int readData = 0;
	int SpiData_dpul[512];
	int ret;

	//start
	spi_read(0, 0, 0x001c, 2, &readData);

	//wait data ready
	for (i = 0; i < 60; i++)
	{
		spi_read(0, 0, 0x000E, 2, &readData);
		printf("SPI read data = %x  \n",readData);
		if (readData & 0x0001)
		{
			break;
		}
	}

	//read data
	for(i = 0;i < 512; i++){
		SpiData_dpul[i] = spi_read(0, 0, 0x000f, 2, &readData);
		printf("num, %d,pluse data, %d \n", i * 10, readData);
    }
	return;
}
/*
 * 函数说明：SPI 读取能谱数据
 * 入参说明：
 * */
void read_energy(void)
{
	int i = 0;
    unsigned short int readData = 0;
	unsigned int spiDataEnergy[8192];
	int ret;
	//start
	spi_read(0, 0, 0x001D, 2, &readData);
	//read data
	for(i = 0;i < 8192; i++){
		spi_read(0, 0, 0x0003, 2, &readData);
		spiDataEnergy[i] = (unsigned int)readData;
		printf("num, %d,energy data, %d \n", i, readData);
    }
	// for(i = 0;i < 120; i++){
	// 	spiDataEnergy[i] = 0x66;
    // }
	energy_check(spiDataEnergy);

}
void energy_check(unsigned int * energyData)
{
	int i;
	int count = 0;

	for(i = 500;i < 5000; i++){
		if ((energyData[i] == energyData[i + 1]) &&
		(energyData[i + 1] == energyData[i + 2]) &&
		(energyData[i + 2] == energyData[i + 3]) &&
		(energyData[i + 3] == energyData[i + 4]) &&
		(energyData[i + 4] == energyData[i + 5]) &&
		(energyData[i + 5] == energyData[i + 6]))
		{
			count++;
		}

    }

    char data_time1[25] = {};
    char spi_en[204800] = {};
    char p_log1[205800] = {};//add data

    FILE *fd_en;
    fd_en = fopen("/usr/local/apache/htdocs/yangpai/data-eu-gdp-growth.json", "w+b");
    for(i = 0; i < 8192; i++)
    {
        sprintf(data_time1, "[%d,%d],", i, energyData[i]);
        strcat(spi_en, data_time1);
    }

    sprintf(p_log1, "%s%s%s", "{\"label\":\"能谱曲线\",\"data\":[",spi_en,"[8192,0]]}");

    fputs(p_log1, fd_en);
    fclose(fd_en);

	if (count > 100)
	{
		fd_en = fopen("/gp/energy.json", "w+b");
    	for(i = 0; i < 8192; i++)
    	{
        	sprintf(data_time1, "[%d,%d],", i, energyData[i]);
        	strcat(spi_en, data_time1);
    	}

    	sprintf(p_log1, "%s%s%s", "{\"label\":\"能谱曲线\",\"data\":[",spi_en,"[8192,0]]}");

    	fputs(p_log1, fd_en);
    	fclose(fd_en);
	}

	return 0;
}
/*
 * 函数说明：触发窗口计数
 * 入参说明：
 * */
int trigger_win(void)
{
	unsigned short int readDataStart = 0;
	spi_read(0, 0, 0x001E, 2, &readDataStart);
	printf("SPI read 0x001E data = %x  \n",readDataStart);
}

/*
 * 函数说明：获取窗口计数
 * 入参说明：
 * */
void read_win(void)
{
	int i = 0;
	int k = 0;
	int j = 0;
    unsigned short int readData = 0;
  	int win[1200] = {0};
	int win_C[200][6] = {0};
	int sumWin[5] = {0};
	double winSum[5] = {0};
	double aveWin[5] = {0};
	//spi_read(0, 0, 0x001E, 2, &readData);

	//wait data ready
	for (i = 0; i < 20000; i++)
	{
		usleep(100);
		spi_read(0, 0, 0x000E, 2, &readData);
		//printf("SPI read 0x000E data = %x  \n",readData);
		if (readData & 0x0020)
		{
			break;
		}
	}
	if (i == 19999)
	{
		printf("please re-run \n");
		return;
	}

	for (i = 0; i < 200; i++)
	{
		spi_read(0, 0, 0x0010, 2, &readData);
		//printf("SPI read 0x0010 data = %x  \n",readData);
		if (readData & 0xaa55)
		{
			win[0] = 0xaa55;
			break;
		}
		
	}
	if (i == 199)
	{
		printf("please re-run \n");
		return;
	}

	for(i = 1; i < 1200; i++)
	{
		spi_read(0, 0, 0x0010, 2, &readData);
		win[i] = readData;
    }
	
	// for(i = 0; i < 1200; i = i + 6)
	// {
	// 	printf("WIN data[%d][0x%x], A = %d, B = %d, C = %d, D = %d, E = %d \n",
	// 		i/6, win[0], win[i + 1], win[i + 2], win[i + 3], win[i + 4], win[i + 5]);
	// }

	for(k = 0, i = 0; k < 200; k++)
    {
        if(win[i++] == 0xaa55)
		{
            for(j = 0; j < 5; j++)
            {
                if(win[i] != 0xaa55)
                {
                    win_C[k][j] = win[i++];
					//printf("tmp = %d \n", win_C[k][j]);
                }
                else
                {
                    i--;
                    break;
                }
            }
        }
    }

	for(j = 0; j < 5; j++)
	{
		k = 0;

		for(i = 0; i < 99; i++)
		{
			if(win_C[i][j] < 1000)
			{
				//printf("sumWin[%d] = %d win_C[%d][%d] = %d \n", j, sumWin[j], i, j, win_C[i][j]);
				sumWin[j] = sumWin[j] + win_C[i][j];
				k++;
			}
		}
		//printf("aveWin = %d k= %d \n", (int)aveWin[j], k);
		aveWin[j] = (double)sumWin[j] / k;
		if(aveWin[j] == 0)
		{
			aveWin[j] = 1;
		}

		for(i = 0; i < 200; i++)
		{
			if(win_C[i][j] >= 1000)
			{
				//printf("@@@ \n");
				win_C[i][j] = (int)aveWin[j];
			}
			if(win_C[i][j] > (int)(aveWin[j] * 10))
			{
				//printf("@@@ \n");
				win_C[i][j] = (int)aveWin[j];
			}
		}
	}
	//printf("aveWinA %d aveWinB %d aveWinC %d aveWinD %d aveWinE %d \n",
	//	(int)aveWin[0],(int)aveWin[1],(int)aveWin[2],(int)aveWin[3],(int)aveWin[4]);

	for(j = 0; j < 200; j++)
    {
        winSum[0] = winSum[0] + win_C[j][0];
		winSum[1] = winSum[1] + win_C[j][1];
		winSum[2] = winSum[2] + win_C[j][2];
		winSum[3] = winSum[3] + win_C[j][3];
		winSum[4] = winSum[4] + win_C[j][4];	
    }
	printf(" win %d\n win %d \n win %d \n win %d \n win %d \n",
		(int)winSum[0],(int)winSum[1],(int)winSum[2],(int)winSum[3],(int)winSum[4]);
	return;
}

/*
 * 函数说明：设置FPGA参数
 * 入参说明：
 * int reg ：寄存器地址
 * int value： 寄存器值
 * */
void set_th(int reg, int value)
{
	char reg1,reg2,data1,data2;
	int tmp;

    reg1 =(char) (0x81 + reg * 2);
    reg2 =(char) (0x82 + reg * 2);
    data1 = (char) (0x00ff & value);
    data2 = (char) (value >> 8);
	if ((reg == 0x1f) || (reg == 0x21) || (reg == 0x23) || (reg == 0x25))
	{
		reg1 =(char) (0x81 + reg);
    	reg2 =(char) (0x82 + reg);
	}

	tmp = ((data1 * 256) + reg1);
	printf("tmp = 0x%x \n", tmp);
	spi_write(0,0,tmp);
	tmp = ((data2 * 256) + reg2);
	printf("tmp = 0x%x \n", tmp);
	spi_write(0,0,tmp);
	return;
}
/*
 * 函数说明：设置一组默认FPGA参数
 * 入参说明：
 * */
void set_fpga_default(void)
{
	set_th(0x09, 201);
	set_th(0x00, 400);

	set_th(0x01, 100 * 2);
	set_th(0x02, 200 * 2);
	set_th(0x03, 175 * 2);
	set_th(0x04, 925 * 2);
	set_th(0x08, 1050 * 2);
	set_th(0x0e, 1825 * 2);
	set_th(0x1f, 2800 * 2);
	set_th(0x21, 3100 * 2);
	set_th(0x23, 3500 * 2);
	set_th(0x25, 6628 * 2);

	set_th(0x05, 5);
	set_th(0x07, 30000);
	set_th(0x0d, 400);
	set_th(0x0b, 40);
	set_th(0x0c, 200);
	return;	
}
/*
 * 函数说明：读取FPGA寄存器地址的值
 * 入参说明：
 * int reg：寄存器地址
 * */
int spi_readData(int reg)
{
	unsigned short int readDataStart = 0;
	spi_read(0, 0, reg, 2, &readDataStart);
	printf("SPI read %x data = %x  \n",reg, readDataStart);
}
void __attribute__ ((constructor)) my_init(void)
{
	spi_init();
}
void __attribute__ ((destructor)) my_fnit(void)
{
	spi_closed();
}
