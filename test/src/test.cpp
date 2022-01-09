#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

#define IOMUXC_BASE          0x020E0000
#define IOMUXC_SIZE          0x00003FFF

/*
000 ALT0 — Select signal ECSPI1_SS3.
- Configure register IOMUXC_ECSPI1_SS3_SELECT_INPUT for mode ALT0.
001 ALT1 — Select signal ENET_CRS.
010 ALT2 — Select signal HDMI_TX_DDC_SCL.
- Configure register IOMUXC_HDMI_II2C_CLKIN_SELECT_INPUT for mode ALT2.
011 ALT3 — Select signal KEY_COL3.
100 ALT4 — Select signal I2C2_SCL.
- Configure register IOMUXC_I2C2_SCL_IN_SELECT_INPUT for mode ALT4.
101 ALT5 — Select signal GPIO4_IO12.
110 ALT6 — Select signal SPDIF_IN.
- Configure register IOMUXC_SPDIF_SPDIF_IN1_SELECT_INPUT for mode ALT6.
*/
#define Pad_Mux_Register         0x210
#define I2C2_SCL_IN_SELECT_INPUT 0x8A0
//gpio4-12
#define GPIO_BASE          0x0209C000
#define GPIO_SIZE          0x0001801c
#define GPIO_GROUP_DR      0x0000C000
#define GPIO_GROUP_DIR     0x0000C004
#define GPIO_GROUP_PSR     0x0000C008
int main ()
{
   unsigned int reg;
   int dev_fd = open( "/dev/mem", O_RDWR | O_NDELAY );
   if (dev_fd == -1)
   {
      perror("open /dev/mem");
      return -1;
   }
   unsigned char *map_base=(unsigned char * )mmap(NULL, IOMUXC_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, dev_fd, IOMUXC_BASE );  
   unsigned int mux_reg = *(volatile unsigned int *)(map_base + Pad_Mux_Register);
   /*将IIC clk 改为gpio模式*/
   printf("IOMUXC_SW_MUX_CTL_PAD_KEY_COL3 = 0x%x \n", *(volatile unsigned int *)(map_base + Pad_Mux_Register));
   *(volatile unsigned int *)(map_base + Pad_Mux_Register) = (mux_reg & 0xFFFFFFE8) | 0x5;
   printf("IOMUXC_SW_MUX_CTL_PAD_KEY_COL3 = 0x%x \n", *(volatile unsigned int *)(map_base + Pad_Mux_Register));

   printf("I2C2_SCL_IN_SELECT_INPUT = 0x%x \n", *(volatile unsigned int *)(map_base + I2C2_SCL_IN_SELECT_INPUT));
   *(volatile unsigned int *)(map_base + I2C2_SCL_IN_SELECT_INPUT) = 0x0;
   printf("I2C2_SCL_IN_SELECT_INPUT = 0x%x \n", *(volatile unsigned int *)(map_base + I2C2_SCL_IN_SELECT_INPUT));
   
   
   unsigned char *gpio_base=(unsigned char * )mmap(NULL, GPIO_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, dev_fd, GPIO_BASE );
   /*读取gpio寄存器状态*/
   unsigned int gpio_dr = *(volatile unsigned int *)(gpio_base + GPIO_GROUP_DR);
   printf("gpio_4_dr(bit 12) = 0x%x  \n", gpio_dr);
   
   unsigned int gpio_dir = *(volatile unsigned int *)(gpio_base + GPIO_GROUP_DIR);
   printf("gpio_4_dir(bit 12) = 0x%x  \n", gpio_dir);

   unsigned int gpio_psr = *(volatile unsigned int *)(gpio_base + GPIO_GROUP_PSR);
   printf("gpio_4_psr(bit 12) = 0x%x  \n", gpio_psr);
   
   /*设置gpio方向*/
   printf("gpio_4_12 set out mode \n");
   gpio_dir = *(volatile unsigned int *)(gpio_base + GPIO_GROUP_DIR);
   *(volatile unsigned int *)(gpio_base + GPIO_GROUP_DIR) |= 0x1<<12;
   printf("gpio_4_dir(bit 12) = 0x%x  \n", *(volatile unsigned int *)(gpio_base + GPIO_GROUP_DIR));

   /*时钟输出*/
   for (int i = 0; i < 5; i++)
   {
      gpio_dr = *(volatile unsigned int *)(gpio_base + GPIO_GROUP_DR);
      *(volatile unsigned int *)(gpio_base + GPIO_GROUP_DR) |= 0x1<<12;
      printf("gpio_4_dr(bit 12) = 0x%x  \n", *(volatile unsigned int *)(gpio_base + GPIO_GROUP_DR));
      usleep(200);
      gpio_dr = *(volatile unsigned int *)(gpio_base + GPIO_GROUP_DR);
      *(volatile unsigned int *)(gpio_base + GPIO_GROUP_DR) &= ~(0x1<<12);
      printf("gpio_4_dr(bit 12) = 0x%x  \n", *(volatile unsigned int *)(gpio_base + GPIO_GROUP_DR));
      usleep(200);
   }

   mux_reg = *(volatile unsigned int *)(map_base + Pad_Mux_Register);
   printf("IOMUXC_SW_MUX_CTL_PAD_KEY_COL3 = 0x%x \n", *(volatile unsigned int *)(map_base + Pad_Mux_Register));
   *(volatile unsigned int *)(map_base + Pad_Mux_Register) = (mux_reg & 0xFFFFFFE8) | 0x14;
   printf("IOMUXC_SW_MUX_CTL_PAD_KEY_COL3 = 0x%x \n", *(volatile unsigned int *)(map_base + Pad_Mux_Register));

   printf("I2C2_SCL_IN_SELECT_INPUT = 0x%x \n", *(volatile unsigned int *)(map_base + I2C2_SCL_IN_SELECT_INPUT));
   *(volatile unsigned int *)(map_base + I2C2_SCL_IN_SELECT_INPUT) = 0x1;
   printf("I2C2_SCL_IN_SELECT_INPUT = 0x%x \n", *(volatile unsigned int *)(map_base + I2C2_SCL_IN_SELECT_INPUT));

   close(dev_fd);
   if (map_base == MAP_FAILED)
   {
      perror("mmap");
      return -1;
   }
   munmap(map_base, IOMUXC_SIZE);
   munmap(gpio_base, IOMUXC_SIZE);
   return 0;
}