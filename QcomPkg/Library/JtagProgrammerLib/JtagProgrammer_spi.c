/*===========================================================================

JtagProgrammer_spi.c

 This is the Jtag programmer tool for SPI_NOR implementation.

 Copyright (c) 2018 Qualcomm Technologies, Incorporated.
 All Rights Reserved.
 Qualcomm Confidential and Proprietary

============================================================================*/


/*===========================================================================

                           EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.



when         who     what, where, why
----------   ---     ----------------------------------------------------------
2017-05-25  md        Initial Release

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/ 
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "spinor_api.h"
#include "JtagProgrammer.h"
#include "JtagProgrammerParam.h"
#include <Library/BaseMemoryLib.h>
#include "JtagProgrammerBsp.h"
#include "ClockBoot.h"


/*===========================================================================

                           DEFINITIONS

===========================================================================*/
#define SIZE_64K 65536
#define SIZE_32K 32768
#define SIZE_4K  4096
#define SIZE_Page 256 


/* enum to indicate status of probe */
//static probe_code_type probe_status = MEM_DEVICE_NOT_PROBED;

#define Block_Erase 0x1
#define Bulk_Erase 0x2


UINT32 dev_id= 0;
/* NOR device structure */
spinor_handle_t handle_spinor;
spinor_handle_t unit_test_handle;

struct spinor_info dev_info;

/* NOR device parameters */
UINT32 total_block_count;  /* number of erasable units in spinor */
UINT32 page_size_bytes;    /* number of bytes in a page         */
UINT32 total_pages;        /* number of pages in the device     */
char device_name[32];
uint32 dev_is_initialized = 0;


typedef struct
{
   UINT32 block_size_bytes;
   UINT32 pages_per_block;  
   UINT32 block_count;  
   UINT32 total_block_count;  /* number of erasable units in spinor */
   UINT32 page_size_bytes;	  /* number of bytes in a page		   */
   UINT32 total_pages;		  /* number of pages in the device	   */
   char device_name[32];
} NOR_DEVICE_INFO;

static NOR_DEVICE_INFO nor_device_info;

#define SIZE_IN_PAGES(x) ( ((x % nor_device_info.page_size_bytes) == 0) ? x/nor_device_info.page_size_bytes : x/nor_device_info.page_size_bytes+1 )

 /*===========================================================================

                     MACRO DEFINITIONS FOR MODULE

===========================================================================*/


/*===========================================================================

                      FUNCTION PROTOTYPES

===========================================================================*/
UINT32 jsdcc_mem_read_spi(void);
UINT32 jsdcc_mem_write_spi(void);
UINT32 jsdcc_mem_erase_spi(void);
UINT32 jsdcc_mem_compute_crc_spi(void);
UINT32 jsdcc_mem_print_info_spi(void);
UINT32 jsdcc_mem_program_spi(void);
UINT32 jsdcc_mem_chip_erase_spi(void);
UINT32 jsdcc_mem_erase_bulk_spi(void);
UINT32 jsdcc_mem_unit_test_spi(void);

void nor_utils_strcpy(char* dest, const char *src, UINT32 len)
{
  if ((strlcat(dest,src,len))>= len)
  {
   outputMessage ("Device ID too long\n ");
  }
  dest[len-1] ='\0';
}

void spinor_utils_deinit(void)
{
  if (dev_is_initialized )
  {
    spinor_close(handle_spinor);
    dev_is_initialized = FALSE;
  }
}

