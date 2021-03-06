// $Id: RtclRw11Virt.cpp 887 2017-04-28 19:32:52Z mueller $
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
// 2017-04-07   868   1.0.1  M_dump: use GetArgsDump and Dump detail
// 2017-03-11   859   1.0    Initial version
// ---------------------------------------------------------------------------

/*!
  \file
  \brief   Implemenation of RtclRw11Virt.
*/

#include "librtcltools/RtclStats.hpp"

#include "RtclRw11Virt.hpp"

using namespace std;

/*!
  \class Retro::RtclRw11Virt
  \brief FIXME_docs
*/


// all method definitions in namespace Retro
namespace Retro {

//------------------------------------------+-----------------------------------
//! Constructor

RtclRw11Virt::RtclRw11Virt(Rw11Virt* pvirt)
  : RtclCmdBase(),
    fpVirt(pvirt),
    fGets(),
    fSets()
{
  AddMeth("get",    boost::bind(&RtclRw11Virt::M_get,   this, _1));
  AddMeth("set",    boost::bind(&RtclRw11Virt::M_set,   this, _1));
  AddMeth("stats",  boost::bind(&RtclRw11Virt::M_stats, this, _1));
  AddMeth("dump",   boost::bind(&RtclRw11Virt::M_dump,  this, _1));
}

//------------------------------------------+-----------------------------------
//! FIXME_docs

RtclRw11Virt::~RtclRw11Virt()
{}

//------------------------------------------+-----------------------------------
//! FIXME_docs

int RtclRw11Virt::M_get(RtclArgs& args)
{
  // synchronize with server thread
  boost::lock_guard<RlinkConnect> lock(Virt()->Cpu().Connect());
  return fGets.M_get(args);
}

//------------------------------------------+-----------------------------------
//! FIXME_docs

int RtclRw11Virt::M_set(RtclArgs& args)
{
  // synchronize with server thread
  boost::lock_guard<RlinkConnect> lock(Virt()->Cpu().Connect());
  return fSets.M_set(args);
}

//------------------------------------------+-----------------------------------
//! FIXME_docs

int RtclRw11Virt::M_stats(RtclArgs& args)
{
  RtclStats::Context cntx;
  if (!RtclStats::GetArgs(args, cntx)) return kERR;
  if (!RtclStats::Collect(args, cntx, Virt()->Stats())) return kERR;
  return kOK;
}

//------------------------------------------+-----------------------------------
//! FIXME_docs

int RtclRw11Virt::M_dump(RtclArgs& args)
{
  int detail=0;
  if (!GetArgsDump(args, detail)) return kERR;
  if (!args.AllDone()) return kERR;

  std::ostringstream sos;
  Virt()->Dump(sos, 0, "", detail);
  args.SetResult(sos);
  return kOK;
}

} // end namespace Retro
