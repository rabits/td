/**
 * @file    CObject.cpp
 * @date    2010-10-06T12:18:13+0400
 *
 * @author  Rabits <home.rabits@gmail.com>
 * @copyright GNU General Public License, version 3 <http://www.gnu.org/licenses/>
 *
 * This file is a part of Total Destruction project <http://www.rabits.ru/td>
 *
 * @brief   Base world object
 *
 *
 */


#include "CObject.h"
#include "CGame.h"

CObject::CObject(const char* name, CWorld& pWorld, const Ogre::Vector3& pos, const btScalar mass)
    : CMaster(name)
    , m_pNode()
    , m_HasChild(false)
    , m_Childrens()
    , m_itChildrens()
    , m_pParent()
    , m_pGame(CGame::getInstance())
    , m_pWorld(&pWorld)
    , m_Position(pos)
    , m_pEntity()
    , m_pBody()
    , m_pShape()
    , m_Mass(mass)
    , m_pState()
{
}

CObject::~CObject()
{
    clearChildrens();
    m_pParent = NULL;
}

void CObject::clearChildrens()
{
    m_itChildrens = m_Childrens.begin();
    for( ; m_itChildrens != m_Childrens.end(); m_itChildrens++ )
        delete *m_itChildrens;
    m_Childrens.clear();
    m_HasChild = false;
}

void CObject::attachChild(CObject* pChild)
{
    pChild->setParent(this);
    pChild->init();
    m_Childrens.push_back(pChild);
    m_HasChild = true;
}

std::vector<CObject*>* CObject::getChildrens()
{
    return &m_Childrens;
}
