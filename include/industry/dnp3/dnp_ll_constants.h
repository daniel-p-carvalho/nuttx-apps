/*****************************************************************************
 * apps/include/dnp3/dnp_ll_constants.h
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 *****************************************************************************/

#ifndef __DNP_LL_CONSTANTS_H
#define __DNP_LL_CONSTANTS_H

/*****************************************************************************
 * Included Files
 *****************************************************************************/

/*****************************************************************************
 * Pre-processor Definitions
 *****************************************************************************/

/* Link layer frame format
 *
 * Header block 0 - Link layer header
 *
 *    |----------------------------------------------------------------|
 *    |    START    | LEN | CTRL | DESTINATION |  SOURCE   |   CRC16   |
 *    | 0X05 | 0X64 |     |      |  MSB | LSB  | MSB | LSB | MSB | LSB |
 *    |----------------------------------------------------------------|
 *        0     1      2      3      4     5      6     7     8     9
 *
 * Header block 1-N - User data
 *
 *    |----------------------------------------------------------------|
 *    |               USER DATA (16 OCTETS)                |   CRC16   |
 *    |                                                    | MSB | LSB |
 *    |----------------------------------------------------------------|
 *
 */

/* Size of link layer frame fields */

#define LS_MIN_LENGTH           5
#define LS_MAX_LENGTH           255
#define LS_HEADER_SIZE          10
#define LS_DATA_BLOCK_SIZE      16
#define LS_CRC_SIZE             2
#define LS_DATA_PLUS_CRC_SIZE   18
#define LS_MAX_USER_DATA_SIZE   250
#define LS_MAX_FRAME_SIZE       292 /* HEADER + USER DATA + CRC BLOCKs */

/* Index of link layer frame fields */

#define LI_START_05             0
#define LI_START_64             1
#define LI_LENGTH               2
#define LI_CONTROL              3
#define LI_DESTINATION          4
#define LI_SOURCE               6
#define LI_CRC                  8

/* CONTROL field bit definitions
 *
 *    |---------------------------------------|
 *    |     |     | FCB | FCV |               |
 *    | DIR | PRM |-----|-----| FUNCTION CODE |
 *    |     |     |  0  | DFC |               |
 *    |---------------------------------------|
 *       7     6     5     4    3   2   1   0
 */

#define LL_CTRL_DIR_BIT_SHIFT    7
#define LL_CTRL_PRM_BIT_SHIFT    6
#define LL_CTRL_FCB_BIT_SHIFT    5
#define LL_CTRL_FCV_BIT_SHIFT    4
#define LL_CTRL_DFC_BIT_SHIFT    4
#define LL_CTRL_FUNC_BIT_SHIFT   0

#define MASK_DIR    0x80
#define MASK_PRM    0x40
#define MASK_FCB    0x20
#define MASK_FCV    0x10
#define MASK_FUNC   0x0f

/*****************************************************************************
 * Public Types
 *****************************************************************************/

/* Link layer function codes
 *
 * For simplicity the PRM bit is included to make the Pri-To-Sec and
 * Sec-To-Pri codes non-overlapping.
 */

enum ll_function_code_e
{
  /* PRM = 0 + Funtion code */

  LL_CTRL_FC_SEC_ACK              = 0x00,
  LL_CTRL_FC_SEC_NACK             = 0x01,
  LL_CTRL_FC_SEC_LINK_STATUS      = 0x0b,
  LL_CTRL_FC_SEC_NOT_SUPPORTED    = 0x0f,

  /* PRM = 1 + Funtion code */

  LL_CTRL_FC_PRI_RESET_LINK_STATES     = 0x40,
  LL_CTRL_FC_PRI_TEST_LINK_STATES      = 0x42,
  LL_CTRL_FC_PRI_CONFIRMED_USER_DATA   = 0x43,
  LL_CTRL_FC_PRI_UNCONFIRMED_USER_DATA = 0x44,
  LL_CTRL_FC_PRI_REQUEST_LINK_STATUS   = 0x49
};

#endif /* __DNP_LL_CONSTANTS_H */