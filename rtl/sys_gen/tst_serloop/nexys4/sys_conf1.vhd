-- $Id: sys_conf1.vhd 751 2016-03-25 19:46:11Z mueller $
--
-- Copyright 2015-2016 by Walter F.J. Mueller <W.F.J.Mueller@gsi.de>
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
-- Description:    Definitions for sys_tst_serloop1_n4 (for synthesis)
--
-- Dependencies:   -
-- Tool versions:  viv 2014.4-2015.4; ghdl 0.31-0.33
-- Revision History: 
-- Date         Rev Version  Comment
-- 2016-03-25   751   1.1    clock now from cmt and configurable
-- 2015-02-01   641   1.0    Initial version
------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;

use work.slvtypes.all;

package sys_conf is

  -- configure clocks --------------------------------------------------------
  constant sys_conf_clksys_vcodivide   : positive :=   1;
  constant sys_conf_clksys_vcomultiply : positive :=  12;   -- vco 1200 MHz
  constant sys_conf_clksys_outdivide   : positive :=  10;   -- sys  120 MHz
  constant sys_conf_clksys_gentype     : string   := "MMCM";
  
  constant sys_conf_clkdiv_usecdiv : integer  :=  100; -- default usec 
  constant sys_conf_clkdiv_msecdiv  : integer := 1000; -- default msec

  -- configure hio interfaces -----------------------------------------------
  constant sys_conf_hio_debounce : boolean := true;   -- instantiate debouncers

  -- configure serport ------------------------------------------------------  
  constant sys_conf_uart_defbaud : integer := 115200;   -- default 115k baud

  -- derived constants =======================================================
  constant sys_conf_clksys : integer :=
    ((100000000/sys_conf_clksys_vcodivide)*sys_conf_clksys_vcomultiply) /
    sys_conf_clksys_outdivide;
  constant sys_conf_clksys_mhz : integer := sys_conf_clksys/1000000;

  constant sys_conf_uart_cdinit : integer :=
    (sys_conf_clksys/sys_conf_uart_defbaud)-1;
  
end package sys_conf;
