
#include "AT24Cxx_dynamic.h"
#include <dynamic_twi.h>
#include "ioport.h"
#include "delay.h"
#include <asf.h>

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
#endif
/**INDENT-ON**/
/// @endcond

#ifndef AT24C_MEM_ADDR_LEN
#  error Please define AT24C_MEM_ADDR_LEN according to the using AT24C
#else
#  if AT24C_MEM_ADDR_LEN>3
#    error AT24C_MEM_ADDR_LEN over 3 not supported
#  elif AT24C_MEM_ADDR_LEN>2
#    define at24c_build_word_address(p_u8, addr) \
	do {\
		p_u8[0] = (uint8_t)(addr >> 16);\
		p_u8[1] = (uint8_t)(addr >>  8);\
		p_u8[2] = (uint8_t)(addr >>> 0);\
	} while (0)
#  elif AT24C_MEM_ADDR_LEN>1
#    define at24c_build_word_address(p_u8, addr) \
	do {\
		p_u8[0] = (uint8_t)(addr >> 8);\
		p_u8[1] = (uint8_t)(addr >> 0);\
	} while (0)
#  else /* 1 */
#    define at24c_build_word_address(p_u8, addr) \
	do {\
		p_u8[0] = (uint8_t)(addr >> 0);\
	} while (0)
#  endif
#endif /* AT24C_MEM_ADDR_LEN */

#ifndef AT24MAC_MEM_ADDR_LEN
#  error Please define AT24MAC_MEM_ADDR_LEN according to the using AT24MAC
#else
#  if AT24MAC_MEM_ADDR_LEN>3
#    error AT24MAC_MEM_ADDR_LEN over 3 not supported
#  elif AT24MAC_MEM_ADDR_LEN>2
#    define at24mac_build_word_address(p_u8, addr) \
do {\
	p_u8[0] = (uint8_t)(addr >> 16);\
	p_u8[1] = (uint8_t)(addr >>  8);\
	p_u8[2] = (uint8_t)(addr >>> 0);\
} while (0)
#  elif AT24MAC_MEM_ADDR_LEN>1
#    define at24mac_build_word_address(p_u8, addr) \
do {\
	p_u8[0] = (uint8_t)(addr >> 8);\
	p_u8[1] = (uint8_t)(addr >> 0);\
} while (0)
#  else /* 1 */
#    define at24mac_build_word_address(p_u8, addr) \
do {\
	p_u8[0] = (uint8_t)(addr >> 0);\
} while (0)
#  endif
#endif /* AT24MAC_MEM_ADDR_LEN */

/**
 * \brief Poll the acknowledge from AT24CXX.
 *
 * \param twi_package Pointer to TWI data package. Only the slave address is
 * used in the acknowledge polling.
 */
/*
static void at24cxx_acknowledge_polling(twi_package_t *twi_package)
{
	uint8_t data = 0;
	
	//for (dummy_counter=0;dummy_counter<13500;dummy_counter++);
	//delay_us(2020);
	// Store the package parameters 
	uint8_t addr = twi_package->addr[0];
	uint32_t addr_length = twi_package->addr_length;
	void *buffer = twi_package->buffer;
	uint32_t length = twi_package->length;

	// Configure the data packet to be transmitted
	twi_package->addr[0] = 0;
	twi_package->addr_length = 0;
	twi_package->buffer = &data;
	twi_package->length = 1;
	

	while (twi_master_write(BOARD_AT24C_TWI_INSTANCE, twi_package) !=
		TWI_SUCCESS);
	// Restore the package parameters
	twi_package->addr[0] = addr;
	twi_package->addr_length = addr_length;
	twi_package->buffer = buffer;
	twi_package->length = length;
}*/

/**
 * \brief Poll the acknowledge from AT24MACXX.
 *
 * \param twihs_package Pointer to TWI data package. Only the slave address is
 * used in the acknowledge polling.
 */
static void at24macxx_acknowledge_polling(twihs_package_t *twihs_package)
{
	uint8_t data = 0;

	/* Store the package parameters */
	uint8_t addr = twihs_package->addr[0];
	uint32_t addr_length = twihs_package->addr_length;
	void *buffer = twihs_package->buffer;
	uint32_t length = twihs_package->length;

	/* Configure the data packet to be transmitted */
	twihs_package->addr[0] = 0;
	twihs_package->addr_length = 0;
	twihs_package->buffer = &data;
	twihs_package->length = 1;

	while (twihs_master_write(BOARD_AT24MAC_TWIHS_INSTANCE, twihs_package) !=
		TWIHS_SUCCESS);

	/* Restore the package parameters */
	twihs_package->addr[0] = addr;
	twihs_package->addr_length = addr_length;
	twihs_package->buffer = buffer;
	twihs_package->length = length;
}

