/* Copyright 2020 QMK
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#define CH_CFG_ST_RESOLUTION 16
#define CH_CFG_ST_FREQUENCY 1000
#define CH_CFG_INTERVALS_SIZE 64

#include_next <chconf.h>

#undef CH_CFG_SYSTEM_HALT_HOOK
#define CH_CFG_SYSTEM_HALT_HOOK(reason) {\
  /* System halt code here.*/\
  extern void chibi_system_halt_hook(const char *);\
  chibi_system_halt_hook(reason); }

#undef CH_CFG_TRACE_HOOK
#define CH_CFG_TRACE_HOOK(tep) {\
  /* Trace code here.*/\
  extern void chibi_system_trace_hook(void *);\
  chibi_system_trace_hook(tep); }

