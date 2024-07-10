#ifndef __FDL_CHANNEL_H_
#define __FDL_CHANNEL_H_

//for debug
/*
typedef struct FDL_ChannelHandlerUart
{
	unsigned int Regbase;
	unsigned int baudRate;
    int (*Open) (struct FDL_ChannelHandlerUart *channel, unsigned int  baudrate);
    int (*Read) (struct FDL_ChannelHandlerUart *channel, const unsigned char *buf, unsigned int  len);
    char (*GetChar) (struct FDL_ChannelHandlerUart *channel);
    int (*GetSingleChar) (struct FDL_ChannelHandlerUart *channel);
    int (*Write) (struct FDL_ChannelHandlerUart, const unsigned char *buf, unsigned int  len);
    int (*PutChar) (struct FDL_ChannelHandlerUart, const unsigned char ch);
    int (*SetBaudrate) (struct FDL_ChannelHandlerUart, unsigned int  baudrate);
    int (*Close) (struct FDL_ChannelHandlerUart *channel);

} FDL_ChannelHandlerUart_T;
*/


typedef struct FDL_ChannelHandler
{
    int (*Open) (struct FDL_ChannelHandler *channel, unsigned int  baudrate);
    int (*Read) (struct FDL_ChannelHandler *channel, const unsigned char *buf, unsigned int  len);
    char (*GetChar) (struct FDL_ChannelHandler *channel);
    int (*GetSingleChar) (struct FDL_ChannelHandler *channel);
    int (*Write) (struct FDL_ChannelHandler *channel, const unsigned char *buf, unsigned int  len);
    int (*PutChar) (struct FDL_ChannelHandler *channel, const unsigned char ch);
    int (*SetBaudrate) (struct FDL_ChannelHandler *channel, unsigned int  baudrate);
    int (*Close) (struct FDL_ChannelHandler *channel);
    void   *priv;
} FDL_ChannelHandler_T;

extern struct FDL_ChannelHandler *FDL_ChannelGet(void);

#endif