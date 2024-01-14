#include "bsp_uart.h"
#include "usart.h"

typedef struct
{
	UART_HandleTypeDef	*huart;
	uint8_t*			rx_buf;
	uint16_t			rx_buf_len;
}bsp_uart_priv_t;

static bsp_uart_priv_t bsp_uart_priv[1];

int8_t bsp_uart_init(bsp_uart_t index)
{	
	UART_HandleTypeDef* huart = bsp_uart_priv[index].huart;
	if( huart == NULL )
	{
		return 0;
	}

	switch(index)
	{
		case DEBUG_UART:
			MX_USART1_UART_Init();
			bsp_uart_priv[DEBUG_UART].huart = &huart1;
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

int16_t bsp_uart_write(bsp_uart_t index , uint8_t* buf , uint16_t len)
{
	UART_HandleTypeDef* huart = bsp_uart_priv[index].huart;

	if( huart == NULL )
	{
		return -1;
	}

	HAL_StatusTypeDef ret = HAL_UART_Transmit_IT( huart , buf , len );
	while( HAL_BUSY ==  ret )
	{
		ret = HAL_UART_Transmit_IT( huart , buf , len );
	}

    return ret == HAL_OK ? len : -1;
}

int16_t bsp_uart_read(bsp_uart_t index , uint8_t* buf , uint16_t len)
{
	UART_HandleTypeDef* huart = bsp_uart_priv[index].huart;

	if( huart == NULL )
	{
		return -1;
	}

	//异步接收 todo
	return -1;
}
