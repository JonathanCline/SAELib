#pragma once
#ifndef SAELIB_CONFIG_H
#define SAELIB_CONFIG_H

#ifndef SAELIB_CXX_STANDARD
#error "SAELIB_CXX_STANDARD is not defined. If you are not using cmake then set this to the c++ version you are using"
#endif



#if SAELIB_CXX_STANDARD >= 11
#define SAELIB_CONSTEXPR constexpr
#else
#define SAELIB_CONSTEXPR const
#endif

namespace sae
{



}

#endif