
#include "MPU9150.h"

uint8_t deviceAddress;
uint8_t buffer[16];

void MPU9150Init(void)
{
    MPU9150InitLib(MPU9150_ADD_DEFAULT);
    I2CDeviceSetDeviceAddress(deviceAddress);
    MPU9150SetPowerManagement1((1 << 7));
    uKernelDelayMiliseconds(250);
    MPU9150SetPowerManagement1(0x09);
    uKernelDelayMiliseconds(500);
    MPU9150SetGyroConfig(3 << 3);
    uKernelDelayMiliseconds(250);
    MPU9150SetAccelConfig(3 << 3);
}

void MPU9150InitLib(uint8_t address)
{
    if (address == 0)
        deviceAddress = MPU9150_ADD_DEFAULT;
    else
        deviceAddress = address;
}

bool MPU9150TestConnection(void)
{
    return MPU9150GetDeviceID() == 0x68;
}

void MPU9150GetSelfTestTrim(uint8_t *xa, uint8_t *ya, uint8_t *za,
                            uint8_t *xg, uint8_t *yg, uint8_t *zg)
{
    I2CDeviceSetDeviceAddress(deviceAddress);
    I2CDeviceReadBytes(MPU9150_REG_SELF_TEST_X, 4, &buffer[0]);

    *xg = buffer[0] & MPU9150_GYRO_TRIM;
    *yg = buffer[1] & MPU9150_GYRO_TRIM;
    *zg = buffer[2] & MPU9150_GYRO_TRIM;

    *xa = ((buffer[0] & MPU9150_ACCEL_TRIM_HIGH) >> 3) |
            ((buffer[3] & MPU9150_XACCEL_TRIM_LOW) >> 4);

    *ya = ((buffer[1] & MPU9150_ACCEL_TRIM_HIGH) >> 3) |
            ((buffer[3] & MPU9150_YACCEL_TRIM_LOW) >> 2);

    *za = ((buffer[2] & MPU9150_ACCEL_TRIM_HIGH) >> 3) |
            (buffer[3] & MPU9150_ZACCEL_TRIM_LOW);
}

uint8_t MPU9150GetSampleRateDivider(void)
{
    I2CDeviceSetDeviceAddress(deviceAddress);
    buffer[0] = I2CDeviceReadByte(MPU9150_REG_SMPRT_DIV);
    return buffer[0];
}

void MPU9150SetSampleRateDivider(uint8_t div)
{
    I2CDeviceSetDeviceAddress(deviceAddress);
    I2CDeviceWriteByte(MPU9150_REG_SMPRT_DIV, div);
}

uint8_t MPU9150GetConfig(void)
{
    I2CDeviceSetDeviceAddress(deviceAddress);
    buffer[0] = I2CDeviceReadByte(MPU9150_REG_CONFIG);
    return buffer[0];
}

void MPU9150SetConfig(uint8_t config)
{
    I2CDeviceSetDeviceAddress(deviceAddress);
    I2CDeviceWriteByte(MPU9150_REG_CONFIG, config);
}

uint8_t MPU9150GetGyroConfig(void)
{
    I2CDeviceSetDeviceAddress(deviceAddress);
    buffer[0] = I2CDeviceReadByte(MPU9150_REG_GYRO_CONFIG);
    return buffer[0];
}

void MPU9150SetGyroConfig(uint8_t config)
{
    I2CDeviceSetDeviceAddress(deviceAddress);
    I2CDeviceWriteByte(MPU9150_REG_GYRO_CONFIG, config);
}

uint8_t MPU9150GetAccelConfig(void)
{
    I2CDeviceSetDeviceAddress(deviceAddress);
    buffer[0] = I2CDeviceReadByte(MPU9150_REG_ACCEL_CONFIG);
    return buffer[0];
}

void MPU9150SetAccelConfig(uint8_t config)
{
    I2CDeviceSetDeviceAddress(deviceAddress);
    I2CDeviceWriteByte(MPU9150_REG_ACCEL_CONFIG, config);
}

