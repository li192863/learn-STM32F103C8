#include <stdio.h>
#include <stdarg.h>
#include "stm32f10x.h"

/**
  * @brief  串口初始化
  * @retval 无
  */
void Serial_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    USART_InitTypeDef USART_InitStruct;
    USART_InitStruct.USART_BaudRate = 9600;
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStruct.USART_Mode = USART_Mode_Tx;
    USART_InitStruct.USART_Parity = USART_Parity_No;
    USART_InitStruct.USART_StopBits = USART_StopBits_1;
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;
    USART_Init(USART1, &USART_InitStruct);
    
    USART_Cmd(USART1, ENABLE);
}

/**
  * @brief  串口发送字节
  * @param  Byte 待发送字节
  * @retval 无
  */
void Serial_SendByte(uint8_t Byte)
{
    USART_SendData(USART1, Byte);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}

/**
  * @brief  串口发送数组
  * @param  Array 待发送数组
  * @param  Length 数组长度
  * @retval 无
  */
void Serial_SendArray(uint8_t *Array, uint16_t Length)
{
    uint16_t i;
    for (i = 0; i < Length; i++)
    {
        Serial_SendByte(Array[i]);
    }
}

/**
  * @brief  串口发送字符串
  * @param  String 待发送字符串
  * @retval 无
  */
void Serial_SendString(char *String)
{
    uint16_t i;
    for (i = 0; String[i]; i++)
    {
        Serial_SendByte(String[i]);
    }
}

/**
  * @brief  串口计算幂函数
  * @param  X 底数
  * @param  Y 指数
  * @retval 结果X^Y
  */
uint32_t Serial_Pow(uint32_t X, uint32_t Y)
{
    uint32_t i, Result = 1;
    for (i = 0; i < Y; i++)
    {
        Result *= X;
    }
    return Result;
}

/**
  * @brief  串口发送数字
  * @param  Number 数字
  * @param  Length 数字长度
  * @retval 无
  */
void Serial_SendNumber(uint32_t Number, uint8_t Length)
{
    uint8_t i;
    for (i = 0; i < Length; i++)
    {
        Serial_SendByte(Number / Serial_Pow(10, Length - i - 1) % 10 + '0');
    }
}

/**
  * @brief  串口输出字符
  * @param  ch 字符
  * @param  f 文件
  * @retval 字符
  */
int fputc(int ch, FILE *f)
{
    Serial_SendByte(ch);
    return ch;
}

/**
  * @brief  串口打印函数
  * @param  format 格式化字符串
  * @retval 无
  */
void Serial_Printf(char *format, ...)
{
    char String[100];
    va_list arg;
    va_start(arg, format);
    vsprintf(String, format, arg);
    va_end(arg);
    Serial_SendString(String);
}
