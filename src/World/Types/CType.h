/**
 * @file    CType.h
 * @date    2011-12-12T01:46:30+0400
 *
 * @author  Rabits <home.rabits@gmail.com>
 * @copyright GNU General Public License, version 3 <http://www.gnu.org/licenses/>
 *
 * This file is a part of Total Destruction project <http://www.rabits.ru/td>
 *
 * @brief   Object type
 *
 *
 */

#ifndef CTYPE_H
#define CTYPE_H

#include "Common.h"
#include "CMaster.h"

class CType
    : public CMaster
{
public:
    /** @brief Type abstract constructor
     *
     * @param name
     * @param description
     */
    CType(const char* name, const char* description)
        : CMaster(name, description) {  }

protected:

    /** @brief Class for contain need value with limiters
     *
     */
    template <class T>
    class CTypeParameter
        : public CMaster
    {
    public:
        /** @brief Default simple constructor
         *
         * @param name
         * @param description
         */
        CTypeParameter(const char* name, const char* description)
            : CMaster(name, description)
            , m_Limit(LIMIT_NONE)
            , m_Value()
            , m_ValueMin()
            , m_ValueMax()
            , m_ValuesAvailable() {  }

        /** @brief Constructor for numeric limits
         *
         * @param name
         * @param description
         * @param min
         * @param max
         */
        CTypeParameter(const char* name, const char* description, T min, T max)
            : CMaster(name, description)
            , m_Limit(LIMIT_MINMAX)
            , m_Value()
            , m_ValueMin(min)
            , m_ValueMax(max)
            , m_ValuesAvailable() { m_Limit = LIMIT_MINMAX; }

        /** @brief Set value
         *
         * @param value
         */
        void value(const T& value);

        /** @brief Get value
         *
         * @return T&
         */
        T& value() const { return &m_Value; }

        void setMinMax(const T &min, const T &max) { m_ValueMin = min; m_ValueMax = max; m_Limit = LIMIT_MINMAX; }

        void addAvailable(const T &value, const std::string &description) { m_ValuesAvailable[value] = description; m_Limit = LIMIT_AVAILABLE; }
    private:
        enum Limiter {
            LIMIT_NONE      = 0,
            LIMIT_MINMAX    = 1,
            LIMIT_AVAILABLE = 2
        };

        Limiter                  m_Limit;           ///< Value is limited

        T                        m_Value;           ///< Current value
        T                        m_ValueMin;        ///< Min value
        T                        m_ValueMax;        ///< Max value
        std::map<T, std::string> m_ValuesAvailable; ///< List of available values
    };

private:

};

#endif // CTYPE_H
