/*****************************************************************************
 * apps/dnp3/dnp_link_layer.c
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

#include "industry/dnp3/dnp_link_layer.h"

/*****************************************************************************
 * Pre-processor Definitions
 *****************************************************************************/

#ifdef CONFIG_DNP_MASTER
#  define IS_MASTER true
#else
#  define IS_MASTER false
#endif

#ifdef CONFIG_DNP_USE_CONFIRMATION
#  define USE_CONFIRMATION true
#else
#  define USE_CONFIRMATION false
#endif

#ifndef CONFIG_DNP_RETRIES
#  define CONFIG_DNP_RETRIES 0
#endif

#ifndef CONFIG_DNP_TIMEOUT_MS
#  define CONFIG_DNP_TIMEOUT_MS 1000
#endif

#ifndef CONFIG_DNP_LOCAL_ADDR
#  define CONFIG_DNP_LOCAL_ADDR 1024
#endif

#ifndef CONFIG_DNP_REMOTE_ADDR
#  define CONFIG_DNP_REMOTE_ADDR 1
#endif

/*****************************************************************************
 * Private Data
 *****************************************************************************/

static struct ll_config_s g_ll_config =
{
  .is_master        = IS_MASTER,
  .use_confirmation = USE_CONFIRMATION,
  .retries          = CONFIG_DNP_RETRIES,
  .timeout          = CONFIG_DNP_TIMEOUT_MS,
  .local_addr       = CONFIG_DNP_LOCAL_ADDR,
  .remote_addr      = CONFIG_DNP_REMOTE_ADDR
};

struct link_layer_s g_link_layer =
{
  .sec_station_is_reset = false,
  .nfcb                 = false,
  .link_is_reset        = false,
  .efcb                 = false,

  .ll_config            = &g_ll_config
};

/*****************************************************************************
 * Private Function Prototypes
 *****************************************************************************/

static void reset_nfcb(FAR struct link_layer_s *ll);
static void toggle_nfcb(FAR struct link_layer_s *ll);
static bool next_nfcb(FAR struct link_layer_s *ll);
static void reset_efcb(FAR struct link_layer_s *ll);
static void toggle_efcb(FAR struct link_layer_s *ll);
static bool next_efcb(FAR struct link_layer_s *ll);

static bool validate(FAR struct link_layer_s *ll, bool is_master,
                     uint16_t src, uint16_t dest);

/*****************************************************************************
 * Private Functions
 *****************************************************************************/

// void print_header(FAR struct dnp_ll_header_s *header)
// {
//   _info("START: 0x%x")
//   return header->length;
// }

/*****************************************************************************
 * Name: send_ack
 *****************************************************************************/

static void send_ack(FAR struct link_layer_s *ll)
{
  FAR struct ll_config_s *config = ll->ll_config;
  FAR struct ll_frame_s  *frame  = ll->ll_frame;
  FAR struct ll_frame_ops_s *frame_ops = ll_frame_ops();

  frame_ops->format_ack(frame, config->is_master, false, config->remote_addr,
                        config->local_addr);
}

/*****************************************************************************
 * Name: reset_nfcb
 *****************************************************************************/

static void reset_nfcb(FAR struct link_layer_s *ll)
{
  ll->nfcb = true;
}

/*****************************************************************************
 * Name: toggle_nfcb
 *****************************************************************************/

static void toggle_nfcb(FAR struct link_layer_s *ll)
{
  ll->nfcb = !ll->nfcb;
}

/*****************************************************************************
 * Name: next_nfcb
 *****************************************************************************/

static bool next_nfcb(FAR struct link_layer_s *ll)
{
  return ll->nfcb;
}

/*****************************************************************************
 * Name: reset_efcb
 *****************************************************************************/

static void reset_efcb(FAR struct link_layer_s *ll)
{
  ll->efcb = true;
  ll->link_is_reset = true;
}

/*****************************************************************************
 * Name: toggle_efcb
 *****************************************************************************/

static void toggle_efcb(FAR struct link_layer_s *ll)
{
  ll->efcb = !ll->efcb;
}

/*****************************************************************************
 * Name: next_efcb
 *****************************************************************************/

static bool next_efcb(FAR struct link_layer_s *ll)
{
  return ll->efcb;
}

/*****************************************************************************
 * Name: validate
 *****************************************************************************/

static bool validate(FAR struct link_layer_s *ll, bool is_master,
                     uint16_t src, uint16_t dest)
{
  if (is_master == ll->ll_config->is_master)
    {
      /* Master frame received from master or Slave frame received from
       * slave
       */

      _err("MASTER frame rcvd from MASTER or SLAVE frame rcvd from SLAVE\n");

      return false;
    }

  if (src != ll->ll_config->remote_addr)
    {
      /* Frame received from unknown source */

      _err("Frame received from unknown source\n");

      return false;
    }

  if (src != ll->ll_config->local_addr)
    {
      /* Frame received for unknown destination */

      _err("Frame received for unknown destination\n");

      return false;
    }

  return true;
}

/*****************************************************************************
 * Public Functions
 *****************************************************************************/

/*****************************************************************************
 * Name: link_layer_new
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

struct link_layer_s *link_layer_new(void)
{

  g_link_layer.ll_frame = ll_frame_new();

  return &g_link_layer;
}

/*****************************************************************************
 * Name: dnp_ll_reset_link_states
 *
 * Description:
 *   Synchronizes a secondary station’s states so that it properly processes
 *   primary-to-secondary frames with the FCV bit set.
 *
 * Input Parameters:
 *
 *
 * Returned Value:
 *
 *
 *****************************************************************************/

void ll_reset_link_states(FAR struct link_layer_s *ll, bool is_master,
                              uint16_t src, uint16_t dest)
{
  /* After completing the reset link states transaction, a secondary shall
   * expect the FCB bit to be 1 in the next message with the FCV bit set.
   * Similarly, after completing the reset link states transaction, a primary
   * station shall set the FCB bit to 1 in the next message it sends with the
   * FCV bit set
   */

  /* Check header */

  if (validate(ll, is_master, dest, src))
    {
      /* ISSUE - The FCV bit must be checked. According Table 51 the NFCB bit
       *         is reseted only if FCV bit is 0
       */

      _info("Header is valid.\n");

      reset_efcb(ll);
      //TODO: SEND ACK
    
      return;
    }
  
  _err("Header is NOT valid");
}