static uint32_t at24macxx_acknowledge_check(twihs_package_t *twihs_package)
{
	uint8_t data = 0, retval;

	/* Store the package parameters */
	uint8_t addr = twihs_package->addr[0];
	uint32_t addr_length = twihs_package->addr_length;
	void *buffer = twihs_package->buffer;
	uint32_t length = twihs_package->length;

	/* Configure the data packet to be transmitted */
	twihs_package->addr[0] = 0;
	twihs_package->addr_length = 0;
	twihs_package->buffer = &data;
	twihs_package->length = 1;

	retval = twihs_master_write(BOARD_AT24MAC_TWIHS_INSTANCE, twihs_package);

	/* Restore the package parameters */
	twihs_package->addr[0] = addr;
	twihs_package->addr_length = addr_length;
	twihs_package->buffer = buffer;
	twihs_package->length = length;
	
	return retval;
}
/**
 * \brief Write multiple bytes to AT24MACXX.
 *
 * \param u32_address Address of the byte to write.
 * \param u8_value Value that will be written to the specified address.
 *
 * \return AT24MAC_WRITE_SUCCESS if single byte was written, AT24MAC_WRITE_FAIL
 * otherwise.
 */
uint32_t at24macxx_write_continous_modified(uint32_t u32_start_address,
											uint16_t u16_length, uint8_t const *p_wr_buffer)
{
	twihs_package_t twihs_package;

	/* Configure the data packet to be transmitted */
	twihs_package.chip = BOARD_AT24MAC_ADDRESS;
	at24mac_build_word_address(twihs_package.addr, u32_start_address);
	twihs_package.addr_length = AT24MAC_MEM_ADDR_LEN;
	twihs_package.buffer = p_wr_buffer;
	twihs_package.length = u16_length;

	if (at24macxx_acknowledge_check(&twihs_package))
	{
		return AT24MAC_WRITE_FAIL;
	}

	if (twihs_master_write(BOARD_AT24MAC_TWIHS_INSTANCE, &twihs_package) != TWIHS_SUCCESS) {
		return AT24MAC_WRITE_FAIL;
	}
	//at24macxx_acknowledge_polling(&twihs_package);

	return AT24MAC_WRITE_SUCCESS;
}

struct 
{
	uint8_t at24cxx_dynamic_state;
	uint32_t u32_start_address;
	uint16_t u16_length;
	uint8_t *p_buffer;
	uint8_t n;
}at24cxx_instance[2];

