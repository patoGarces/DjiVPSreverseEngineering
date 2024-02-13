#include <stdio.h>
#include "include/DJI_VPS.h"
#include "driver/uart.h"
#include "string.h"
#include "esp_log.h"
#include "freertos/queue.h"

#define TAG "DJI_VPS"

const int rxPin = 16;
const int txPin = 17;
const int uartPort= UART_NUM_2;

uint16_t makeUint16(uint8_t low,uint8_t high){
    return (uint16_t)(high * 0xff) + low;
}

void receiveTask(void *pvParameters ){

    uint8_t data[98],length = 0;
    channels_control_t newControl;
    int16_t channels[17];

    while(1){      
        uart_get_buffered_data_len(uartPort, (size_t*)&length);          // obtengo datos para leer
        
        if(length > 0){    
            length = uart_read_bytes(uartPort,data,length,0);

            // esp_log_buffer_hex(TAG, &data, length);

            // byte 0-4:    header:     550B00F803
            // byte 5-6:    distance in cm
            // byte 7:      unknown
            // byte 8:      counter1
            // byte 9,10:   unknown
            // byte 11,12:  static:     0x5557
            // byte 13-15:  static:     0x00C202
            // byte 16-17:  counter2
            // byte 18-22:  ceros
            // byte 23:     camera?     0x00 or 0xff    -> with covered camera-> value=0x00, unknown
            // byte 24-31   static      ceros when disconnect from ph3, else 0x574D3333305F4D56 with my ph3 connected
            // byte 32-33   unknown     2 bytes are very noisy
            // byte 34-35   camera?     2 bytes are stable and looks like uin16,when i move my hand look like affected, but i dont know if its becouse camera or ultrasonic sensor
            // byte 36-37   static:     change value when i reboot the vps
            // byte 38-39   camera      change values when i move object in front of camera,looks like a pairs of 2 bytes (uint16), but its a little noissy, when dont move the vps, some of bytes are zeros, but other have a static value
            // byte 40-41   ceros
            // byte 42-51   camera      idem bytes 38-39
            // byte 52-59   unknown     noissy values,even with covered camera
            // byte 60-67   camera      idem bytes 38-51
            // byte 68-85   static:     all zeros
            // byte 86-87   camera:     noissy value, zero whit covered camera
            // byte 88-89   static:     all zeros
            // byte 90-93   camera:     static with covered camera, but not zero
            // byte 94-97   unknown:    noissy bytes,even with covered camera   
       

            if(data[0] == 0x55 && data[1] == 0x0B && data[2] == 0x00 && data[3] == 0xF8 && data[4] == 0x03){
                if(length == 98){
                    uint8_t index=90;
                    
                    uint8_t counter1 = data[8];
                    uint8_t counter2 = makeUint16(data[16],data[17]);
                    uint16_t distance = makeUint16(data[5],data[6]);
                    int16_t camera1 = makeUint16(data[34],data[35]);
                    int16_t camera2 = makeUint16(data[38],data[39]);
                    int16_t camera3 = makeUint16(data[42],data[43]);
                    int16_t camera4 = makeUint16(data[44],data[45]);
                    int16_t camera5 = makeUint16(data[46],data[47]);
                    int16_t camera6 = makeUint16(data[48],data[49]);
                    int16_t camera7 = makeUint16(data[50],data[51]);
                    
                    printf("%d,%d,%d,%d\n",camera1,camera2,camera3,camera4);
}
                // else if(length == 11){
                //     uint8_t calcChecksum = data[0] ^ data[1] ^ data[2] ^ data[3] ^ data[4] ^ data[5] ^ data[6] ^ data[7] ^ data[8] ^ data[9];
                //     printf("short packet: %d, %d,supuesto check: %d. calculate check: %d\n",data[8],data[9],data[10],calcChecksum);
                // }
            }
            else{
                // ESP_LOGE(TAG,"RESYNC");
                uart_flush(uartPort);
            }
        }
        vTaskDelay(pdMS_TO_TICKS(14));
    }
}

void djiVpsInit(void){

    /*configuro uart*/
    uart_config_t uart_config={
        .baud_rate = 115200,//100000,
        .data_bits = UART_DATA_8_BITS,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .parity = UART_PARITY_EVEN,
        .rx_flow_ctrl_thresh = 112,
        .stop_bits = UART_STOP_BITS_2,
        .source_clk = UART_SCLK_DEFAULT,
    };
    uart_param_config(uartPort,&uart_config);

    /* configuro pines a utilizar por el uart*/
    uart_set_pin(uartPort,txPin,rxPin,-1,-1);

    // uart_set_line_inverse(uartPort,UART_SIGNAL_RXD_INV);

    const int uart_buffer_size = (1024 * 2);
    QueueHandle_t uart_queue;
    // Install UART driver using an event queue here
    uart_driver_install(uartPort, uart_buffer_size, uart_buffer_size, 10, &uart_queue, 0);

    xTaskCreate(&receiveTask,"read dji vps", 4096, NULL, 3 , NULL);
}
