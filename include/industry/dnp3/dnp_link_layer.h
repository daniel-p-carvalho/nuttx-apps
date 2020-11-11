/*****************************************************************************
 * apps/include/dnp3/dnp_link_layer.h
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

#ifndef __DNP_LINK_LAYER_H
#define __DNP_LINK_LAYER_H

/*****************************************************************************
 * Included Files
 *****************************************************************************/

#include "industry/dnp3/dnp_ll_config.h"

/*****************************************************************************
 * Pre-processor Definitions
 *****************************************************************************/

/*****************************************************************************
 * Public Types
 *****************************************************************************/

struct dnp_link_layer_s
{
  /* Primary station variables */

  bool sec_station_is_reset;  /* Indicates that the primary station sent a
                               * RESET_LINK_STATES frame to the secondary and
                               * received an ACK confirmation back
                               */
  bool nfcb;                  /* State of the next FCB bit to be included when
                               * transmitting a new primary-tosecondary frame
                               * with the FCV bit set
                               */

  /* Secondary station variables */

  bool link_is_reset;         /* Indicates whether the link was reset via
                               * receipt of a RESET_LINK_STATES frame from the
                               * primary station */
  bool efcb;                  /* State of the FCB bit expected in the next
                               * frame from the primary station with the FCV
                               * bit set
                               */

  FAR const struct dnp_ll_config_s *ll_config;
};

#endif /* __DNP_LINK_LAYER_H */