char at24cxx_d_datasent = 0;
void at24cxx_dynamic_rw_exe(void)
{
	uint16_t i;
	
	uint8_t *p_wr_buffer;	
	uint32_t u32_m_start_address, dyn_ret, chunk_ret;
	uint16_t u16_m_length;
	for(i=0; i<1; i++)
	{
		if (at24cxx_instance[i].at24cxx_dynamic_state == AT24CXX_DYNAMIC_HANDLER_WRITE)
		{
			p_wr_buffer = at24cxx_instance[i].p_buffer;
			chunk_ret = get_chunks(at24cxx_instance[i].u32_start_address, at24cxx_instance[i].u16_length,
				at24cxx_instance[i].n, &u32_m_start_address, &u16_m_length, &p_wr_buffer);
			if(chunk_ret == AT24CXX_DYNAMICS_CHUNKS_OK)
			{
				if(1/*(d_twi_get_state(i) == DYNAMIC_TWI_TCOM)*/ && (at24cxx_d_datasent == 1))
				{
					at24cxx_instance[i].n++;
					at24cxx_d_datasent = 0;
					d_twi_reset_state(i);
					return;
				}
				else if(0) //(d_twi_get_state(i) == DYNAMIC_TWI_SBUSY) && (at24cxx_d_datasent == 1))
				{
					at24cxx_d_datasent = 0;
					d_twi_reset_state(i);
					return;
				}
				//dyn_ret = at24cxx_dynamic_write_continuous(i, u32_m_start_address, u16_m_length, p_wr_buffer);
				dyn_ret = at24macxx_write_continous_modified(u32_m_start_address, u16_m_length, p_wr_buffer);
				if(dyn_ret == AT24C_WRITE_SUCCESS) 
					at24cxx_d_datasent = 1; 
					
			}
			else if (chunk_ret == AT24CXX_DYNAMICS_CHUNKS_END)
			{
				if(1/*(d_twi_get_state(i) == DYNAMIC_TWI_TCOM)*/ && (at24cxx_d_datasent == 1))
				{
					at24cxx_instance[i].at24cxx_dynamic_state = AT24CXX_DYNAMIC_HANDLER_IDLE;
					at24cxx_instance[i].n = 0;
					at24cxx_d_datasent = 0;
					d_twi_reset_state(i);
					return;
				}
				else if((d_twi_get_state(i) == DYNAMIC_TWI_SBUSY) && (at24cxx_d_datasent == 1))
				{
					at24cxx_d_datasent = 0;
					d_twi_reset_state(i);
					return;
				}
				//dyn_ret = at24cxx_dynamic_write_continuous(i, u32_m_start_address, u16_m_length, p_wr_buffer);
				dyn_ret = at24macxx_write_continuous(u32_m_start_address, u16_m_length, p_wr_buffer);
				if(dyn_ret == AT24C_WRITE_SUCCESS)
				{
					at24cxx_d_datasent = 1;
				}
				
			}
		}
		else if (at24cxx_instance[i].at24cxx_dynamic_state == AT24CXX_DYNAMIC_HANDLER_READ)
		{
			p_wr_buffer = at24cxx_instance[i].p_buffer;
			u32_m_start_address = at24cxx_instance[i].u32_start_address;
			u16_m_length = at24cxx_instance[i].u16_length;
			
			if(1/*(d_twi_get_state(i) == DYNAMIC_TWI_TCOM)*/ && (at24cxx_d_datasent == 1))
			{
				at24cxx_instance[i].at24cxx_dynamic_state = AT24CXX_DYNAMIC_HANDLER_IDLE;
				at24cxx_instance[i].n = 0;
				at24cxx_d_datasent = 0;
				d_twi_reset_state(i);
				return;
			}
			else if((d_twi_get_state(i) == DYNAMIC_TWI_SBUSY) && (at24cxx_d_datasent == 1))
			{
				at24cxx_d_datasent = 0;
				d_twi_reset_state(i);
				return;
			}
				
			//dyn_ret = at24cxx_dynamic_read_continuous(i, u32_m_start_address, u16_m_length, p_wr_buffer);
			dyn_ret = at24macxx_read_continuous(u32_m_start_address, u16_m_length, p_wr_buffer);
			if(dyn_ret == AT24C_READ_SUCCESS)
			{
				at24cxx_d_datasent = 1;
			}
		}
	}
}


#define AT24CXX_DYNAMIC_SCH_ARR_MAX 20
uint8_t at24cxx_dynamic_SCHarray_read_ptr = 0;
uint8_t at24cxx_dynamic_SCHarray_write_ptr = 0;
uint8_t at24cxx_dynamic_SCHarray_count = 0;
at24cxx_dynamic_SCHarray_t at24cxx_dynamic_SCHarray_buff[AT24CXX_DYNAMIC_SCH_ARR_MAX];

uint32_t at24cxx_dynamic_sch_wrbuff_full(void)
{
	uint32_t i = at24cxx_dynamic_SCHarray_write_ptr;
	if(at24cxx_dynamic_SCHarray_count < AT24CXX_DYNAMIC_SCH_ARR_MAX)
	{
		i++;
		at24cxx_dynamic_SCHarray_count++;
		if(i >= AT24CXX_DYNAMIC_SCH_ARR_MAX) i = 0;	
		at24cxx_dynamic_SCHarray_write_ptr = i;
		return 0;	
	}
	else return 1;

}

uint32_t at24cxx_dynamic_sch_rdbuff_empty(void)
{
	if (at24cxx_dynamic_SCHarray_count == 0)
	{
		return 1;
	} 
	else
	{
		return 0;
	}
}

