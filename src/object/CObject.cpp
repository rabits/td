/** // COBJECT_CPP
 *  --------------------------------------
 *   Total Destruction Project
 *   Rabits <home.rabits@gmail.com>  2010
 *  --------------------------------------
 */

#include "object/CObject.h"
#include "app/CGame.h"

CObject::CObject()
    : m_pParent(NULL),
    m_pWorld(NULL)
{
    m_ChildrenList.clear();
    m_sObjectName.clear();
    m_bHasChild = false;
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
