#include "bsp_uart.h"
#include "usart.h"
#include <string.h>

typedef struct
{
	UART_HandleTypeDef	*huart;
	uint8_t*            tx_buf;
	uint16_t 		    tx_buf_len;
	uint8_t*			rx_buf;
	uint16_t			rx_buf_len;
}bsp_uart_priv_t;

static bsp_uart_priv_t bsp_uart_priv[1];

int8_t bsp_uart_init(bsp_uart_t index)
{	
	static uint8_t debug_uart_tx_buf[512];

	UART_HandleTypeDef* huart = bsp_uart_priv[index].huart;
	if( huart != NULL )
	{
		return 0;
	}

	switch(index)
	{
		case DEBUG_UART:
			MX_LPUART1_UART_Init();
			bsp_uart_priv[DEBUG_UART].huart = &hlpuart1;
			bsp_uart_priv[DEBUG_UART].tx_buf = debug_uart_tx_buf;
			bsp_uart_priv[DEBUG_UART].tx_buf_len = sizeof(debug_uart_tx_buf);
			break;
	}

    return 0;
}

int8_t bsp_uart_deinit(bsp_uart_t index)
{
    UART_HandleTypeDef* huart = bsp_uart_priv[index].huart;
	HAL_StatusTypeDef ret = HAL_OK;

	if( huart == NULL )
	{
		return -1;
	}

	ret = HAL_UART_Abort(huart);
	if( ret != HAL_OK)
	{
		return -1;
	}

	ret = HAL_UART_DeInit(huart);
	if( ret != HAL_OK)
	{
		return -1;
	}

	HAL_UART_MspDeInit(huart);

	bsp_uart_priv[index].huart = NULL;

	return 0;
}

int32_t bsp_uart_write(bsp_uart_t index , uint8_t* buf , uint16_t len)
{
	bsp_uart_priv_t*	bsp_uart = &bsp_uart_priv[index];

	if( bsp_uart->huart == NULL )
	{
		return -1;
	}

	//防止覆盖新数据覆盖未完成发送的buf
	while( (HAL_UART_GetState(bsp_uart->huart) & HAL_UART_STATE_BUSY_TX) != HAL_UART_STATE_READY );
	if( len < bsp_uart->tx_buf_len )
	{
		memcpy( bsp_uart->tx_buf , buf , len );
	}
	else
	{
		return -1;
	}

	HAL_StatusTypeDef ret = HAL_UART_Transmit_IT( bsp_uart->huart , bsp_uart->tx_buf , len );

    return ret == HAL_OK ? len : -1;
}

int32_t bsp_uart_read(bsp_uart_t index , uint8_t* buf , uint16_t len)
{
	UART_HandleTypeDef* huart = bsp_uart_priv[index].huart;

	if( huart == NULL )
	{
		return -1;
	}

	//异步接收 todo
	return -1;
}
