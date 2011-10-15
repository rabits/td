#ifndef CDATA_H
#define CDATA_H

#include "config.h"
#include "pugixml/pugixml.hpp"

class CData
{
    public:
        CData();
        virtual ~CData();
    protected:
        pugi::xml_document *m_pData;
    private:
};

#endif // CDATA_H