UINT32 spinor_utils_init(void)
{
  INT32 result = SPINOR_DEVICE_DONE;
  UINT32 block_size_bytes = 0;
 
  if (dev_is_initialized)
  {
    return SPINOR_DEVICE_DONE;
  }

  if(SPINOR_DEVICE_DONE != spinor_open(&handle_spinor))                 
  {
    outputMessage("Error Opening Spinor Device\n");
    return SPINOR_DEVICE_FAIL;
  }

  result = spinor_get_info(handle_spinor, &dev_info);
  if(SPINOR_DEVICE_DONE != result)
  {
    outputMessage("Get Info Error\n");
    result = SPINOR_DEVICE_FAIL;
    goto close_and_detach;
  }

  /* Get some information about the device */
  total_block_count = dev_info.block_count;
  page_size_bytes = dev_info.page_size_bytes;

  nor_utils_strcpy(device_name, dev_info.device_name, 
  sizeof(device_name));

  nor_utils_strcpy(nor_device_info.device_name, dev_info.device_name, 
  sizeof(device_name));

  total_pages = dev_info.pages_per_block * total_block_count;
  block_size_bytes = dev_info.pages_per_block * page_size_bytes;

  BLOCK_SIZE = block_size_bytes;
  nor_device_info.block_size_bytes = block_size_bytes;
  nor_device_info.pages_per_block= dev_info.pages_per_block;
  nor_device_info.total_block_count = total_block_count;
  nor_device_info.page_size_bytes = page_size_bytes;
  nor_device_info.total_pages = total_pages;

  dev_is_initialized = TRUE;

  return result;

close_and_detach:
   spinor_close(handle_spinor);
   return result;
}

UINT32 do_probe_spi(void)
{
  UINT32 ret_code;
  ret_code = spinor_utils_init();
  return ret_code;
}  /* do_probe */

/*===========================================================================

DESCRIPTION
  This function is a wrapper for the real memory read routine. It extracts
  the required parameters from TRACE32 and passes it onto the real thing.

DEPENDENCIES
  Assumes jtag_prog_param (global variable) has valid parameters.

RETURN VALUE
  If success, return STATUS_SUCCESS else STATUS_FAIL

SIDE EFFECTS

===========================================================================*/
UINT32 jsdcc_mem_read_spi(void)
{
  UINT32 page, data_length,byte_offset, byte_count;
  INT32 result = SPINOR_DEVICE_DONE;
  byte *pdata;

  pdata = jtag_prog_param.data;
  page = jtag_prog_param.addr * 16;
  data_length = jtag_prog_param.data_length * 16;

	/*convert pages to bytes */
	byte_offset = page * SIZE_Page;
	byte_count = data_length *SIZE_Page;
  
  outputMessage("Reading Sector 0x%x \n", jtag_prog_param.addr);
     
  result = spinor_read(handle_spinor,byte_offset, byte_count,pdata);
  
  if (SPINOR_DEVICE_DONE != result)
  {
    outputMessage("Failed with status %d\n", result);
    spinor_utils_deinit();
    return STATUS_FAIL;
  }
  else
  {
      outputMessage("Success\n");
  }

  return STATUS_SUCCESS;
}  // jsdcc_mem_read


/*===========================================================================

DESCRIPTION
  This function is a wrapper for the real memory write routine. It extracts
  the required parameters from TRACE32 and passes it onto the real thing.

DEPENDENCIES
  Assumes jtag_prog_param (global variable) has valid parameters.

RETURN VALUE
  If success, return STATUS_SUCCESS else STATUS_FAIL

SIDE EFFECTS

===========================================================================*/
UINT32 jsdcc_mem_write_spi(void)
{
  UINT32 page, data_length,byte_offset, byte_count;
  byte *pdata;
  INT32 result = SPINOR_DEVICE_DONE;

  page = jtag_prog_param.addr * 16;
  data_length = jtag_prog_param.data_length * 16;

	/*convert pages to bytes */
	byte_offset = page * SIZE_Page;
	byte_count = data_length *SIZE_Page;
  
  result = spinor_erase (handle_spinor, SIZE_IN_BLOCKS (byte_offset), SIZE_IN_BLOCKS (byte_count));
  if (SPINOR_DEVICE_DONE != result)
  {
	  outputMessage("Erase Operation Before Mem write Failed with status %d\n", result);
	  spinor_utils_deinit();
	  return STATUS_FAIL;
  }

  outputMessage("Writing to sector 0x%x\n", jtag_prog_param.addr);
  
  pdata = jtag_prog_param.data;
  result = spinor_write(handle_spinor, byte_offset, byte_count,pdata);
  
  if (SPINOR_DEVICE_DONE != result)
  {
      outputMessage("Failed with status %d\n", result);
      spinor_utils_deinit();
	    return STATUS_FAIL;
  }
  else
  {
    outputMessage("Success\n");
  }

  return STATUS_SUCCESS;
}  // jsdcc_mem_write