uint32_t at24cxx_dynamic_sch_get_next_rdptr(void)
{
	uint32_t i = at24cxx_dynamic_SCHarray_read_ptr;
	i++;
	if(i >= AT24CXX_DYNAMIC_SCH_ARR_MAX) i = 0;
	
	return i;
}
void at24cxx_dynamic_handler(void)
{
	
	at24cxx_dynamic_SCHarray_t *p_arr;
	at24cxx_dynamic_rw_exe();
	
	if(at24cxx_dynamic_sch_rdbuff_empty() == 1) return;
	p_arr = &at24cxx_dynamic_SCHarray_buff[at24cxx_dynamic_sch_get_next_rdptr()];
	
	if (p_arr->u8_rw == 1)
	{
		if(at24cxx_dynamic_read_array(p_arr->u8_twi_instance, p_arr->u32_start_address, p_arr->u16_length, p_arr->p_buffer) == AT24CXX_DYNAMIC_OK)
		{
			at24cxx_dynamic_SCHarray_read_ptr = at24cxx_dynamic_sch_get_next_rdptr();
			at24cxx_dynamic_SCHarray_count--;
		}
	} 
	else
	{
		if(at24cxx_dynamic_write_array(p_arr->u8_twi_instance, p_arr->u32_start_address, p_arr->u16_length, p_arr->p_buffer) == AT24CXX_DYNAMIC_OK)
		{	
			at24cxx_dynamic_SCHarray_read_ptr = at24cxx_dynamic_sch_get_next_rdptr();
			at24cxx_dynamic_SCHarray_count--;
		}
	}
}

//a function to divide a data to be stored into no more than page size chunks
uint32_t get_chunks(uint32_t u32_start_address, uint16_t u16_length, 
		uint8_t n, uint32_t *u32_m_start_address, uint16_t *u16_m_length, uint8_t **p_wr_buffer)
{
	uint32_t start_page_add;
	int32_t last_chunk_size_offset;
	
	if(u16_length==0)
	{
		//return error
		return AT24CXX_DYNAMICS_CHUNKS_ERROR;
	}
	start_page_add = (u32_start_address & ~AT24CXX_DYNAMICS_PAGE_MASK);
	if (n==0)
	{
		//first chunk
		*u32_m_start_address = u32_start_address;
		*u16_m_length = AT24CXX_DYNAMICS_PAGE_SIZE - (u32_start_address & AT24CXX_DYNAMICS_PAGE_MASK);
	}
	else
	{
		//subsequent chunks
		*u32_m_start_address = n * AT24CXX_DYNAMICS_PAGE_SIZE + start_page_add;
		/*if(n==1) */*p_wr_buffer+= n * AT24CXX_DYNAMICS_PAGE_SIZE - (u32_start_address & AT24CXX_DYNAMICS_PAGE_MASK);
		//else *p_wr_buffer+= n * AT24CXX_DYNAMICS_PAGE_SIZE;
		*u16_m_length = AT24CXX_DYNAMICS_PAGE_SIZE;
	}
	last_chunk_size_offset = (*u32_m_start_address + *u16_m_length) - (u32_start_address + u16_length);
	if((last_chunk_size_offset < AT24CXX_DYNAMICS_PAGE_SIZE) && (last_chunk_size_offset >= 0))
	{
		*u16_m_length -= last_chunk_size_offset;
		return AT24CXX_DYNAMICS_CHUNKS_END;	
	}
	else if(last_chunk_size_offset > 0){
		return AT24CXX_DYNAMICS_CHUNKS_ERROR;
	}
	
	return AT24CXX_DYNAMICS_CHUNKS_OK;	
}

/**
 * \brief Write multiple bytes continuously to AT24CXX.
 */
uint32_t at24cxx_dynamic_write_continuous(uint32_t u32_twi_instance, uint32_t u32_start_address,
		uint16_t u16_length, uint8_t *p_wr_buffer)
{
	/*
	twi_package_t twi_package;

	// Configure the data packet to be transmitted 
	twi_package.chip = BOARD_AT24C_ADDRESS;
	at24c_build_word_address(twi_package.addr, u32_start_address);
	twi_package.addr_length = AT24C_MEM_ADDR_LEN;
	twi_package.buffer = (uint8_t *) p_wr_buffer;
	twi_package.length = u16_length;

	if (twi_master_write_pdc(u32_twi_instance, &twi_package) !=
			DYNAMIC_TWI_T_success) {
		return AT24C_WRITE_FAIL;
	}
	//at24cxx_acknowledge_polling(&twi_package);

	return AT24C_WRITE_SUCCESS;*/
}

