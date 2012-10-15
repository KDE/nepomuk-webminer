#ifndef METADATAEXTRACTORKCM_H
#define METADATAEXTRACTORKCM_H

#include <KDE/KCModule>

namespace NepomukMetaDataExtractor {

class ConfigFetcher;
class PluginList;
class ConfigService;

namespace Extractor
{
    class ExtractorFactory;
}

/**
 * @brief KCM class to show and manage all config widgets for the MetaData Extractor
 */
class MetaDataExtractorKCM : public KCModule
{
    Q_OBJECT
public:
    explicit MetaDataExtractorKCM(QWidget *parent, const QVariantList &list);

    void save();
    void load();

private:
    Extractor::ExtractorFactory *m_ef;
    ConfigFetcher *cfd;
    PluginList *pl;
    ConfigService *csd;
};
}

#endif // METADATAEXTRACTORKCM_H
