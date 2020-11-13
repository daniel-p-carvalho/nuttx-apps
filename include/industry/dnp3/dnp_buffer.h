/*****************************************************************************
 * apps/include/dnp3/dnp_buffer.h
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

#ifndef __DNP_BUFFER_H
#define __DNP_BUFFER_H

/*****************************************************************************
 * Included Files
 *****************************************************************************/

#include <stdint.h>
#include <stdbool.h>

/*****************************************************************************
 * Pre-processor Definitions
 *****************************************************************************/

/*****************************************************************************
 * Public Types
 *****************************************************************************/

struct dnp_buffer_s
{
  uint8_t *data;
  uint32_t rd_pos;
  uint32_t wr_pos;
  uint32_t length;
  size_t size;
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
 * Name: dnp_buff_sync
 *
 * Description:
 *   Searches in buffer for a pattern. If a match is found the read index is
 *   advanced to the beginning of pattern sequence. If a partial match is
 *   found the read index is moved to the beginning of the match and the
 *   partial match is preserved. If no match is found the read index is
 *   advanced to the end of the read subsequence.
 *
 * Input Parameters:
 *   *pattern     - pointer to first byte in the input pattern buffer
 *   *pattern_len - number of bytes in pattern
 * 
 * Returned Value:
 *
 *
 *****************************************************************************/

bool dnp_buff_sync(uint8_t *pattern, size_t pattern_len);

/*****************************************************************************
 * Name: dnp_buff_delete
 *
 * Description:
 *
 * Input Parameters:
 *   *dnp_buff -
 * 
 * Returned Value:
 *
 *
 *****************************************************************************/

void dnp_buff_delete(FAR struct dnp_buffer_s *dnp_buff);

/*****************************************************************************
 * Name: dnp_buff_new
 *
 * Description:
 *
 * Input Parameters:
 *   *dnp_buff -
 * 
 * Returned Value:
 *
 *
 *****************************************************************************/

struct dnp_buffer_s *dnp_buff_new(uint8_t *data, size_t size);

#undef EXTERN
#ifdef __cplusplus
}
#endif
#endif /* __ASSEMBLY__ */

#endif /* __DNP_BUFFER_H */
