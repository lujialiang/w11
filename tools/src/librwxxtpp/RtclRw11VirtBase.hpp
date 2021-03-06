// $Id: RtclRw11VirtBase.hpp 887 2017-04-28 19:32:52Z mueller $
//
// Copyright 2017- by Walter F.J. Mueller <W.F.J.Mueller@gsi.de>
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
// 2017-03-11   859   1.0    Initial version
// ---------------------------------------------------------------------------


/*!
  \file
  \brief   Declaration of class RtclRw11VirtBase.
*/

#ifndef included_Retro_RtclRw11VirtBase
#define included_Retro_RtclRw11VirtBase 1

#include "RtclRw11Virt.hpp"

namespace Retro {

  template <class TO>
  class RtclRw11VirtBase : public RtclRw11Virt {
    public:
                    RtclRw11VirtBase(TO* pobj);
                   ~RtclRw11VirtBase();

      TO&           Obj();

    protected:
      TO*           fpObj;                 //!< ptr to object
  };
  
} // end namespace Retro

// implementation is all inline
#include "RtclRw11VirtBase.ipp"

#endif
