#ifndef _TZOS_H
#define _TZOS_H

typedef enum
{
  TZOS_APP_START_COMMAND = 0xFF01,
  TZOS_APP_SHUTDOWN_COMMAND,
  TZOS_APP_GET_ID_COMMAND
}tzos_command;




/*-------------------------------------------------------------
  Enum for different response types that are sent by TZOS
  ------------------------------------------------------------*/
typedef enum
{
  TZOS_STATUS_SUCCESS = 0,
  TZOS_STATUS_ERROR,
  TZOS_STATUS_RESERVED = 0x7FFFFFFF
}tzos_command_rsp_status;

/*-------------------------------------------------------------
  Enum for different response types that are sent by TZOS
  ------------------------------------------------------------*/
typedef enum
{
  TZOS_RSP_APP_ID = 0xEE01,
  TZOS_RSP_RESULT,
  TZOS_RSP_RESERVED = 0x7FFFFFFF
}tzos_command_rsp_type;



typedef struct tzos_command_app_get_id_req_type
{
  char              app_name[32];
} tzos_command_app_get_id_req_t;


/*------------------------------------------------------
  Structure for TZOS app auth cmd request
 ---------------------------------------------------------*/
typedef struct tzos_command_req_type
{
  tzos_command          command;
  void*                 data;
  uint32                appid;
} tzos_command_req_t;

/*------------------------------------------------------
  Structure for TZOS app auth cmd response
 ---------------------------------------------------------*/
typedef struct tzos_command_rsp_type
{
  tzos_command_rsp_status status;
  tzos_command_rsp_type   rsp_type;
  void*                   data;
  uint32                  appid;
} tzos_command_rsp_t;

/*-----------------------------------------------------------
  Structure for TZOS syscall
-------------------------------------------------------------*/
typedef struct tzos_syscall_type
{
  tzbsp_syscall_req_t            common_req;
  tzos_command_req_t             req;
  uint32                         req_len;
  tzbsp_syscall_rsp_t            common_rsp;
  tzos_command_rsp_t             rsp;
  uint32                         rsp_len;
} tzos_syscall_t;

typedef struct tzos_getappname_syscall_type
{
  tzos_syscall_t                 tzossyscall;
  tzos_command_app_get_id_req_t  appid;
} tzos_getappname_syscall_t;


#endif // _TZOS_H
