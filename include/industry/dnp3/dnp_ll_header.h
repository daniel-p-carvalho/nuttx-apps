/*****************************************************************************
 * apps/include/dnp3/dnp_ll_header.h
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

#ifndef __DNP_LL_HEADER_H
#define __DNP_LL_HEADER_H

/*****************************************************************************
 * Included Files
 *****************************************************************************/

#include <stdint.h>
#include <stdbool.h>

/*****************************************************************************
 * Pre-processor Definitions
 *****************************************************************************/

/*****************************************************************************
 * Public Type Definitions
 *****************************************************************************/

struct dnp_ll_header_s;

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

uint8_t ll_header_get_len(FAR struct dnp_ll_header_s *header);

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

void ll_header_set_len(FAR struct dnp_ll_header_s *header, uint8_t len);

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

uint16_t ll_header_get_destination(FAR struct dnp_ll_header_s *header);

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
                                  uint16_t dest);

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

uint16_t ll_header_get_source(FAR struct dnp_ll_header_s *header);

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
 *
 *
 *****************************************************************************/

void ll_header_set_source(FAR struct dnp_ll_header_s *header,
                              uint16_t src);

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

uint8_t ll_header_get_control(FAR struct dnp_ll_header_s *header);

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
                              uint8_t control);

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

bool ll_header_is_frommaster(FAR struct dnp_ll_header_s *header);

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

bool ll_header_is_pritosec(FAR struct dnp_ll_header_s *header);

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

void ll_header_write(FAR struct dnp_ll_header_s *header, uint8_t *buff);

#undef EXTERN
#ifdef __cplusplus
}
#endif
#endif /* __ASSEMBLY__ */

#endif /* __DNP_LL_HEADER_H */