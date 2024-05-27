#include "iic_master_920.h"
/*
This document will tell you how to use the interrupt
triggered way to implement iic data read and write.
if you need poll please see iic.c
*/

//#define IIC_MASTER_DEBUG
#ifdef IIC_MASTER_DEBUG
#define DEBUG_LOG(...)      platform_printf(__VA_ARGS__)
#else
#define DEBUG_LOG(...)
#endif

uint8_t write_data_len = 0,write_data_cnt = 0,
        read_data_len = 0,read_data_cnt = 0;
static char* send_buf_ptr = NULL;
static char* read_buf_ptr = NULL;
//if you dont set revice buff, use normal buf.and recive not start
uint8_t read_data_buf[20];
/*
this is a flag mean use read or write
0 : read data only.
1 : write data only
you can use read a write function to change it.
*/
static WR_STATE wr_flag = iic_write;

static void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = I2C0_IRQ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = 1;
    /* init NVIC */
    NVIC_Init(&NVIC_InitStructure);
}

void setup_peripherals_i2c_module(void)
{
    /*set master*/
    I2C_Config(IIC_BASE,I2C_ROLE_MASTER,I2C_ADDRESSING_MODE_07BIT,ADDRESS);
    I2C_ConfigClkFrequency(IIC_BASE,I2C_CLOCKFREQUENY_FASTMODE_PLUS);
    I2C_Enable(IIC_BASE,1);
    /*enable iic interrupts please check intterrupts enable every send data*/
    //only enable cmpl interrput, fifo full and emp enable in send or read func
    I2C_IntEnable(IIC_BASE,(1<<I2C_INT_CMPL));
    I2C_IntDisable(APB_I2C0,(1 << I2C_INT_FIFO_EMPTY) | (1 << I2C_INT_FIFO_FULL));
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
static uint32_t peripherals_i2c_isr(void *user_data)
{
    static uint8_t dir = 2;
    uint32_t status = I2C_GetIntState(APB_I2C0);

    //fifo full read data
    if(status & (1 << I2C_STATUS_FIFO_FULL))
    {
        DEBUG_LOG("iic fifo full\r\n");
        if(wr_flag == iic_read)
        {
            for(; read_data_cnt < read_data_len; read_data_cnt++)
            {
                if(I2C_FifoEmpty(APB_I2C0)){ break; }
                read_buf_ptr[read_data_cnt] = I2C_DataRead(APB_I2C0);
                DEBUG_LOG("iic read data buf : %x\r\n",read_buf_ptr[read_data_cnt]);
            }
        }
    }
    //fifo empty send data
    if(status & (1 << I2C_STATUS_FIFO_EMPTY))
    {
        DEBUG_LOG("fifo empty\r\n");
        if(wr_flag == iic_write)
        {
            // push data until fifo is full
            for(; write_data_cnt < write_data_len; write_data_cnt++)
            {
                if(I2C_FifoFull(APB_I2C0)){ break; }
                I2C_DataWrite(APB_I2C0,send_buf_ptr[write_data_cnt]);
                DEBUG_LOG("iic write data\r\n");
            }
            // if its the last, disable empty int
            if(write_data_cnt == write_data_len)
            {
                DEBUG_LOG("iic disable empty\r\n");
                I2C_IntDisable(APB_I2C0,(1 << I2C_INT_FIFO_EMPTY));
//                I2C_CommandWrite(APB_I2C0, I2C_COMMAND_CLEAR_FIFO);
                //if last clear cnt.
                write_data_cnt = 0;
            }
        }
    }
    if(status & (1 << I2C_STATUS_CMPL))
    {
        DEBUG_LOG("iic cmpl\r\n");
        if(wr_flag == iic_read)
        {
            for(; read_data_cnt < read_data_len; read_data_cnt++)
            {
                read_buf_ptr[read_data_cnt] = I2C_DataRead(APB_I2C0);
                DEBUG_LOG("iic read data buf : %x\r\n",read_buf_ptr[read_data_cnt]);
            }
            read_data_cnt = 0;
        }
        else
        {
            write_data_cnt = 0;
            I2C_CommandWrite(APB_I2C0, I2C_COMMAND_CLEAR_FIFO);
        }
        I2C_ClearIntState(APB_I2C0, (1 << I2C_STATUS_CMPL));
    }
    if(status & (1 << I2C_STATUS_ADDRHIT))
    {
        DEBUG_LOG("iic addr hit\r\n");
        I2C_ClearIntState(APB_I2C0, (1 << I2C_STATUS_ADDRHIT));
    }
    return 0;
}

void I2C0_IRQHandler(void)
{
    peripherals_i2c_isr(NULL);
}

//when you will send data please use this function
int peripheral_i2c_send_data(const void* buf,uint8_t data_len)
{
    if(buf == NULL)
        return 0;
    DEBUG_LOG("start send data len: %d\r\n",data_len);
    wr_flag = iic_write;
    send_buf_ptr = (char*)buf;
    write_data_len = data_len;
    I2C_IntDisable(APB_I2C0,(1 << I2C_INT_FIFO_FULL));
    I2C_IntEnable(APB_I2C0,(1 << I2C_INT_FIFO_EMPTY) | (1<<I2C_INT_CMPL));
    I2C_CtrlUpdateDirection(APB_I2C0,I2C_TRANSACTION_MASTER2SLAVE);
    I2C_CtrlUpdateDataCnt(APB_I2C0, data_len);
    I2C_CommandWrite(APB_I2C0, I2C_COMMAND_ISSUE_DATA_TRANSACTION);

    return 1;
}
/*warning!!!! must make set buf ptr if you want recive data*/
int peripheral_i2c_read_data(const void* buf,uint8_t data_len)
{
    if(buf == NULL)
    {
        read_buf_ptr = (char*)read_data_buf;
        read_data_len = 20;
        return 0;
    }
    wr_flag = iic_read;
    DEBUG_LOG("start read data len: %d\r\n",data_len);
    read_buf_ptr = (char*)buf;
    read_data_len = data_len;
    I2C_IntDisable(APB_I2C0,(1 << I2C_INT_FIFO_EMPTY));
    I2C_IntEnable(APB_I2C0,(1 << I2C_INT_FIFO_FULL) | (1<<I2C_INT_CMPL) | (1<<I2C_INT_ADDR_HIT));
    I2C_CtrlUpdateDirection(APB_I2C0,I2C_TRANSACTION_SLAVE2MASTER);
    I2C_CtrlUpdateDataCnt(APB_I2C0, data_len);
    I2C_CommandWrite(APB_I2C0, I2C_COMMAND_ISSUE_DATA_TRANSACTION);

    return 1;
}

void iic_init(void)
{
    iic_gpio_config();
    /*Register interrupt callback functions*/
    NVIC_Configuration();
//    platform_set_irq_callback(PLATFORM_CB_IRQ_I2C0, peripherals_i2c_isr, NULL);
    setup_peripherals_i2c_module();
    //iic init only in read_only mode.
}

char iic_buf[20] = {0};
void iic_test(void)
{
    iic_init();
    while (1)
    {
        peripheral_i2c_send_data(iic_buf,20);
        SYSCTRL_DelayCycles(24000,1000);
    }
}



