#include "bsp_qspi_flash.h"
#include "bsp_qspi_flash_cmd_def.h"
#include "quadspi.h"

typedef enum
{	
	W25QXX_QSPIMode,
	W25QXX_SPIMode,
} w25qxx_StatusTypeDef;

static w25qxx_StatusTypeDef w25qxx_mode = W25QXX_SPIMode;

//重置w25q64
static uint32_t bsp_qspi_reset_device(QSPI_HandleTypeDef *hqspi)
{
    QSPI_CommandTypeDef s_command;

    /* Initialize the reset enable command */
    s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
    s_command.Instruction       = W25X_EnableReset;
    s_command.AddressMode       = QSPI_ADDRESS_NONE;
    s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    s_command.DataMode          = QSPI_DATA_NONE;
    s_command.DummyCycles       = 0;
    s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
    s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
    s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

    //使用单线/四线模式分别发送 W25X_EnableReset 和 W25X_ResetDevice 指令

    /* Send the command */
    if (HAL_QSPI_Command(hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
    return -1;
    }

    /* Send the reset device command */
    s_command.Instruction = W25X_ResetDevice;
    if (HAL_QSPI_Command(hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
    return -1;
    }

    s_command.InstructionMode   = QSPI_INSTRUCTION_4_LINES;
    s_command.Instruction       = W25X_EnableReset;
    /* Send the command */
    if (HAL_QSPI_Command(hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
    return -1;
    }

    /* Send the reset memory command */
    s_command.Instruction = W25X_ResetDevice;
    if (HAL_QSPI_Command(hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
    return -1;
    }

    return 0;
}

/**
 * @brief	QSPI发送命令
 *
 * @param   instruction		要发送的指令
 * @param   address			发送到的目的地址
 * @param   addressSize	发送到的目的地址大小
 * @param   dummyCycles		空指令周期数
 * @param   instructionMode		指令模式;
 * @param   addressMode		地址模式; QSPI_ADDRESS_NONE,QSPI_ADDRESS_1_LINE,QSPI_ADDRESS_2_LINES,QSPI_ADDRESS_4_LINES
 * @param   dataMode		数据模式; QSPI_DATA_NONE,QSPI_DATA_1_LINE,QSPI_DATA_2_LINES,QSPI_DATA_4_LINES
 * @param   dataSize        待传输的数据长度
 *
 * @return  uint8_t			w25qxx_OK:正常
 *                      w25qxx_ERROR:错误
 */
static uint8_t QSPI_Send_CMD(QSPI_HandleTypeDef *hqspi,uint32_t instruction, uint32_t address,uint32_t addressSize,uint32_t dummyCycles, 
                    uint32_t instructionMode,uint32_t addressMode, uint32_t dataMode, uint32_t dataSize)
{
    QSPI_CommandTypeDef Cmdhandler;

    Cmdhandler.Instruction        = instruction;   
    Cmdhandler.InstructionMode    = instructionMode;  

    Cmdhandler.Address            = address;
    Cmdhandler.AddressSize        = addressSize;
    Cmdhandler.AddressMode        = addressMode;
        
    Cmdhandler.AlternateBytes     = 0x00;
    Cmdhandler.AlternateBytesSize = QSPI_ALTERNATE_BYTES_8_BITS;
    Cmdhandler.AlternateByteMode  = QSPI_ALTERNATE_BYTES_NONE;                              
    Cmdhandler.DummyCycles        = dummyCycles;                   
                                            
    Cmdhandler.DataMode           = dataMode;
    Cmdhandler.NbData             = dataSize; 

    Cmdhandler.DdrMode            = QSPI_DDR_MODE_DISABLE;           	
    Cmdhandler.DdrHoldHalfCycle   = QSPI_DDR_HHC_ANALOG_DELAY;
    Cmdhandler.SIOOMode           = QSPI_SIOO_INST_EVERY_CMD;

    if(HAL_QSPI_Command(hqspi, &Cmdhandler, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
        return -1;

    return 0;
}

void bsp_qspi_flash_init(void)
{
    HAL_Delay(5);
    MX_QUADSPI_Init();
    bsp_qspi_reset_device(&hqspi);
    HAL_Delay(0); //空等几纳秒

}

void bsp_qspi_flash_deinit(void)
{

}

uint16_t bsp_qspi_flash_get_id(void)
{
    uint8_t ID[6];
    uint16_t deviceID;

    if( w25qxx_mode == W25QXX_SPIMode)
        QSPI_Send_CMD(&hqspi,W25X_QUAD_ManufactDeviceID,0x00,QSPI_ADDRESS_24_BITS,6,QSPI_INSTRUCTION_1_LINE,QSPI_ADDRESS_4_LINES, QSPI_DATA_4_LINES, sizeof(ID));
    else
        QSPI_Send_CMD(&hqspi,W25X_ManufactDeviceID,0x00,QSPI_ADDRESS_24_BITS,0,QSPI_INSTRUCTION_4_LINES,QSPI_ADDRESS_4_LINES, QSPI_DATA_4_LINES, sizeof(ID));

    /* Reception of the data */
    if (HAL_QSPI_Receive(&hqspi, ID, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return -1;
    }
    deviceID = (ID[0] << 8) | ID[1];

    return deviceID;
}

int32_t bsp_qspi_flash_program(uint32_t addr,uint8_t* data,uint16_t len)
{
    return -1;
}

int32_t bsp_qspi_flash_erase(uint32_t addr , uint16_t len)
{
    return -1;
}


