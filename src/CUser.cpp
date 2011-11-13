/**
 * @file    CUser.cpp
 * @date    2010-09-26T13:44:21+0400
 *
 * @author  Rabits <home.rabits@gmail.com>
 * @copyright GNU General Public License, version 3 <http://www.gnu.org/licenses/>
 *
 * This file is a part of Total Destruction project <http://www.rabits.ru/td>
 *
 * @brief   User
 *
 *
 */

#include "CUser.h"

#include "CGame.h"
#include "Nerv/CSensor.h"
#include "Nerv/CAction.h"

CUser::CUser()
    : CData("User")
    , m_Name()
    , m_pWorld()
    , m_Nervs()
    , m_NervMaps()
    , m_CurrentNervMap()
    , m_pControlledObject()
{
    // Loading default skeleton config
    fs::path skeleton_path(CGame::getInstance()->getPrefix());
    skeleton_path /= fs::path(CGame::getInstance()->path("root_data")) / fs::path(CGame::getInstance()->path("users"));
    skeleton_path /= fs::path("skeleton") / fs::path("user.xml");
    init(skeleton_path.c_str());
}

CUser::CUser(const char* data_file)
    : CData("User")
    , m_Name()
    , m_pWorld()
    , m_Nervs()
    , m_NervMaps()
    , m_CurrentNervMap()
    , m_pControlledObject()
{
    init(data_file);
}

CUser::~CUser()
{
}

void CUser::init(const char* data_file)
{
    loadData(data_file);

    pugi::xml_node user_config = m_data.child("config");

    // Set name
    m_Name = user_config.child_value("name");

    // Controlling set

    // Parsing nervs
    pugi::xml_node nervs = user_config.child("control").child("nervs");
    if( nervs )
    {
        for( auto nerv = nervs.begin(); nerv != nervs.end(); nerv++ )
            addNerv(nerv->attribute("name").value(), nerv->attribute("id").as_int());
    }
    else
        log_warn("Not found nervs for user %s", m_Name.c_str());

    // Parsing mappings
    pugi::xml_node mapping = user_config.child("control").child("mapping");
    if( mapping )
    {
        CAction* act;
        for( auto action = mapping.begin(); action != mapping.end(); action++ )
        {
            log_debug("Found %s: %d", action->attribute("object").value(), action->attribute("id").as_int());

            if( std::strcmp(action->attribute("object").value(), "Game") == 0 )
            {
                act = CGame::getInstance()->getAction(action->attribute("name").value());
                if( act != NULL )
                {
                    log_debug("\tmapping %d->%s", action->attribute("id").as_int(), act->name());
                    setNervMapping(action->attribute("id").as_int(), act);
                }
                else
                    log_warn("\tnot found action %s", action->attribute("name").value());
            }
            else
                log_warn("\tnot found key \"%s\"", action->attribute("object").value());
        }
    }
    else
        log_warn("Not found nerv mappings for user %s", m_Name.c_str());
}

void CUser::update(const Ogre::FrameEvent& evt)
{
}

void CUser::addNerv(const char* name, unsigned int id)
{
    log_debug("Creating nerv %s %d", name, id);
    CGame::getInstance()->inputHandler()->addSubscribe(id, this);
    m_Nervs[id] = name;
}

void CUser::delNerv(unsigned int id)
{
    CGame::getInstance()->inputHandler()->delSubscribe(id);
    m_Nervs.erase(id);
}

bool CUser::nervSignal(CSignal &sig)
{
    log_debug("USER %s: Recieved signal %d: %f", m_Name.c_str(), sig.id(), sig.value());
    std::pair<NervMap::iterator, NervMap::iterator> itp = m_NervMaps[m_CurrentNervMap.c_str()].equal_range(sig.id());
    for( NervMap::iterator it = itp.first; it != itp.second; ++it )
        it->second->action(sig);

    return true;
}

void CUser::setNervMapping(unsigned int nerv_id, CAction const* action)
{
    m_NervMaps[m_CurrentNervMap.c_str()].insert(std::pair<unsigned int, CAction const*>(nerv_id, action));
}

void CUser::save()
{
}
