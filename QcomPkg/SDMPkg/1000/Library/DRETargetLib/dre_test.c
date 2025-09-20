/*=============================================================================
                              DRE TEST.C
GENERAL DESCRIPTION
This is the target header file for DRE Testing.

Copyright 2020 by Qualcomm Technologies, Inc.  All Rights Reserved.

===========================================================================

                            EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.


when       who        what, where, why
--------   ---        --------------------------------------------------------
07/13/20   adawar     Added DRE change to poipuAU from hanaAU
=============================================================================

*/
#include "dre_hwio.h"
#include "ddr_drivers.h"
#include "boot_logger.h"
#include "ddr_log.h"

#define DEBUG_PRINT 0

typedef struct
{
    uint8 inputData[64]; // 256 bit data
    uint16 syndrome; //11 bit syndrome
} sDataSyndrome;

sDataSyndrome testData[20];
sDataSyndrome bitFlipTestData[20];
sDataSyndrome twobitFlipTestData[20];

int numTestSamples = 0;

/* ============================================================================
**  Function : copy_nbytes
** ============================================================================
*/
/**
*   @brief
*   This function copies buffer from source to destination.
*
*   @return 
*
*   @dependencies
*   None
*
*/

static void copy_nbytes(uint8* dst, uint8* src, uint8 n)
{
    for (int i = 0; i < n; i++)
    {
        dst[i] = src[i];
    }
}

/* ============================================================================
**  Function : compare_nbytes
** ============================================================================
*/
/**
*   @brief
*   This function compares destination and source buffer .
*
*   @return 1 if buffer is equal, else 0
*
*   @dependencies
*   None
*
*/
static boolean compare_nbytes(uint8* dest, uint8* src, uint8 n)
{
    for (int i = 0; i < n; i++)
    {
        if (dest[i] != src[i])
        {
            return 0;
        }
    }
    return 1;
}

/* ============================================================================
**  Function : createTestData
** ============================================================================
*/
/**
*   @brief
*   This function creates test data.
*
*   @return 
*
*   @dependencies
*   None
*
*/
void createTestData()
{
    numTestSamples = 2;

    /* 2 Test Samples :
    Test Sample 1 is all 0s
    Test Sample 2 is all 1s*/
    for (int i = 0; i < 64; i++)
    {
        testData[0].inputData[i] = 0x0;
        bitFlipTestData[0].inputData[i] = 0x0;
        twobitFlipTestData[0].inputData[i] = 0x0;
    }
    testData[0].syndrome = 0x0;
    bitFlipTestData[0].inputData[0] = 0x1;
    twobitFlipTestData[0].inputData[0] = 0x3;


    for (int i = 0; i < 64; i++)
    {
        testData[1].inputData[i] = 0xff;
        bitFlipTestData[1].inputData[i] = 0xff;
        twobitFlipTestData[1].inputData[i] = 0xff;
    }
    testData[1].syndrome = 0x483;
    bitFlipTestData[1].inputData[0] = 0xFE;
    twobitFlipTestData[1].inputData[0] = 0xFC;

}

/* ============================================================================
**  Function : read_ce_intr
** ============================================================================
*/
/**
*   @brief
*   This function reads correctable error interrupt status.
*
*   @return 
*
*   @dependencies
*   None
*
*/

uint8 read_ce_intr()
{
    uint8 sec_ded_interrupt = 0;
    uint8 is_log_entry_valid =  HWIO_DRE_DRE_ECC_LOG_ERROR_i_INFO_INMI(0, HWIO_DRE_DRE_ECC_LOG_ERROR_i_INFO_ENTRY_VALID_BMSK);
    if ((is_log_entry_valid))
    {
        sec_ded_interrupt  = HWIO_DRE_DRE_ECC_LOG_ERROR_i_INFO_INMI(0, HWIO_DRE_DRE_ECC_LOG_ERROR_i_INFO_SEC_DED_BMSK) >> HWIO_DRE_DRE_ECC_LOG_ERROR_i_INFO_SEC_DED_SHFT;
    }
    return (sec_ded_interrupt == 0x1);
}

