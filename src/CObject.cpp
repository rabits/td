/**
 * @file    CObject.cpp
 * @date    2010-10-06T12:18:13+0400
 *
 * @author  Rabits <home.rabits@gmail.com>
 * @copyright GNU General Public License, version 3 <http://www.gnu.org/licenses/>
 *
 * This file is a part of Total Destruction project <http://www.rabits.ru/td>
 *
 * @brief   Base object
 *
 *
 */


#include "CObject.h"
#include "CGame.h"

CObject::CObject()
    : m_HasChild(false)
    , m_ObjectName()
    , m_pNode()
    , m_Childrens()
    , m_itChildrens()
    , m_pParent()
    , m_pGame(CGame::getInstance())
    , m_pWorld()
    , m_Position()
    , m_pEntity()
    , m_pBody()
    , m_pShape()
    , m_Mass(0.0)
    , m_pState()
{
}

CObject::~CObject()
{
    clearChildrens();
    m_pParent = NULL;
}

void CObject::setParent(CObject* pParent)
{
    m_pParent = pParent;
}

void CObject::setWorld(CObjectWorld* pWorld)
{
    m_pWorld = pWorld;
}

void CObject::clearChildrens()
{
    m_itChildrens = m_Childrens.begin();
    for( ; m_itChildrens != m_Childrens.end(); m_itChildrens++ )
        delete *m_itChildrens;
    m_Childrens.clear();
    m_HasChild = false;
}

void CObject::clearActions()
{
    std::vector<CAction*>::iterator it = m_Actions.begin();
    for( ; it != m_Actions.end(); it++ )
        delete *it;
    m_Actions.clear();
}

void CObject::attachChild(CObject* pChild)
{
    pChild->setParent(this);
    pChild->init();
    m_Childrens.push_back(pChild);
    m_HasChild = true;
}

std::vector<CObject*> *CObject::getChildrens()
{
    return &m_Childrens;
}