uint8_t MPU9150GetFreefallThreshold(void)
{
    I2CDeviceSetDeviceAddress(deviceAddress);
    buffer[0] = I2CDeviceReadByte(MPU9150_REG_FF_THR);
    return buffer[0];
}

void MPU9150SetFreefallThreshold(uint8_t thr)
{
    I2CDeviceSetDeviceAddress(deviceAddress);
    I2CDeviceWriteByte(MPU9150_REG_FF_THR, thr);
}

uint8_t MPU9150GetFreefallDuration(void)
{
    I2CDeviceSetDeviceAddress(deviceAddress);
    buffer[0] = I2CDeviceReadByte(MPU9150_REG_FF_DUR);
    return buffer[0];
}

void MPU9150SetFreefallDuration(uint8_t dur)
{
    I2CDeviceSetDeviceAddress(deviceAddress);
    I2CDeviceWriteByte(MPU9150_REG_FF_DUR, dur);
}

uint8_t MPU9150GetMotionThreshold(void)
{
    I2CDeviceSetDeviceAddress(deviceAddress);
    buffer[0] = I2CDeviceReadByte(MPU9150_REG_MOT_THR);
    return buffer[0];
}

void MPU9150SetMotionThreshold(uint8_t thr)
{
    I2CDeviceSetDeviceAddress(deviceAddress);
    I2CDeviceWriteByte(MPU9150_REG_MOT_THR, thr);
}

uint8_t MPU9150GetMotionDuration(void)
{
    I2CDeviceSetDeviceAddress(deviceAddress);
    buffer[0] = I2CDeviceReadByte(MPU9150_REG_MOT_DUR);
    return buffer[0];
}

void MPU9150SetMotionDuration(uint8_t dur)
{
    I2CDeviceSetDeviceAddress(deviceAddress);
    I2CDeviceWriteByte(MPU9150_REG_MOT_DUR, dur);
}

uint8_t MPU9150GetZeroMotionThreshold(void)
{
    I2CDeviceSetDeviceAddress(deviceAddress);
    buffer[0] = I2CDeviceReadByte(MPU9150_REG_ZRMOT_THR);
    return buffer[0];
}

void MPU9150SetZeroMotionThreshold(uint8_t thr)
{
    I2CDeviceSetDeviceAddress(deviceAddress);
    I2CDeviceWriteByte(MPU9150_REG_ZRMOT_THR, thr);
}

uint8_t MPU9150GetZeroMotionDuration(void)
{
    I2CDeviceSetDeviceAddress(deviceAddress);
    buffer[0] = I2CDeviceReadByte(MPU9150_REG_ZRMOT_DUR);
    return buffer[0];
}

void MPU9150SetZeroMotionDuration(uint8_t dur)
{
    I2CDeviceSetDeviceAddress(deviceAddress);
    I2CDeviceWriteByte(MPU9150_REG_ZRMOT_DUR, dur);
}

uint8_t MPU9150GetFifoEn(void)
{
    I2CDeviceSetDeviceAddress(deviceAddress);
    buffer[0] = I2CDeviceReadByte(MPU9150_REG_FIFO_EN);
    return buffer[0];
}

void MPU9150SetFifoEn(uint8_t config)
{
    I2CDeviceSetDeviceAddress(deviceAddress);
    I2CDeviceWriteByte(MPU9150_REG_FIFO_EN, config);
}

uint8_t MPU9150GetI2CMasterControl(void)
{
    I2CDeviceSetDeviceAddress(deviceAddress);
    buffer[0] = I2CDeviceReadByte(MPU9150_REG_I2C_MST_CTRL);
    return buffer[0];
}

void MPU9150SetI2CMasterControl(uint8_t config)
{
    I2CDeviceSetDeviceAddress(deviceAddress);
    I2CDeviceWriteByte(MPU9150_REG_I2C_MST_CTRL, config);
}

uint8_t MPU9150GetI2CSlave0Address(void)
{
    I2CDeviceSetDeviceAddress(deviceAddress);
    buffer[0] = I2CDeviceReadByte(MPU9150_REG_I2C_SLV0_ADDR);
    return buffer[0];
}

