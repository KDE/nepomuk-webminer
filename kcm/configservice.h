#ifndef CONFIGSERVICE_H
#define CONFIGSERVICE_H

#include <QWidget>

namespace Ui {
    class ConfigService;
}

class QDBusServiceWatcher;

namespace NepomukMetaDataExtractor {

/**
 * @brief The ConfigService class is used to enable/disable and setup the Nepomuk2::Service for the metadata extractor.
 */
class ConfigService : public QWidget
{
    Q_OBJECT
    
public:
    explicit ConfigService(QWidget *parent = 0);
    ~ConfigService();
    
signals:
    void configChanged(bool changed);

public slots:
    void saveConfig();
    void resetConfig();

private slots:
    void serviceEnabled(bool enabled);

    void serviceRegistered();
    void serviceUnregistered();
    void changeSettings();

private:
    Ui::ConfigService *ui;
    QDBusServiceWatcher *m_watcher;
};
}
#endif // CONFIGSERVICE_H
