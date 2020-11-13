/*****************************************************************************
 * apps/dnp3/dnp_ll_frame.c
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

/*****************************************************************************
 * Included Files
 *****************************************************************************/

#include <assert.h>
#include <debug.h>
#include <stdint.h>
#include <stdbool.h>

#include "industry/dnp3/dnp_ll_frame.h"

/*****************************************************************************
 * Pre-processor Definitions
 *****************************************************************************/

/*****************************************************************************
 * Private Types
 *****************************************************************************/

struct ll_frame_s
{
  struct dnp_ll_header_s *ll_header;

  uint32_t size;
  uint8_t buffer[LS_MAX_FRAME_SIZE];
};

/*****************************************************************************
 * Private Function Prototypes
 *****************************************************************************/

static uint32_t frame_size(uint32_t data_len);

static void format_header(FAR struct ll_frame_s *frame, uint32_t data_len,
                          uint8_t control_field, uint16_t dest, uint16_t src);

static void format_ack(FAR struct ll_frame_s *frame, bool is_master,
                      bool is_rcv_buff_full, uint16_t dest, uint16_t src);
static void format_nack(FAR struct ll_frame_s *frame, bool is_master,
                      bool is_rcv_buff_full, uint16_t dest, uint16_t src);
static void format_linkstatus(FAR struct ll_frame_s *frame,
                      bool is_master, bool is_rcv_buff_full, uint16_t dest,
                      uint16_t src);
static void format_notsupported(FAR struct ll_frame_s *frame,
                      bool is_master, bool is_rcv_buff_full, uint16_t dest,
                      uint16_t src);

static void format_reset_linkstates(FAR struct ll_frame_s *frame,
                      bool is_master, uint16_t dest, uint16_t src);
static void format_test_linkstates(FAR struct ll_frame_s *frame,
                      bool is_master, bool fcb, uint16_t dest, uint16_t src);
static void format_confirmed_userdata(FAR struct ll_frame_s *frame,
                      bool is_master, bool fcb, uint16_t dest, uint16_t src,
                      uint8_t *data, uint32_t data_len);
static void format_unconfirmed_userdata(FAR struct ll_frame_s *frame,
                      bool is_master, uint16_t dest, uint16_t src,
                      uint8_t *data, uint32_t data_len);
static void format_request_linkstatus(FAR struct ll_frame_s *frame,
                      bool is_master, uint16_t dest, uint16_t src);

/*****************************************************************************
 * Private Data
 *****************************************************************************/

static struct ll_frame_s g_ll_frame;

/* Functions for formatting outgoing frames */

static struct ll_frame_ops_s g_ll_frame_ops =
{
  .ll_frame_size = frame_size,

  /* Functions for formatting outgoing frames - Sec to Pri */

  .format_ack           = format_ack,
  .format_nack          = format_nack,
  .format_linkstatus    = format_linkstatus,
  .format_notsupported  = format_notsupported,

  /* Functions for formatting outgoing frames- Pri to Sec */

  .format_reset_linkstates     = format_reset_linkstates,
  .format_test_linkstates      = format_test_linkstates,
  .format_confirmed_userdata   = format_confirmed_userdata,
  .format_unconfirmed_userdata = format_unconfirmed_userdata,
  .format_request_linkstatus   = format_request_linkstatus
};

/*****************************************************************************
 * Private Functions
 *****************************************************************************/

/*****************************************************************************
 * Name: frame_size
 *****************************************************************************/

static uint32_t frame_size(uint32_t data_len)
{
  uint32_t frame_size;
  uint32_t c_blocks;
  uint32_t m16;

  /* data_len must be less then or equal LS_MAX_USER_DATA_SIZE */

  DEBUGASSERT(data_len <= LS_MAX_USER_DATA_SIZE);

  frame_size = LS_HEADER_SIZE;
  if (data_len > 0)
    {
      /* Calc the number of complete blocks and partial block */

      c_blocks = (data_len / LS_DATA_BLOCK_SIZE) * LS_DATA_PLUS_CRC_SIZE;
      frame_size += c_blocks;

      m16 = data_len % LS_DATA_BLOCK_SIZE;
      if (m16)
        {
          frame_size += m16 + LS_CRC_SIZE;
        }
    }

  return frame_size;
}

/*****************************************************************************
 * Name: format_ack
 *****************************************************************************/

static void format_header(FAR struct ll_frame_s *frame, uint32_t data_len,
                          uint8_t ctrl, uint16_t dest, uint16_t src)
{
  FAR struct dnp_ll_header_s *header = frame->ll_header;

  frame->size = frame_size(data_len);

  ll_header_set_len(header, data_len);
  ll_header_set_destination(header, dest);
  ll_header_set_source(header, src);
  ll_header_set_destination(header, ctrl);
}