void MPU9150SetI2CSlave0Address(uint8_t addr)
{
    I2CDeviceSetDeviceAddress(deviceAddress);
    I2CDeviceWriteByte(MPU9150_REG_I2C_SLV0_ADDR, addr);
}

uint8_t MPU9150GetI2CSlave0Register(void)
{
    I2CDeviceSetDeviceAddress(deviceAddress);
    buffer[0] = I2CDeviceReadByte(MPU9150_REG_I2C_SLV0_REG);
    return buffer[0];
}

void MPU9150SetI2CSlave0Register(uint8_t ra)
{
    I2CDeviceSetDeviceAddress(deviceAddress);
    I2CDeviceWriteByte(MPU9150_REG_I2C_SLV0_REG, ra);
}

uint8_t MPU9150GetI2CSlave0Control(void)
{
    I2CDeviceSetDeviceAddress(deviceAddress);
    buffer[0] = I2CDeviceReadByte(MPU9150_REG_I2C_SLV0_CTRL);
    return buffer[0];
}

void MPU9150SetI2CSlave0Control(uint8_t config)
{
    I2CDeviceSetDeviceAddress(deviceAddress);
    I2CDeviceWriteByte(MPU9150_REG_I2C_SLV0_CTRL, config);
}

uint8_t MPU9150GetI2CSlave1Address(void)
{
    I2CDeviceSetDeviceAddress(deviceAddress);
    buffer[0] = I2CDeviceReadByte(MPU9150_REG_I2C_SLV1_ADDR);
    return buffer[0];
}

void MPU9150SetI2CSlave1Address(uint8_t addr)
{
    I2CDeviceSetDeviceAddress(deviceAddress);
    I2CDeviceWriteByte(MPU9150_REG_I2C_SLV1_ADDR, addr);
}

uint8_t MPU9150GetI2CSlave1Register(void)
{
    I2CDeviceSetDeviceAddress(deviceAddress);
    buffer[0] = I2CDeviceReadByte(MPU9150_REG_I2C_SLV1_REG);
    return buffer[0];
}

void MPU9150SetI2CSlave1Register(uint8_t ra)
{
    I2CDeviceSetDeviceAddress(deviceAddress);
    I2CDeviceWriteByte(MPU9150_REG_I2C_SLV1_REG, ra);
}

uint8_t MPU9150GetI2CSlave1Control(void)
{
    I2CDeviceSetDeviceAddress(deviceAddress);
    buffer[0] = I2CDeviceReadByte(MPU9150_REG_I2C_SLV1_CTRL);
    return buffer[0];
}

void MPU9150SetI2CSlave1Control(uint8_t config)
{
    I2CDeviceSetDeviceAddress(deviceAddress);
    I2CDeviceWriteByte(MPU9150_REG_I2C_SLV1_CTRL, config);
}

uint8_t MPU9150GetI2CSlave2Address(void)
{
    I2CDeviceSetDeviceAddress(deviceAddress);
    buffer[0] = I2CDeviceReadByte(MPU9150_REG_I2C_SLV2_ADDR);
    return buffer[0];
}

void MPU9150SetI2CSlave2Address(uint8_t addr)
{
    I2CDeviceSetDeviceAddress(deviceAddress);
    I2CDeviceWriteByte(MPU9150_REG_I2C_SLV2_ADDR, addr);
}

uint8_t MPU9150GetI2CSlave2Register(void)
{
    I2CDeviceSetDeviceAddress(deviceAddress);
    buffer[0] = I2CDeviceReadByte(MPU9150_REG_I2C_SLV2_REG);
    return buffer[0];
}

void MPU9150SetI2CSlave2Register(uint8_t ra)
{
    I2CDeviceSetDeviceAddress(deviceAddress);
    I2CDeviceWriteByte(MPU9150_REG_I2C_SLV2_REG, ra);
}

uint8_t MPU9150GetI2CSlave2Control(void)
{
    I2CDeviceSetDeviceAddress(deviceAddress);
    buffer[0] = I2CDeviceReadByte(MPU9150_REG_I2C_SLV2_CTRL);
    return buffer[0];
}

