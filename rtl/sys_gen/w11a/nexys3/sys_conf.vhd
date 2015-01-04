-- $Id: sys_conf.vhd 621 2014-12-26 21:20:05Z mueller $
--
-- Copyright 2011-2014 by Walter F.J. Mueller <W.F.J.Mueller@gsi.de>
--
-- This program is free software; you may redistribute and/or modify it under
-- the terms of the GNU General Public License as published by the Free
-- Software Foundation, either version 2, or at your option any later version.
--
-- This program is distributed in the hope that it will be useful, but
-- WITHOUT ANY WARRANTY, without even the implied warranty of MERCHANTABILITY
-- or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
-- for complete details.
--
------------------------------------------------------------------------------
-- Package Name:   sys_conf
-- Description:    Definitions for sys_w11a_n3 (for synthesis)
--
-- Dependencies:   -
-- Tool versions:  xst 13.1-14.7; ghdl 0.29-0.31
-- Revision History: 
-- Date         Rev Version  Comment
-- 2014-12-26   621   1.2.2  use 68 MHz, get occasional problems with 72 MHz
-- 2014-12-22   619   1.2.1  add _rbmon_awidth
-- 2013-10-06   538   1.2    pll support, use clksys_vcodivide ect
-- 2013-10-05   537   1.1.1  use 72 MHz, no closure w/ ISE 14.x for 80 anymore
-- 2013-04-21   509   1.1    add fx2 settings
-- 2011-11-26   433   1.0.1  use 80 MHz clksys (no closure for 85 after rev 432)
-- 2011-11-20   430   1.0    Initial version (derived from _n2 version)
------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;

use work.slvtypes.all;

-- valid system clock / delay combinations (see n2_cram_memctl_as.vhd):
--  div mul  clksys  read0 read1 write
--    2   1   50.0     2     2     3
--    4   3   75.0     4     4     5   (also 70 MHz)
--    5   4   80.0     5     5     5  
--   20  17   85.0     5     5     6  
--   10   9   90.0     6     6     6   (also 95 MHz)
--    1   1  100.0     6     6     7  

package sys_conf is

  constant sys_conf_clksys_vcodivide   : positive :=  25;
  constant sys_conf_clksys_vcomultiply : positive :=  17;   -- dcm   68 MHz
  constant sys_conf_clksys_outdivide   : positive :=   1;   -- sys   68 MHz
  constant sys_conf_clksys_gentype     : string   := "DCM";
  
  constant sys_conf_memctl_read0delay : positive := 4;
  constant sys_conf_memctl_read1delay : positive := sys_conf_memctl_read0delay;
  constant sys_conf_memctl_writedelay : positive := 5;

  constant sys_conf_ser2rri_defbaud : integer := 115200;   -- default 115k baud

  constant sys_conf_rbmon_awidth : integer := 9; -- use 0 to disable rbmon

  -- fx2 settings: petowidth=10 -> 2^10 30 MHz clocks -> ~33 usec
  constant sys_conf_fx2_petowidth  : positive := 10;
  constant sys_conf_fx2_ccwidth  : positive := 5;

  constant sys_conf_hio_debounce : boolean := true;    -- instantiate debouncers

  constant sys_conf_bram           : integer :=  0;      -- no bram, use cache
  constant sys_conf_bram_awidth    : integer := 14;      -- bram size (16 kB)
  constant sys_conf_mem_losize     : integer := 8#167777#; --   4 MByte
--constant sys_conf_mem_losize     : integer := 8#003777#; -- 128 kByte (debug)

--  constant sys_conf_bram           : integer :=  1;      --  bram only 
--  constant sys_conf_bram_awidth    : integer := 15;      -- bram size (32 kB)
--  constant sys_conf_mem_losize     : integer := 8#000777#; -- 32 kByte
  
  constant sys_conf_cache_fmiss    : slbit   := '0';     -- cache enabled

  -- derived constants

  constant sys_conf_clksys : integer :=
    ((100000000/sys_conf_clksys_vcodivide)*sys_conf_clksys_vcomultiply) /
    sys_conf_clksys_outdivide;
  constant sys_conf_clksys_mhz : integer := sys_conf_clksys/1000000;

  constant sys_conf_ser2rri_cdinit : integer :=
    (sys_conf_clksys/sys_conf_ser2rri_defbaud)-1;
  
end package sys_conf;

-- Note: mem_losize holds 16 MSB of the PA of the addressable memory
--        2 211 111 111 110 000 000 000
--        1 098 765 432 109 876 543 210
--
--        0 000 000 011 111 111 000 000  -> 00037777  --> 14bit -->  16 kByte
--        0 000 000 111 111 111 000 000  -> 00077777  --> 15bit -->  32 kByte
--        0 000 001 111 111 111 000 000  -> 00177777  --> 16bit -->  64 kByte
--        0 000 011 111 111 111 000 000  -> 00377777  --> 17bit --> 128 kByte
--        0 011 111 111 111 111 000 000  -> 03777777  --> 20bit -->   1 MByte
--        1 110 111 111 111 111 000 000  -> 16777777  --> 22bit -->   4 MByte
--                                          upper 256 kB excluded for 11/70 UB
