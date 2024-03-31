#include <stdint.h>
#include <string.h>
#include "nano_coder.h"
#include "nano_coder_config.h"

typedef enum{
    DECODE_FIND_HEADER_1,
    DECODE_FIND_HEADER_2,
    DECODE_TAKE_PACK_ID,
    DECODE_TAKE_CMD_LOW_BYTE,
    DECODE_TAKE_CMD_HIGHT_BYTE,
    DECODE_TAKE_RESER_VALUE_1,
    DECODE_TAKE_RESER_VALUE_2,
    DECODE_TAKE_PAYLOAD_LEN_1,
    DECODE_TAKE_PAYLOAD_LEN_2,
    DECODE_TAKE_PAYLOAD_DATA,
    DECODE_TAKE_CRC_VALUE_BYTE_1,
    DECODE_TAKE_CRC_VALUE_BYTE_2,
    DECODE_OK,
}decode_status_t;

struct nano_decoder_t{
    decode_status_t status;
    nano_decode_cb_t cb;
    void*   cb_ctx;
    nano_pack_t pack;

    uint8_t payload_buf[NANO_DECODE_PAYLOAD_BUF_SIZE];
    uint16_t payload_rec_len;
    uint16_t rec_crc_value;     //接收到的crc校验值
    uint16_t cali_crc_value;    //字节计算的crc校验值
};

struct nano_encoder_t{
    uint8_t status;
};

/*******************************************Nano解码器*******************************************************/

/**
 * @brief 静态创建一个nano解码器
 * 
 * @param static_buf 静态内存
 * @param buf_len 静态内存大小
 * @return nano_decoder_handle_t nano解码器句柄
 */
nano_decoder_handle_t nano_decoder_static_create(uint8_t* static_buf,uint16_t buf_len)
{
    if( buf_len < sizeof(struct nano_decoder_t) )
    {
        return NULL;
    }

    nano_decoder_handle_t handle = (nano_decoder_handle_t)static_buf;
    nano_decoder_reset(handle);

    handle->cb = NULL;

    return handle;
}

/**
 * @brief 重置nano解码器
 * 
 * @param handle 解码器句柄
 * @return 0:成功 其他:失败
 * @note 注意!这个api不会解绑掉解码回调函数
 */
int32_t nano_decoder_reset(nano_decoder_handle_t handle)
{
    nano_decode_cb_t cb = handle->cb;
    void* cb_ctx = handle->cb_ctx;

    memset( handle , 0 , sizeof(struct nano_decoder_t) );

    handle->pack.payload_data = handle->payload_buf;
    handle->cb = cb;
    handle->cb_ctx = cb_ctx;

    return 0;
}


/**
 * @brief 为解码器设置一个解码过程回调
 * 
 * @param handle 解码器句柄
 * @param cb 回调函数
 * @param ctx 要一同传入到回调中的参数
 * @return int32_t 0:成功 其他:失败
 */
int32_t nano_decode_set_cb(nano_decoder_handle_t handle,nano_decode_cb_t cb,void* ctx)
{
    handle->cb = cb;
    handle->cb_ctx = ctx;

    return 0;
}

static void nano_decoder_error_handler(nano_decoder_handle_t handle,uint8_t err_byte)
{
    switch( handle->status )
    {
        case DECODE_FIND_HEADER_2:
            if( err_byte != PACK_HEADER_1 )
            {
                handle->status = DECODE_FIND_HEADER_1;
                handle->cali_crc_value = 0;
            }
            break;

        //此处可以通过pack中的信息还原接收，重新寻找帧头，但是暂时不做处理，直接舍弃前面接收到的数据
        case DECODE_TAKE_PAYLOAD_LEN_2:
            handle->status = DECODE_FIND_HEADER_1;
            handle->cali_crc_value = 0;
            break;

        default:
            handle->status = DECODE_FIND_HEADER_1;
            handle->cali_crc_value = 0;
            break;
    }

    if( handle->cb )
    {
        handle->cb( NANO_DECODE_ERR , &handle->pack , handle->cb_ctx );
    }
}