void MPU9150SetI2CSlave2Control(uint8_t config)
{
    I2CDeviceSetDeviceAddress(deviceAddress);
    I2CDeviceWriteByte(MPU9150_REG_I2C_SLV2_CTRL, config);
}

uint8_t MPU9150GetI2CSlave3Address(void)
{
    I2CDeviceSetDeviceAddress(deviceAddress);
    buffer[0] = I2CDeviceReadByte(MPU9150_REG_I2C_SLV3_ADDR);
    return buffer[0];
}

void MPU9150SetI2CSlave3Address(uint8_t addr)
{
    I2CDeviceSetDeviceAddress(deviceAddress);
    I2CDeviceWriteByte(MPU9150_REG_I2C_SLV3_ADDR, addr);
}

uint8_t MPU9150GetI2CSlave3Register(void)
{
    I2CDeviceSetDeviceAddress(deviceAddress);
    buffer[0] = I2CDeviceReadByte(MPU9150_REG_I2C_SLV3_REG);
    return buffer[0];
}

void MPU9150SetI2CSlave3Register(uint8_t ra)
{
    I2CDeviceSetDeviceAddress(deviceAddress);
    I2CDeviceWriteByte(MPU9150_REG_I2C_SLV3_REG, ra);
}

uint8_t MPU9150GetI2CSlave3Control(void)
{
    I2CDeviceSetDeviceAddress(deviceAddress);
    buffer[0] = I2CDeviceReadByte(MPU9150_REG_I2C_SLV3_CTRL);
    return buffer[0];
}

void MPU9150SetI2CSlave3Control(uint8_t config)
{
    I2CDeviceSetDeviceAddress(deviceAddress);
    I2CDeviceWriteByte(MPU9150_REG_I2C_SLV3_CTRL, config);
}

uint8_t MPU9150GetI2CSlave4Address(void)
{
    I2CDeviceSetDeviceAddress(deviceAddress);
    buffer[0] = I2CDeviceReadByte(MPU9150_REG_I2C_SLV4_ADDR);
    return buffer[0];
}

void MPU9150SetI2CSlave4Address(uint8_t addr)
{
    I2CDeviceSetDeviceAddress(deviceAddress);
    I2CDeviceWriteByte(MPU9150_REG_I2C_SLV4_ADDR, addr);
}

uint8_t MPU9150GetI2CSlave4Register(void)
{
    I2CDeviceSetDeviceAddress(deviceAddress);
    buffer[0] = I2CDeviceReadByte(MPU9150_REG_I2C_SLV4_REG);
    return buffer[0];
}

void MPU9150SetI2CSlave4Register(uint8_t ra)
{
    I2CDeviceSetDeviceAddress(deviceAddress);
    I2CDeviceWriteByte(MPU9150_REG_I2C_SLV4_REG, ra);
}

uint8_t MPU9150GetI2CSlave4DataOut(void)
{
    I2CDeviceSetDeviceAddress(deviceAddress);
    buffer[0] = I2CDeviceReadByte(MPU9150_REG_I2C_SLV4_DO);
    return buffer[0];
}

void MPU9150SetI2CSlave4DataOut(uint8_t data)
{
    I2CDeviceSetDeviceAddress(deviceAddress);
    I2CDeviceWriteByte(MPU9150_REG_I2C_SLV4_DO, data);
}

uint8_t MPU9150GetI2CSlave4Control(void)
{
    I2CDeviceSetDeviceAddress(deviceAddress);
    buffer[0] = I2CDeviceReadByte(MPU9150_REG_I2C_SLV4_CTRL);
    return buffer[0];
}

void MPU9150SetI2CSlave4Control(uint8_t config)
{
    I2CDeviceSetDeviceAddress(deviceAddress);
    I2CDeviceWriteByte(MPU9150_REG_I2C_SLV4_CTRL, config);
}

uint8_t MPU9150GetI2CSlave4DataIn(void)
{
    I2CDeviceSetDeviceAddress(deviceAddress);
    buffer[0] = I2CDeviceReadByte(MPU9150_REG_I2C_SLV4_DI);
    return buffer[0];
}

