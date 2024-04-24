/*
This document will tell you how to use the interrupt 
triggered way to implement iic data read and write.
if you need poll please see iic.c
*/

#include "iic_slave_interrupt.h"

static uint8_t write_data_len = 0,write_data_cnt = 0,
                read_data_len = 0,read_data_cnt = 0;
static char* send_buf_ptr = NULL;
static char* read_buf_ptr = NULL;
//if you dont set revice buff, use normal buf.and recive not start
uint8_t read_data_buf[20];
uint8_t write_data_buf[20];

/*
this is a flag mean use read or write 
0 : read data only.
1 : write data only 
you can use read a write function to change it.
*/
static WR_STATE wr_flag = iic_read;


int peripheral_i2c_send_data(uint8_t len);
int peripheral_i2c_read_data(uint8_t len);


void setup_peripherals_i2c_module(void)
{
    /*set master*/
    I2C_Config(IIC_BASE,I2C_ROLE_SLAVE,I2C_ADDRESSING_MODE_07BIT,ADDRESS);
    I2C_ConfigClkFrequency(IIC_BASE,I2C_CLOCKFREQUENY_STANDARD);
    I2C_Enable(IIC_BASE,1);
    /*enable iic interrupts please check intterrupts enable every send data*/
    //only enable cmpl interrput, fifo full and emp enable in send or read func
    I2C_IntEnable(IIC_BASE,(1<<I2C_INT_CMPL) | (1<<I2C_INT_ADDR_HIT));
}


void iic_gpio_config(void)
{
    //enabele iic and gpio sysctrl ctrl
    //if you use anoter pin,please check gpio clk is true (like GPIO0 or GPIO1)
    SYSCTRL_ClearClkGateMulti((1 << SYSCTRL_ClkGate_APB_GPIO0)|
    (1 << SYSCTRL_ClkGate_APB_I2C0)|(1 << SYSCTRL_ITEM_APB_PinCtrl));
    PINCTRL_SelI2cIn(IIC_PORT,IIC_SCL_PIN,IIC_SDA_PIN);
}

/*iic interrupt callback functions*/
//you can change write and read len.
static uint32_t peripherals_i2c_isr(void *user_data)
{
    static uint8_t dir = 2;
    uint32_t status = I2C_GetIntState(APB_I2C0);

    if(status & (1 << I2C_STATUS_ADDRHIT))
    {
        dir = I2C_GetTransactionDir(APB_I2C0);
        if(dir == I2C_TRANSACTION_MASTER2SLAVE)
        {
            platform_printf("dir read\r\n");
            peripheral_i2c_read_data(READ_DATA_LEN);
        }
        else if(dir == I2C_TRANSACTION_SLAVE2MASTER)
        {
            platform_printf("dir write\r\n");
            peripheral_i2c_send_data(WRITE_DATA_LEN);
        }
        I2C_ClearIntState(APB_I2C0, (1 << I2C_STATUS_ADDRHIT));
    }
    
    //fifo full read data
    if(status & (1 << I2C_STATUS_FIFO_FULL))
    {
        platform_printf("fifo full\r\n");
        //master write
        if(wr_flag == iic_read)
            {
                for(; read_data_cnt < read_data_len; read_data_cnt++)
                {
                    if(I2C_FifoEmpty(APB_I2C0)){ break; }
                    platform_printf("read fifo len:%d cnt:%d\r\n",read_data_len,read_data_cnt);
                    read_buf_ptr[read_data_cnt] = I2C_DataRead(APB_I2C0);
                }
            }
    }
    //fifo empty send data
    if(status & (1 << I2C_STATUS_FIFO_EMPTY))
    {
        //master read
        if(wr_flag == iic_write)
        {
            platform_printf("fifo empty\r\n");
            // push data until fifo is full
            for(; write_data_cnt < write_data_len; write_data_cnt++)
            {
                platform_printf("write data\r\n");
                if(I2C_FifoFull(APB_I2C0)){ break; }
                I2C_DataWrite(APB_I2C0,send_buf_ptr[write_data_cnt]);
            }
        }
    }
    if(status & (1 << I2C_STATUS_CMPL))
    {
        platform_printf("cmpl int wr flag: %d\r\n",wr_flag);
        if(wr_flag == iic_read)
        {
            for(; read_data_cnt < read_data_len; read_data_cnt++)
            {
                platform_printf("cmpl read len:%d cnt: %d\r\n");
                read_buf_ptr[read_data_cnt] = I2C_DataRead(APB_I2C0);
            }
            read_data_cnt = 0;
        }
        else
        {
            platform_printf("cmpl write\r\n");
            write_data_cnt = 0;
        }
        I2C_ClearIntState(APB_I2C0, (1 << I2C_STATUS_CMPL));
        if(dir == I2C_TRANSACTION_MASTER2SLAVE)
        {
            platform_printf("close read\r\n");
            I2C_IntDisable(APB_I2C0,(1 << I2C_INT_FIFO_FULL));
        }
        else if(dir == I2C_TRANSACTION_SLAVE2MASTER)
        {
            platform_printf("close write\r\n");
            I2C_IntDisable(APB_I2C0,(1 << I2C_INT_FIFO_EMPTY));
        }
    
        
    }

    
    return 0;
}

//when you will send data please use this function
int peripheral_i2c_send_data(uint8_t len)
{
//    I2C_IntEnable(APB_I2C0,(1<<I2C_INT_CMPL)|(1 << I2C_INT_FIFO_EMPTY));

    wr_flag = iic_write;
    send_buf_ptr = (char*)write_data_buf;
    write_data_len = len;
    // only test use
    memset(send_buf_ptr,0x12,len);
    I2C_IntEnable(APB_I2C0,(1 << I2C_INT_FIFO_EMPTY) | (1<<I2C_INT_CMPL));
    return 1;
}
/*warning!!!! must make set buf ptr if you want recive data*/
int peripheral_i2c_read_data(uint8_t len)
{
    wr_flag = iic_read;
    read_buf_ptr = (char*)read_data_buf;
    read_data_len = len;
    I2C_IntEnable(APB_I2C0,(1 << I2C_INT_FIFO_FULL) | (1<<I2C_INT_CMPL));
    return 1;
}

void iic_init(void)
{
    iic_gpio_config();
    /*Register interrupt callback functions*/
    platform_set_irq_callback(PLATFORM_CB_IRQ_I2C0, peripherals_i2c_isr, NULL);
    setup_peripherals_i2c_module();
}



