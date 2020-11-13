/*****************************************************************************
 * apps/include/dnp3/dnp_crc16.h
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

#ifndef __DNP_CRC16_H
#define __DNP_CRC16_H

/*****************************************************************************
 * Included Files
 *****************************************************************************/

#include <stdint.h>

/*****************************************************************************
 * Public Types
 *****************************************************************************/

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
 * Name: dnp_crc16_calc
 *
 * Description:
 *   Calculates the two octet CRC for the data passed to it.
 *
 * Input Parameters:
 *   *buff * - pointer to data buffer
 *   len     - length of data
 *
 * Returned Value:
 *   CRC16
 *
 *****************************************************************************/

uint16_t dnp_crc16_calc(uint8_t *buff, uint32_t len);

/*****************************************************************************
 * Name: dnp_crc16_add
 *
 * Description:
 *   Calculates the two octet CRC for the data and append the CRC to it.
 *
 * Input Parameters:
 *   *buff - pointer to data buffer
 *   len   - length of data
 *
 * Returned Value:
 *
 *
 *****************************************************************************/

void dnp_crc16_add(uint8_t *buff, uint32_t len);

/*****************************************************************************
 * Name: dnp_crc16_check
 *
 * Description:
 *   Check if the CRC16 of data passed to it. This function assumes that the
 *   last two octets of data passed to it is the CRC for the preceding data.
 *   The CRC for the preceding data is calculated and compared to the final
 *   two octets.
 *
 * Input Parameters:
 *   *buff - pointer to data buffer
 *   len   - length of data
 *
 * Returned Value:
 *
 *
 *****************************************************************************/

bool dnp_crc16_check(uint8_t *buff, uint32_t len);

#undef EXTERN
#ifdef __cplusplus
}
#endif
#endif /* __ASSEMBLY__ */

#endif /* __DNP_RTU_H */
