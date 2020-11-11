/*****************************************************************************
 * apps/include/dnp3/dnp_ll_config.h
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

#ifndef __DNP_LL_CONFIG_H
#define __DNP_LL_CONFIG_H

/*****************************************************************************
 * Included Files
 *****************************************************************************/

#include <stdint.h>
#include <stdbool.h>

/*****************************************************************************
 * Public Types
 *****************************************************************************/

struct dnp_ll_config_s
{
  bool is_master;         /* If true, master/slave bit is set on all
                           * messages
                           */
  bool use_confirmation;  /* If true, the link layer will send data
                           * requesting confirmation
                           */
  uint32_t retries;       /* Number of retries that link layer will attempt
                           * before fail
                           */
  uint32_t timeout;       /* Response timeout in milliseconds for confirmed
                           * requests
                           */
  uint16_t local_addr;    /* Local address is the address of the outstation */
  uint16_t remote_addr;   /* Remote address is an address for DNP3 master to
                           * which outstation reports
                           */
};

#endif /* __DNP_LL_CONFIG_H */