/**
 * @file    CData.h
 * @date    2011-10-22T17:01:20+0400
 *
 * @author  Rabits <home.rabits@gmail.com>
 * @url     http://www.rabits.ru/td
 *
 * @copyright GNU General Public License, version 3 <http://www.gnu.org/licenses/>
 *
 * @brief   XML data container for object
 *
 *
 */
#ifndef CDATA_H
#define CDATA_H

#include "config.h"
#include "pugixml/pugixml.hpp"
pugi::format_indent

class CData
{
    public:
        CData();
        virtual ~CData();
    protected:
        pugi::xml_document m_pDataRoot;
        pugi::xml_node m_pData;
    private:
};

#endif // CDATA_H
