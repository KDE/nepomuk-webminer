#ifndef _NBIB_PRESENTATION_H_
#define _NBIB_PRESENTATION_H_

#include <QtCore/QVariant>
#include <QtCore/QStringList>
#include <QtCore/QUrl>
#include <QtCore/QDate>
#include <QtCore/QTime>
#include <QtCore/QDateTime>

#include "dms-copy/simpleresource.h"

#include "nbib/publication.h"

namespace Nepomuk {
namespace NBIB {
/**
 * Slides for a presentation 
 */
class Presentation : public virtual NBIB::Publication
{
public:
    Presentation(const QUrl& uri = QUrl())
      : SimpleResource(uri), NIE::InformationElement(uri, QUrl::fromEncoded("http://www.example.com/nbib#Presentation", QUrl::StrictMode)), NBIB::Publication(uri, QUrl::fromEncoded("http://www.example.com/nbib#Presentation", QUrl::StrictMode)) {
    }

    Presentation(const SimpleResource& res)
      : SimpleResource(res), NIE::InformationElement(res, QUrl::fromEncoded("http://www.example.com/nbib#Presentation", QUrl::StrictMode)), NBIB::Publication(res, QUrl::fromEncoded("http://www.example.com/nbib#Presentation", QUrl::StrictMode)) {
    }

    Presentation& operator=(const SimpleResource& res) {
        SimpleResource::operator=(res);
        addType(QUrl::fromEncoded("http://www.example.com/nbib#Presentation", QUrl::StrictMode));
        return *this;
    }

protected:
    Presentation(const QUrl& uri, const QUrl& type)
      : SimpleResource(uri), NIE::InformationElement(uri, type), NBIB::Publication(uri, type) {
    }
    Presentation(const SimpleResource& res, const QUrl& type)
      : SimpleResource(res), NIE::InformationElement(res, type), NBIB::Publication(res, type) {
    }
};
}
}

#endif
