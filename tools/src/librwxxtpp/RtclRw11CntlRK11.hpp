// $Id: RtclRw11CntlRK11.hpp 887 2017-04-28 19:32:52Z mueller $
//
// Copyright 2013-2017 by Walter F.J. Mueller <W.F.J.Mueller@gsi.de>
//
// This program is free software; you may redistribute and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, either version 2, or at your option any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, without even the implied warranty of MERCHANTABILITY
// or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
// for complete details.
// 
// Revision History: 
// Date         Rev Version  Comment
// 2017-04-16   878   1.2    derive from RtclRw11CntlDiskBase
// 2015-01-03   627   1.1    add local M_stat
// 2013-03-06   495   1.0    Initial version
// 2013-02-08   484   0.1    First draft
// ---------------------------------------------------------------------------


/*!
  \file
  \brief   Declaration of class RtclRw11CntlRK11.
*/

#ifndef included_Retro_RtclRw11CntlRK11
#define included_Retro_RtclRw11CntlRK11 1

#include "RtclRw11CntlDiskBase.hpp"
#include "librw11/Rw11CntlRK11.hpp"

namespace Retro {

  class RtclRw11CntlRK11 : public RtclRw11CntlDiskBase<Rw11CntlRK11> {
    public:
                    RtclRw11CntlRK11();
                   ~RtclRw11CntlRK11();

      virtual int   FactoryCmdConfig(RtclArgs& args, RtclRw11Cpu& cpu);

    protected:
      virtual int   M_stats(RtclArgs& args);
  };
  
} // end namespace Retro

//#include "RtclRw11CntlRK11.ipp"

#endif
