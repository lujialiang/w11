// $Id: Rw11CntlRK11.ipp 887 2017-04-28 19:32:52Z mueller $
//
// Copyright 2015- by Walter F.J. Mueller <W.F.J.Mueller@gsi.de>
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
// 2015-01-03   627   1.0    Initial version
// ---------------------------------------------------------------------------

/*!
  \file
  \brief   Implemenation (inline) of Rw11CntlRK11.
*/


// all method definitions in namespace Retro
namespace Retro {

//------------------------------------------+-----------------------------------
//! FIXME_docs

inline void Rw11CntlRK11::SetChunkSize(size_t chunk)
{
  fRdma.SetChunkSize(chunk);
  return;
}

//------------------------------------------+-----------------------------------
//! FIXME_docs

inline size_t Rw11CntlRK11::ChunkSize() const
{
  return fRdma.ChunkSize();
}

//------------------------------------------+-----------------------------------
//! FIXME_docs

inline const Rstats& Rw11CntlRK11::RdmaStats() const
{
  return fRdma.Stats();
}


} // end namespace Retro
