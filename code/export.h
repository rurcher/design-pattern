/**
 * @brief dll导出
 * @author rurcher
 */
#ifdef _MSC_VER
# pragma once
#endif

#ifndef DES_EXPORT_H_
#define DES_EXPORT_H_
# if defined (_WIN32)
#	if defined (DES_EXPORT_DLL)
#		defined DES_EXPORT __declspec(dllexport)
#	else
#		defined DES_EXPORT __declspec(dllimport)
#	endif
# else
#	define DES_EXPORT
# endif
#endif /* DES_EXPORT_H_ */