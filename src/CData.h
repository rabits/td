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

#include "config.h"
#include "pugixml/pugixml.hpp"

/** @brief XML data for needled objects (for auto-restore or configuration save/load)
 */
class CData
{
    public:
        CData();
        virtual ~CData();
    protected:
        pugi::xml_document m_pDataRoot; ///< XML Root, containing td data
        pugi::xml_node m_pData; ///< Data of object
    private:
};

#endif // CDATA_H