/* ============================================================================
**  Function : read_ue_intr
** ============================================================================
*/
/**
*   @brief
*   This function reads uncorrectable error interrupt status
*
*   @return 
*
*   @dependencies
*   None
*
*/

uint8 read_ue_intr()
{
    uint8 sec_ded_interrupt = 0;
    uint8 is_log_entry_valid =  HWIO_DRE_DRE_ECC_LOG_ERROR_i_INFO_INMI(0, HWIO_DRE_DRE_ECC_LOG_ERROR_i_INFO_ENTRY_VALID_BMSK);
    if ((is_log_entry_valid))
    {
        sec_ded_interrupt  = HWIO_DRE_DRE_ECC_LOG_ERROR_i_INFO_INMI(0, HWIO_DRE_DRE_ECC_LOG_ERROR_i_INFO_SEC_DED_BMSK) >> HWIO_DRE_DRE_ECC_LOG_ERROR_i_INFO_SEC_DED_SHFT;
    }
    return (sec_ded_interrupt == 0x10);
}

/* ============================================================================
**  Function : flush_syndrome
** ============================================================================
*/
/**
*   @brief
*   This function flushes syndrome cache.
*
*   @return 
*
*   @dependencies
*   None
*
*/

static void flush_syndrome()
{
    HWIO_DRE_DRE_SYNDROME_CFLUSH_CTRL_0_OUT(0x1);

    while (HWIO_DRE_DRE_SYNDROME_CFLUSH_STATUS_INM(HWIO_DRE_DRE_SYNDROME_CFLUSH_STATUS_DONE_BMSK) != 0x1)
    {
        ddr_printf(DDR_ERROR, "Waiting for flush status to  1");
    }
}


/* ============================================================================
**  Function : test_ecc
** ============================================================================
*/
/**
*   @brief
*   This function does a simple unit test of DRE HW.
*
*   @param[in] pDataAddressDRE 
*   @param[in] pSyndromeAddress 
*
*   @return 
*
*   @dependencies
*   None
*
*/

