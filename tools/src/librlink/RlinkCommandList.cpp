// $Id: RlinkCommandList.cpp 606 2014-11-24 07:08:51Z mueller $
//
// Copyright 2011-2014 by Walter F.J. Mueller <W.F.J.Mueller@gsi.de>
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
// 2014-11-23   606   1.2    new rlink v4 iface
// 2014-08-02   576   1.1    rename LastExpect->SetLastExpect
// 2013-05-06   495   1.0.3  add RlinkContext to Print() args
// 2013-02-03   481   1.0.2  use Rexception
// 2011-04-25   380   1.0.1  use boost/foreach
// 2011-03-05   366   1.0    Initial version
// 2011-01-15   355   0.1    First draft
// ---------------------------------------------------------------------------

/*!
  \file
  \version $Id: RlinkCommandList.cpp 606 2014-11-24 07:08:51Z mueller $
  \brief   Implemenation of class RlinkCommandList.
 */

#include <string>

#include "boost/foreach.hpp"
#define foreach_ BOOST_FOREACH

#include "RlinkCommandList.hpp"

#include "librtools/RosPrintf.hpp"
#include "librtools/RosFill.hpp"
#include "librtools/Rexception.hpp"

using namespace std;

/*!
  \class Retro::RlinkCommandList
  \brief FIXME_docs
*/

