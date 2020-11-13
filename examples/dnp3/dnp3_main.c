/*****************************************************************************
 * examples/dnp3/dnp3_main.c
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

#include <nuttx/config.h>

#include <stdio.h>
#include <stdlib.h>

#include "industry/dnp3/dnp_link_layer.h"

/*****************************************************************************
 * Private Functions
 *****************************************************************************/

/*****************************************************************************
 * dnp3_main
 *****************************************************************************/

int main(int argc, FAR char *argv[])
{
  FAR struct link_layer_s *link_layer;
  int ret = 0;

  link_layer = link_layer_new();

  ll_reset_link_states(link_layer, false, 1024, 1);

  return ret;
}
