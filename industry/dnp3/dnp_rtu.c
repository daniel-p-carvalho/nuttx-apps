/*****************************************************************************
 * apps/dnp3/dnp_rtu.c
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

/*****************************************************************************
 * Public Function Prototypes
 *****************************************************************************/

static int rtu_connect(struct abnt14522_reader_s *reader);
static int rtu_sync(struct abnt14522_reader_s *reader);
static ssize_t rtu_write(struct abnt14522_reader_s *reader, uint32_t len);
static ssize_t rtu_read(struct abnt14522_reader_s *reader, uint8_t *buf,
                        uint32_t len);
static void rtu_close(struct abnt14522_reader_s *reader);
static int rtu_select(struct abnt14522_reader_s *reader, struct timespec *ts);
static void rtu_free(struct abnt14522_reader_s *reader);
