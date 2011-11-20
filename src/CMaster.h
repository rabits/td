/**
 * @file    CMaster.h
 * @date    2011-11-21T02:23:07+0400
 *
 * @author  Rabits <home.rabits@gmail.com>
 * @copyright GNU General Public License, version 3 <http://www.gnu.org/licenses/>
 *
 * This file is a part of Total Destruction project <http://www.rabits.ru/td>
 *
 * @brief   Master object with simple params
 *
 *
 */
#ifndef CMASTER_H
#define CMASTER_H

#include "Common.h"

class CMaster
{
public:
    /** @brief Master object
     *
     * @param name const char*
     */
    CMaster(const char* name);
    virtual ~CMaster();

    /** @brief Get name
     *
     * @return const std::string&
     */
    const std::string& name() const { return m_Name; }

protected:
    std::string                          m_Name;    ///< Name
};

#endif // CMASTER_H
