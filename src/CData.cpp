/**
 * @file    CData.cpp
 * @date    2011-10-22T17:01:20+0400
 *
 * @author  Rabits <home.rabits@gmail.com>
 * @copyright GNU General Public License, version 3 <http://www.gnu.org/licenses/>
 *
 * This file is a part of Total Destruction project <http://www.rabits.ru/td>
 *
 * @brief   XML data container for object
 *
 */

#include "CData.h"

#include <cstring>
#include <sstream>

CData::CData(const char* name)
    : m_dataRoot()
    , m_dataBefore()
    , m_data()
    , m_dataName(name)
{
    m_data = m_dataRoot.append_child(CONFIG_TD_NAME).append_child(name);
    m_dataRoot.child(CONFIG_TD_NAME).append_attribute("version").set_value(CONFIG_TD_VERSION);
}

CData::~CData()
{
}

bool CData::loadData(const char* datafile)
{
    log_info("Loading %s data file: \"%s\"", m_dataName, datafile);

    pugi::xml_document new_data;
    pugi::xml_parse_result result = new_data.load_file(datafile, pugi::parse_full);

#ifdef CONFIG_DEBUG
    log_debug("New data for merge:");
    new_data.save(std::cout, "  ");
#endif

    if( !result )
        return log_error("\tLoading failed: %s#%d", result.description(), result.offset);

    // Verify config
    if( ! verifyData(new_data) )
        return log_error("\tFerifying of data failed");

    // Starting merge
    pugi::xml_node new_child = new_data.child(CONFIG_TD_NAME).child(m_dataName);
    mergeData(new_child);

#ifdef CONFIG_DEBUG
    log_debug("Data before merge:");
    m_dataBefore.save(std::cout, "  ");
    log_debug("Data After merge:");
    m_dataRoot.save(std::cout, "  ");
#endif

    log_info("\tComplete loading %s data file: \"%s\"", m_dataName, datafile);
    return true;
}

void CData::saveData(std::ostream& stream)
{
    m_dataRoot.save(stream, "  ", pugi::format_default, pugi::xml_encoding::encoding_utf8);
}

bool CData::verifyData(pugi::xml_document& document) const
{
    // Verify root tag name
    if( ! document.child(CONFIG_TD_NAME) )
        return log_error("It is not %s data. Root tag is \"%s\"", CONFIG_TD_FULLNAME, document.child(CONFIG_TD_NAME).name());

    pugi::xml_node root(document.child(CONFIG_TD_NAME));

    // Verify version
    if( std::strcmp(CONFIG_TD_VERSION, root.attribute("version").value()) )
    {
        log_info("Versions of program and data is different (data v%s)", root.attribute("version").value());
        unsigned int n_major, n_minor, n_patch, c_major, c_minor, c_patch;

        std::sscanf(CONFIG_TD_VERSION, "%u.%u.%u", &c_major, &c_minor, &c_patch);
        std::sscanf(root.attribute("version").value(), "%u.%u.%u", &n_major, &n_minor, &n_patch);

        if( c_major != n_major )
        {
            log_error("Version of data difference in major part (%s != %s):", CONFIG_TD_VERSION, root.attribute("version").value());
            log_notice("\tYou can fix this problem in three ways:");
            log_notice("\t\t1. Upgrade/downgrade game to same version of data;");
            log_notice("\t\t2. Try to remove \"User configuration directory\" (your state and configuration will be removed);");
            log_notice("\t\t3. Go to %s website and read, how to fix problem with difference major version;", CONFIG_TD_FULLNAME);
            return log_error("\tPlease, read information and changelog on %s website, or contact with support team to fix this problem.", CONFIG_TD_FULLNAME);
        }

        if( c_minor > n_minor )
        {
            log_warn("Version of data difference in minor part (%s > %s):", CONFIG_TD_VERSION, root.attribute("version").value());
            log_warn("\tThis can cause some warnings or errors - features will be added (read changelog)");
        }
        if( c_minor < n_minor )
        {
            log_warn("Version of data difference in minor part (%s < %s):", CONFIG_TD_VERSION, root.attribute("version").value());
            log_warn("\tThis can cause some warnings or errors - some features can be unavailable");
        }

        if( c_patch > n_patch )
            log_notice("Version of data difference in patch part (%s ~> %s). Nothing dangerous.", CONFIG_TD_VERSION, root.attribute("version").value());
        if( c_patch < n_patch )
        {
            log_notice("Version of data difference in patch part (%s ~< %s):", CONFIG_TD_VERSION, root.attribute("version").value());
            log_notice("\tNothing dangerous, but you can get some warnings and bugs.");
        }
    }

    // Verify data container
    if( ! root.child(m_dataName) )
        return log_error("Data not consist %s container", m_dataName);

    return true;
}

bool CData::mergeData(pugi::xml_node& new_node, pugi::xml_node* cur_node)
{
    // @todo Review this function and create cool debug

    // If running in first time
    if( cur_node == NULL )
    {
        // Merging  only container nodes
        if( std::strcmp(new_node.name(), m_dataName) )
        {
            log_error("Name not match %s != %s", new_node.name(), m_dataName);
            throw EXCEPTION("Configuration is not valid");
        }

        // Copy tree before merge
        std::stringstream buffer(std::stringstream::in | std::stringstream::out);
        m_dataRoot.save(buffer, "");
        m_dataBefore.load(buffer);

        cur_node = &m_data;
    }
    // Processing current level of tree
    pugi::xml_node cur_child;

    for( auto it = new_node.begin(); it != new_node.end(); ++it )
    {
        log_debug("Processing NEW: %s, CUR: %s", it->name(), cur_node->name());
        // Append node, if it not set or use exist node
        if( ! it->empty() )
        {
            if( (it->type() == pugi::node_element) && (!cur_node->child(it->name()) || it->attribute("value") || it->attribute("id")) )
                cur_child = cur_node->append_child(it->name());
            else if( (it->type() == pugi::node_element) && cur_node->child(it->name()) )
            {
                cur_child = cur_node->child(it->name());
                log_debug("\tGet bad child by name %s->%s", it->name(), cur_child.name());
            }
            else if( !cur_node->first_child() )
            {
                cur_child = cur_node->append_child(it->type());
                log_debug("\tAdd child by type %d->%d", it->type());
            }
            else
            {
                cur_child = cur_node->child(it->type());
                log_debug("\tGet child by type %d->%d", it->type(), cur_child.type());
            }
        }

        // Processing attributes
        for( auto ait = it->attributes_begin(); ait != it->attributes_end(); ++ait )
        {
            // Append attribute if it not present
            if( ! cur_child.attribute(ait->name()) )
                cur_child.append_attribute(ait->name());

            // Set value of attribute
            cur_child.attribute(ait->name()).set_value(ait->value());
        }

        // Set value of node
        if( it->value() )
        {
            log_debug("\tSet value %s, %d->%d", it->value(), it->type(), cur_child.type());
            cur_child.set_value(it->value());
        }

        mergeData((*it), &cur_child);
    }

    return true;
}