/*===========================================================================

DESCRIPTION
  This function erases specified number of sectors from the card

DEPENDENCIES
  Assumes jtag_prog_param (global variable) has valid parameters.

RETURN VALUE
  If success, return STATUS_SUCCESS else STATUS_FAIL

SIDE EFFECTS

===========================================================================*/

UINT32 jsdcc_mem_erase_partition_spi()
{
  UINT16 index;
  UINT32 start_block;
   
  for(index = 0; index < jtag_prog_param.data_length; index++) 
  {
    start_block = jtag_prog_param.addr + index;

    if (SPINOR_DEVICE_DONE != spinor_erase(handle_spinor, start_block, 1))
      {
      	 outputMessage("Erase Failed\n");
      	 return STATUS_FAIL;
      }
   }
   return STATUS_SUCCESS;
}


/*===========================================================================

DESCRIPTION
  This function is a wrapper for the real memory program routine. It extracts
  the required parameters from TRACE32 and passes it onto the real thing.  
  Program is very similar to write, except the length of the data is divided
  by BLOCK_SIZE.

DEPENDENCIES
  Assumes jtag_prog_param (global variable) has valid parameters.

RETURN VALUE
  If success, return STATUS_SUCCESS else STATUS_FAIL

SIDE EFFECTS

===========================================================================*/
UINT32 jsdcc_mem_program_spi(void)
{
  byte *pdata;
  INT32 result = SPINOR_DEVICE_DONE;
	UINT32 byte_offset,data_length;

  if(sector_writing) 
  {
    jtag_prog_param.addr = redirect_offset * 16 + SIZE_IN_PAGES(jtag_prog_param.addr);
    jtag_prog_param.data_length = jtag_prog_param.data_length < redirect_partition ? jtag_prog_param.data_length : redirect_partition;
    if (jtag_prog_param.data_length == 0) 
    {
      return STATUS_SUCCESS;
    }
    redirect_partition -= jtag_prog_param.data_length;
  }
  pdata = jtag_prog_param.data;
  byte_offset = jtag_prog_param.addr*SIZE_Page;
	data_length = jtag_prog_param.data_length;
	
  result = spinor_erase (handle_spinor, SIZE_IN_BLOCKS (byte_offset), SIZE_IN_BLOCKS (data_length));

	if (SPINOR_DEVICE_DONE != result)
  {
	  outputMessage("Erase Operation Before Mem Program Failed with status %d\n", result);
	  spinor_utils_deinit();
	  return STATUS_FAIL;
  }
  
	result = spinor_write(handle_spinor, 
	byte_offset,data_length,pdata);
   
  if (SPINOR_DEVICE_DONE != result)
  {
	  outputMessage("Mem Program Failed with status %d\n", result);
	  spinor_utils_deinit();
	  return STATUS_FAIL;
  }

  return STATUS_SUCCESS;
}  // jsdcc_mem_program


/*===========================================================================

DESCRIPTION
  This function calls the mem_get_device_info API to get the information of 
  the memory device attached.

DEPENDENCIES
  Assumes jtag_prog_param (global variable) has valid parameters.

RETURN VALUE
  If success, return STATUS_SUCCESS else STATUS_FAIL

SIDE EFFECTS

===========================================================================*/
UINT32 jsdcc_mem_print_info_spi(void)
{
   
   outputMessage("Device Name = %s\n", nor_device_info.device_name); 
   outputMessage("Block size = %d \n", nor_device_info.block_size_bytes);
   outputMessage("Pages per block = %d \n", nor_device_info.pages_per_block);
   outputMessage("Total Block count is %d \n", nor_device_info.total_block_count);
   outputMessage("Page size  is %d \n", nor_device_info.page_size_bytes);
   outputMessage("Total Pages Avail is %d \n", nor_device_info.total_pages);
   
   return STATUS_SUCCESS;
}  /* jsdcc_mem_print_info */