static void nano_decoder_input_one_byte(nano_decoder_handle_t handle,uint8_t byte)
{

    switch( handle->status )
    {
        case DECODE_FIND_HEADER_1:
            handle->cali_crc_value = byte;
            if( byte == PACK_HEADER_1 )
            {
                handle->status = DECODE_FIND_HEADER_2;
            }
            break;
        
        case DECODE_FIND_HEADER_2:
            handle->cali_crc_value += byte;
            if( byte == PACK_HEADER_2 )
            {
                handle->status = DECODE_TAKE_PACK_ID;
            }
            else
            {
                nano_decoder_error_handler(handle,byte);
            }
            break;

        case DECODE_TAKE_PACK_ID:
            handle->cali_crc_value += byte;
            handle->pack.pack_id = byte;
            handle->status = DECODE_TAKE_CMD_LOW_BYTE;
            break;

        case DECODE_TAKE_CMD_LOW_BYTE:
            handle->cali_crc_value += byte;
            handle->pack.cmd = byte;
            handle->status = DECODE_TAKE_CMD_HIGHT_BYTE;
            break;

        case DECODE_TAKE_CMD_HIGHT_BYTE:
            handle->cali_crc_value += byte;
            handle->pack.cmd |= ( (byte & (0xff>>(8-6)) ) << 8);
            handle->pack.need_ack = ( byte & (0x01<<6) ) ?  1 : 0;
            handle->pack.is_ack = ( byte & (0x01<<7) ) ? 1 : 0;
            handle->status = DECODE_TAKE_RESER_VALUE_1;
            break;

        case DECODE_TAKE_RESER_VALUE_1:
            handle->cali_crc_value += byte;
            handle->pack.reserved = byte;
            handle->status = DECODE_TAKE_RESER_VALUE_2;
            break;

        case DECODE_TAKE_RESER_VALUE_2:
            handle->pack.reserved |= byte<<8;
            handle->cali_crc_value += byte;
            handle->status = DECODE_TAKE_PAYLOAD_LEN_1;
            break;

        case DECODE_TAKE_PAYLOAD_LEN_1:
            handle->cali_crc_value += byte;
            handle->pack.payload_len = byte;
            handle->status = DECODE_TAKE_PAYLOAD_LEN_2;
            break;
        
        case DECODE_TAKE_PAYLOAD_LEN_2:
            handle->cali_crc_value += byte;
            if( (handle->pack.payload_len & 0x0f) != byte >> 4  )
            {
                nano_decoder_error_handler(handle,byte);
            }
            else
            {
                handle->pack.payload_len |= (byte & 0x0f) << 4 ;
                handle->status = DECODE_TAKE_PAYLOAD_DATA;
            }
            break;

        case DECODE_TAKE_PAYLOAD_DATA:
            handle->cali_crc_value += byte;
            handle->payload_buf[ handle->payload_rec_len ] = byte;
            handle->payload_rec_len++;
            if( handle->payload_rec_len != handle->pack.payload_len )
            {
                handle->payload_rec_len++;
            }
            else
            {
                handle->status = DECODE_TAKE_CRC_VALUE_BYTE_1;
            }
            break;

        case DECODE_TAKE_CRC_VALUE_BYTE_1:
            handle->rec_crc_value = byte;
            handle->status = DECODE_TAKE_CRC_VALUE_BYTE_2;
            break;

        case DECODE_TAKE_CRC_VALUE_BYTE_2:
            handle->rec_crc_value |= (byte<<8);
            if( handle->rec_crc_value != ( (~handle->cali_crc_value) ^ PACK_CRC_TEMP_VALUE ) )
            {
                nano_decoder_error_handler(handle,byte);
            }
            else
            {
                handle->status = NANO_DECODE_OK;
            }
            break;

        default:
            nano_decoder_error_handler(handle,byte);
            break;
    }
}

