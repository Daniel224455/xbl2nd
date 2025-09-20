/**
  @file  EFICate.h
  @brief CATE Protocol for UEFI.
*/

/*=============================================================================
  Copyright (c) 2018 Qualcomm Technologies, Incorporated.
  All rights reserved.
  Qualcomm Technologies, Confidential and Proprietary.
=============================================================================*/

/*=============================================================================
                              EDIT HISTORY


 when       who     what, where, why
 --------   ---     -----------------------------------------------------------
 01/25/18   yw      Created.

=============================================================================*/
#ifndef __EFICATE_H__
#define __EFICATE_H__

/*===========================================================================
  INCLUDE FILES
===========================================================================*/
#include<DalDevice.h>
#include<DALStdErr.h>
#include<DALStdDef.h>
#include<DALDeviceId.h>

/*===========================================================================
  MACRO DECLARATIONS
===========================================================================*/

#define      TfPrint(fmt, ...)    \
    DEBUG((EFI_D_WARN, fmt, ##__VA_ARGS__))

// Configurable Properties
#define MIN_TEST_PERIOD 50
#define MAX_TEST_NAME_LENGTH 32
#define MAX_TEST_NUM 200
#define MAX_BUF_SIZE 2048

// Parameter Types
#define TF_PARAM_IDENTIFIER_SIZE 2  /**< -- # of bytes in each TF_PARAM_*              */
#define TF_PARAM_UINT8  "0"         /**< -- Specifies that the parameter is a uint8    */
#define TF_PARAM_UINT16 "1"         /**< -- Specifies that the parameter is a uint16   */
#define TF_PARAM_UINT32 "2"         /**< -- Specifies that the parameter is a uint32   */
#define TF_PARAM_INT8   "3"         /**< -- Specifies that the parameter is a byte     */
#define TF_PARAM_INT16  "4"         /**< -- Specifies that the parameter is a int16    */
#define TF_PARAM_INT32  "5"         /**< -- Specifies that the parameter is a int32    */
#define TF_PARAM_STRING "6"         /**< -- Specifies that the parameter is a string   */

/** @brief Testing Framework User (APTTESTS) Response Codes
   @detail
   These are the values that should be returned from test cases.
 */
typedef enum
{
   TF_RESULT_CODE_SUCCESS     = 0,     /**< -- if test case runs successfully          */
   TF_RESULT_CODE_FAILURE     = 1,     /**< -- if test case run unsuccessfully         */
   TF_RESULT_CODE_BAD_PARAM   = 2      /**< -- if test case's parameters are invalid   */
} TF_TestCaseResponseCodes;

/** @brief Testing Framework API return Codes
   @detail
   These are the values that should be returned from testframework internal apis only.
 */
typedef enum
{
   TF_SUCCESS                 = 0, 		/**< -- api successful     */
   TF_FAILURE				  = 0xFFFF, /**< -- api failure     */
   TF_ERROR_GENERIC           = 0x8001, /**< -- undefined error       */
   TF_ERROR_TEST_NONEXIST 	  = 0x8002, /**< -- test case not found               */
   TF_ERROR_ALLOC             = 0x8003, /**< -- malloc operation failed            */
   TF_ERROR_BAD_PARAM         = 0x8004, /**< -- bad arg was passed in  */
   TF_ERROR_MEM_FULL          = 0x8005, /**< -- memory full  */
   TF_ERROR_SYNC			  = 0x8006, /**< -- sync error need reset driver context  */
   TF_ERROR_CMD_UNKNOWN       = 0x8007
}TF_Result;

/** @brief Test Case Function Pointer Type -- Version 0
    @detail
    Function prototype of test case entry points; no return buffer.
 */
typedef uint32 (*TF_TCFunctionPtr)(uint32 dwParam, char *apszParam[]);

/** @brief Testing Framwork Parameter Descriptions
    @detail
    Description of a test case parameter's type, name and help/description.
 */
typedef struct 
{
   char *pszType;         /**< -- parameter's type. Values are of type TF_PARAM        */ 
   char *pszName;         /**< -- name of the parameter. Nullable                      */ 
   char *pszHelpString;   /**< -- General help info about this parameter. Nullable     */
} TF_ParamDescr;

/** @brief Testing Framework Test Context description
    @detail
    Used to describe a test case's context: what the test case is being used on
 */
typedef struct
{
   uint8 cSubsysId;            /**< -- Subsystem id of the test case                   */
   uint16 wContextId;          /**< -- Context id of the test case                     */
} TF_ContextDescr;

/** @brief Help Description
   @detail 
   Contains information about a specific test case. 
   Parameter information does not need to be included.
 */
typedef struct 
{
   const char *pszFunctionDescr;  /**< --  Test case description. Should not be NULL    */
   uint32 dwParametersLength;      /**< --  Number of parameters expected.   Can be zero */
   const TF_ParamDescr *pParameters;/**< --  Parameter descriptions. Can be NULL         */
} TF_HelpDescr;

/** @brief Testing Framework Test Function Description
    @detail
    Describes everything about a test case needed to run it. The testing table is
    an array of these elements. Each one of these is used to register a single
    test case into the Test Framework.
 */
typedef struct 
{
   const char *pszTestName;        /**< --  Unique name for the test case. Nonnullable  */
   TF_TCFunctionPtr pTestFunc;     /**< --  Pointer to test entry point. Nonnullable    */
   const TF_HelpDescr *pHelp;      /**< --  Information about test case. Nullable       */
   const TF_ContextDescr *pContext;/**< --  Context the tc desiged for. Nullable        */
   uint32 dwTestFuncType;          /**< --  Determines version for pTestFunc member;
                                            must be listed as at least the 5th member
                                            of this struct so it gets initialized to
                                            0 in old test case code.                    */
} TF_TestFunction_Descr;


typedef struct 
{
   const char *pszName;        				/**< --  Unique name for the test list  */
   const TF_TestFunction_Descr** TestList;        /**< --  array of the test list        **/
   
} TF_TestList_Descr;


typedef struct
{
   struct DalDevice DalDevice;
   DALResult (*AddTests)(DalDeviceHandle *, const TF_TestFunction_Descr*[], uint32);
   DALResult (*RemoveTests)(DalDeviceHandle *, const TF_TestFunction_Descr*[], uint32);
   DALResult (*HandleCommand)(DalDeviceHandle *, const char*);
}CateTbl;

//this is the DalDeviceHandle
typedef struct
{
   uint32 dwDalHandleId;
   const CateTbl * pVtbl;
   void * pClientCtxt; 
}CateHandle;


/** @addtogroup
@{ */
/**
  Protocol version.
*/
#define EFI_CATE_PROTOCOL_VERSION 0x0000000000010002

/*  Protocol GUID definition */

#define EFI_CATE_PROTOCOL_GUID \
  { 0x3bec87db, 0x4e76, 0x491c, { 0x96, 0x6c, 0x7c, 0xa5, 0x81, 0x2a, 0x64, 0xc9 } }

/** @cond */
/*===========================================================================
  EXTERNAL VARIABLES
===========================================================================*/
/**
  External reference to the CATE Protocol GUID.
*/
extern EFI_GUID gEfiCateProtocolGuid;

/** @endcond */

/*===========================================================================
  TYPE DEFINITIONS
===========================================================================*/

/* EFI_CATE_ADD_TESTS */
/** @ingroup EFI_CATE_ADD_TESTS
  @par Summary
  add test cases into test framework

  @return
  EFI_SUCCESS -- Function completed successfully.
 */
typedef EFI_STATUS (EFIAPI *EFI_CATE_ADD_TESTS)(
  IN CONST TF_TestFunction_Descr** Tests,
  IN UINTN							Num_Tests
);
 

/* EFI_CATE_REMOVE_TESTS */
/** @ingroup EFI_CATE_REMOVE_TESTS
  @par Summary
  remove test cases from test framework

  @return
  EFI_SUCCESS -- Function completed successfully.
 */

typedef EFI_STATUS (EFIAPI *EFI_CATE_REMOVE_TESTS)(
  IN CONST TF_TestFunction_Descr** Tests,
  IN UINTN							Num_Tests
);
/* EFI_CATE_HANDLE_COMMAND */
/** @ingroup EFI_CATE_HANDLE_COMMAND
  @par Summary
  process given command

  @return
  EFI_SUCCESS -- Function completed successfully.
 */
typedef EFI_STATUS (EFIAPI *EFI_CATE_HANDLE_COMMAND)(
   IN CONST CHAR8* Request
);

/*===========================================================================
  PROTOCOL INTERFACE
===========================================================================*/
/** @ingroup _EFI_CATE_PROTOCOL
  @par Summary
  CATE Protocol interface.

  @par Parameters
  @inputprotoparams
*/
typedef struct _EFI_CATE_PROTOCOL {
   UINT64                           Version;
   EFI_CATE_ADD_TESTS				AddTests;
   EFI_CATE_REMOVE_TESTS			RemoveTests;
   EFI_CATE_HANDLE_COMMAND			HandleCommand;
}EFI_CATE_PROTOCOL;

#endif  /* __EFICATE_H__ */
