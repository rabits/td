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

#include "CData.h"
#include "CMaster.h"

class CSynaps;
class CSignal;
class CObjectKernel;

typedef std::multimap<uint, CSynaps*> SynapsMap; ///< SignalId->Action multimap
typedef std::map<std::string, SynapsMap> NervMaps; ///< Name->SynapsMap map
typedef std::map<uint, std::string> Nervs; ///< Nervs of current user (for unsubscribe and list)

/** @brief Self user
 *
 * Can control objects
 */
class CUser
    : CData
    , public CMaster
{
public:
    /** @brief Constructor of user
     *
     */
    CUser();

    /** @brief Constructor with data file path
     *
     * @param data_file - path to User xml data
     */
    CUser(const char* data_file);

    /** @brief Destructor of user
     */
    ~CUser();


    /** @brief Initialise member vars from xml data
     *
     * @param data_file - path to user datafile
     */
    void init(const char* data_file);

    /** @brief Updating user state
     *
     * @param time_since_last_frame
     */
    void update(const Ogre::Real time_since_last_frame);


    /** @brief Create new nerv
     *
     * @param name - name of nerv
     * @param id - id of need event
     */
    void addNerv(const char* name, uint id);

    /** @brief Remove nerv with specified id
     *
     * @param id - id of nerv
     */
    void delNerv(uint id);

    /** @brief Select current nerv map
     *
     * @return NervMaps::iterator*
     */
    SynapsMap* currentSynapsMap() { return &m_NervMaps[m_CurrentSynapsMap.c_str()]; }

    /** @brief Select or create new nerv map with specified name
     *
     * @param name - name of map
     * @return NervMaps::iterator*
     */
    SynapsMap* currentSynapsMap(const char* name) { m_CurrentSynapsMap = name; return &m_NervMaps[name]; }

    /** @brief Map nerv to action in selected nerv map
     *
     * @param nerv_id - nerv identificator
     * @param synaps - synaps with need action
     */
    void setSynapsMapping(uint nerv_id, CSynaps* synaps);

    /** @brief Recieve, modify and route to controlled object signal from Sensor
     *
     * @param sig - input event from human
     * @return bool
     */
    bool nervSignal(CSignal& sig);


    /** @brief Set main kernel object
     *
     * @param kernel
     *
     */
    void kernel(CObjectKernel* kernel);

    /** @brief Get main kernel object
     *
     * @return CObjectKernel*
     *
     */
    CObjectKernel* kernel() { return m_pKernel; }


    /** @brief Saving user configs and state
     */
    void save();

protected:
    Ogre::Camera*                      m_pCamera; ///< User's camera

    Nervs                              m_Nervs; ///< Subscribed events
    NervMaps                           m_NervMaps; ///< Lists with mappings of nervs to actions
    std::string                        m_CurrentSynapsMap; ///< Current selected map

    CObjectKernel*                     m_pKernel; ///< User's main object in world

private:
    /** @brief Fake copy constructor
     *
     * @param obj
     *
     * @todo create copy constructor
     */
    CUser(const CUser& obj);
    /** @brief Fake eq operator
     *
     * @param obj
     *
     * @toto create eq copy operator
     */
    CUser& operator=(const CUser& obj);

    /** @brief Fake initialize object
     *
     */
    void init() {  }
};

#endif // CUSER_H
