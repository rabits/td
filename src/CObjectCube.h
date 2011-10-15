/** // COBJECTCUBE_H
 *  --------------------------------------
 *   Total Destruction Project
 *   Rabits <home.rabits@gmail.com>  2010
 *  --------------------------------------
 */

#ifndef COBJECTCUBE_H_INCLUDED
#define COBJECTCUBE_H_INCLUDED

#include "CObject.h"

class CGame;

class CObjectCube : public CObject
{
public:
    enum Cube_Size
    {
        CUBE     = 1,
        ACUBE    = 10,
        BCUBE    = 20,
        CCUBE    = 30,
        DCUBE    = 40,
        ECUBE    = 50,
        SCUBE    = 60,
        TCUBE    = 70,
        XCUBE    = 80,
        YCUBE    = 90,
        ZCUBE    = 100,
    };

    CObjectCube();
    CObjectCube(CGame &pGame, CObjectWorld &pWorld, CObjectCube::Cube_Size size = CObjectCube::ACUBE, const Ogre::Vector3 &pos = Ogre::Vector3(0.0f));
    ~CObjectCube();

    void setCubeSize(Cube_Size eCubeSize);
    // Update object data, it may be animation or just object translation or so on.
    void update();
    // Initialize object
    void init();
    // Seeting up object state.
    void setObjectState(int State);
private:
    CObjectCube::Cube_Size m_cubeSize;
    int m_gravityVolumes[6];
};


#endif // COBJECTCUBE_H_INCLUDED