/*===========================================================================

DESCRIPTION
  This function erases a specified sector from the card

DEPENDENCIES
  Assumes jtag_prog_param (global variable) has valid parameters.

RETURN VALUE
  If success, return STATUS_SUCCESS else STATUS_FAIL

SIDE EFFECTS

===========================================================================*/
UINT32 jsdcc_mem_erase_spi()
{
  UINT32 start_block = jtag_prog_param.addr;
  outputMessage("Erasing Block 0x%x\n", start_block);
  
  /* Ensure block is in range */
  if (start_block >= total_block_count)
  {
    outputMessage("is out of range \n");
    return SPINOR_DEVICE_FAIL;
  }
       
  if (SPINOR_DEVICE_DONE != spinor_erase(handle_spinor, start_block, 1))     
  {
    outputMessage("Failed\n");
    spinor_utils_deinit();
    return STATUS_FAIL;
  }
  else
  {
    outputMessage("Success\n");
  }

 return STATUS_SUCCESS;
}

/*===========================================================================

DESCRIPTION
  This function erases a specified sector from the card

DEPENDENCIES
  Assumes jtag_prog_param (global variable) has valid parameters.

RETURN VALUE
  If success, return STATUS_SUCCESS else STATUS_FAIL

SIDE EFFECTS

===========================================================================*/
UINT32 jsdcc_mem_erase_bulk_spi()
{
  UINT32 start_block,data_length;
  start_block= jtag_prog_param.addr;
  data_length=jtag_prog_param.data_length;
  

   outputMessage("Bulk Erasing Block 0x%x 0x%x \n", start_block, data_length);
  
    /* Ensure block is in range */
  if (start_block >= total_block_count)
  {
    outputMessage("is out of range \n");
    return SPINOR_DEVICE_FAIL;
  }
  
  if (SPINOR_DEVICE_DONE != spinor_erase_bulk(handle_spinor, start_block, data_length))     
  {
    outputMessage("Failed\n");
    spinor_utils_deinit();
    return STATUS_FAIL;
  }
  else
  {
    outputMessage("Success\n");
  }

  return STATUS_SUCCESS;
}
/*===========================================================================

DESCRIPTION
  This function is for unit test of spinor driver apis.

DEPENDENCIES
  Assumes jtag_prog_param (global variable) has valid parameters.

RETURN VALUE
  If success, return STATUS_SUCCESS else STATUS_FAIL

SIDE EFFECTS

===========================================================================*/