// all method definitions in namespace Retro
namespace Retro {

//------------------------------------------+-----------------------------------
//! Default constructor

RlinkCommandList::RlinkCommandList()
  : fList(),
    fLaboIndex(-1)
{
  fList.reserve(16);                        // should prevent most re-alloc's
}

//------------------------------------------+-----------------------------------
//! Copy constructor

RlinkCommandList::RlinkCommandList(const RlinkCommandList& rhs)
  : fList()
{
  operator=(rhs);
}

//------------------------------------------+-----------------------------------
//! Destructor

RlinkCommandList::~RlinkCommandList()
{
  foreach_ (RlinkCommand* pcmd, fList) { delete pcmd; }
}

//------------------------------------------+-----------------------------------
//! FIXME_docs

size_t RlinkCommandList::AddCommand(RlinkCommand* cmd)
{
  size_t ind = fList.size();
  fList.push_back(cmd);
  return ind;
}

//------------------------------------------+-----------------------------------
//! FIXME_docs

size_t RlinkCommandList::AddCommand(const RlinkCommand& cmd)
{
  return AddCommand(new RlinkCommand(cmd));
}

//------------------------------------------+-----------------------------------
//! FIXME_docs

size_t RlinkCommandList::AddCommand(const RlinkCommandList& clist)
{
  size_t ind = fList.size();
  for (size_t i=0; i<clist.Size(); i++) {
    AddCommand(new RlinkCommand(clist[i]));
  }
  return ind;
}

//------------------------------------------+-----------------------------------
//! FIXME_docs

size_t RlinkCommandList::AddRreg(uint16_t addr)
{
  RlinkCommand* pcmd = new RlinkCommand();
  pcmd->CmdRreg(addr);
  return AddCommand(pcmd);
}

//------------------------------------------+-----------------------------------
//! FIXME_docs

size_t RlinkCommandList::AddRblk(uint16_t addr, size_t size)
{
  RlinkCommand* pcmd = new RlinkCommand();
  pcmd->CmdRblk(addr, size);
  return AddCommand(pcmd);
}

//------------------------------------------+-----------------------------------
//! FIXME_docs

size_t RlinkCommandList::AddRblk(uint16_t addr, uint16_t* block, size_t size)
{
  RlinkCommand* pcmd = new RlinkCommand();
  pcmd->CmdRblk(addr, block, size);
  return AddCommand(pcmd);
}

//------------------------------------------+-----------------------------------
//! FIXME_docs

size_t RlinkCommandList::AddWreg(uint16_t addr, uint16_t data)
{
  RlinkCommand* pcmd = new RlinkCommand();
  pcmd->CmdWreg(addr, data);
  return AddCommand(pcmd);
}

//------------------------------------------+-----------------------------------
//! FIXME_docs

size_t RlinkCommandList::AddWblk(uint16_t addr, std::vector<uint16_t> block)
{
  RlinkCommand* pcmd = new RlinkCommand();
  pcmd->CmdWblk(addr, block);
  return AddCommand(pcmd);
}

//------------------------------------------+-----------------------------------
//! FIXME_docs

size_t RlinkCommandList::AddWblk(uint16_t addr, const uint16_t* block,
                                 size_t size)
{
  RlinkCommand* pcmd = new RlinkCommand();
  pcmd->CmdWblk(addr, block, size);
  return AddCommand(pcmd);
}

//------------------------------------------+-----------------------------------
//! FIXME_docs

size_t RlinkCommandList::AddLabo()
{
  RlinkCommand* pcmd = new RlinkCommand();
  pcmd->CmdLabo();
  return AddCommand(pcmd);
}

//------------------------------------------+-----------------------------------
//! FIXME_docs

size_t RlinkCommandList::AddAttn()
{
  RlinkCommand* pcmd = new RlinkCommand();
  pcmd->CmdAttn();
  return AddCommand(pcmd);
}

//------------------------------------------+-----------------------------------
//! FIXME_docs

size_t RlinkCommandList::AddInit(uint16_t addr, uint16_t data)
{
  RlinkCommand* pcmd = new RlinkCommand();
  pcmd->CmdInit(addr, data);
  return AddCommand(pcmd);
}

//------------------------------------------+-----------------------------------
//! FIXME_docs

void RlinkCommandList::SetLastExpect(RlinkCommandExpect* pexp)
{
  size_t ncmd = fList.size();
  if (ncmd == 0)
    throw Rexception("RlinkCommandList::SetLastExpect()",
                     "Bad state: list empty");
  fList[ncmd-1]->SetExpect(pexp);
  return;
}

//------------------------------------------+-----------------------------------
//! FIXME_docs

void RlinkCommandList::Clear()
{
  foreach_ (RlinkCommand* pcmd, fList) { delete pcmd; }
  fList.clear();
  fLaboIndex = -1;
  return;
}

//------------------------------------------+-----------------------------------
//! FIXME_docs

void RlinkCommandList::Print(std::ostream& os, const RlinkContext& cntx,
                             const RlinkAddrMap* pamap, size_t abase, 
                             size_t dbase, size_t sbase) const
{
  foreach_ (RlinkCommand* pcmd, fList) {
    pcmd->Print(os, cntx, pamap, abase, dbase, sbase);
  }
  return;
}

//------------------------------------------+-----------------------------------
//! FIXME_docs

void RlinkCommandList::Dump(std::ostream& os, int ind, const char* text) const
{
  RosFill bl(ind);
  os << bl << (text?text:"--") << "RlinkCommandList @ " << this << endl;

  os << bl << "  fLaboIndex:      " << fLaboIndex << endl;
  for (size_t i=0; i<Size(); i++) {
    string pref("fList[");
    pref << RosPrintf(i) << RosPrintf("]: ");
    fList[i]->Dump(os, ind+2, pref.c_str());
  }

  return;
}

//------------------------------------------+-----------------------------------
//! FIXME_docs

RlinkCommandList& 
  Retro::RlinkCommandList::operator=( const RlinkCommandList& rhs)
{
  if (&rhs == this) return *this;
  
  foreach_ (RlinkCommand* pcmd, fList) { delete pcmd; }
  fList.clear();
  for (size_t i=0; i<rhs.Size(); i++) AddCommand(rhs[i]);
  return *this;
}

//------------------------------------------+-----------------------------------
//! FIXME_docs

Retro::RlinkCommand& Retro::RlinkCommandList::operator[](size_t ind)
{
  return *fList.at(ind);
}

//------------------------------------------+-----------------------------------
//! FIXME_docs

const Retro::RlinkCommand& Retro::RlinkCommandList::operator[](size_t ind) const
{
  return *fList.at(ind);
}

} // end namespace Retro