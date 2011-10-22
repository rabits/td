/**
 * @file    CControlled.h
 * @date    2010-10-07T15:55:38+0400
 *
 * @author  Rabits <home.rabits@gmail.com>
 * @url     http://www.rabits.ru/td
 *
 * @copyright GNU General Public License, version 3 <http://www.gnu.org/licenses/>
 *
 * @brief   Controlled object
 *
 *
 */


#ifndef CCONTROLLED_H
#define CCONTROLLED_H

#include <vector>
#include <string>

class CControlled
{
public:
    CControlled();
    ~CControlled();

    std::vector<std::string>        getAvailableActions();
protected:
    std::vector<std::string>        m_vActions;

private:
};

#endif // CCONTROLLED_H
