/**
 * \file
 *
 * \brief API driver for component AT24MACXX.
 *
 * Copyright (c) 2015 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#ifndef AT24MACXX_H_INCLUDED
#define AT24MACXX_H_INCLUDED

#include "twihs_master.h"

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
#endif
/**INDENT-ON**/
/// @endcond

//! Checking board configuration of the AT24MAC EEPROM
#if !defined(BOARD_AT24MAC_TWIHS_INSTANCE)
# warning The AT24MAC TWIHS instance has not been defined. Using default settings.
# define BOARD_AT24MAC_TWIHS_INSTANCE      TWIHS0 /* TWIHS instance (TWIHS0) */
#endif
#if !defined(BOARD_AT24MAC_ADDRESS)
# warning The AT24MAC TWI address has not been defined. Using default settings.
# define BOARD_AT24MAC_ADDRESS            (0x50u) /*(0xAE >> 1)*/ /* TWIHS eeprom address */
#endif
#if !defined(BOARD_CLK_TWIHS_EEPROM)
# warning The AT24MAC TWIHS CLK pin index has not been defined. Using default.
# define BOARD_CLK_TWIHS_EEPROM            0 /* TWIHS eeprom clock pin index */
#endif
#if !defined(BOARD_CLK_TWIHS_MUX_EEPROM)
# warning The AT24MAC TWIHS CLK peripheral has not been defined. Using default.
# define BOARD_CLK_TWIHS_MUX_EEPROM        0 /* TWIHS eeprom clock pin peripheral */
#endif

/* AT24MACXX internal address length */
#define AT24MAC_MEM_ADDR_LEN               2

/* Return value for accessing AT24MACXX */
#define AT24MAC_WRITE_SUCCESS              0
#define AT24MAC_WRITE_FAIL                 1
#define AT24MAC_READ_SUCCESS               0
#define AT24MAC_READ_FAIL                  1

void at24macxx_reset(void);
uint32_t at24macxx_write_byte(uint32_t u32_address, uint8_t uc_value);
uint32_t at24macxx_write_continuous(uint32_t u32_start_address,
		uint16_t u16_length, uint8_t const *p_wr_buffer);
uint32_t at24macxx_read_byte(uint32_t u32_address, uint8_t *p_rd_byte);
uint32_t at24macxx_read_continuous(uint32_t u32_start_address, uint16_t u16_length,
		uint8_t *p_rd_buffer);
uint32_t at24macxx_write_page(uint32_t u32_page_address,
		uint32_t u32_page_size, uint8_t const *p_wr_buffer);
uint32_t at24macxx_read_page(uint32_t u32_page_address,
		uint32_t u32_page_size, uint8_t *p_rd_buffer);
uint32_t at24macxx_fill_pattern(uint32_t u32_start_address,
		uint32_t u32_end_address, uint8_t u8_pattern);

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/// @endcond

#endif /* AT24MACXX_H_INCLUDED */
