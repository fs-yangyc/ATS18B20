//=============================================================================

#ifndef TYPEDEFS_H
#define TYPEDEFS_H

//=============================================================================
// basic types: making the size of types clear
//=============================================================================
typedef unsigned char   u8t;      ///< range: 0 .. 255
typedef signed char     i8t;      ///< range: -128 .. +127
                                      
typedef unsigned int    u16t;     ///< range: 0 .. 65535
typedef signed int      i16t;     ///< range: -32768 .. +32767
                                      
typedef unsigned long   u32t;     ///< range: 0 .. 4'294'967'295
typedef signed long     i32t;     ///< range: -2'147'483'648 .. +2'147'483'647
                                      
typedef float           ft;       ///< range: +-1.18E-38 .. +-3.39E+38
typedef double          dt;       ///< range:            .. +-1.79E+308

// Error codes
typedef enum{
  NO_ERROR         = 0x00, // no error
  OPER_ERROR       = 0x01, // operrtion error
  DISCONNECT_ERROR = 0x02, // disconnect 
  INT_ERROR        = 0x03, // initerrupt error
  TIMEOUT_ERROR    = 0x04, // timeout error
  TOKEN_ERROR      = 0x05, // issue	token error
  REV_DATA_ERROR   = 0x06, // receive data error
  ELSE_ERROR       = 0x80, // else error
}etError;

#endif