void MPU9150SetI2CSlave4DataIn(uint8_t data)
{
    I2CDeviceSetDeviceAddress(deviceAddress);
    I2CDeviceWriteByte(MPU9150_REG_I2C_SLV4_DI, data);
}

uint8_t MPU9150GetI2CMasterStatus(void)
{
    I2CDeviceSetDeviceAddress(deviceAddress);
    buffer[0] = I2CDeviceReadByte(MPU9150_REG_I2C_MST_STATUS);
    return buffer[0];
}

uint8_t MPU9150GetInterruptPinConfig(void)
{
    I2CDeviceSetDeviceAddress(deviceAddress);
    buffer[0] = I2CDeviceReadByte(MPU9150_REG_INT_PIN_CFG);
    return buffer[0];
}

void MPU9150SetInterruptPinConfig(uint8_t config)
{
    I2CDeviceSetDeviceAddress(deviceAddress);
    I2CDeviceWriteByte(MPU9150_REG_INT_PIN_CFG, config);
}

uint8_t MPU9150GetInterruptConfig(void)
{
    I2CDeviceSetDeviceAddress(deviceAddress);
    buffer[0] = I2CDeviceReadByte(MPU9150_REG_INT_ENABLE);
    return buffer[0];
}

void MPU9150SetInterruptConfig(uint8_t config)
{
    I2CDeviceSetDeviceAddress(deviceAddress);
    I2CDeviceWriteByte(MPU9150_REG_INT_ENABLE, config);
}

uint8_t MPU9150GetInterruptStatus(void)
{
    I2CDeviceSetDeviceAddress(deviceAddress);
    buffer[0] = I2CDeviceReadByte(MPU9150_REG_INT_STATUS);
    return buffer[0];
}

int16_t MPU9150GetAccelX(void)
{
    I2CDeviceSetDeviceAddress(deviceAddress);
    I2CDeviceReadBytes(MPU9150_REG_ACCEL_XOUT_H, 2, buffer);
    return (buffer[0] << 8) || (buffer[1]);
}

int16_t MPU9150GetAccelY(void)
{
    I2CDeviceSetDeviceAddress(deviceAddress);
    I2CDeviceReadBytes(MPU9150_REG_ACCEL_YOUT_H, 2, buffer);
    return (buffer[0] << 8) || (buffer[1]);
}

int16_t MPU9150GetAccelZ(void)
{
    I2CDeviceSetDeviceAddress(deviceAddress);
    I2CDeviceReadBytes(MPU9150_REG_ACCEL_ZOUT_H, 2, buffer);
    return (buffer[0] << 8) || (buffer[1]);
}

int16_t MPU9150GetTemp(void)
{
    I2CDeviceSetDeviceAddress(deviceAddress);
    I2CDeviceReadBytes(MPU9150_REG_TEMP_OUT_H, 2, buffer);
    return (buffer[0] << 8) || (buffer[1]);
}

int16_t MPU9150GetGyroX(void)
{
    I2CDeviceSetDeviceAddress(deviceAddress);
    I2CDeviceReadBytes(MPU9150_REG_GYRO_XOUT_H, 2, buffer);
    return (buffer[0] << 8) || (buffer[1]);
}

int16_t MPU9150GetGyroY(void)
{
    I2CDeviceSetDeviceAddress(deviceAddress);
    I2CDeviceReadBytes(MPU9150_REG_GYRO_YOUT_H, 2, buffer);
    return (buffer[0] << 8) || (buffer[1]);
}

int16_t MPU9150GetGyroZ(void)
{
    I2CDeviceSetDeviceAddress(deviceAddress);
    I2CDeviceReadBytes(MPU9150_REG_GYRO_ZOUT_H, 2, buffer);
    return (buffer[0] << 8) || (buffer[1]);
}

uint8_t MPU9150GetMotionDetectionStatus(void)
{
    I2CDeviceSetDeviceAddress(deviceAddress);
    buffer[0] = I2CDeviceReadByte(MPU9150_REG_MOT_DETECT_STATUS);
    return buffer[0];
}

