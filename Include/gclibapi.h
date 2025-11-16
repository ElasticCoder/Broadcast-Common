/******************************************************************************
 * Project     : Grid Control
 * File        : gclibapi.h
 * Author      : Duncan Olesen
 * Date        : 20/12/96
 *
 * Description : Grid Control Library export/import definition
 *****************************************************************************/

#ifndef GCLIBAPI

#ifndef _GCLIB_
#define GCLIBAPI __declspec(dllimport)
#else
#define GCLIBAPI __declspec(dllexport)
#endif _GCLIB_

#endif GCLIBAPI
