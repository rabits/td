/** // COBJECTKERNEL_H
 *  --------------------------------------
 *   Total Destruction Project
 *   Rabits <home.rabits@gmail.com>  2010
 *  --------------------------------------
 */

#ifndef COBJECTKERNEL_H_INCLUDED
#define COBJECTKERNEL_H_INCLUDED

#include "config.h"

#include "CControlled.h"
#include "CObject.h"

class CObjectKernel
    : public CObject
    , public CControlled
{
public:
    CObjectKernel();
    CObjectKernel(CGame &pGame, CObjectWorld & pWorld, const btScalar mass, const Ogre::Vector3 &pos = Ogre::Vector3(0.0f));
    ~CObjectKernel();

    // Update object data, it may be animation or just object translation or so on.
    void update();
    // Initialize object
    void init();
    // Seeting up object state.
    void setObjectState(int State);

private:
};

#endif // COBJECTKERNEL_H_INCLUDED