UINT32 jsdcc_mem_unit_test_spi()
{
  INT32 result = SPINOR_DEVICE_DONE;
  UINT32 page,data_length,erase_start_block;
  char Test_Write_Buffer[4096];
  char Test_Read_Buffer[4096];
  UINT16 i,j, flag;
  INT8 Erase_type = jtag_prog_param.reserved;
  UINT32 Block_count;
  char  WriteTestChar = 0xAA;
	char  EraseTestChar = 0xFF;
  UINT32 rw_page;
  UINT32 block_no;
  flag =0;
  page = jtag_prog_param.addr * 16 * SIZE_Page;
  data_length = jtag_prog_param.data_length * 16 * SIZE_Page;
  erase_start_block = jtag_prog_param.addr;
  Block_count = jtag_prog_param.data_length;

  if (Block_count <= 0)
  {
    outputMessage("Invalid Arguments, total block count should be greater than 0 \n");
		flag =1;
    goto jsdcc_mem_unit_test_spi_end;
		
  }

  result = spinor_open(&handle_spinor);
  if(SPINOR_DEVICE_DONE != result)                 
  {
    outputMessage("Error Opening The Device For TestClient-1\n");
		flag =2;
    goto jsdcc_mem_unit_test_spi_end;
  }

  result = spinor_open(&unit_test_handle);
  if(SPINOR_DEVICE_DONE != result)                 
  {
    outputMessage("Error Opening The Device For TestClient-1\n");
		flag = 3;
    goto jsdcc_mem_unit_test_spi_end;
  }

  outputMessage("Done with Flash open");

  if (Erase_type == Block_Erase)
  {
    if (SPINOR_DEVICE_DONE != spinor_erase(handle_spinor, erase_start_block, Block_count))     
    {
      outputMessage("Block Erase Failed \n");
			flag =4;
      goto jsdcc_mem_unit_test_spi_end;
    }
    else
    {
     outputMessage("Block Erase Successful!\n");
    }
  }
  else if (Erase_type == Bulk_Erase)
  {
    if (SPINOR_DEVICE_DONE != spinor_erase_bulk(handle_spinor, erase_start_block ,  Block_count))     
    {
      outputMessage("Bulk Erase Failed \n");
			flag = 5;
      goto jsdcc_mem_unit_test_spi_end;
    }
    else
    {		
      outputMessage("Bulk Erase Successful!\n");
    }
  }
  else
  {
   outputMessage("Invalid Option Selected!\n");
	 flag = 6;
   goto jsdcc_mem_unit_test_spi_end;
  }

  for(i=0; i<4096; i++)
  {
    Test_Write_Buffer[i]= WriteTestChar;
  }
  outputMessage("Buffer initialized\n");

  rw_page = (jtag_prog_param.addr) * 16 * SIZE_Page;
  block_no = jtag_prog_param.addr;

  for (i= 0; i < Block_count; i++)
  {
	  result = spinor_write(handle_spinor,rw_page, (16 * SIZE_Page), &Test_Write_Buffer);
    if (SPINOR_DEVICE_DONE != result)
    {
      outputMessage("Write Flash Failed with status %d\n", result);
			flag = 7;
      goto jsdcc_mem_unit_test_spi_end;
    }
    else
    {
      //outputMessage("Write Successful for block %d\n",block_no+i);
    }
    memset(&Test_Read_Buffer, 0, sizeof(Test_Read_Buffer));

    result = spinor_read(handle_spinor, rw_page, (16 * SIZE_Page), &Test_Read_Buffer);
    if (SPINOR_DEVICE_DONE != result)
    {
      outputMessage("Read Flash Failed with status %d\n", result);
			flag = 8;
      goto jsdcc_mem_unit_test_spi_end;
    }
    else
    {
      //outputMessage("Read Successful for block %d\n",block_no+i);
    }

    for(j=0; j<4096; j++)
    {
      if (WriteTestChar != Test_Read_Buffer[j] )
      {
       outputMessage("Failed!!Read Write mismatch at %d\n", j);
			 flag = 9;
       goto jsdcc_mem_unit_test_spi_end;
      }
    }

  outputMessage("Data comparison between Read and Write operations Successful! \n");
  rw_page = rw_page + (16 * SIZE_Page);

  }

  if (Erase_type == Block_Erase)
  {
    outputMessage("Performing Block Erase after Write Operation!!\n");
    if (SPINOR_DEVICE_DONE != spinor_erase(handle_spinor, erase_start_block ,  Block_count))     
    {
      outputMessage("Block Erase Iteration 2 failed \n");
			flag = 10;
      goto jsdcc_mem_unit_test_spi_end;
    }
    else
    {
     outputMessage("Block Erase Iteration 2 Successful!\n");
    }
  }
  else if (Erase_type == Bulk_Erase)
  {
   if (SPINOR_DEVICE_DONE != spinor_erase_bulk(handle_spinor, erase_start_block ,  Block_count))     
   {
     outputMessage("Bulk Erase Iteration 2 failed \n");
		 flag = 11;
     goto jsdcc_mem_unit_test_spi_end;
   }
   else
   {		
     outputMessage("Bulk Erase Iteration 2 Successful!\n");
   }
  }

	rw_page = (jtag_prog_param.addr) * 16 * SIZE_Page;
  block_no = jtag_prog_param.addr;
	
	for (i= 0; i < Block_count; i++)
  {
    memset(&Test_Read_Buffer, 0, sizeof(Test_Read_Buffer));

    result = spinor_read(handle_spinor, rw_page, (16 * SIZE_Page), &Test_Read_Buffer);
    if (SPINOR_DEVICE_DONE != result)
    {
      outputMessage("Read Flash Iteration 2 Failed with status %d\n", result);
			flag = 12;
      goto jsdcc_mem_unit_test_spi_end;
    }
    else
    {
      outputMessage("Read Flash Iteration 2 Successful for block %d\n",block_no+i);
    }

    for(j=0; j<4096; j++)
    {
      if (EraseTestChar != Test_Read_Buffer[j] )
      {
       outputMessage("Failed!!Read Write mismatch at %d for iteration 2\n", j);
			 flag = 13;
       goto jsdcc_mem_unit_test_spi_end;
      }
    }

  outputMessage("Data comparison between Read and Write operations Successful for Iteration 2! \n");
  rw_page = rw_page + (16 * SIZE_Page);

  }
	
  result = spinor_close(handle_spinor);
  if(SPINOR_DEVICE_DONE != result)                 
  {
   outputMessage("Error Closing The Device For TestClient-1\n");
	 flag = 14;
   goto jsdcc_mem_unit_test_spi_end;
  } 

  result = spinor_close(unit_test_handle);
  if(SPINOR_DEVICE_DONE != result)                 
  {
    outputMessage("Error Closing The Device For TestClient-1\n");
		flag = 15;
    goto jsdcc_mem_unit_test_spi_end;
  }
  jsdcc_mem_unit_test_spi_end:
  if (flag ==0)
  {
    outputMessage("Unit Test succesful!!\n");
    return STATUS_SUCCESS;
  }
	else 
	{
	  outputMessage ("Failed with flag status %d",flag);
		spinor_utils_deinit();
		return SPINOR_DEVICE_FAIL;	 
	}
}


