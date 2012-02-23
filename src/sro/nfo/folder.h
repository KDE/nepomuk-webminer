#ifndef _NFO_FOLDER_H_
#define _NFO_FOLDER_H_

#include <QtCore/QVariant>
#include <QtCore/QStringList>
#include <QtCore/QUrl>
#include <QtCore/QDate>
#include <QtCore/QTime>
#include <QtCore/QDateTime>

#include "dms-copy/simpleresource.h"

#include "nfo/datacontainer.h"

namespace Nepomuk {
namespace NFO {
/**
 * A folder/directory. Examples of folders include folders on 
 * a filesystem and message folders in a mailbox. 
 */
class Folder : public virtual NFO::DataContainer
{
public:
    Folder(const QUrl& uri = QUrl())
      : SimpleResource(uri), NFO::DataContainer(uri, QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#Folder", QUrl::StrictMode)) {
    }

    Folder(const SimpleResource& res)
      : SimpleResource(res), NFO::DataContainer(res, QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#Folder", QUrl::StrictMode)) {
    }

    Folder& operator=(const SimpleResource& res) {
        SimpleResource::operator=(res);
        addType(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#Folder", QUrl::StrictMode));
        return *this;
    }

protected:
    Folder(const QUrl& uri, const QUrl& type)
      : SimpleResource(uri), NFO::DataContainer(uri, type) {
    }
    Folder(const SimpleResource& res, const QUrl& type)
      : SimpleResource(res), NFO::DataContainer(res, type) {
    }
};
}
}

#endif