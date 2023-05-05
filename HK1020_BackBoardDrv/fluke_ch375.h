//=============================================================================
#ifndef FLUKE_CH375_H
#define FLUKE_CH375_H

//-- Includes -----------------------------------------------------------------
#include "typedefs.h"

//CH375 command-----------------------------------------------------------------
#define WR_USB_DATA7  0x2B
#define ISSUE_TKN_X   0x4E
#define ISSUE_TOKEN   0x4F
#define RESET_ALL     0x05
#define AUTO_SETUP    0x4D
#define TEST_CONNECT  0x16
#define GET_STATUS    0x22
#define GET_DEV_RATE  0x0A
#define SET_USB_SPEED 0x04
#define RD_USB_DATA0  0x27
#define RD_USB_DATA   0x28
#define SET_BAUDRATE  0x02
#define GET_DESCR     0x46
#define SET_ENDP6     0x1C
#define SET_ENDP7     0x1D
#define SET_USB_MODE  0x15


//CH375 Host Mode-----------------------------------------------------------------
#define Ch375_Mode4   0x04
#define Ch375_Mode5   0x05
#define Ch375_Mode6   0x06
#define Ch375_Mode7   0x07

//Define USB transaction PID -----------------------------------------------------
#define usb_setup  0x01
#define usb_out    0x02
#define usb_in     0x03

//Define USB DESCR TYPE -----------------------------------------------------
#define descr_device  0x01
#define descr_config  0x02


//--------------------------------------------------------------------------------
etError get_oper_status(void);
etError get_int_status(void);
//etError get_connect_status(void);
void CH375_Reset(void);
etError CH375_USB_Mode(u8t mode);
etError USB_dev_autoset(void);
//etError USB_dev_test_connect(void);
//etError set_CH375_baudrate(u8t fre_coefficient,u8t fre_const);
//u8t get_USB_dev_speed_(void);
void wr_host_out_buffer(u8t *fluke_cmd);
void set_host_point_rev(void);
void set_host_point_send(void);
etError issue_token(u8t token_pid);
etError send_cmd_to_fluke(u8t *fluke_command);
etError rev_data_from_CH375(u8t *fluke_data);
etError rev_data_from_Fluke(u8t data_chan[][15]);
void config_fluke_mode(void);
void config_CH375(void);
void Uart4_Send375Cmd(u8t cmd);
void Uart4_Send375Data(u8t dat);
void Uart4_SendFlukeCmd(u8t *datad);

#endif