/**
 * \brief Read multiple bytes continuously from AT24CXX.

 */
uint32_t at24cxx_dynamic_read_continuous(uint32_t u32_twi_instance, uint32_t u32_start_address,
		uint16_t u16_length, uint8_t *p_rd_buffer)
{
	/*
	twi_package_t twi_package;

	// Configure the data packet to be received
	twi_package.chip = BOARD_AT24C_ADDRESS;
	at24c_build_word_address(twi_package.addr, u32_start_address);
	twi_package.addr_length = AT24C_MEM_ADDR_LEN;
	twi_package.buffer = p_rd_buffer;
	twi_package.length = u16_length;

	if (twi_master_read_pdc(u32_twi_instance, &twi_package) !=
			TWI_SUCCESS) {
		return AT24C_READ_FAIL;
	}

	return AT24C_READ_SUCCESS;*/
}


uint32_t at24cxx_dynamic_write_array(uint32_t u32_twi_instance, uint32_t u32_start_address,
	uint16_t u16_length, uint8_t *p_wr_buffer)
{
	if(at24cxx_instance[u32_twi_instance].at24cxx_dynamic_state != AT24CXX_DYNAMIC_HANDLER_IDLE)
		return AT24CXX_DYNAMIC_BUSSY;	
	at24cxx_instance[u32_twi_instance].at24cxx_dynamic_state = AT24CXX_DYNAMIC_HANDLER_WRITE;
	at24cxx_instance[u32_twi_instance].p_buffer = p_wr_buffer;
	at24cxx_instance[u32_twi_instance].u16_length = u16_length;
	at24cxx_instance[u32_twi_instance].u32_start_address = u32_start_address;
	at24cxx_instance[u32_twi_instance].n = 0;	
	
	return AT24CXX_DYNAMIC_OK;
}
uint32_t at24cxx_dynamic_read_array(uint32_t u32_twi_instance, uint32_t u32_start_address,
uint16_t u16_length, uint8_t *p_rd_buffer)
{
	if(at24cxx_instance[u32_twi_instance].at24cxx_dynamic_state != AT24CXX_DYNAMIC_HANDLER_IDLE)
		return AT24CXX_DYNAMIC_BUSSY;
	at24cxx_instance[u32_twi_instance].at24cxx_dynamic_state = AT24CXX_DYNAMIC_HANDLER_READ;
	at24cxx_instance[u32_twi_instance].p_buffer = p_rd_buffer;
	at24cxx_instance[u32_twi_instance].u16_length = u16_length;
	at24cxx_instance[u32_twi_instance].u32_start_address = u32_start_address;
	at24cxx_instance[u32_twi_instance].n = 0;
	
	return AT24CXX_DYNAMIC_OK;
}



uint32_t at24cxx_dynamic_sch_write_array(uint32_t u32_twi_instance, uint32_t u32_start_address,
uint16_t u16_length, uint8_t *p_wr_buffer)
{
	if(at24cxx_dynamic_sch_wrbuff_full()) 
	{
		return 0; // no place in the buffer to write an array
	}
	at24cxx_dynamic_SCHarray_t *arr_ptr;
	arr_ptr = &at24cxx_dynamic_SCHarray_buff[at24cxx_dynamic_SCHarray_write_ptr];
	
	arr_ptr->u8_rw = 0;
	arr_ptr->u8_twi_instance = u32_twi_instance;
	arr_ptr->u32_start_address = u32_start_address;
	arr_ptr->u16_length = u16_length;
	arr_ptr->p_buffer = p_wr_buffer;
	
	return 1;	
}

uint32_t at24cxx_dynamic_sch_read_array(uint32_t u32_twi_instance, uint32_t u32_start_address,
uint16_t u16_length, uint8_t *p_rd_buffer)
{
	if(at24cxx_dynamic_sch_wrbuff_full()) return 0; // no place in the buffer to read an array
	at24cxx_dynamic_SCHarray_t *arr_ptr;
	arr_ptr = &at24cxx_dynamic_SCHarray_buff[at24cxx_dynamic_SCHarray_write_ptr];
	
	arr_ptr->u8_rw = 1;
	arr_ptr->u8_twi_instance = u32_twi_instance;
	arr_ptr->u32_start_address = u32_start_address;
	arr_ptr->u16_length = u16_length;
	arr_ptr->p_buffer = p_rd_buffer;
	
	return 1;
}