/*****************************************************************************
 * Name: format_ack
 *****************************************************************************/

static void format_ack(FAR struct ll_frame_s *frame, bool is_master,
                     bool is_rcv_bufffull, uint16_t dest, uint16_t src)
{
  uint8_t control;

  control = LL_CTRL_FC_SEC_ACK;                                 /* FC code */
                                                                /* 0       */
  control |= is_rcv_bufffull ? (1 << LL_CTRL_DIR_BIT_SHIFT) : 0;/* DFC bit */
                                                                /* PRM bit */
  control |= is_master ? (1 << LL_CTRL_DIR_BIT_SHIFT) : 0;      /* DIR bit */
            
  format_header(frame, 0, control, dest, src);
}

/*****************************************************************************
 * Name: format_nack
 *****************************************************************************/

static void format_nack(FAR struct ll_frame_s *frame, bool is_master,
                      bool is_rcv_bufffull, uint16_t dest, uint16_t src)
{
  uint8_t control;

  control = LL_CTRL_FC_SEC_NACK;                                /* FC code */
                                                                /* 0       */
  control |= is_rcv_bufffull ? (1 << LL_CTRL_DIR_BIT_SHIFT) : 0;/* DFC bit */
                                                                /* PRM bit */
  control |= is_master ? (1 << LL_CTRL_DIR_BIT_SHIFT) : 0;      /* DIR bit */
            
  format_header(frame, 0, control, dest, src);
}

/*****************************************************************************
 * Name: format_linkstatus
 *****************************************************************************/

static void format_linkstatus(FAR struct ll_frame_s *frame,
                      bool is_master, bool is_rcv_buff_full, uint16_t dest,
                      uint16_t src)
{
}

/*****************************************************************************
 * Name: format_notsupported
 *****************************************************************************/

static void format_notsupported(FAR struct ll_frame_s *frame,
                      bool is_master, bool is_rcv_buff_full, uint16_t dest,
                      uint16_t src)
{
}

/*****************************************************************************
 * Name: format_reset_linkstates
 *****************************************************************************/

static void format_reset_linkstates(FAR struct ll_frame_s *frame,
                      bool is_master, uint16_t dest, uint16_t src)
{
}

/*****************************************************************************
 * Name: format_test_linkstates
 *****************************************************************************/

static void format_test_linkstates(FAR struct ll_frame_s *frame,
                      bool is_master, bool fcb, uint16_t dest, uint16_t src)
{
}

/*****************************************************************************
 * Name: format_confirmed_userdata
 *****************************************************************************/

static void format_confirmed_userdata(FAR struct ll_frame_s *frame,
                      bool is_master, bool fcb, uint16_t dest, uint16_t src,
                      uint8_t *data, uint32_t data_len)
{
}

/*****************************************************************************
 * Name: format_unconfirmed_userdata
 *****************************************************************************/

static void format_unconfirmed_userdata(FAR struct ll_frame_s *frame,
                      bool is_master, uint16_t dest, uint16_t src,
                      uint8_t *data, uint32_t data_len)
{
}

/*****************************************************************************
 * Name: format_request_linkstatus
 *****************************************************************************/

static void format_request_linkstatus(FAR struct ll_frame_s *frame,
                      bool is_master, uint16_t dest, uint16_t src)
{
}

/*****************************************************************************
 * Public Functions
 *****************************************************************************/

/*****************************************************************************
 * Name: ll_frame_new
 *
 * Description:
 *
 *
 * Input Parameters:
 *
 *
 * Returned Value:
 *
 *
 *****************************************************************************/

struct ll_frame_s *ll_frame_new(void)
{
  /* FIXME - This function is return a pointer to a static reference of 
   *         ll_frame_s. To allow multiple instances of ll_frame_s we should
   *         use dynamic memory allocation
   */
  
  return &g_ll_frame;
}

/*****************************************************************************
 * Name: ll_frame_delete
 *
 * Description:
 *
 *
 * Input Parameters:
 *
 *
 * Returned Value:
 *
 *
 *****************************************************************************/

void ll_frame_delete(FAR struct ll_frame_s *ll_frame)
{
  /* FIXME - We are using static allocation for ll_frame_s so there's nothing
   *         to do.
   */
  
  UNUSED(ll_frame);
}

/*****************************************************************************
 * Name: ll_frame_ops
 *
 * Description:
 *
 *
 * Input Parameters:
 *
 *
 * Returned Value:
 *   Pointer to link layer operations struct
 *
 *****************************************************************************/

struct ll_frame_ops_s *ll_frame_ops(void)
{
  return &g_ll_frame_ops;
}