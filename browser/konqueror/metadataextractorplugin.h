#ifndef METADATAEXTRACTORPLUGIN_H
#define METADATAEXTRACTORPLUGIN_H

#include <KParts/Plugin>
#include <KParts/ReadOnlyPart>
#include <KDE/KUrlLabel>

namespace NepomukMetaDataExtractor {
    namespace Extractor {
        class ExtractorFactory;
    }
}

/**
 * @brief KPart to integrate the meta fetcher into @c Konqueror
 *
 * Shows a little nepomuk icon in the bottom right status par when a supported side is found, left click the
 * icon to start the fetching.
 */
class MetaDataExtractorPlugin: public KParts::Plugin
{
    Q_OBJECT

public:
    explicit MetaDataExtractorPlugin(QObject *parent = 0, const QVariantList &args = QVariantList());
    virtual ~MetaDataExtractorPlugin();
	 
protected slots:
    void urlSwitched();
	void extract();

private slots:
    void lateInitialization();
    void pushDataToNepomuk(const QString &resourceType, const QVariantMap &entry);
	
private:
	KParts::ReadOnlyPart *m_Part;
	KUrlLabel m_icon;

    NepomukMetaDataExtractor::Extractor::ExtractorFactory *m_ef;
    bool extractionInProgress;
	
};

#endif // METADATAEXTRACTORPLUGIN_H