/**
 * @brief 输入字节到编码器中
 * 
 * @param handle 解码器句柄
 * @param bytes 字节
 * @param bytes_len 字节长度
 * @param curr_time 当前时间(用于等待数据超时处理，暂时未使用)
 * @note 无论解码是否成功都会调用解码过程回调，可以通过判断回调中的stage确认解码是否完成
 */
void nano_decoder_input(nano_decoder_handle_t handle,uint8_t* bytes,uint16_t bytes_len,uint32_t curr_time)
{
    (void)curr_time;
    uint8_t* decode_byte = bytes;
    uint8_t* bytes_end = bytes + bytes_len;

    do{
        nano_decoder_input_one_byte( handle , *decode_byte );
        decode_byte++;
    }while( (handle->status != DECODE_OK) && decode_byte != bytes_end );

    if( handle->cb )
    {
        handle->cb( handle->status == DECODE_OK ? NANO_DECODE_OK : NANO_WAIT_BYTE , &handle->pack , handle->cb_ctx );
    }

    if( handle->status == DECODE_OK )
    {
        // nano_decoder_reset(handle)   //耗时过长
        handle->status = DECODE_FIND_HEADER_1;
    }


}

/*******************************************Nano编码器*******************************************************/

/**
 * @brief 静态创建一个nano编码器
 * 
 * @param static_buf 静态内存
 * @param buf_len 静态内存大小
 * @return nano_encoder_handle_t nano编码器句柄
 */
nano_encoder_handle_t nano_encoder_static_create(uint8_t* static_buf,uint16_t buf_len)
{
    if( buf_len < sizeof(struct nano_encoder_t) )
    {
        return NULL;
    }

    nano_encoder_handle_t handle = (nano_encoder_handle_t)static_buf;

    return handle;
}

/**
 * @brief 对pack进行编码
 * 
 * @param handle 编码器
 * @param pack 要编码的包
 * @param buf 编码后存入的内存
 * @param buf_len 编码内存大小
 * @return 编码后所占用的空间
 */
uint32_t nano_encoder_output(nano_encoder_handle_t handle,nano_pack_t* pack,uint8_t* buf , uint16_t buf_len)
{
    uint32_t all_len = pack->payload_len + 11;
    if( buf_len < all_len )
    {
        return 0;
    }

    uint8_t* byte = buf;
    uint16_t crc_value;

    *byte = PACK_HEADER_1;
    crc_value = PACK_HEADER_1;
    byte++;

    *byte = PACK_HEADER_2;
    crc_value += *byte;
    byte++;

    *byte = pack->pack_id;
    crc_value += *byte;
    byte++;

    *byte = pack->cmd & 0xff;
    crc_value += *byte;
    byte++;

    *byte = pack->cmd>>8 & (0x3f);
    *byte |= pack->need_ack ? (0x01 << 6) : 0;
    *byte |= pack->is_ack ? (0x01 << 7) : 0;
    crc_value += *byte;
    byte++;

    *byte = 0;
    crc_value += *byte;
    byte++;

    *byte = 0;
    crc_value += *byte;
    byte++;

    *byte = pack->payload_len & 0xff;
    crc_value += *byte;
    byte++;

    *byte = (pack->payload_len >> 8) & 0x0f;
    *byte |= (pack->payload_len << 4) & 0xf0;
    crc_value += *byte;
    byte++;

    for(uint16_t temp = 0 ; temp < pack->payload_len ; temp++ )
    {
        *byte = pack->payload_data[temp];
        crc_value += *byte;
        byte++;
    }

    crc_value = (~crc_value) ^ PACK_CRC_TEMP_VALUE;

    *byte = crc_value & 0xff;
    byte++;

    *byte = crc_value >> 8;
    byte++;

    return all_len;
}
