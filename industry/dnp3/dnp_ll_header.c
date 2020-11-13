/*****************************************************************************
 * apps/dnp3/dnp_ll_header.c
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

#include <stdint.h>
#include <stdbool.h>

#include "industry/dnp3/dnp_crc16.h"
#include "industry/dnp3/dnp_ll_constants.h"
#include "industry/dnp3/dnp_ll_header.h"

/*****************************************************************************
 * Pre-processor Definitions
 *****************************************************************************/

/*****************************************************************************
 * Private Types
 *****************************************************************************/

/* Fields read from header */

struct dnp_ll_header_s
{
  uint8_t length;   /* Specifies the count of non-CRC octets that follow in
                     * the header and data blocks. Mininum values is 5 and
                     * maximum value is 255
                     */
  uint16_t dest;    /* Station to which the frame is directed */
  uint16_t src;     /* Station from where the frame originates */
  uint8_t ctrl;     /* Contains information about the frame’s direction,
                     * transaction initiator, error and flow control, and
                     * function
                     */
};

/*****************************************************************************
 * Private Function Prototypes
 *****************************************************************************/

/*****************************************************************************
 * Private Data
 *****************************************************************************/

/*****************************************************************************
 * Private Functions
 *****************************************************************************/

/*****************************************************************************
 * Public Functions
 *****************************************************************************/

/*****************************************************************************
 * Name: ll_header_get_len
 *
 * Description:
 *   This function returns the count of non-CRC octets that follow in the
 *   header and data blocks.
 *
 * Input Parameters:
 *   *header -
 *
 * Returned Value:
 *   Count of non-CRC octets that follow in the header and data blocks
 *
 *****************************************************************************/

uint8_t ll_header_get_len(FAR struct dnp_ll_header_s *header)
{
  return header->length;
}

/*****************************************************************************
 * Name: ll_header_set_len
 *
 * Description:
 *   This function sets the count of non-CRC octets that follow in the
 *   header and data blocks.
 *
 * Input Parameters:
 *   *header -
 *   len     -
 *
 * Returned Value:
 *
 *
 *****************************************************************************/

void ll_header_set_len(FAR struct dnp_ll_header_s *header, uint8_t len)
{
  header->length = len;
}

/*****************************************************************************
 * Name: ll_header_get_destination
 *
 * Description:
 *   This function returns the address of the station to which the frame is
 *   directed.
 *
 * Input Parameters:
 *   *header -
 *
 * Returned Value:
 *   Destination address
 *
 *****************************************************************************/

uint16_t ll_header_get_destination(FAR struct dnp_ll_header_s *header)
{
  return header->dest;
}

/*****************************************************************************
 * Name: ll_header_set_destination
 *
 * Description:
 *   This function sets the address of the station to which the frame is
 *   directed.
 *
 * Input Parameters:
 *   *header -
 *   dest    -
 *
 * Returned Value:
 *   Destination address
 *
 *****************************************************************************/

void ll_header_set_destination(FAR struct dnp_ll_header_s *header,
                                  uint16_t dest)
{
  header->dest = dest;
}

/*****************************************************************************
 * Name: ll_header_get_source
 *
 * Description:
 *   This function returns the address of the station from where the frame
 *   originates.
 *
 * Input Parameters:
 *   *header -
 *
 * Returned Value:
 *   Source address
 *
 *****************************************************************************/

uint16_t ll_header_get_source(FAR struct dnp_ll_header_s *header)
{
  return header->src;
}

/*****************************************************************************
 * Name: ll_header_set_source
 *
 * Description:
 *   This function sets the address of the station from where the frame
 *   originates.
 *
 * Input Parameters:
 *   *header -
 *
 * Returned Value:
 *   Source address
 *
 *****************************************************************************/

void ll_header_set_source(FAR struct dnp_ll_header_s *header,
                              uint16_t src)
{
  header->src = src;
}

/*****************************************************************************
 * Name: ll_header_get_control
 *
 * Description:
 *   This function returns the control field.
 *
 * Input Parameters:
 *   *header -
 *
 * Returned Value:
 *   Control field
 *
 *****************************************************************************/

uint8_t ll_header_get_control(FAR struct dnp_ll_header_s *header)
{
  return header->ctrl;
}

/*****************************************************************************
 * Name: ll_header_set_control
 *
 * Description:
 *   This function sets the control field.
 *
 * Input Parameters:
 *   *header -
 *
 * Returned Value:
 *
 *
 *****************************************************************************/

void ll_header_set_control(FAR struct dnp_ll_header_s *header,
                              uint8_t control)
{
}

/*****************************************************************************
 * Name: ll_header_is_frommaster
 *
 * Description:
 *
 *
 * Input Parameters:
 *   *header -
 *
 * Returned Value:
 *
 *
 *****************************************************************************/

bool ll_header_is_frommaster(FAR struct dnp_ll_header_s *header)
{
  return (header->ctrl & MASK_DIR) != 0;
}

/*****************************************************************************
 * Name: ll_header_is_pritosec
 *
 * Description:
 *
 *
 * Input Parameters:
 *   *header -
 *
 * Returned Value:
 *
 *
 *****************************************************************************/

bool ll_header_is_pritosec(FAR struct dnp_ll_header_s *header)
{
  return (header->ctrl & MASK_PRM) != 0;
}

/*****************************************************************************
 * Name: ll_header_write
 *
 * Description:
 *   This function writes the header to buffer, including the START field and
 *   CRC16.
 *
 * Input Parameters:
 *   *header -
 *   *buff   - pointer to the buffer where the header will be writen. At least
 *             10 bytes.
 *
 * Returned Value:
 *
 *
 *****************************************************************************/

void ll_header_write(FAR struct dnp_ll_header_s *header, uint8_t *buff)
{
  buff[LI_START_05] = 0x05;
  buff[LI_START_64] = 0x64;

  buff[LI_LENGTH] = header->length;
  buff[LI_DESTINATION] = (uint8_t)(header->dest & 0xff);
  buff[LI_DESTINATION + 1] = (uint8_t)((header->dest >> 8) & 0xff);
  buff[LI_SOURCE] = (uint8_t)(header->src & 0xff);
  buff[LI_SOURCE + 1] = (uint8_t)((header->src >> 8) & 0xff);

  crc16_add(buff, LI_CRC);
}
