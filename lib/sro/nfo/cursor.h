#ifndef _NFO_CURSOR_H_
#define _NFO_CURSOR_H_

#include <QtCore/QVariant>
#include <QtCore/QStringList>
#include <QtCore/QUrl>
#include <QtCore/QDate>
#include <QtCore/QTime>
#include <QtCore/QDateTime>

#include "dms-copy/simpleresource.h"

#include "nfo/rasterimage.h"

namespace Nepomuk {
namespace NFO {
/**
 * A Cursor. 
 */
class Cursor : public virtual NFO::RasterImage
{
public:
    Cursor(const QUrl& uri = QUrl())
      : SimpleResource(uri), NFO::Media(uri, QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#Cursor", QUrl::StrictMode)), NFO::Visual(uri, QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#Cursor", QUrl::StrictMode)), NFO::Image(uri, QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#Cursor", QUrl::StrictMode)), NFO::RasterImage(uri, QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#Cursor", QUrl::StrictMode)) {
    }

    Cursor(const SimpleResource& res)
      : SimpleResource(res), NFO::Media(res, QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#Cursor", QUrl::StrictMode)), NFO::Visual(res, QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#Cursor", QUrl::StrictMode)), NFO::Image(res, QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#Cursor", QUrl::StrictMode)), NFO::RasterImage(res, QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#Cursor", QUrl::StrictMode)) {
    }

    Cursor& operator=(const SimpleResource& res) {
        SimpleResource::operator=(res);
        addType(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#Cursor", QUrl::StrictMode));
        return *this;
    }

protected:
    Cursor(const QUrl& uri, const QUrl& type)
      : SimpleResource(uri), NFO::Media(uri, type), NFO::Visual(uri, type), NFO::Image(uri, type), NFO::RasterImage(uri, type) {
    }
    Cursor(const SimpleResource& res, const QUrl& type)
      : SimpleResource(res), NFO::Media(res, type), NFO::Visual(res, type), NFO::Image(res, type), NFO::RasterImage(res, type) {
    }
};
}
}

#endif