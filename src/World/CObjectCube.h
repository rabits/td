/**
 * @file    CObjectCube.h
 * @date    2010-10-06T12:18:13+0400
 *
 * @author  Rabits <home.rabits@gmail.com>
 * @copyright GNU General Public License, version 3 <http://www.gnu.org/licenses/>
 *
 * This file is a part of Total Destruction project <http://www.rabits.ru/td>
 *
 * @brief   Cube world object
 *
 *
 */

#ifndef COBJECTCUBE_H_INCLUDED
#define COBJECTCUBE_H_INCLUDED

#include "CObject.h"

class CGame;

/** @brief It is meat of world
 *
 * World consists of cubes. Cubes has gravity and can travel in world.
 */
class CObjectCube
    : public CObject
{
public:
    /** @brief Sizes of cubes
     */
    enum Cube_Size
    {
        CUBE     = 1,   ///< Simple Cube
        ACUBE    = 10,  ///< A-Cube
        BCUBE    = 20,  ///< B-Cube
        CCUBE    = 30,  ///< C-Cube
        DCUBE    = 40,  ///< D-Cube
        ECUBE    = 50,  ///< E-Cube
        SCUBE    = 60,  ///< Super-Cube
        TCUBE    = 70,  ///< Terra-Cube
        XCUBE    = 80,  ///< Xen-Cube
        YCUBE    = 90,  ///< Yet-Cube
        ZCUBE    = 100  ///< Zet-Cube
    };

    /** @brief Constructor
     *
     * @param pWorld CWorld&
     * @param size CObjectCube::Cube_Size (default CObjectCube::ACUBE)
     * @param pos const Ogre::Vector3& (default Ogre::Vector3(0.0f))
     */
    CObjectCube(CWorld& pWorld, CObjectCube::Cube_Size size = CObjectCube::ACUBE, const Ogre::Vector3& pos = Ogre::Vector3(0.0f));

    /** @brief Simple destructor
     */
    ~CObjectCube();


    /** @brief Set cube size
     *
     * @param eCubeSize CObjectCube::Cube_Size
     * @return void
     *
     */
    void setCubeSize(CObjectCube::Cube_Size eCubeSize);

    /** @brief Update object data, it may be animation or just object translation or so on
     *
     * @return void
     *
     */
    void update();

    /** @brief Initialize object
     *
     * @return void
     *
     */
    void init();

    /** @brief Setting up object state
     *
     * @param State int
     * @return void
     *
     */
    void setObjectState(int State);

private:
    CObjectCube::Cube_Size  m_cubeSize; ///< Size of cube
    int                     m_gravityVolumes[6]; ///< Ids of connected gravity elements
};


#endif // COBJECTCUBE_H_INCLUDED