uint8_t MPU9150GetI2CSlave0DataOut(void)
{
    I2CDeviceSetDeviceAddress(deviceAddress);
    buffer[0] = I2CDeviceReadByte(MPU9150_REG_I2C_SLV0_DO);
    return buffer[0];
}

void MPU9150SetI2CSlave0DataOut(uint8_t data)
{
    I2CDeviceSetDeviceAddress(deviceAddress);
    I2CDeviceWriteByte(MPU9150_REG_I2C_SLV0_DO, data);
}

uint8_t MPU9150GetI2CSlave1DataOut(void)
{
    I2CDeviceSetDeviceAddress(deviceAddress);
    buffer[0] = I2CDeviceReadByte(MPU9150_REG_I2C_SLV1_DO);
    return buffer[0];
}

void MPU9150SetI2CSlave1DataOut(uint8_t data)
{
    I2CDeviceSetDeviceAddress(deviceAddress);
    I2CDeviceWriteByte(MPU9150_REG_I2C_SLV1_DO, data);
}

uint8_t MPU9150GetI2CSlave2DataOut(void)
{
    I2CDeviceSetDeviceAddress(deviceAddress);
    buffer[0] = I2CDeviceReadByte(MPU9150_REG_I2C_SLV2_DO);
    return buffer[0];
}

void MPU9150SetI2CSlave2DataOut(uint8_t data)
{
    I2CDeviceSetDeviceAddress(deviceAddress);
    I2CDeviceWriteByte(MPU9150_REG_I2C_SLV2_DO, data);
}

uint8_t MPU9150GetI2CSlave3DataOut(void)
{
    I2CDeviceSetDeviceAddress(deviceAddress);
    buffer[0] = I2CDeviceReadByte(MPU9150_REG_I2C_SLV3_DO);
    return buffer[0];
}

void MPU9150SetI2CSlave3DataOut(uint8_t data)
{
    I2CDeviceSetDeviceAddress(deviceAddress);
    I2CDeviceWriteByte(MPU9150_REG_I2C_SLV3_DO, data);
}

uint8_t MPU9150GetI2CMasterDelayControl(void)
{
    I2CDeviceSetDeviceAddress(deviceAddress);
    buffer[0] = I2CDeviceReadByte(MPU9150_REG_I2C_MST_DELAY_CTRL);
    return buffer[0];
}

void MPU9150SetI2CMasterDelayControl(uint8_t config)
{
    I2CDeviceSetDeviceAddress(deviceAddress);
    I2CDeviceWriteByte(MPU9150_REG_I2C_MST_DELAY_CTRL, config);
}

void MPU9150SetSignalPathReset(uint8_t config)
{
    I2CDeviceSetDeviceAddress(deviceAddress);
    I2CDeviceWriteByte(MPU9150_REG_SIGNAL_PATH_RESET, config);
}

uint8_t MPU9150GetMotionDetectionControl(void)
{
    I2CDeviceSetDeviceAddress(deviceAddress);
    buffer[0] = I2CDeviceReadByte(MPU9150_REG_MOT_DETECT_CTRL);
    return buffer[0];
}

void MPU9150SetMotionDetectionControl(uint8_t config)
{
    I2CDeviceSetDeviceAddress(deviceAddress);
    I2CDeviceWriteByte(MPU9150_REG_MOT_DETECT_CTRL, config);
}

uint8_t MPU9150GetUserControl(void)
{
    I2CDeviceSetDeviceAddress(deviceAddress);
    buffer[0] = I2CDeviceReadByte(MPU9150_REG_USER_CTRL);
    return buffer[0];
}

void MPU9150SetUserControl(uint8_t config)
{
    I2CDeviceSetDeviceAddress(deviceAddress);
    I2CDeviceWriteByte(MPU9150_REG_USER_CTRL, config);
}

uint8_t MPU9150GetPowerManagement1(void)
{
    I2CDeviceSetDeviceAddress(deviceAddress);
    buffer[0] = I2CDeviceReadByte(MPU9150_REG_PWR_MGMT_1);
    return buffer[0];
}

void MPU9150SetPowerManagement1(uint8_t config)
{
    I2CDeviceSetDeviceAddress(deviceAddress);
    I2CDeviceWriteByte(MPU9150_REG_PWR_MGMT_1, config);
}

