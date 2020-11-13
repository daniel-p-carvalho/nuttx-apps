/*****************************************************************************
 * apps/dnp3/dnp_buffer.c
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

#include "industry/dnp3/dnp_buffer.h"

/*****************************************************************************
* Pre-processor Definitions
 *****************************************************************************/

/*****************************************************************************
 * Private Types
 *****************************************************************************/

/*****************************************************************************
 * Private Data
 *****************************************************************************/

static struct dnp_buffer_s g_dnp_buffer;

/*****************************************************************************
 * Private Function Prototypes
 *****************************************************************************/

/*****************************************************************************
 * Private Functions
 *****************************************************************************/

/*****************************************************************************
 * Public Functions
 *****************************************************************************/

uint32_t dnp_buff_write(FAR struct dnp_buffer_s *buff, uint8_t *data,
                        uint32_t data_len)
{
  uint32_t idx;
  uint32_t count;
  uint32_t len;

  len = buffer->length;
  if (data_len > (buff->size + 1 - len))
    {
      data_len = buff->size + 1 - len;
    }

  for (count = 0; count < data_len; count++)
    {
      idx = (buff->data[buff->wr_pos] + buff->data[buff->wr_pos]) &
             buff->size;

      buff->data[idx] = *data++;
      buff->wr_pos++;
    }
  buff->length += data_len;

  return data_len;
}

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
 *****************************************************************************/

bool dnp_buff_sync(FAR struct dnp_buffer_s *buff, uint8_t *pattern,
                   uint32_t pattern_len)
{
  uint8_t *rd_ptr;
  uint32_t read_bytes;
  uint32_t i, j, k, lim;

  /* Some consistency tests */

  if (pattern_len < 1)
    {
      _err("Pattern must have at least one byte\n");
      return false;
    }

  if (pattern_len > dnp_buff->size)
    {
      _err("Pattern is greater than buffer size\n");
      return false;
    }

  /* Get the read pointer and the number o bytes in buffer */

  rd_ptr = buff->data + buff->rd_pos;
  read_bytes = buff->wr_pos - buff->rd_pos;

  for (i = 0; i < read_bytes; i++)
    {
      k = i;
      lim = read_bytes - i;
      lim = lim >= pattern_len ? pattern_len : lim - 1;
      for (j = 0; j < lim; j++)
        {
          if (pattern[j] == rd_ptr[k])
            {
              k++;
            }
          else
            {
              break;
            }
        }
      if (j == pattern_len)
        {
          /* Full pattern found. Advance read index to the beginning of 
           * pattern 
           */

          return true;
        }
    }
  
  /* If j is greater than zero we have a partial match at the end of */

  return false;
}

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
 *****************************************************************************/

void dnp_buff_delete(FAR struct dnp_buffer_s *dnp_buff)
{
  /* FIXME - We are using static allocation for ll_frame_s so there's nothing
   *         to do.
   */

  UNUSED(dnp_buff);
}

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
 *****************************************************************************/

struct dnp_buffer_s *dnp_buff_new(uint8_t *data, size_t size)
{
  /* FIXME - This function is return a pointer to a static reference of 
   *         ll_frame_s. To allow multiple instances of ll_frame_s we should
   *         use dynamic memory allocation
   */

  FAR dnp_buffer_s dnp_buffer = &g_dnp_buffer;

  dnp_buffer->data = data;
  dnp_buffer->rd_pos = 0;
  dnp_buffer->wr_pos = 0;
  dnp_buffer->length = 0;
  dnp_buffer->size = size - 1;

  return dnp_buffer;
}