boolean test_ecc(uint64 pDataAddressDRE, volatile uint64 pSyndromeAddress)
{
    /* Create Test samples*/
    createTestData();

    /* Test 1: Write data to ECC region and read back*/
    volatile uint16 outputSyndrome =  0;
    uint8 outputData[64];
    boolean isDataSame = FALSE, isSyndromeSame = FALSE;
    for (int i = 0; i < numTestSamples; i++)
    {
        /* Write 64 bytes data into DRE Address */
        copy_nbytes((uint8*)(pDataAddressDRE), testData[i].inputData, 64);

        /*Read 64 bytes from DRE Address*/
        copy_nbytes(outputData, (uint8*)pDataAddressDRE, 64);

        flush_syndrome();

        /*Read the syndrome*/
        copy_nbytes((uint8*)&outputSyndrome, (uint8*)pSyndromeAddress, 2);

        /*check if data and syndrome are as expected*/
        isDataSame = compare_nbytes(outputData, testData[i].inputData, 64);
        isSyndromeSame = ((testData[i].syndrome & 0x7FF) == (outputSyndrome & 0x7FF));

        if (isDataSame && isSyndromeSame)
        {
#if DEBUG_PRINT
            ddr_printf(DDR_ERROR, "Test 1 sample is %d isDataSame and isSyndromeSame are %d %d ", i,  isDataSame, isSyndromeSame);
            ddr_printf(DDR_ERROR, "Test 1 - Write and read of ecc data success !!!");
#endif
        } else
        {
            ddr_printf(DDR_ERROR, "Test 1 sample is %d isDataSame and isSyndromeSame are %d %d ", i,  isDataSame, isSyndromeSame);
            ddr_printf(DDR_ERROR, "DRE Test 1 - FAILED");
			return FALSE;
        }
    }

    /*Test 2 - Introduce 1 bit error and check for interrupts*/
    uint64  pDataAddresNonDRE = pDataAddressDRE & (0x7FFFFFFFF); /*This is the DDR address to access outside DRE*/
    uint8 ce_intr_status = 0;
    /* 1 bit Error Test*/
    for (int i = 0; i < numTestSamples; i++)
    {

        /*Write 64 bytes data into DRE Address*/
        copy_nbytes((uint8*)(pDataAddressDRE), testData[i].inputData, 64);

        /*flush syndrome to cache */
        flush_syndrome();

        /*Write 64 bytes (with 1 bit flip) into non DRE address*/
        copy_nbytes((uint8*)pDataAddresNonDRE, bitFlipTestData[i].inputData, 64);

        /*Read the data through DRE address back*/
        copy_nbytes(outputData, (uint8*)pDataAddressDRE, 64);

        /*Check the CE interrupt status (correctable error interrupt status)*/
        ce_intr_status = read_ce_intr();

        /*Check if the data read is corrected for 1 bit error*/
        isDataSame = compare_nbytes(outputData, testData[i].inputData, 64);
        /*snprintf(log_string, 256, "Test -2 sample is %d isDataSame and ce_intr_status are %d %d ", i,  isDataSame, ce_intr_status);
        boot_log_message(log_string);*/
        if ((ce_intr_status == 1) && isDataSame)
        {
#if DEBUG_PRINT
            ddr_printf(DDR_ERROR, "Test 2- SEC interrupt generation success !!!");
            //Clear interrupts
#endif
        } else
        {
            ddr_printf(DDR_ERROR, "DRE Test 2 - FAILED");
			return FALSE;
        }
    }

    for (int i = 0; i < numTestSamples; i++)
    {

        /* Fault injection */
        HWIO_DRE_DRE_ECC_CONTROL_OUTM(HWIO_DRE_DRE_ECC_CONTROL_ENABLE_FAULT_INJECT_BMSK, (0xA) << (HWIO_DRE_DRE_ECC_CONTROL_ENABLE_FAULT_INJECT_SHFT));
        HWIO_DRE_DRE_ECC_FAULT_INJ_ADDR_OUT(pDataAddressDRE >> 4);
        HWIO_DRE_DRE_ECC_FAULT_INJ_i_DATA_OUTI(0, 0x400);

        /*Write 64 bytes data into DRE Address */
        copy_nbytes((uint8*)(pDataAddressDRE), testData[i].inputData, 64);

        /*flush syndrome to cache*/
        flush_syndrome();

        /*Read the data through DRE address back*/
        copy_nbytes(outputData, (uint8*)pDataAddressDRE, 64);

        /*Check the CE interrupt status (correctable error interrupt status)*/
        ce_intr_status = read_ce_intr();

        /*Check if the data read is corrected for 1 bit error*/
        isDataSame = compare_nbytes(outputData, testData[i].inputData, 64);
        /*snprintf(log_string, 256, "Test -2 sample is %d isDataSame and ce_intr_status are %d %d ", i,  isDataSame, ce_intr_status);
        boot_log_message(log_string);*/
        if ((ce_intr_status == 1) && isDataSame)
        {
#if DEBUG_PRINT
            ddr_printf(DDR_ERROR, "Test 3- SEC interrupt generation success !!!");
            //Clear interrupts
#endif
        } else
        {
            ddr_printf(DDR_ERROR, "DRE Test 3 - FAILED");
			return FALSE;
        }
    }
	
    /*Remove fault injection*/
    HWIO_DRE_DRE_ECC_CONTROL_OUTM(HWIO_DRE_DRE_ECC_CONTROL_ENABLE_FAULT_INJECT_BMSK, (0x0));
    HWIO_DRE_DRE_ECC_FAULT_INJ_ADDR_OUT(0x0);
    HWIO_DRE_DRE_ECC_FAULT_INJ_i_DATA_OUTI(0,0x0);
    return TRUE;
}

