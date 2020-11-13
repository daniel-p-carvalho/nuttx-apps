/*****************************************************************************
 * apps/include/dnp3/dnp_rtu_priv.h
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

#ifndef __DNP_RTU_PRIV_H
#define __DNP_RTU_PRIV_H

/*****************************************************************************
 * Included Files
 *****************************************************************************/

#include <stdint.h>
#include <termios.h>

/*****************************************************************************
 * Public Types
 *****************************************************************************/

struct dnp_rtu_data_s
{
  int fd;             /* File descriptor */
  char *device;       /* Device: "/dev/ttyS0" */
  int32_t baud;       /* Bauds: 9600, 19200, 57600, 115200, etc */
  uint8_t data_bit;   /* Data bit */
  uint8_t stop_bit;   /* Stop bit */ 
  char parity;        /* Parity: 'N', 'O', 'E' */
  struct termios old_tios;
};

#endif /* __DNP_RTU_PRIV_H */