uint8_t MPU9150GetPowerManagement2(void)
{
    I2CDeviceSetDeviceAddress(deviceAddress);
    buffer[0] = I2CDeviceReadByte(MPU9150_REG_PWR_MGMT_2);
    return buffer[0];
}

void MPU9150SetPowerManagement2(uint8_t config)
{
    I2CDeviceSetDeviceAddress(deviceAddress);
    I2CDeviceWriteByte(MPU9150_REG_PWR_MGMT_2, config);
}

uint16_t MPU9150GetFIFOCount(void)
{
    uint16_t temp;

    I2CDeviceSetDeviceAddress(deviceAddress);
    I2CDeviceReadBytes(MPU9150_REG_FIFO_COUNT_H, 2, &buffer[0]);

    temp = (uint16_t) (buffer[0] & 0x07);
    temp <<= 8;
    temp |= buffer[1];

    return temp;
}

uint8_t MPU9150GetFIFO(void)
{
    I2CDeviceSetDeviceAddress(deviceAddress);
    buffer[0] = I2CDeviceReadByte(MPU9150_REG_FIFO_R_W);
    return buffer[0];
}

void MPU9150SetFIFO(uint8_t data)
{
    I2CDeviceSetDeviceAddress(deviceAddress);
    I2CDeviceWriteByte(MPU9150_REG_FIFO_R_W, data);
}

uint8_t MPU9150GetDeviceID(void)
{
    I2CDeviceSetDeviceAddress(deviceAddress);
    buffer[0] = I2CDeviceReadByte(MPU9150_REG_WHO_AM_I);
    return buffer[0];
}

uint8_t MPU9150GetMagID(void)
{
    I2CDeviceSetDeviceAddress(MPU9150_ADD_MAG);
    buffer[0] = I2CDeviceReadByte(MPU9150_REG_WIA);
    return buffer[0];
}

uint8_t MPU9150GetMagInfo(void)
{
    I2CDeviceSetDeviceAddress(MPU9150_ADD_MAG);
    buffer[0] = I2CDeviceReadByte(MPU9150_REG_INFO);
    return buffer[0];
}

uint8_t MPU9150GetMagStatus1(void)
{
    I2CDeviceSetDeviceAddress(MPU9150_ADD_MAG);
    buffer[0] = I2CDeviceReadByte(MPU9150_REG_ST1);
    return buffer[0];
}

uint8_t MPU9150GetMagStatus2(void)
{
    I2CDeviceSetDeviceAddress(MPU9150_ADD_MAG);
    buffer[0] = I2CDeviceReadByte(MPU9150_REG_ST2);
    return buffer[0];
}

int16_t MPU9150GetMagX(void)
{
    I2CDeviceSetDeviceAddress(MPU9150_ADD_MAG);
    I2CDeviceReadBytes(MPU9150_REG_HXL, 2, &buffer[0]);
    return (buffer[1] << 8) || buffer[0];
}

int16_t MPU9150GetMagY(void)
{
    I2CDeviceSetDeviceAddress(MPU9150_ADD_MAG);
    I2CDeviceReadBytes(MPU9150_REG_HYL, 2, buffer);
    return (buffer[1] << 8) || buffer[0];
}

int16_t MPU9150GetMagZ(void)
{
    I2CDeviceSetDeviceAddress(MPU9150_ADD_MAG);
    I2CDeviceReadBytes(MPU9150_REG_HZL, 2, buffer);
    return (buffer[1] << 8) || buffer[0];
}

uint8_t MPU9150GetMagControl(void)
{
    I2CDeviceSetDeviceAddress(MPU9150_ADD_MAG);
    buffer[0] = I2CDeviceReadByte(MPU9150_REG_CNTL);
    return buffer[0];
}

void MPU9150SetMagControl(uint8_t config)
{
    I2CDeviceSetDeviceAddress(MPU9150_ADD_MAG);
    I2CDeviceWriteByte(MPU9150_REG_CNTL, config);
}

