#2021-11-21
第一个问题就是：DMA流控，不会。。。




#2021-06-19 23:24
移植freertos源码，删除了原有的硬件、外围、算法文件，目前测试尚可。

Pin Config
==========




|Function|Pin|
|---|----|
|U2_RX|PA2|
|U2_TX|PA3|
|SPI1_NSS|PA4|
|SPI1_SCK(SCK)|PA5|
|SPI1_MOSI|PA6|
|SPI1_MISO/pwm14_ch1|PA7|
|U1_RX|PA9|
|U1_TX|PA10|
|CAN1_RX|PA11|
|CAN1_TX|PA12|
|SWDIO|PA13|
|SWCLK|PA14|
|NONE|PA15|
|||
|||
|||
|RFID_IRQ|PB1|
|BOOT1|PB2|
|NONE|PB3|
|NONE|PB4|
|CAN2_RX|PB5|
|CAN2_TX|PB6|
|RFID_Reset/pwm4_ch2|PB7|
|I2C2_SCL|PB10|
|I2C2_SDA|PB11|
|||
|||
|||
|LED0|PC0|
|LED1|PC1|
|LED2|PC2|
|LED3|PC3|
|KEY0|PC4|
|KEY1|PC5|
|U6_TX|PC6|
|U6_RX|PC7|
|||
|||
|||
|BEEP|PD0|
|U3_TX(232)|PD8|
|U3_RX(232)|PD9|
|||
|||
|||
|||
|||
|||
