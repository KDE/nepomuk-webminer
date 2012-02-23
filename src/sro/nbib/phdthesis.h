#ifndef _NBIB_PHDTHESIS_H_
#define _NBIB_PHDTHESIS_H_

#include <QtCore/QVariant>
#include <QtCore/QStringList>
#include <QtCore/QUrl>
#include <QtCore/QDate>
#include <QtCore/QTime>
#include <QtCore/QDateTime>

#include "dms-copy/simpleresource.h"

#include "nbib/thesis.h"

namespace Nepomuk {
namespace NBIB {
/**
 * A PhD thesis. 
 */
class PhdThesis : public virtual NBIB::Thesis
{
public:
    PhdThesis(const QUrl& uri = QUrl())
      : SimpleResource(uri), NBIB::Publication(uri, QUrl::fromEncoded("http://www.example.com/nbib#PhdThesis", QUrl::StrictMode)), NBIB::Thesis(uri, QUrl::fromEncoded("http://www.example.com/nbib#PhdThesis", QUrl::StrictMode)) {
    }

    PhdThesis(const SimpleResource& res)
      : SimpleResource(res), NBIB::Publication(res, QUrl::fromEncoded("http://www.example.com/nbib#PhdThesis", QUrl::StrictMode)), NBIB::Thesis(res, QUrl::fromEncoded("http://www.example.com/nbib#PhdThesis", QUrl::StrictMode)) {
    }

    PhdThesis& operator=(const SimpleResource& res) {
        SimpleResource::operator=(res);
        addType(QUrl::fromEncoded("http://www.example.com/nbib#PhdThesis", QUrl::StrictMode));
        return *this;
    }

protected:
    PhdThesis(const QUrl& uri, const QUrl& type)
      : SimpleResource(uri), NBIB::Publication(uri, type), NBIB::Thesis(uri, type) {
    }
    PhdThesis(const SimpleResource& res, const QUrl& type)
      : SimpleResource(res), NBIB::Publication(res, type), NBIB::Thesis(res, type) {
    }
};
}
}

#endif