uint8_t MPU9150GetMagASTC(void)
{
    I2CDeviceSetDeviceAddress(MPU9150_ADD_MAG);
    buffer[0] = I2CDeviceReadByte(MPU9150_REG_ASTC);
    return buffer[0];
}

void MPU9150SetMagASTC(uint8_t config)
{
    I2CDeviceSetDeviceAddress(MPU9150_ADD_MAG);
    I2CDeviceWriteByte(MPU9150_REG_ASTC, config);
}

bool MPU9150GetMagI2CDisable(void)
{
    I2CDeviceSetDeviceAddress(MPU9150_ADD_MAG);
    buffer[0] = I2CDeviceReadByte(MPU9150_REG_I2CDIS);
    return (buffer[0] & MPU9150_MAG_I2C_DISABLE);
}

void MPU9150SetMagI2CDisable(bool disable)
{
    I2CDeviceSetDeviceAddress(MPU9150_ADD_MAG);

    if (disable)
    {
        I2CDeviceWriteByte(MPU9150_REG_I2CDIS, MPU9150_MAG_I2C_DISABLE);
    }
    else
    {
        I2CDeviceWriteByte(MPU9150_REG_I2CDIS, 0x00);
    }
}

uint8_t MPU9150GetMagASAX(void)
{
    I2CDeviceSetDeviceAddress(MPU9150_ADD_MAG);
    buffer[0] = I2CDeviceReadByte(MPU9150_REG_ASAX);
    return buffer[0];
}

void MPU9150SetMagASAX(uint8_t data)
{
    I2CDeviceSetDeviceAddress(MPU9150_ADD_MAG);
    I2CDeviceWriteByte(MPU9150_REG_ASAX, data);
}

uint8_t MPU9150GetMagASAY(void)
{
    I2CDeviceSetDeviceAddress(MPU9150_ADD_MAG);
    buffer[0] = I2CDeviceReadByte(MPU9150_REG_ASAY);
    return buffer[0];
}

void MPU9150SetMagASAY(uint8_t data)
{
    I2CDeviceSetDeviceAddress(MPU9150_ADD_MAG);
    I2CDeviceWriteByte(MPU9150_REG_ASAY, data);
}

uint8_t MPU9150GetMagASAZ(void)
{
    I2CDeviceSetDeviceAddress(MPU9150_ADD_MAG);
    buffer[0] = I2CDeviceReadByte(MPU9150_REG_ASAZ);
    return buffer[0];
}

void MPU9150SetMagASAZ(uint8_t data)
{
    I2CDeviceSetDeviceAddress(MPU9150_ADD_MAG);
    I2CDeviceWriteByte(MPU9150_REG_ASAZ, data);
}

void MPU9150Get9AxisValue(int16_t* ax, int16_t* ay, int16_t* az,
                          int16_t* gx, int16_t* gy, int16_t* gz,
                          int16_t* mx, int16_t* my, int16_t* mz)
{
    I2CDeviceSetDeviceAddress(deviceAddress);
    I2CDeviceReadBytes(MPU9150_REG_ACCEL_XOUT_H, 14, &buffer[0]);
    *ax = (((int16_t) buffer[0]) << 8) | buffer[1];
    *ay = (((int16_t) buffer[2]) << 8) | buffer[3];
    *az = (((int16_t) buffer[4]) << 8) | buffer[5];
    *gx = (((int16_t) buffer[8]) << 8) | buffer[9];
    *gy = (((int16_t) buffer[10]) << 8) | buffer[11];
    *gz = (((int16_t) buffer[12]) << 8) | buffer[13];

    I2CDeviceWriteByte(MPU9150_REG_INT_PIN_CFG, 0x02);
    I2CDeviceSetDeviceAddress(MPU9150_ADD_MAG);
    I2CDeviceWriteByte(MPU9150_REG_CNTL, 0x01); //enable the magnetometer
    I2CDeviceReadBytes(MPU9150_REG_HXL, 6, &buffer[0]);
    *mx = (((int16_t) buffer[0]) << 8) | buffer[1];
    *my = (((int16_t) buffer[2]) << 8) | buffer[3];
    *mz = (((int16_t) buffer[4]) << 8) | buffer[5];
}