/*===========================================================================

DESCRIPTION
  This function computes the CRC32 over a buffer

DEPENDENCIES
  Assumes jtag_prog_param (global variable) has valid parameters.

RETURN VALUE
  If success, return STATUS_SUCCESS else STATUS_FAIL

SIDE EFFECTS

===========================================================================*/
UINT32 jsdcc_mem_compute_crc_spi(void)
{ 
   UINT32 page, data_length, byte_offset, byte_count;
   INT32 result = SPINOR_DEVICE_DONE;
   byte *pdata;

   pdata = jtag_prog_param.data;
   page = jtag_prog_param.addr * 16;
   data_length = SIZE_IN_PAGES(jtag_prog_param.data_length);

	 byte_offset = page * SIZE_Page;
	 byte_count = data_length *SIZE_Page;
   
   outputMessage("Reading Sector 0x%x \n", jtag_prog_param.addr);
	  
   result = spinor_read(handle_spinor, byte_offset, byte_count,pdata);
   
   if (SPINOR_DEVICE_DONE != result) 
   {
     outputMessage("Failed with status %d\n", result);
     spinor_utils_deinit();
     return STATUS_FAIL;
   }
   else
   {
	   outputMessage("Success\n");
   }

   jtag_prog_param.data_length = calc_crc32((UINT8 *)pdata, jtag_prog_param.data_length);
   
   outputMessage("CRC Compute: Success! \n");

   return STATUS_SUCCESS;
}

/*===========================================================================

DESCRIPTION
  This function is a wrapper for the real memory erase routine.  
  The step to erase the entire memory device is to check the size
  of each physical partition and erase the partition by issue the 
  block erase command.

RETURN VALUE
  If success, return STATUS_SUCCESS else STATUS_FAIL

SIDE EFFECTS

===========================================================================*/

UINT32 jsdcc_mem_chip_erase_spi(void)
{
  
  INT32 result = SPINOR_DEVICE_DONE;
  UINT32 block_count; 
  UINT32 start_block;

  block_count = nor_device_info.total_block_count/(SIZE_64K/BLOCK_SIZE);
    /* Erase every block in the spinor, one at a time */
  start_block = jtag_prog_param.addr;
  result = spinor_erase_bulk(handle_spinor, start_block, block_count);  

  if (result != SPINOR_DEVICE_DONE)
  {
    outputMessage("Erase chip failed");
    return STATUS_FAIL;
  }  
  
  return STATUS_SUCCESS;
}  

/*===========================================================================

DESCRIPTION
  This function call the API to set the active boot partition

DEPENDENCIES

RETURN VALUE
  If success, return the starting sector of the givin partition, if
  failed, return 0

SIDE EFFECTS

===========================================================================*/
UINT32 jsdcc_mem_set_active_boot_spi(void)
{
   return STATUS_SUCCESS;
}


