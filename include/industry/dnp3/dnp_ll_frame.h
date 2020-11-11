/*****************************************************************************
 * apps/include/dnp3/dnp_ll_frame.h
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

#ifndef __DNP_LL_FRAME_H
#define __DNP_LL_FRAME_H

/*****************************************************************************
 * Included Files
 *****************************************************************************/

#include <stdint.h>
#include <stdbool.h>

#include "industry/dnp3/dnp_ll_header.h"

/*****************************************************************************
 * Pre-processor Definitions
 *****************************************************************************/

/*****************************************************************************
 * Public Types
 *****************************************************************************/

struct dnp_ll_frame_s
{
  struct dnp_ll_header_s *ll_header;

  uint32_t size;
  uint8_t buffer[LS_MAX_FRAME_SIZE];
};

struct dnp_ll_frame_ops_s
{
  /* Functions for formatting outgoing frames - Sec to Pri */

  void (*format_ack)(FAR struct dnp_ll_frame_s *frame, bool is_master,
                     bool is_rcv_buff_full, uint16_t dest, uint16_t src);

  void (*format_nack)(FAR struct dnp_ll_frame_s *frame, bool is_master,
                      bool is_rcv_buff_full, uint16_t dest, uint16_t src);

  void (*format_linkstatus)(FAR struct dnp_ll_frame_s *frame, bool is_master,
                      bool is_rcv_buff_full, uint16_t dest, uint16_t src);

  void (*format_notsupported)(FAR struct dnp_ll_frame_s *frame,
                              bool is_master, bool is_rcv_buff_full,
                              uint16_t dest, uint16_t src);

  /* Functions for formatting outgoing frames- Pri to Sec */

  void (*format_reset_linkstates)(FAR struct dnp_ll_frame_s *frame,
                      bool is_master, uint16_t dest, uint16_t src);

  void (*format_test_linkstates)(FAR struct dnp_ll_frame_s *frame,
                      bool is_master, bool fcb, uint16_t dest, uint16_t src);

  void (*format_confirmed_userdata)(FAR struct dnp_ll_frame_s *frame,
                      bool is_master, bool fcb, uint16_t dest, uint16_t src,
                      uint8_t *data, uint32_t data_len);

  void (*format_unconfirmed_userdata)(FAR struct dnp_ll_frame_s *frame,
                      bool is_master, uint16_t dest, uint16_t src,
                      uint8_t *data, uint32_t data_len);

  void (*format_request_linkstatus)(FAR struct dnp_ll_frame_s *frame,
                      bool is_master, uint16_t dest, uint16_t src);
};

/*****************************************************************************
 * Public Function Prototypes
 *****************************************************************************/

#ifndef __ASSEMBLY__
#ifdef __cplusplus
#define EXTERN extern "C"
extern "C"
{
#else
#define EXTERN extern
#endif

/*****************************************************************************
 * Name: ll_frame_size
 *
 * Description:
 *   Writes 10 header bytes to buffer including 0x0564, all fields, and CRC
 *
 * Input Parameters:
 *   data_len - Length of user data
 *
 * Returned Value:
 *   Size of frame
 *
 *****************************************************************************/

uint32_t ll_frame_size(uint32_t data_len);

#undef EXTERN
#ifdef __cplusplus
}
#endif
#endif /* __ASSEMBLY__ */

#endif /* __DNP_LL_FRAME_H */