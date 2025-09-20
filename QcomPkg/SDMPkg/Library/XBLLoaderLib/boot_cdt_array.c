
/*==========================================================================	
                        NOTE: This is a generated file!
===========================================================================*/	
/*==========================================================================

                   INCLUDE FILES

===========================================================================*/
#include "boot_comdef.h"
#include "boot_config_data.h"

/*=============================================================================

        LOCAL DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, types,
variables and other items needed by this module.

=============================================================================*/
/**
* fixed size array that holds the cdt table in memory, it's intialized to 
* CDT v1 with only one data block which is platform id. The platform id type 
* is UNKNOWN as default.
*/
uint8 config_data_table[CONFIG_DATA_TABLE_MAX_SIZE] = 
{
	/* Header */

	0x43, 0x44, 0x54, 0x00, 
	0x01, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 

	/* Meta data */

	0x12, 0x00, 0x06, 0x00, 

	/* Block data */

	0x03, 0x08, 0x01, 0x00, 
	0x00, 0x00, 

};

/**
    cdt table size
*/
uint32 config_data_table_size = 24;
