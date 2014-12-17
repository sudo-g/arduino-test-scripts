/**
 * \file BtStackTypes.ino
 * \brief Declares types for use with KFP
 * \author George Xian
 * \version 0.1
 * \date 2014-12-16
 */
 
#define SLIP_ESC 0xDB
#define SLIP_END 0xC0
#define SLIP_ESC_ESC 0xDD
#define SLIP_ESC_END 0xDC 
 
#define KFP_FRAME_SIZE 14	//! No. of data bytes in Killalot frame protocol
#define KFP_WORST_SIZE 26	//! Maximum frame size if escape characters are used

typedef enum {KFPPRINTFORMAT_ASCII, KFPPRINTFORMAT_HEX} KfpPrintFormat;

/**
 * \struct BtStack_Id
 * \brief Allows ID field of KFP to be accessed bytewise or wordwise of Thumb or ARM
 */
typedef union
{
  uint8_t b8[4];		//! bytewise
  uint16_t b16[2];		//! thumb wordwise
  uint32_t b32;			//! ARM wordwise
} BtStack_Id;

/**
 * \struct BtStack_Data
 * \brief Allows payload field of KFP to be accessed bytewise or wordwise of Thumb or ARM
 */
typedef union
{
  uint8_t b8[4];		//! bytewise
  uint16_t b16[2];		//! thumb wordwise
  uint32_t b32;			//! ARM wordwise
} BtStack_Data;

/**
 * \struct BtStack_Frame
 * \brief Represents KFP, allows access as structure or bytestream
 */
typedef union
{
  struct
  {
    BtStack_Id id;		//! 4 bytes ID
    BtStack_Data payload;	//! 8 bytes payload
  };
  uint8_t b8[KFP_FRAME_SIZE-2];	//! bytestream access
} BtStack_Frame;
