/**
 * @file    main.h
 * @date    2010-09-26T13:44:21+0400
 *
 * @author  Rabits <home.rabits@gmail.com>
 * @url     http://www.rabits.ru/td
 *
 * @copyright GNU General Public License, version 3 <http://www.gnu.org/licenses/>
 *
 * @brief   Main header
 *
 *
 */


#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#include "config.h"

#include "CGame.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    #define WIN32_LEAN_AND_MEAN
    #include "windows.h"
#endif

#endif // MAIN_H_INCLUDED
