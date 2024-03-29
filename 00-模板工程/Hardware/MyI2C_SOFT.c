#include "stm32f10x.h"
#include "Delay.h"

/**
  * @brief  I2C写SCL
  * @param  BitValue 0或1
  * @retval 无
  */
void MyI2C_W_SCL(uint8_t BitValue)
{
    GPIO_WriteBit(GPIOB, GPIO_Pin_10, (BitAction) BitValue);
    Delay_us(10);
}

/**
  * @brief  I2C写SDA
  * @param  BitValue 0或1
  * @retval 无
  */
void MyI2C_W_SDA(uint8_t BitValue)
{
    GPIO_WriteBit(GPIOB, GPIO_Pin_11, (BitAction) BitValue);
    Delay_us(10);
}

/**
  * @brief  I2C读SDA
  * @retval BitValue 0或1
  */
uint8_t MyI2C_R_SDA(void)
{
    uint8_t BitValue;
    BitValue = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11);
    Delay_us(10);
    return BitValue;
}

/**
  * @brief  I2C初始化
  * @retval 无
  */
void MyI2C_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; // 开漏 软件I2C开启普通开漏模式即可
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;  // 引脚
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    // 默认电平
    GPIO_SetBits(GPIOB, GPIO_Pin_10 | GPIO_Pin_11); // 高电平空闲状态
}

/**
  * @brief  I2C起始
  * @retval 无
  */
void MyI2C_Start(void)
{
    MyI2C_W_SDA(1); // 确保SDA先被拉低以兼容重复起始条件
    MyI2C_W_SCL(1);
    MyI2C_W_SDA(0);
    MyI2C_W_SCL(0);
}

/**
  * @brief  I2C终止
  * @retval 无
  */
void MyI2C_Stop(void)
{
    MyI2C_W_SDA(0);
    MyI2C_W_SCL(1);
    MyI2C_W_SDA(1);
}

/**
  * @brief  I2C发送一个字节
  * @param  Byte 要发送的字节
  * @retval 无
  */
void MyI2C_SendByte(uint8_t Byte)
{
    uint8_t i;
    for (i = 0; i < 8; i++)
    {
        MyI2C_W_SDA(Byte & (0x80 >> i));
        MyI2C_W_SCL(1);
        MyI2C_W_SCL(0);
    }
}

/**
  * @brief  I2C接收一个字节
  * @retval 接收到的字节
  */
uint8_t MyI2C_ReceiveByte(void)
{
    uint8_t i, Byte = 0x00;

    MyI2C_W_SDA(1);
    for (i = 0; i < 8; i++)
    {
        MyI2C_W_SCL(1);
        if (MyI2C_R_SDA() == 1) Byte |= (0x80 >> i);
        MyI2C_W_SCL(0);
    }

    return Byte;
}

/**
  * @brief  I2C发送应答
  * @param  AckBit 发送的应答位
  * @retval 无
  */
void MyI2C_SendAck(uint8_t AckBit)
{
    MyI2C_W_SDA(AckBit);
    MyI2C_W_SCL(1);
    MyI2C_W_SCL(0);
}

/**
  * @brief  I2C接收应答
  * @retval 接收到的应答位
  */
uint8_t MyI2C_ReceiveAck(void)
{
    uint8_t AckBit;

    MyI2C_W_SDA(1);
    MyI2C_W_SCL(1);
    AckBit = MyI2C_R_SDA();
    MyI2C_W_SCL(0);

    return AckBit;
}
