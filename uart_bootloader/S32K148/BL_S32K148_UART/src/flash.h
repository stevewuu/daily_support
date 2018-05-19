/*
 * flash.h
 *
 *  Created on: 2017��7��20��
 *      Author: 4337
 */

#ifndef FLASH_H_
#define FLASH_H_

#include "device_registers.h"



void flash_init();
void flash_erase_sector(uint32_t addr);
void flash_program_data(uint32_t addr, uint8_t *pdata, uint32_t byte_size);
int _write_image(uint8_t * data, uint16_t data_size, uint32_t addr);
#endif /* FLASH_H_ */