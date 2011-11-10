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

CUser::CUser()
    : CData("User")
    , m_Name()
    , m_pWorld()
    , m_Nervs()
    , m_NervMaps()
    , m_pCurrentNervMap()
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
    , m_pCurrentNervMap()
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

    }
    else
        log_warn("Not found nervs for user %s", m_Name.c_str());

    // Parsing mappings
    pugi::xml_node mappings = user_config.child("control").child("mappings");
    if( mappings )
    {

    }
    else
        log_warn("Not found nerv mappings for user %s", m_Name.c_str());
}

void CUser::update(const Ogre::FrameEvent& evt)
{
}

void CUser::addNerv(const char *name, int id)
{
    // Subscribe nerv in InputHandler and add to user list
}

void CUser::delNerv(int id)
{
    // Unsubscribe from InputHandler and delete from list
}

NervMap* CUser::currentNervMap(const char *name)
{
}

void CUser::setNervMapping(int nerv_id, CAction *action)
{
}

bool CUser::nervSignal(CInputEvent &event)
{
}
