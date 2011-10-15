/** // CCONTROLLED_H
 *  --------------------------------------
 *   Total Destruction Project
 *   Rabits <home.rabits@gmail.com>  2010
 *  --------------------------------------
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
