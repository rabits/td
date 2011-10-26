/**
 * @file    CData.h
 * @date    2011-10-22T17:01:20+0400
 *
 * @author  Rabits <home.rabits@gmail.com>
 * @copyright GNU General Public License, version 3 <http://www.gnu.org/licenses/>
 *
 * This file is a part of Total Destruction project <http://www.rabits.ru/td>
 *
 * @brief   XML data container for object
 *
 *
 */

#ifndef CDATA_H
#define CDATA_H

#include "Common.h"
#include "pugixml/pugixml.hpp"

#include <iostream>

/** @brief XML data for needled objects (for auto-restore or configuration save/load)
 */
class CData
{
    public:
        /** @brief Constructor with container name
         *
         * @param name char* - Name of data container
         *
         */
        CData(const char *name);
        virtual ~CData();

        /** @brief Convert object data into string format
         *
         * @param stream std::ostream& - Output stream (like std::cout or any file stream)
         * @return void
         *
         */
        void saveData(std::ostream &stream);

    protected:
        pugi::xml_document  m_dataRoot; ///< XML Root, containing td data
        pugi::xml_node      m_data; ///< Data of object
    private:
};

#endif // CDATA_H
