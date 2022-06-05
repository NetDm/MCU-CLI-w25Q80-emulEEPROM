# CLIw25Q80

Проект управления serial SPI FLASH w25Q80 эмуляция EEPROM по средствам консоли redLine CLI

Поддерживаются следующие операции: стирание всей микросхемы, стирание и перезапись произвольных байтов

Консоль redline CLI для MCU без RTOS с памятью комманд, редактором, удобным общим файлом списка комманд с мануалами (https://github.com/NetDm/CLIw25Q80/blob/master/UserCodes/CLIcommands.c)

https://github.com/NetDm/CLIw25Q80/blob/master/remarks/putty-to-github-CLI-SPI-flash.png?raw=true
![изображение](https://user-images.githubusercontent.com/36101745/172067353-de8d5784-76c7-4e05-962f-c7a9ae3c2555.png)

![изображение](https://user-images.githubusercontent.com/36101745/172067361-4b6e30f8-470a-4cec-add0-607cc083279c.png)

Написана блокирующая низкоуровневая библиотека в папке UserCodes/**NvmSpi25Qxx**/

сопутствующий файл настроек SPI flas 'UserCodes/**w25q80def.h**'

Неблокирующий автомат процесса записи потока из UART верхнего уровня **UserCodes/ApL/NVM25Q80.c** по средствам низкоуровневой библиотеки **NvmSpi25Qxx**

в случае подмены потока данных, функцией NVM25Q80.c::bool_t **pfStreamForErase(uint8_t* retByte)** производиться истирание произвольных ячеек
