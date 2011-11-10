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
        CData(const char* name);
        virtual ~CData();

        /** @brief Load data file for object
         *
         * @param datafile const char* - Path to xml data file
         * @return bool
         *
         * Merge with previous loaded data.
         */
        bool loadData(const char* datafile);

        /** @brief Convert object data into string format
         *
         * @param stream std::ostream& - Output stream (like std::cout or any file stream)
         * @return void
         *
         */
        void saveData(std::ostream &stream);

        /** @brief Verifying of xml document to consistent (good root td_name, root version, container name, ...)
         *
         * @param document pugi::xml_document& - Document to verify
         * @return bool - consistent data
         */
        bool verifyData(pugi::xml_document &document) const;

        /** @brief Merging current data and new data. Current data root will be copied to m_dataBefore
         *
         * @param new_node pugi::xml_node& - Merge with it container node
         * @param cur_node pugi::xml_node* - For recurse
         * @return bool
         */
        bool mergeData(pugi::xml_node &new_node, pugi::xml_node* cur_node = NULL);

    protected:
        pugi::xml_document  m_dataRoot; ///< XML Root, containing td data
        pugi::xml_document  m_dataBefore; ///< Before merge dataRoot
        pugi::xml_node      m_data; ///< Data of object
        const char*         m_dataName; ///< Data container name
    private:
};

#endif // CDATA_H
