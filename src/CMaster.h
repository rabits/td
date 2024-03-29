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
     * @param name
     */
    CMaster(const char* name)
        : m_Name(name), m_Description("") {  }

    /** @brief Master object
     *
     * @param name
     */
    CMaster(const char* name, const char* description)
        : m_Name(name), m_Description(description) {  }

    virtual ~CMaster() {  }

    /** @brief Get name
     *
     * @return const std::string&
     */
    const std::string& name() const { return m_Name; }

    /** @brief Change name
     *
     * @param name
     */
    void name(const std::string& name) { m_Name = name; }

    /** @brief Get description
     *
     * @return const std::string&
     */
    const std::string& description() const { return m_Description; }

    /** @brief Change description
     *
     * @param description
     */
    void description(const std::string& description) { m_Description = description; }

    /** @brief Update object data, it may be animation or just object translation or so on
     *
     * @param time_since_last_frame
     *
     */
    virtual void update(const Ogre::Real) {  }

    /** @brief Initialize object
     *
     */
    virtual void init() {  }

protected:
    std::string                          m_Name;    ///< Name
    std::string                          m_Description; ///< Description
};

#endif // CMASTER_H
