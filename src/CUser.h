/**
 * @file    CUser.h
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

#ifndef CUSER_H
#define CUSER_H

#include "Common.h"

#include <OGRE/Ogre.h>

#include "Nerv/CControlled.h"
#include "Nerv/CSignal.h"
#include "CData.h"
#include "CWorld.h"

typedef std::multimap<unsigned int, CAction const*> NervMap; ///< SignalId->Action multimap
typedef std::map<std::string, NervMap> NervMaps; ///< Name->NervMap map
typedef std::map<unsigned int, std::string> Nervs; ///< Nervs of current user (for unsubscribe and list)

/** @brief Self user
 *
 * Can control objects
 */
class CUser
    : CData
{
public:
    /** @brief Constructor of user
     */
    CUser();

    /** @brief Constructor with data file path
     */
    CUser(const char* data_file);

    /** @brief Destructor of user
     */
    ~CUser();


    /** @brief Initialise member vars from xml data
     *
     * @param data_file const char*
     */
    void init(const char* data_file);

    /** @brief Set user world
     *
     * @param world CWorld*
     * @return void
     *
     */
    void world(CWorld* world) { m_pWorld = world; }

    /** @brief Get user world
     *
     * @return CWorld*
     *
     */
    CWorld* world(){ return m_pWorld; }

    /** @brief Updating user state
     *
     * @param evt Ogre::FrameEvent&
     */
    void update(const Ogre::FrameEvent& evt);


    /** @brief Create new nerv
     *
     * @param name const char* - name of nerv
     * @param id int - id of need event
     */
    void addNerv(const char* name, unsigned int id);

    /** @brief Remove nerv with specified id
     *
     * @param id int - id of nerv
     */
    void delNerv(unsigned int id);

    /** @brief Select current nerv map
     *
     * @return NervMaps::iterator*
     */
    NervMap* currentNervMap(){ return &m_NervMaps[m_CurrentNervMap.c_str()]; }

    /** @brief Select or create new nerv map with specified name
     *
     * @param name const char* - name of map
     * @return NervMaps::iterator*
     */
    NervMap* currentNervMap(const char* name){ m_CurrentNervMap = name; return &m_NervMaps[name]; }

    /** @brief Map nerv to action in selected nerv map
     *
     * @param nerv_id int - nerv identificator
     * @param action CAction* - action of need object
     */
    void setNervMapping(unsigned int nerv_id, CAction const* action);

    /** @brief Recieve, modify and route to controlled object signal from Sensor
     *
     * @param sig CSignal& - input event from human
     * @return bool
     */
    bool nervSignal(CSignal& sig);


    /** @brief Set object, which will be controlled
     *
     * @param obj CControlled*
     * @return void
     *
     */
    void controlledObject(CControlled* obj){ m_pControlledObject = obj; }

    /** @brief Get object, which will be controlled
     *
     * @return CControlled*
     *
     */
    CControlled* controlledObject(){ return m_pControlledObject; }


    /** @brief Change name of user
     *
     * @param name std::string& - new name
     */
    void name(std::string& name){ m_Name = name; }

    /** @brief Gets name of user
     *
     * @return std::string& - curent name of user
     */
    std::string& name(){ return m_Name; }

    /** @brief Saving user configs and state
     */
    void save();

protected:
    std::string                        m_Name; ///< User name
    CWorld*                            m_pWorld; ///< User's world

    Nervs                              m_Nervs; ///< Subscribed events
    NervMaps                           m_NervMaps; ///< Lists with mappings of nervs to actions
    std::string                        m_CurrentNervMap; ///< Current selected map

    CControlled*                       m_pControlledObject; ///< Object under control

private:
    /** @brief Fake copy constructor
     *
     * @param obj
     */
    CUser(const CUser& obj);
    /** @brief Fake eq operator
     *
     * @param obj
     */
    CUser& operator=(const CUser& obj);
};

#endif // CUSER_H
