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
#include "Nerv/CSignal.h"
#include "Nerv/CSynaps.h"
#include "CEye.h"

CUser::CUser()
    : CData("User")
    , CMaster("")
    , m_pCamera(NULL)
    , m_Nervs()
    , m_NervMaps()
    , m_CurrentSynapsMap()
    , m_pKernel(NULL)
{
    // Loading default skeleton config
    fs::path skeleton_path(CGame::getInstance()->getPrefix());
    skeleton_path /= fs::path(CGame::getInstance()->path("root_data")) / fs::path(CGame::getInstance()->path("users"));
    skeleton_path /= fs::path("skeleton") / fs::path("user.xml");
    init(skeleton_path.c_str());
}

CUser::CUser(const char* data_file)
    : CData("User")
    , CMaster("")
    , m_pCamera(NULL)
    , m_Nervs()
    , m_NervMaps()
    , m_CurrentSynapsMap()
    , m_pKernel(NULL)
{
    init(data_file);
}

CUser::~CUser()
{
    for( NervMaps::iterator its = m_NervMaps.begin(); its != m_NervMaps.end(); ++its )
    {
        for( SynapsMap::iterator it = its->second.begin(); it != its->second.end(); ++it )
            delete it->second;
    }

    CGame::getInstance()->m_pSceneMgr->destroyCamera(m_pCamera);
}

void CUser::init(const char* data_file)
{
    loadData(data_file);

    pugi::xml_node user_config = m_data.child("config");

    // Set name
    name(user_config.child_value("name"));

    // Create user's personal camera
    m_pCamera = CGame::getInstance()->m_pSceneMgr->createCamera(Common::Name::next(name()));
    // For debug attach cameta to first object in world
    // @todo remove this debug
    //m_pEye->style(CEye::CS_ORBIT);
    //std::vector<CObject*>* childrens = m_pWorld->getChildrens();
    //m_pEye->target(childrens->front()->node());

    // Controlling set

    // Parsing nervs
    pugi::xml_node nervs = user_config.child("control").child("nervs");
    if( nervs )
    {
        for( auto nerv = nervs.begin(); nerv != nervs.end(); nerv++ )
            addNerv(nerv->attribute("name").value(), static_cast<uint>(nerv->attribute("id").as_int()));
    }
    else
        log_warn("Not found nervs for user %s", name().c_str());

    // Parsing mappings
    pugi::xml_node mapping = user_config.child("control").child("mapping");
    if( mapping )
    {
        CControlled* obj = NULL;
        for( auto action = mapping.begin(); action != mapping.end(); action++ )
        {
            log_debug("Found %s: %d", action->attribute("object").value(), action->attribute("id").as_int());

            if( std::strcmp(action->attribute("object").value(), "Game") == 0 )
                obj = CGame::getInstance();
            else if( action->attribute("object").as_int() > 0 )
                obj = CControlled::getControlledObject(static_cast<uint>(action->attribute("object").as_int()));
            else
                log_warn("\tnot found key \"%s\"", action->attribute("object").value());

            if( obj != NULL )
            {
                CAction* act = obj->getAction(action->attribute("name").value());
                if( act != NULL )
                {
                    uint id = static_cast<uint>(action->attribute("id").as_int());
                    log_debug("\tmapping %d->%s (sens:%f, limit:%f)", id, act->name(),
                              action->attribute("sensitivity").as_float(), action->attribute("limit").as_float());
                    setSynapsMapping(id, new CSynaps(id, act, action->attribute("sensitivity").as_float(),
                                                     action->attribute("limit").as_float()));
                }
                else
                    log_warn("\tnot found action %s", action->attribute("name").value());
            }
            else
                log_warn("\tnot found object %s", action->attribute("object").value());
        }
    }
    else
        log_warn("Not found nerv mappings for user %s", name().c_str());
}

void CUser::update(const Ogre::Real)
{
}

void CUser::addNerv(const char* name, uint id)
{
    log_debug("Creating nerv %s %d", name, id);
    CGame::getInstance()->inputHandler()->addSubscribe(id, this);
    m_Nervs[id] = name;
}

void CUser::delNerv(uint id)
{
    CGame::getInstance()->inputHandler()->delSubscribe(id);
    m_Nervs.erase(id);
}

bool CUser::nervSignal(CSignal& sig)
{
    log_debug("USER %s: Recieved signal %d: %f", name().c_str(), sig.id(), sig.value());
    std::pair<SynapsMap::iterator, SynapsMap::iterator> itp = m_NervMaps[m_CurrentSynapsMap.c_str()].equal_range(sig.id());
    for( SynapsMap::iterator it = itp.first; it != itp.second; ++it )
        it->second->route(sig);

    return true;
}

void CUser::setSynapsMapping(uint nerv_id, CSynaps* synaps)
{
    m_NervMaps[m_CurrentSynapsMap.c_str()].insert(std::pair<uint, CSynaps*>(nerv_id, synaps));
}

void CUser::kernel(CObjectKernel* kernel)
{
    if( kernel != m_pKernel )
    {
        if( m_pKernel != NULL )
            m_pKernel->camera(NULL);

        if( kernel != NULL )
            kernel->camera(m_pCamera);

        m_pKernel = kernel;
    }
}

void CUser::save()
{
    // @todo Realize this function
}
