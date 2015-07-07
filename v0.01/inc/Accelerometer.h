#include "stm32f4xx_gpio.h"
#include "stm32f4xx_spi.h"

#define ACCEL_INFO1     (0x0D)
#define ACCEL_INFO02    (0x0E)
#define ACCEL_WHO_AM_I  (0x0F)
#define ACCEL_CTRL_REG3 (0x23)
#define ACCEL_CTRL_REG4 (0x20)
#define ACCEL_CTRL_REG5 (0x24)
#define ACCEL_CTRL_REG6 (0x25)
#define ACCEL_OUT_T     (0x0C)
#define ACCEL_OUT_XL    (0x28)
#define ACCEL_OUT_XH    (0x29)
#define ACCEL_OUT_YL    (0x2A)
#define ACCEL_OUT_YH    (0x2B)
#define ACCEL_OUT_ZL    (0x2C)
#define ACCEL_OUT_ZH    (0x2D)

#define UPSIDEDOWN 0x0040
#define RIGHTSIDE  0x0080
#define LEFTSIDE   0x0100

struct accelerometer
{
	short int x;
	short int y;
	short int z;
};

typedef struct accelerometer accelerometer;

extern accelerometer accel;

void accelInit(void);

void updateAccel(void);

void SPI_send(uint8_t address, uint8_t data);

uint8_t SPI_read(uint8_t address);
