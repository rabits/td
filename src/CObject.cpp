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
    : m_bHasChild(false)
    , m_sObjectName()
    , m_pParent(NULL)
    , m_pWorld(NULL)
    , m_pNode(NULL)
    , m_ChildrenList()
    , m_itChildrenList(NULL)
    , m_pGame(NULL)
    , m_position()
    , m_pEntity(NULL)
    , m_pBody(NULL)
    , m_pShape(NULL)
    , m_mass(0.0)
    , m_pState(NULL)
{
    m_ChildrenList.clear();
    m_sObjectName.clear();
}

CObject::~CObject()
{
    clearChildrenList();
    m_pParent = NULL;
    m_sObjectName.clear();
}

void CObject::clearChildrenList()
{
    std::vector<CObject*>::iterator it = m_ChildrenList.begin();
    for( ; it != m_ChildrenList.end(); it++ )
        delete *it;
    m_ChildrenList.clear();
    m_bHasChild = false;
}

void CObject::setParent(CObject* pParent)
{
    m_pParent = pParent;
}

void CObject::setWorld(CObjectWorld* pWorld)
{
    m_pWorld = pWorld;
}

void CObject::setGame(CGame *pGame)
{
    m_pGame = pGame;
}

void CObject::attachChild(CObject* pChild)
{
    pChild->setParent(this);
    pChild->init();
    m_ChildrenList.push_back(pChild);
    m_bHasChild = true;
}

std::vector<CObject*> *CObject::getChildrenList()
{
    return &m_ChildrenList;
}
