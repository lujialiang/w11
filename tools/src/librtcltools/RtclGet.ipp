// $Id: RtclGet.ipp 887 2017-04-28 19:32:52Z mueller $
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
// 2017-04-16   876   1.2    add Tcl_Obj*
// 2017-02-20   854   1.1    add Rtime
// 2013-02-12   487   1.0    Initial version
// ---------------------------------------------------------------------------

/*!
  \file
  \brief   Implemenation (inline) of class RtclGet.
*/

/*!
  \class Retro::RtclGet
  \brief FIXME_docs
*/

#include "librtools/Rtime.hpp"

// all method definitions in namespace Retro
namespace Retro {

//------------------------------------------+-----------------------------------
//! FIXME_docs

template <class TP>
inline RtclGet<TP>::RtclGet(const boost::function<TP()>& get)
  : fGet(get)
{}

//------------------------------------------+-----------------------------------
//! FIXME_docs

template <class TP>
inline RtclGet<TP>::~RtclGet()
{}

//------------------------------------------+-----------------------------------
//! FIXME_docs

template <>
inline Tcl_Obj* RtclGet<bool>::operator()() const 
{
  bool val = fGet();
  return Tcl_NewBooleanObj((int)val);
}

//------------------------------------------+-----------------------------------
//! FIXME_docs

template <>
inline Tcl_Obj* RtclGet<char>::operator()() const 
{
  char val = fGet();
  return Tcl_NewIntObj((int)val);
}

//------------------------------------------+-----------------------------------
//! FIXME_docs

template <>
inline Tcl_Obj* RtclGet<signed char>::operator()() const 
{
  signed char val = fGet();
  return Tcl_NewIntObj((int)val);
}

//------------------------------------------+-----------------------------------
//! FIXME_docs

template <>
inline Tcl_Obj* RtclGet<unsigned char>::operator()() const 
{
  unsigned char val = fGet();
  return Tcl_NewIntObj((int)val);
}

//------------------------------------------+-----------------------------------
//! FIXME_docs

template <>
inline Tcl_Obj* RtclGet<short>::operator()() const 
{
  short val = fGet();
  return Tcl_NewIntObj((int)val);
}

//------------------------------------------+-----------------------------------
//! FIXME_docs

template <>
inline Tcl_Obj* RtclGet<unsigned short>::operator()() const 
{
  unsigned short val = fGet();
  return Tcl_NewIntObj((int)val);
}

//------------------------------------------+-----------------------------------
//! FIXME_docs

template <>
inline Tcl_Obj* RtclGet<int>::operator()() const 
{
  int val = fGet();
  return Tcl_NewIntObj(val);
}

//------------------------------------------+-----------------------------------
//! FIXME_docs

template <>
inline Tcl_Obj* RtclGet<unsigned int>::operator()() const 
{
  unsigned int val = fGet();
  return Tcl_NewIntObj((int)val);
}

//------------------------------------------+-----------------------------------
//! FIXME_docs

template <>
inline Tcl_Obj* RtclGet<long>::operator()() const 
{
  long val = fGet();
  return Tcl_NewLongObj(val);
}

//------------------------------------------+-----------------------------------
//! FIXME_docs

template <>
inline Tcl_Obj* RtclGet<unsigned long>::operator()() const 
{
  unsigned long val = fGet();
  return Tcl_NewLongObj((long)val);
}

//------------------------------------------+-----------------------------------
//! FIXME_docs

template <>
inline Tcl_Obj* RtclGet<float>::operator()() const 
{
  float val = fGet();
  return Tcl_NewDoubleObj(val);
}

//------------------------------------------+-----------------------------------
//! FIXME_docs

template <>
inline Tcl_Obj* RtclGet<double>::operator()() const 
{
  double val = fGet();
  return Tcl_NewDoubleObj(val);
}

//------------------------------------------+-----------------------------------
//! FIXME_docs

template <>
inline Tcl_Obj* RtclGet<std::string>::operator()() const 
{
  std::string val = fGet();
  return Tcl_NewStringObj((char*) val.data(), val.length());
}

//------------------------------------------+-----------------------------------
//! FIXME_docs

template <>
inline Tcl_Obj* RtclGet<const std::string&>::operator()() const 
{
  std::string val = fGet();
  return Tcl_NewStringObj((char*) val.data(), val.length());
}

//------------------------------------------+-----------------------------------
//! FIXME_docs

template <>
inline Tcl_Obj* RtclGet<Rtime>::operator()() const 
{
  Rtime val = fGet();
  return Tcl_NewDoubleObj(double(val));
}

//------------------------------------------+-----------------------------------
//! FIXME_docs

template <>
inline Tcl_Obj* RtclGet<const Rtime&>::operator()() const 
{
  Rtime val = fGet();
  return Tcl_NewDoubleObj(double(val));
}

//------------------------------------------+-----------------------------------
//! FIXME_docs

template <>
inline Tcl_Obj* RtclGet<Tcl_Obj*>::operator()() const 
{
  return fGet();
}


} // end namespace Retro

