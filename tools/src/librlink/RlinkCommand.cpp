// $Id: RlinkCommand.cpp 609 2014-12-07 19:35:25Z mueller $
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
// 2014-12-06   609   1.2    new rlink v4 iface
// 2014-08-15   583   1.1    rb_mreq addr now 16 bit
// 2013-05-06   495   1.0.2  add RlinkContext to Print() args
// 2013-02-03   481   1.0.1  use Rexception
// 2011-03-27   374   1.0    Initial version
// 2011-01-15   355   0.1    First draft
// ---------------------------------------------------------------------------

/*!
  \file
  \version $Id: RlinkCommand.cpp 609 2014-12-07 19:35:25Z mueller $
  \brief   Implemenation of class RlinkCommand.
 */

// debug
#include <iostream>

#include <algorithm>

#include "RlinkCommand.hpp"

#include "librtools/RosFill.hpp"
#include "librtools/RosPrintf.hpp"
#include "librtools/RosPrintBvi.hpp"
#include "librtools/Rexception.hpp"

using namespace std;

/*!
  \class Retro::RlinkCommand
  \brief FIXME_docs
*/

// all method definitions in namespace Retro
namespace Retro {

//------------------------------------------+-----------------------------------
// constants definitions

const uint8_t  RlinkCommand::kCmdRreg;
const uint8_t  RlinkCommand::kCmdRblk;
const uint8_t  RlinkCommand::kCmdWreg;
const uint8_t  RlinkCommand::kCmdWblk;
const uint8_t  RlinkCommand::kCmdLabo;
const uint8_t  RlinkCommand::kCmdAttn;
const uint8_t  RlinkCommand::kCmdInit;

const uint32_t RlinkCommand::kFlagInit;
const uint32_t RlinkCommand::kFlagSend;
const uint32_t RlinkCommand::kFlagDone;
const uint32_t RlinkCommand::kFlagLabo;
const uint32_t RlinkCommand::kFlagPktBeg;
const uint32_t RlinkCommand::kFlagPktEnd;
const uint32_t RlinkCommand::kFlagErrNak;
const uint32_t RlinkCommand::kFlagErrDec;
const uint32_t RlinkCommand::kFlagChkStat;
const uint32_t RlinkCommand::kFlagChkData;

const uint8_t RlinkCommand::kStat_M_Stat;
const uint8_t RlinkCommand::kStat_V_Stat;
const uint8_t RlinkCommand::kStat_B_Stat;
const uint8_t RlinkCommand::kStat_M_Attn;
const uint8_t RlinkCommand::kStat_M_RbNak;
const uint8_t RlinkCommand::kStat_M_RbErr;

//------------------------------------------+-----------------------------------
//! Default constructor

RlinkCommand::RlinkCommand()
  : fRequest(0), 
    fAddress(0), 
    fData(0),
    fBlock(),
    fpBlockExt(0), 
    fBlockExtSize(0), 
    fBlockDone(0), 
    fStatus(0), 
    fFlags(0),
    fRcvSize(0),
    fpExpect(0)
{}

//------------------------------------------+-----------------------------------
//! Copy constructor

RlinkCommand::RlinkCommand(const RlinkCommand& rhs)
  : fRequest(rhs.fRequest), 
    fAddress(rhs.fAddress), 
    fData(rhs.fData),
    fBlock(rhs.fBlock),
    fpBlockExt(rhs.fpBlockExt), 
    fBlockExtSize(rhs.fBlockExtSize), 
    fBlockDone(rhs.fBlockDone), 
    fStatus(rhs.fStatus), 
    fFlags(rhs.fFlags),
    fRcvSize(rhs.fRcvSize),
    fpExpect(rhs.fpExpect ? new RlinkCommandExpect(*rhs.fpExpect) : 0)
{}

//------------------------------------------+-----------------------------------
//! Destructor

RlinkCommand::~RlinkCommand()
{
  delete fpExpect;                          // expect object owned by command
}

//------------------------------------------+-----------------------------------
//! FIXME_docs

void RlinkCommand::CmdRblk(uint16_t addr, size_t size)
{
  SetCommand(kCmdRblk, addr);
  SetBlockRead(size);
  return;
}

//------------------------------------------+-----------------------------------
//! FIXME_docs

void RlinkCommand::CmdRblk(uint16_t addr, uint16_t* pblock, size_t size)
{
  SetCommand(kCmdRblk, addr);
  SetBlockExt(pblock, size);
  return;
}

//------------------------------------------+-----------------------------------
//! FIXME_docs

void RlinkCommand::CmdWblk(uint16_t addr, const std::vector<uint16_t>& block)
{
  SetCommand(kCmdWblk, addr);
  SetBlockWrite(block);
  return;
}

//------------------------------------------+-----------------------------------
//! FIXME_docs

void RlinkCommand::CmdWblk(uint16_t addr, const uint16_t* pblock, size_t size)
{
  SetCommand(kCmdWblk, addr);
  SetBlockExt(const_cast<uint16_t*>(pblock), size);
  return;
}

//------------------------------------------+-----------------------------------
//! FIXME_docs

void RlinkCommand::SetCommand(uint8_t cmd, uint16_t addr, uint16_t data)
{
  if (cmd > kCmdInit) 
    throw Rexception("RlinkCommand::SetCommand()", "Bad args: invalid cmd");
  fRequest   = cmd;
  fAddress   = addr;
  fData      = data;
  fpBlockExt    = 0;
  fBlockExtSize = 0;
  fBlockDone = 0;
  fStatus    = 0;
  fFlags     = kFlagInit;
  fRcvSize   = 0;
  delete fpExpect;
  fpExpect   = 0;
  return;
}

//------------------------------------------+-----------------------------------
//! FIXME_docs

void RlinkCommand::SetAddress(uint16_t addr)
{
  fAddress = addr;
  return;
}

//------------------------------------------+-----------------------------------
//! FIXME_docs

void RlinkCommand::SetBlockWrite(const std::vector<uint16_t>& block)
{
  if (block.size() == 0 || block.size() > 65535) 
    throw Rexception("RlinkCommand::SetBlockWrite()",
                     "Bad args: invalid block size");
  fBlock = block;
  fpBlockExt    = 0;
  fBlockExtSize = 0;
  fBlockDone    = 0;
  return;
}

//------------------------------------------+-----------------------------------
//! FIXME_docs

void RlinkCommand::SetBlockRead(size_t size)
{
  if (size == 0 || size > 65535) 
    throw Rexception("RlinkCommand::SetBlockRead()",
                     "Bad args: invalid block size");
  fBlock.clear();
  fBlock.resize(size);
  fpBlockExt    = 0;
  fBlockExtSize = 0;
  fBlockDone    = 0;
  return;
}

//------------------------------------------+-----------------------------------
//! FIXME_docs

void RlinkCommand::SetBlockExt(uint16_t* pblock, size_t size)
{
  if (pblock == 0) 
    throw Rexception("RlinkCommand::SetBlockExt()",
                     "Bad args: pblock is null");
  if (size == 0 || size > 65535) 
    throw Rexception("RlinkCommand::SetBlockExt()",
                     "Bad args: invalid block size");
  fpBlockExt    = pblock;
  fBlockExtSize = size;
  fBlockDone    = 0;
  return;
}

//------------------------------------------+-----------------------------------
//! FIXME_docs

void RlinkCommand::SetExpect(RlinkCommandExpect* pexp)
{
  delete fpExpect;
  fpExpect = pexp;
  return;
}

//------------------------------------------+-----------------------------------
//! FIXME_docs

void RlinkCommand::Print(std::ostream& os, const RlinkContext& cntx,
                         const RlinkAddrMap* pamap, size_t abase, 
                         size_t dbase, size_t sbase) const
{
  uint8_t ccode = Command();
  
  // separator + command mnemonic, code and flags
  // separator:  ++  first in packet
  //             --  non-first in packet
  //             ??  FIXME: separator for labo canceled commands 
  const char* sep = "??";
  if (TestFlagAny(kFlagPktBeg)) {
    sep = "++";
  } else {
    sep = "--";
  }
  
  os << sep << " " << CommandName(ccode)
     << " (" << RosPrintBvi(Request(), 8)
     << ","  << RosPrintBvi(fFlags, 16, 20)
     << ")";

  // address field
  if (ccode==kCmdRreg || ccode==kCmdRblk ||
      ccode==kCmdWreg || ccode==kCmdWblk ||
      ccode==kCmdInit) {
    os << " a=" << RosPrintBvi(fAddress, abase);
    if (pamap) {
      string name;
      if (!pamap->Find(fAddress, name)) name.clear();
      os << "(" << name << RosFill(pamap->MaxNameLength()-name.length()) << ")";
    }
  }

  // data field (scalar)
  if (ccode== kCmdRreg || ccode==kCmdWreg ||
      ccode== kCmdLabo || ccode==kCmdAttn ||
      ccode== kCmdInit) {
    os << " d=" << RosPrintBvi(fData, dbase);    

    if (fpExpect &&
        (ccode==kCmdRreg || ccode==kCmdLabo || ccode==kCmdAttn)) {
      if (TestFlagAny(kFlagChkData)) {
        os << "#";
        os << " D=" << RosPrintBvi(fpExpect->DataValue(), dbase);
        if (fpExpect->DataMask() != 0x0000)  {
          os << "," << RosPrintBvi(fpExpect->DataMask(), dbase);
        }
      } else if (fpExpect->DataIsChecked()) {
        os << "!";
      } else {
        os << " ";
      }
    } else {
      os << " ";
    }
  }

  if (ccode== kCmdRblk || ccode==kCmdWblk) {
    os << " n=" << RosPrintf(BlockSize(), "d", 3); 
  }

  // status field
  os << " s=" << RosPrintBvi(fStatus, sbase);
  uint8_t scval  = fpExpect ? fpExpect->StatusValue() : cntx.StatusValue();
  uint8_t scmsk  = fpExpect ? fpExpect->StatusMask()  : cntx.StatusMask();
  if (TestFlagAny(kFlagChkStat)) {
    os << "#";
    os << " S=" << RosPrintBvi(scval, sbase);
    if (scmsk != 0x00) {
      os << "," << RosPrintBvi(scmsk, sbase);
    }
  } else {
    os << ( scmsk != 0xff ? "!" : " " );
  }

  if (TestFlagAny(kFlagDone)) {
    if (TestFlagAny(kFlagChkStat|kFlagChkData)) {
      os << " FAIL: " 
         << Rtools::Flags2String(fFlags&(kFlagChkStat|kFlagChkData),
                                 FlagNames(),',');
    } else {
      os << " OK";
    }
  } else if (TestFlagAny(kFlagSend)) {
    os << " FAIL: "
       << Rtools::Flags2String(fFlags&(kFlagErrNak|kFlagErrDec),
                               FlagNames(),',');
  } else {
    os << " PEND";
  }

  // handle data part of rblk and wblk commands
  size_t dwidth = (dbase==2) ? 16 : ((dbase==8) ? 6 : 4);  
  
  if (ccode==kCmdRblk) {
    bool  dcheck = (fpExpect && fpExpect->BlockValue().size() > 0);
    size_t ncol  = (80-4-5)/(dwidth+2);
    
    size_t size  = BlockSize();
    const uint16_t* pdat = BlockPointer();

    for (size_t i=0; i<size; i++) {
      if (i%ncol == 0) os << "\n    " << RosPrintf(i,"d",3) << ": ";
      os << RosPrintBvi(pdat[i], dbase);
      if (dcheck) {
        if (!fpExpect->BlockCheck(i, pdat[i])) {
          os << "#";
        } else {
          os << (fpExpect->BlockIsChecked(i) ? "!" : "-");
        }
      } else {
        os << " ";
      }
      os << " ";
    }

    if (dcheck && TestFlagAny(kFlagChkData)) {
      const vector<uint16_t>& evalvec = fpExpect->BlockValue();
      const vector<uint16_t>& emskvec = fpExpect->BlockMask();
      for (size_t i=0; i<size; i++) {
        if (!fpExpect->BlockCheck(i, pdat[i])) {
          os << "\n      FAIL d[" << RosPrintf(i,"d",3) << "]: "
             << RosPrintBvi(pdat[i], dbase) << "#"
             << "  D=" << RosPrintBvi(evalvec[i], dbase);
          if (i < emskvec.size() && emskvec[i]!=0x0000) {
            os << "," << RosPrintBvi(emskvec[i], dbase);
          }
        }
      }
    } 
  }

  if (ccode==kCmdWblk) {
    const uint16_t* pdat = BlockPointer();
    size_t size = BlockSize();
    size_t ncol = (80-4-5)/(dwidth+2);
    for (size_t i=0; i<size; i++) {
      if (i%ncol == 0) os << "\n    " << RosPrintf(i,"d",3) << ": ";
      os << RosPrintBvi(pdat[i], dbase) << "  ";
    }
  }

  os << endl;

  return;
}

//------------------------------------------+-----------------------------------
//! FIXME_docs

void RlinkCommand::Dump(std::ostream& os, int ind, const char* text) const
{
  RosFill bl(ind);
  os << bl << (text?text:"--") << "RlinkCommand @ " << this << endl;

  os << bl << "  fRequest:        " << RosPrintBvi(fRequest,8)
     << "  seq:" << RosPrintf(SeqNumber(),"d",2)
     << "  cmd:" << RosPrintf(Command(),"d",1)
     << " " << CommandName(Command()) << endl;
  os << bl << "  fAddress:        " << RosPrintBvi(fAddress,0) << endl;
  os << bl << "  fData:           " << RosPrintBvi(fData,0) << endl;
  os << bl << "  fBlock.size:     " << RosPrintf(fBlock.size(),"d",4) << endl;
  os << bl << "  fpBlockExt:      " << fpBlockExt << endl;
  os << bl << "  fBlockExtSize:   " << RosPrintf(fBlockExtSize,"d",4) << endl;
  os << bl << "  fBlockDone:      " << RosPrintf(fBlockDone,"d",4) << endl;
  os << bl << "  fStatus:         " << RosPrintBvi(fStatus,0) << endl;
  os << bl << "  fFlags:          " << RosPrintBvi(fFlags,16,24)
           << "  " << Rtools::Flags2String(fFlags, FlagNames()) << endl;
  os << bl << "  fRcvSize:        " << RosPrintf(fRcvSize,"d",4) << endl;
  if (BlockSize() > 0) {
    size_t ncol  = max(1, (80-ind-4-5)/(4+1));
    os << bl << "  block data:";
    for (size_t i=0; i<BlockSize(); i++) {
      if (i%ncol == 0) os << "\n" << bl << "    " << RosPrintf(i,"d",3) << ": ";
      os << RosPrintBvi(BlockPointer()[i],16) << " ";
    }
    os << endl;
  }
  if (fpExpect) fpExpect->Dump(os, ind+2, "fpExpect: ");
  
  return;
}

//------------------------------------------+-----------------------------------
//! FIXME_docs

const char* RlinkCommand::CommandName(uint8_t cmd)
{
  static const char* cmdname[8] = {"rreg","rblk","wreg","wblk",
                                   "labo","attn","init","????"};
  
  return cmdname[cmd&0x7];
}

//------------------------------------------+-----------------------------------
//! FIXME_docs

const Retro::RflagName* RlinkCommand::FlagNames()
{
  // use msb first order, will also be printing order
  static Retro::RflagName fnam[] = {
    {kFlagChkData, "ChkData"},
    {kFlagChkStat, "ChkStat"},
    {kFlagErrDec,  "ErrDec"},
    {kFlagErrNak,  "ErrNak"},
    {kFlagPktEnd,  "PktEnd"},
    {kFlagPktBeg,  "PktBeg"},
    {kFlagLabo,    "Labo"},
    {kFlagDone,    "Done"},
    {kFlagSend,    "Send"},
    {kFlagInit,    "Init"},
    {0u, ""}
  };
  return fnam;
}

//------------------------------------------+-----------------------------------
//! FIXME_docs

RlinkCommand& RlinkCommand::operator=(const RlinkCommand& rhs)
{
  if (&rhs == this) return *this;
  fRequest      = rhs.fRequest;
  fAddress      = rhs.fAddress; 
  fData         = rhs.fData;
  fBlock        = rhs.fBlock;
  fpBlockExt    = rhs.fpBlockExt; 
  fBlockExtSize = rhs.fBlockExtSize; 
  fBlockDone    = rhs.fBlockDone; 
  fStatus       = rhs.fStatus; 
  fFlags        = rhs.fFlags;
  fRcvSize      = rhs.fRcvSize;
  delete fpExpect;
  fpExpect      = rhs.fpExpect ? new RlinkCommandExpect(*rhs.fpExpect) : 0;
  return *this;
}

} // end namespace Retro