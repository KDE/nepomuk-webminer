#ifndef _NIE_INFORMATIONELEMENT_H_
#define _NIE_INFORMATIONELEMENT_H_

#include <QtCore/QVariant>
#include <QtCore/QStringList>
#include <QtCore/QUrl>
#include <QtCore/QDate>
#include <QtCore/QTime>
#include <QtCore/QDateTime>

#include <nepomuk2/simpleresource.h>

namespace Nepomuk2 {
namespace NIE {
/**
 * A unit of content the user works with. This is a superclass for 
 * all interpretations of a DataObject. 
 */
class InformationElement : public virtual Nepomuk2::SimpleResource
{
public:
    InformationElement(const QUrl& uri = QUrl())
      : SimpleResource(uri) {
        addType(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#InformationElement", QUrl::StrictMode));
    }

    InformationElement(const SimpleResource& res)
      : SimpleResource(res) {
        addType(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#InformationElement", QUrl::StrictMode));
    }

    InformationElement& operator=(const SimpleResource& res) {
        SimpleResource::operator=(res);
        addType(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#InformationElement", QUrl::StrictMode));
        return *this;
    }

    /**
     * Get property http://www.semanticdesktop.org/ontologies/2007/08/15/nao#userVisible. 
     * Mark a property, class, or even resource as user visible or not. 
     * Non-user-visible entities should never be presented to the 
     * user. By default everything is user-visible. 
     */
    bool userVisible() const {
        bool value;
        if(contains(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#userVisible", QUrl::StrictMode)))
            value = property(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#userVisible", QUrl::StrictMode)).first().value<bool>();
        return value;
    }

    /**
     * Set property http://www.semanticdesktop.org/ontologies/2007/08/15/nao#userVisible. 
     * Mark a property, class, or even resource as user visible or not. 
     * Non-user-visible entities should never be presented to the 
     * user. By default everything is user-visible. 
     */
    void setUserVisible(const bool& value) {
        QVariantList values;
        values << value;
        setProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#userVisible", QUrl::StrictMode), values);
    }

    /**
     * Add value to property http://www.semanticdesktop.org/ontologies/2007/08/15/nao#userVisible. 
     * Mark a property, class, or even resource as user visible or not. 
     * Non-user-visible entities should never be presented to the 
     * user. By default everything is user-visible. 
     */
    void addUserVisible(const bool& value) {
        addProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#userVisible", QUrl::StrictMode), value);
    }

    /**
     * Get property http://www.semanticdesktop.org/ontologies/2007/08/15/nao#prefLabel. 
     * A preferred label for a resource 
     */
    QString prefLabel() const {
        QString value;
        if(contains(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#prefLabel", QUrl::StrictMode)))
            value = property(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#prefLabel", QUrl::StrictMode)).first().value<QString>();
        return value;
    }

    /**
     * Set property http://www.semanticdesktop.org/ontologies/2007/08/15/nao#prefLabel. 
     * A preferred label for a resource 
     */
    void setPrefLabel(const QString& value) {
        QVariantList values;
        values << value;
        setProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#prefLabel", QUrl::StrictMode), values);
    }

    /**
     * Add value to property http://www.semanticdesktop.org/ontologies/2007/08/15/nao#prefLabel. 
     * A preferred label for a resource 
     */
    void addPrefLabel(const QString& value) {
        addProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#prefLabel", QUrl::StrictMode), value);
    }

    /**
     * Get property http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#sideChannels. 
     * Number of side channels 
     */
    qint64 sideChannels() const {
        qint64 value;
        if(contains(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#sideChannels", QUrl::StrictMode)))
            value = property(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#sideChannels", QUrl::StrictMode)).first().value<qint64>();
        return value;
    }

    /**
     * Set property http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#sideChannels. 
     * Number of side channels 
     */
    void setSideChannels(const qint64& value) {
        QVariantList values;
        values << value;
        setProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#sideChannels", QUrl::StrictMode), values);
    }

    /**
     * Add value to property http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#sideChannels. 
     * Number of side channels 
     */
    void addSideChannels(const qint64& value) {
        addProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#sideChannels", QUrl::StrictMode), value);
    }

    /**
     * Get property http://www.w3.org/2003/01/geo/wgs84_pos#long. 
     */
    double FAILlong() const {
        double value;
        if(contains(QUrl::fromEncoded("http://www.w3.org/2003/01/geo/wgs84_pos#long", QUrl::StrictMode)))
            value = property(QUrl::fromEncoded("http://www.w3.org/2003/01/geo/wgs84_pos#long", QUrl::StrictMode)).first().value<double>();
        return value;
    }

    /**
     * Set property http://www.w3.org/2003/01/geo/wgs84_pos#long. 
     */
    void setLong(const double& value) {
        QVariantList values;
        values << value;
        setProperty(QUrl::fromEncoded("http://www.w3.org/2003/01/geo/wgs84_pos#long", QUrl::StrictMode), values);
    }

    /**
     * Add value to property http://www.w3.org/2003/01/geo/wgs84_pos#long. 
     */
    void addLong(const double& value) {
        addProperty(QUrl::fromEncoded("http://www.w3.org/2003/01/geo/wgs84_pos#long", QUrl::StrictMode), value);
    }

    /**
     * Get property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#copyright. 
     * Content copyright 
     */
    QStringList copyrights() const {
        QStringList value;
        foreach(const QVariant& v, property(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#copyright", QUrl::StrictMode)))
            value << v.value<QString>();
        return value;
    }

    /**
     * Set property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#copyright. 
     * Content copyright 
     */
    void setCopyrights(const QStringList& value) {
        QVariantList values;
        foreach(const QString& v, value)
            values << v;
        setProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#copyright", QUrl::StrictMode), values);
    }

    /**
     * Add value to property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#copyright. 
     * Content copyright 
     */
    void addCopyright(const QString& value) {
        addProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#copyright", QUrl::StrictMode), value);
    }

    /**
     * Get property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#hasLogicalPart. 
     * Generic property used to express 'logical' containment relationships 
     * between InformationElements. NIE extensions are encouraged 
     * to provide more specific subproperties of this one. It is advisable 
     * for actual instances of InformationElement to use those specific 
     * subproperties. Note the difference between 'physical' containment 
     * (hasPart) and logical containment (hasLogicalPart) 
     */
    QList<QUrl> logicalParts() const {
        QList<QUrl> value;
        foreach(const QVariant& v, property(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#hasLogicalPart", QUrl::StrictMode)))
            value << v.value<QUrl>();
        return value;
    }

    /**
     * Set property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#hasLogicalPart. 
     * Generic property used to express 'logical' containment relationships 
     * between InformationElements. NIE extensions are encouraged 
     * to provide more specific subproperties of this one. It is advisable 
     * for actual instances of InformationElement to use those specific 
     * subproperties. Note the difference between 'physical' containment 
     * (hasPart) and logical containment (hasLogicalPart) 
     */
    void setLogicalParts(const QList<QUrl>& value) {
        QVariantList values;
        foreach(const QUrl& v, value)
            values << v;
        setProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#hasLogicalPart", QUrl::StrictMode), values);
    }

    /**
     * Add value to property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#hasLogicalPart. 
     * Generic property used to express 'logical' containment relationships 
     * between InformationElements. NIE extensions are encouraged 
     * to provide more specific subproperties of this one. It is advisable 
     * for actual instances of InformationElement to use those specific 
     * subproperties. Note the difference between 'physical' containment 
     * (hasPart) and logical containment (hasLogicalPart) 
     */
    void addLogicalPart(const QUrl& value) {
        addProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#hasLogicalPart", QUrl::StrictMode), value);
    }

    /**
     * Get property http://www.semanticdesktop.org/ontologies/2007/08/15/nao#altSymbol. 
     * An alternative symbol representation for a resource 
     */
    QList<QUrl> altSymbols() const {
        QList<QUrl> value;
        foreach(const QVariant& v, property(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#altSymbol", QUrl::StrictMode)))
            value << v.value<QUrl>();
        return value;
    }

    /**
     * Set property http://www.semanticdesktop.org/ontologies/2007/08/15/nao#altSymbol. 
     * An alternative symbol representation for a resource 
     */
    void setAltSymbols(const QList<QUrl>& value) {
        QVariantList values;
        foreach(const QUrl& v, value)
            values << v;
        setProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#altSymbol", QUrl::StrictMode), values);
    }

    /**
     * Add value to property http://www.semanticdesktop.org/ontologies/2007/08/15/nao#altSymbol. 
     * An alternative symbol representation for a resource 
     */
    void addAltSymbol(const QUrl& value) {
        addProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#altSymbol", QUrl::StrictMode), value);
    }

    /**
     * Get property http://www.semanticdesktop.org/ontologies/2007/08/15/nao#prefSymbol. 
     * A unique preferred symbol representation for a resource 
     */
    QUrl prefSymbol() const {
        QUrl value;
        if(contains(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#prefSymbol", QUrl::StrictMode)))
            value = property(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#prefSymbol", QUrl::StrictMode)).first().value<QUrl>();
        return value;
    }

    /**
     * Set property http://www.semanticdesktop.org/ontologies/2007/08/15/nao#prefSymbol. 
     * A unique preferred symbol representation for a resource 
     */
    void setPrefSymbol(const QUrl& value) {
        QVariantList values;
        values << value;
        setProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#prefSymbol", QUrl::StrictMode), values);
    }

    /**
     * Add value to property http://www.semanticdesktop.org/ontologies/2007/08/15/nao#prefSymbol. 
     * A unique preferred symbol representation for a resource 
     */
    void addPrefSymbol(const QUrl& value) {
        addProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#prefSymbol", QUrl::StrictMode), value);
    }

    /**
     * Get property http://www.semanticdesktop.org/ontologies/2007/08/15/nao#personalIdentifier. 
     * Defines a personal string identifier for a resource 
     */
    QStringList personalIdentifiers() const {
        QStringList value;
        foreach(const QVariant& v, property(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#personalIdentifier", QUrl::StrictMode)))
            value << v.value<QString>();
        return value;
    }

    /**
     * Set property http://www.semanticdesktop.org/ontologies/2007/08/15/nao#personalIdentifier. 
     * Defines a personal string identifier for a resource 
     */
    void setPersonalIdentifiers(const QStringList& value) {
        QVariantList values;
        foreach(const QString& v, value)
            values << v;
        setProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#personalIdentifier", QUrl::StrictMode), values);
    }

    /**
     * Add value to property http://www.semanticdesktop.org/ontologies/2007/08/15/nao#personalIdentifier. 
     * Defines a personal string identifier for a resource 
     */
    void addPersonalIdentifier(const QString& value) {
        addProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#personalIdentifier", QUrl::StrictMode), value);
    }

    /**
     * Get property http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#isWriteable. 
     * Defines if this information model can be modified by the user 
     * of the system. This is usually false for imported ontologies 
     * and true for the user's own PersonalInformationModel. 
     */
    QStringList isWriteables() const {
        QStringList value;
        foreach(const QVariant& v, property(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#isWriteable", QUrl::StrictMode)))
            value << v.value<QString>();
        return value;
    }

    /**
     * Set property http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#isWriteable. 
     * Defines if this information model can be modified by the user 
     * of the system. This is usually false for imported ontologies 
     * and true for the user's own PersonalInformationModel. 
     */
    void setIsWriteables(const QStringList& value) {
        QVariantList values;
        foreach(const QString& v, value)
            values << v;
        setProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#isWriteable", QUrl::StrictMode), values);
    }

    /**
     * Add value to property http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#isWriteable. 
     * Defines if this information model can be modified by the user 
     * of the system. This is usually false for imported ontologies 
     * and true for the user's own PersonalInformationModel. 
     */
    void addIsWriteable(const QString& value) {
        addProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#isWriteable", QUrl::StrictMode), value);
    }

    /**
     * Get property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#generator. 
     * Software used to "generate" the contents. E.g. a word processor 
     * name. 
     */
    QStringList generators() const {
        QStringList value;
        foreach(const QVariant& v, property(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#generator", QUrl::StrictMode)))
            value << v.value<QString>();
        return value;
    }

    /**
     * Set property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#generator. 
     * Software used to "generate" the contents. E.g. a word processor 
     * name. 
     */
    void setGenerators(const QStringList& value) {
        QVariantList values;
        foreach(const QString& v, value)
            values << v;
        setProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#generator", QUrl::StrictMode), values);
    }

    /**
     * Add value to property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#generator. 
     * Software used to "generate" the contents. E.g. a word processor 
     * name. 
     */
    void addGenerator(const QString& value) {
        addProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#generator", QUrl::StrictMode), value);
    }

    /**
     * Get property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#keyword. 
     * Adapted DublinCore: The topic of the content of the resource, 
     * as keyword. No sentences here. Recommended best practice is 
     * to select a value from a controlled vocabulary or formal classification 
     * scheme. 
     */
    QStringList keywords() const {
        QStringList value;
        foreach(const QVariant& v, property(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#keyword", QUrl::StrictMode)))
            value << v.value<QString>();
        return value;
    }

    /**
     * Set property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#keyword. 
     * Adapted DublinCore: The topic of the content of the resource, 
     * as keyword. No sentences here. Recommended best practice is 
     * to select a value from a controlled vocabulary or formal classification 
     * scheme. 
     */
    void setKeywords(const QStringList& value) {
        QVariantList values;
        foreach(const QString& v, value)
            values << v;
        setProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#keyword", QUrl::StrictMode), values);
    }

    /**
     * Add value to property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#keyword. 
     * Adapted DublinCore: The topic of the content of the resource, 
     * as keyword. No sentences here. Recommended best practice is 
     * to select a value from a controlled vocabulary or formal classification 
     * scheme. 
     */
    void addKeyword(const QString& value) {
        addProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#keyword", QUrl::StrictMode), value);
    }

    /**
     * Get property http://www.semanticdesktop.org/ontologies/2007/08/15/nao#lastModified. 
     * States the last modification time for a resource 
     */
    QDateTime lastModified() const {
        QDateTime value;
        if(contains(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#lastModified", QUrl::StrictMode)))
            value = property(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#lastModified", QUrl::StrictMode)).first().value<QDateTime>();
        return value;
    }

    /**
     * Set property http://www.semanticdesktop.org/ontologies/2007/08/15/nao#lastModified. 
     * States the last modification time for a resource 
     */
    void setLastModified(const QDateTime& value) {
        QVariantList values;
        values << value;
        setProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#lastModified", QUrl::StrictMode), values);
    }

    /**
     * Add value to property http://www.semanticdesktop.org/ontologies/2007/08/15/nao#lastModified. 
     * States the last modification time for a resource 
     */
    void addLastModified(const QDateTime& value) {
        addProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#lastModified", QUrl::StrictMode), value);
    }

    /**
     * Get property http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#depiction. 
     * Relates an information element to an image which depicts said 
     * element. 
     */
    QList<QUrl> depictions() const {
        QList<QUrl> value;
        foreach(const QVariant& v, property(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#depiction", QUrl::StrictMode)))
            value << v.value<QUrl>();
        return value;
    }

    /**
     * Set property http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#depiction. 
     * Relates an information element to an image which depicts said 
     * element. 
     */
    void setDepictions(const QList<QUrl>& value) {
        QVariantList values;
        foreach(const QUrl& v, value)
            values << v;
        setProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#depiction", QUrl::StrictMode), values);
    }

    /**
     * Add value to property http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#depiction. 
     * Relates an information element to an image which depicts said 
     * element. 
     */
    void addDepiction(const QUrl& value) {
        addProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#depiction", QUrl::StrictMode), value);
    }

    /**
     * Get property http://www.w3.org/2003/01/geo/wgs84_pos#lat. 
     */
    double lat() const {
        double value;
        if(contains(QUrl::fromEncoded("http://www.w3.org/2003/01/geo/wgs84_pos#lat", QUrl::StrictMode)))
            value = property(QUrl::fromEncoded("http://www.w3.org/2003/01/geo/wgs84_pos#lat", QUrl::StrictMode)).first().value<double>();
        return value;
    }

    /**
     * Set property http://www.w3.org/2003/01/geo/wgs84_pos#lat. 
     */
    void setLat(const double& value) {
        QVariantList values;
        values << value;
        setProperty(QUrl::fromEncoded("http://www.w3.org/2003/01/geo/wgs84_pos#lat", QUrl::StrictMode), values);
    }

    /**
     * Add value to property http://www.w3.org/2003/01/geo/wgs84_pos#lat. 
     */
    void addLat(const double& value) {
        addProperty(QUrl::fromEncoded("http://www.w3.org/2003/01/geo/wgs84_pos#lat", QUrl::StrictMode), value);
    }

    /**
     * Get property http://www.semanticdesktop.org/ontologies/2007/08/15/nao#deprecated. 
     * If this property is assigned, the subject class, property, 
     * or resource, is deprecated and should not be used in production 
     * systems any longer. It may be removed without further notice. 
     */
    QUrl deprecated() const {
        QUrl value;
        if(contains(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#deprecated", QUrl::StrictMode)))
            value = property(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#deprecated", QUrl::StrictMode)).first().value<QUrl>();
        return value;
    }

    /**
     * Set property http://www.semanticdesktop.org/ontologies/2007/08/15/nao#deprecated. 
     * If this property is assigned, the subject class, property, 
     * or resource, is deprecated and should not be used in production 
     * systems any longer. It may be removed without further notice. 
     */
    void setDeprecated(const QUrl& value) {
        QVariantList values;
        values << value;
        setProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#deprecated", QUrl::StrictMode), values);
    }

    /**
     * Add value to property http://www.semanticdesktop.org/ontologies/2007/08/15/nao#deprecated. 
     * If this property is assigned, the subject class, property, 
     * or resource, is deprecated and should not be used in production 
     * systems any longer. It may be removed without further notice. 
     */
    void addDeprecated(const QUrl& value) {
        addProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#deprecated", QUrl::StrictMode), value);
    }

    /**
     * Get property http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#groundingForDeletedThing. 
     * This NIE Information Element was used as a grounding occurrence 
     * for the object Thing. The Thing was then deleted by the user manually, 
     * indicating that this Information Element should not cause 
     * an automatic creation of another Thing in the future. The object 
     * resource has no range to indicate that it was completely removed 
     * from the user's PIMO, including the rdf:type statement. Relevant 
     * for data alignment and enrichment algorithms. 
     */
    QList<QUrl> groundingForDeletedThings() const {
        QList<QUrl> value;
        foreach(const QVariant& v, property(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#groundingForDeletedThing", QUrl::StrictMode)))
            value << v.value<QUrl>();
        return value;
    }

    /**
     * Set property http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#groundingForDeletedThing. 
     * This NIE Information Element was used as a grounding occurrence 
     * for the object Thing. The Thing was then deleted by the user manually, 
     * indicating that this Information Element should not cause 
     * an automatic creation of another Thing in the future. The object 
     * resource has no range to indicate that it was completely removed 
     * from the user's PIMO, including the rdf:type statement. Relevant 
     * for data alignment and enrichment algorithms. 
     */
    void setGroundingForDeletedThings(const QList<QUrl>& value) {
        QVariantList values;
        foreach(const QUrl& v, value)
            values << v;
        setProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#groundingForDeletedThing", QUrl::StrictMode), values);
    }

    /**
     * Add value to property http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#groundingForDeletedThing. 
     * This NIE Information Element was used as a grounding occurrence 
     * for the object Thing. The Thing was then deleted by the user manually, 
     * indicating that this Information Element should not cause 
     * an automatic creation of another Thing in the future. The object 
     * resource has no range to indicate that it was completely removed 
     * from the user's PIMO, including the rdf:type statement. Relevant 
     * for data alignment and enrichment algorithms. 
     */
    void addGroundingForDeletedThing(const QUrl& value) {
        addProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#groundingForDeletedThing", QUrl::StrictMode), value);
    }

    /**
     * Get property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#comment. 
     * A user comment about an InformationElement. 
     */
    QStringList comments() const {
        QStringList value;
        foreach(const QVariant& v, property(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#comment", QUrl::StrictMode)))
            value << v.value<QString>();
        return value;
    }

    /**
     * Set property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#comment. 
     * A user comment about an InformationElement. 
     */
    void setComments(const QStringList& value) {
        QVariantList values;
        foreach(const QString& v, value)
            values << v;
        setProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#comment", QUrl::StrictMode), values);
    }

    /**
     * Add value to property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#comment. 
     * A user comment about an InformationElement. 
     */
    void addComment(const QString& value) {
        addProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#comment", QUrl::StrictMode), value);
    }

    /**
     * Get property http://www.semanticdesktop.org/ontologies/2007/08/15/nao#hasSubResource. 
     * Defines a relationship between a resource and one or more sub 
     * resources. Descriptions of sub-resources are only interpretable 
     * when the super-resource exists. Deleting a super-resource 
     * should then also delete all sub-resources, and transferring 
     * a super-resource (for example, sending it to another user) 
     * must also include the sub-resource. 
     */
    QList<QUrl> subResources() const {
        QList<QUrl> value;
        foreach(const QVariant& v, property(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#hasSubResource", QUrl::StrictMode)))
            value << v.value<QUrl>();
        return value;
    }

    /**
     * Set property http://www.semanticdesktop.org/ontologies/2007/08/15/nao#hasSubResource. 
     * Defines a relationship between a resource and one or more sub 
     * resources. Descriptions of sub-resources are only interpretable 
     * when the super-resource exists. Deleting a super-resource 
     * should then also delete all sub-resources, and transferring 
     * a super-resource (for example, sending it to another user) 
     * must also include the sub-resource. 
     */
    void setSubResources(const QList<QUrl>& value) {
        QVariantList values;
        foreach(const QUrl& v, value)
            values << v;
        setProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#hasSubResource", QUrl::StrictMode), values);
    }

    /**
     * Add value to property http://www.semanticdesktop.org/ontologies/2007/08/15/nao#hasSubResource. 
     * Defines a relationship between a resource and one or more sub 
     * resources. Descriptions of sub-resources are only interpretable 
     * when the super-resource exists. Deleting a super-resource 
     * should then also delete all sub-resources, and transferring 
     * a super-resource (for example, sending it to another user) 
     * must also include the sub-resource. 
     */
    void addSubResource(const QUrl& value) {
        addProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#hasSubResource", QUrl::StrictMode), value);
    }

    /**
     * Get property http://www.semanticdesktop.org/ontologies/2007/08/15/nao#description. 
     * A non-technical textual annotation for a resource 
     */
    QStringList naoDescriptions() const {
        QStringList value;
        foreach(const QVariant& v, property(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#description", QUrl::StrictMode)))
            value << v.value<QString>();
        return value;
    }

    /**
     * Set property http://www.semanticdesktop.org/ontologies/2007/08/15/nao#description. 
     * A non-technical textual annotation for a resource 
     */
    void setNaoDescriptions(const QStringList& value) {
        QVariantList values;
        foreach(const QString& v, value)
            values << v;
        setProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#description", QUrl::StrictMode), values);
    }

    /**
     * Add value to property http://www.semanticdesktop.org/ontologies/2007/08/15/nao#description. 
     * A non-technical textual annotation for a resource 
     */
    void addNaoDescription(const QString& value) {
        addProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#description", QUrl::StrictMode), value);
    }

    /**
     * Get property http://www.semanticdesktop.org/ontologies/2007/08/15/nao#altLabel. 
     * An alternative label alongside the preferred label for a resource 
     */
    QStringList altLabels() const {
        QStringList value;
        foreach(const QVariant& v, property(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#altLabel", QUrl::StrictMode)))
            value << v.value<QString>();
        return value;
    }

    /**
     * Set property http://www.semanticdesktop.org/ontologies/2007/08/15/nao#altLabel. 
     * An alternative label alongside the preferred label for a resource 
     */
    void setAltLabels(const QStringList& value) {
        QVariantList values;
        foreach(const QString& v, value)
            values << v;
        setProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#altLabel", QUrl::StrictMode), values);
    }

    /**
     * Add value to property http://www.semanticdesktop.org/ontologies/2007/08/15/nao#altLabel. 
     * An alternative label alongside the preferred label for a resource 
     */
    void addAltLabel(const QString& value) {
        addProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#altLabel", QUrl::StrictMode), value);
    }

    /**
     * Get property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#disclaimer. 
     * A disclaimer 
     */
    QStringList disclaimers() const {
        QStringList value;
        foreach(const QVariant& v, property(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#disclaimer", QUrl::StrictMode)))
            value << v.value<QString>();
        return value;
    }

    /**
     * Set property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#disclaimer. 
     * A disclaimer 
     */
    void setDisclaimers(const QStringList& value) {
        QVariantList values;
        foreach(const QString& v, value)
            values << v;
        setProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#disclaimer", QUrl::StrictMode), values);
    }

    /**
     * Add value to property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#disclaimer. 
     * A disclaimer 
     */
    void addDisclaimer(const QString& value) {
        addProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#disclaimer", QUrl::StrictMode), value);
    }

    /**
     * Get property http://www.semanticdesktop.org/ontologies/2007/08/15/nao#hasTopic. 
     * Defines a relationship between two resources, where the object 
     * is a topic of the subject 
     */
    QList<QUrl> topics() const {
        QList<QUrl> value;
        foreach(const QVariant& v, property(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#hasTopic", QUrl::StrictMode)))
            value << v.value<QUrl>();
        return value;
    }

    /**
     * Set property http://www.semanticdesktop.org/ontologies/2007/08/15/nao#hasTopic. 
     * Defines a relationship between two resources, where the object 
     * is a topic of the subject 
     */
    void setTopics(const QList<QUrl>& value) {
        QVariantList values;
        foreach(const QUrl& v, value)
            values << v;
        setProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#hasTopic", QUrl::StrictMode), values);
    }

    /**
     * Add value to property http://www.semanticdesktop.org/ontologies/2007/08/15/nao#hasTopic. 
     * Defines a relationship between two resources, where the object 
     * is a topic of the subject 
     */
    void addTopic(const QUrl& value) {
        addProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#hasTopic", QUrl::StrictMode), value);
    }

    /**
     * Get property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#links. 
     * A linking relation. A piece of content links/mentions a piece 
     * of data 
     */
    QList<QUrl> linkses() const {
        QList<QUrl> value;
        foreach(const QVariant& v, property(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#links", QUrl::StrictMode)))
            value << v.value<QUrl>();
        return value;
    }

    /**
     * Set property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#links. 
     * A linking relation. A piece of content links/mentions a piece 
     * of data 
     */
    void setLinkses(const QList<QUrl>& value) {
        QVariantList values;
        foreach(const QUrl& v, value)
            values << v;
        setProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#links", QUrl::StrictMode), values);
    }

    /**
     * Add value to property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#links. 
     * A linking relation. A piece of content links/mentions a piece 
     * of data 
     */
    void addLinks(const QUrl& value) {
        addProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#links", QUrl::StrictMode), value);
    }

    /**
     * Get property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#contentLastModified. 
     * The date of the last modification of the original content (not 
     * its corresponding DataObject or local copy). Compare with 
     * nie:lastModified. 
     */
    QDateTime contentLastModified() const {
        QDateTime value;
        if(contains(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#contentLastModified", QUrl::StrictMode)))
            value = property(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#contentLastModified", QUrl::StrictMode)).first().value<QDateTime>();
        return value;
    }

    /**
     * Set property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#contentLastModified. 
     * The date of the last modification of the original content (not 
     * its corresponding DataObject or local copy). Compare with 
     * nie:lastModified. 
     */
    void setContentLastModified(const QDateTime& value) {
        QVariantList values;
        values << value;
        setProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#contentLastModified", QUrl::StrictMode), values);
    }

    /**
     * Add value to property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#contentLastModified. 
     * The date of the last modification of the original content (not 
     * its corresponding DataObject or local copy). Compare with 
     * nie:lastModified. 
     */
    void addContentLastModified(const QDateTime& value) {
        addProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#contentLastModified", QUrl::StrictMode), value);
    }

    /**
     * Get property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#depends. 
     * Dependency relation. A piece of content depends on another 
     * piece of data in order to be properly understood/used/interpreted. 
     */
    QList<QUrl> dependses() const {
        QList<QUrl> value;
        foreach(const QVariant& v, property(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#depends", QUrl::StrictMode)))
            value << v.value<QUrl>();
        return value;
    }

    /**
     * Set property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#depends. 
     * Dependency relation. A piece of content depends on another 
     * piece of data in order to be properly understood/used/interpreted. 
     */
    void setDependses(const QList<QUrl>& value) {
        QVariantList values;
        foreach(const QUrl& v, value)
            values << v;
        setProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#depends", QUrl::StrictMode), values);
    }

    /**
     * Add value to property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#depends. 
     * Dependency relation. A piece of content depends on another 
     * piece of data in order to be properly understood/used/interpreted. 
     */
    void addDepends(const QUrl& value) {
        addProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#depends", QUrl::StrictMode), value);
    }

    /**
     * Get property http://www.semanticdesktop.org/ontologies/2007/08/15/nao#creator. 
     * Refers to the single or group of individuals that created the 
     * resource 
     */
    QUrl naoCreator() const {
        QUrl value;
        if(contains(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#creator", QUrl::StrictMode)))
            value = property(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#creator", QUrl::StrictMode)).first().value<QUrl>();
        return value;
    }

    /**
     * Set property http://www.semanticdesktop.org/ontologies/2007/08/15/nao#creator. 
     * Refers to the single or group of individuals that created the 
     * resource 
     */
    void setNaoCreator(const QUrl& value) {
        QVariantList values;
        values << value;
        setProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#creator", QUrl::StrictMode), values);
    }

    /**
     * Add value to property http://www.semanticdesktop.org/ontologies/2007/08/15/nao#creator. 
     * Refers to the single or group of individuals that created the 
     * resource 
     */
    void addNaoCreator(const QUrl& value) {
        addProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#creator", QUrl::StrictMode), value);
    }

    /**
     * Get property http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#lfeChannels. 
     * Number of Low Frequency Expansion (subwoofer) channels. 
     */
    qint64 lfeChannels() const {
        qint64 value;
        if(contains(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#lfeChannels", QUrl::StrictMode)))
            value = property(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#lfeChannels", QUrl::StrictMode)).first().value<qint64>();
        return value;
    }

    /**
     * Set property http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#lfeChannels. 
     * Number of Low Frequency Expansion (subwoofer) channels. 
     */
    void setLfeChannels(const qint64& value) {
        QVariantList values;
        values << value;
        setProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#lfeChannels", QUrl::StrictMode), values);
    }

    /**
     * Add value to property http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#lfeChannels. 
     * Number of Low Frequency Expansion (subwoofer) channels. 
     */
    void addLfeChannels(const qint64& value) {
        addProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#lfeChannels", QUrl::StrictMode), value);
    }

    /**
     * Get property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#htmlContent. 
     * The HTML content of an information element. This property can 
     * be used to store text including formatting in a generic fashion. 
     */
    QString htmlContent() const {
        QString value;
        if(contains(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#htmlContent", QUrl::StrictMode)))
            value = property(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#htmlContent", QUrl::StrictMode)).first().value<QString>();
        return value;
    }

    /**
     * Set property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#htmlContent. 
     * The HTML content of an information element. This property can 
     * be used to store text including formatting in a generic fashion. 
     */
    void setHtmlContent(const QString& value) {
        QVariantList values;
        values << value;
        setProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#htmlContent", QUrl::StrictMode), values);
    }

    /**
     * Add value to property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#htmlContent. 
     * The HTML content of an information element. This property can 
     * be used to store text including formatting in a generic fashion. 
     */
    void addHtmlContent(const QString& value) {
        addProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#htmlContent", QUrl::StrictMode), value);
    }

    /**
     * Get property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#publicationDate. 
     * The date of the publication 
     */
    QDateTime publicationDate() const {
        QDateTime value;
        if(contains(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#publicationDate", QUrl::StrictMode)))
            value = property(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#publicationDate", QUrl::StrictMode)).first().value<QDateTime>();
        return value;
    }

    /**
     * Set property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#publicationDate. 
     * The date of the publication 
     */
    void setPublicationDate(const QDateTime& value) {
        QVariantList values;
        values << value;
        setProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#publicationDate", QUrl::StrictMode), values);
    }

    /**
     * Add value to property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#publicationDate. 
     * The date of the publication 
     */
    void addPublicationDate(const QDateTime& value) {
        addProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#publicationDate", QUrl::StrictMode), value);
    }

    /**
     * Get property http://www.semanticdesktop.org/ontologies/2007/08/15/nao#isRelated. 
     * Defines an annotation for a resource in the form of a relationship 
     * between the subject resource and another resource 
     */
    QList<QUrl> isRelateds() const {
        QList<QUrl> value;
        foreach(const QVariant& v, property(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#isRelated", QUrl::StrictMode)))
            value << v.value<QUrl>();
        return value;
    }

    /**
     * Set property http://www.semanticdesktop.org/ontologies/2007/08/15/nao#isRelated. 
     * Defines an annotation for a resource in the form of a relationship 
     * between the subject resource and another resource 
     */
    void setIsRelateds(const QList<QUrl>& value) {
        QVariantList values;
        foreach(const QUrl& v, value)
            values << v;
        setProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#isRelated", QUrl::StrictMode), values);
    }

    /**
     * Add value to property http://www.semanticdesktop.org/ontologies/2007/08/15/nao#isRelated. 
     * Defines an annotation for a resource in the form of a relationship 
     * between the subject resource and another resource 
     */
    void addIsRelated(const QUrl& value) {
        addProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#isRelated", QUrl::StrictMode), value);
    }

    /**
     * Get property http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#frontChannels. 
     * Number of front channels. 
     */
    qint64 frontChannels() const {
        qint64 value;
        if(contains(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#frontChannels", QUrl::StrictMode)))
            value = property(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#frontChannels", QUrl::StrictMode)).first().value<qint64>();
        return value;
    }

    /**
     * Set property http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#frontChannels. 
     * Number of front channels. 
     */
    void setFrontChannels(const qint64& value) {
        QVariantList values;
        values << value;
        setProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#frontChannels", QUrl::StrictMode), values);
    }

    /**
     * Add value to property http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#frontChannels. 
     * Number of front channels. 
     */
    void addFrontChannels(const qint64& value) {
        addProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#frontChannels", QUrl::StrictMode), value);
    }

    /**
     * Get property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#license. 
     * Terms and intellectual property rights licensing conditions. 
     */
    QStringList licenses() const {
        QStringList value;
        foreach(const QVariant& v, property(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#license", QUrl::StrictMode)))
            value << v.value<QString>();
        return value;
    }

    /**
     * Set property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#license. 
     * Terms and intellectual property rights licensing conditions. 
     */
    void setLicenses(const QStringList& value) {
        QVariantList values;
        foreach(const QString& v, value)
            values << v;
        setProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#license", QUrl::StrictMode), values);
    }

    /**
     * Add value to property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#license. 
     * Terms and intellectual property rights licensing conditions. 
     */
    void addLicense(const QString& value) {
        addProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#license", QUrl::StrictMode), value);
    }

    /**
     * Get property http://www.semanticdesktop.org/ontologies/2007/08/15/nao#numericRating. 
     * Annotation for a resource in the form of a numeric rating (float 
     * value), allowed values are between 1 and 10 whereas 0 is interpreted 
     * as not set 
     */
    qint64 numericRating() const {
        qint64 value;
        if(contains(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#numericRating", QUrl::StrictMode)))
            value = property(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#numericRating", QUrl::StrictMode)).first().value<qint64>();
        return value;
    }

    /**
     * Set property http://www.semanticdesktop.org/ontologies/2007/08/15/nao#numericRating. 
     * Annotation for a resource in the form of a numeric rating (float 
     * value), allowed values are between 1 and 10 whereas 0 is interpreted 
     * as not set 
     */
    void setNumericRating(const qint64& value) {
        QVariantList values;
        values << value;
        setProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#numericRating", QUrl::StrictMode), values);
    }

    /**
     * Add value to property http://www.semanticdesktop.org/ontologies/2007/08/15/nao#numericRating. 
     * Annotation for a resource in the form of a numeric rating (float 
     * value), allowed values are between 1 and 10 whereas 0 is interpreted 
     * as not set 
     */
    void addNumericRating(const qint64& value) {
        addProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#numericRating", QUrl::StrictMode), value);
    }

    /**
     * Get property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#relatedTo. 
     * A common superproperty for all relations between a piece of 
     * content and other pieces of data (which may be interpreted as 
     * other pieces of content). 
     */
    QList<QUrl> relatedTos() const {
        QList<QUrl> value;
        foreach(const QVariant& v, property(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#relatedTo", QUrl::StrictMode)))
            value << v.value<QUrl>();
        return value;
    }

    /**
     * Set property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#relatedTo. 
     * A common superproperty for all relations between a piece of 
     * content and other pieces of data (which may be interpreted as 
     * other pieces of content). 
     */
    void setRelatedTos(const QList<QUrl>& value) {
        QVariantList values;
        foreach(const QUrl& v, value)
            values << v;
        setProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#relatedTo", QUrl::StrictMode), values);
    }

    /**
     * Add value to property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#relatedTo. 
     * A common superproperty for all relations between a piece of 
     * content and other pieces of data (which may be interpreted as 
     * other pieces of content). 
     */
    void addRelatedTo(const QUrl& value) {
        addProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#relatedTo", QUrl::StrictMode), value);
    }

    /**
     * Get property http://www.semanticdesktop.org/ontologies/2007/03/22/nco#publisher. 
     * An entity responsible for making the InformationElement available. 
     */
    QList<QUrl> publishers() const {
        QList<QUrl> value;
        foreach(const QVariant& v, property(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nco#publisher", QUrl::StrictMode)))
            value << v.value<QUrl>();
        return value;
    }

    /**
     * Set property http://www.semanticdesktop.org/ontologies/2007/03/22/nco#publisher. 
     * An entity responsible for making the InformationElement available. 
     */
    void setPublishers(const QList<QUrl>& value) {
        QVariantList values;
        foreach(const QUrl& v, value)
            values << v;
        setProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nco#publisher", QUrl::StrictMode), values);
    }

    /**
     * Add value to property http://www.semanticdesktop.org/ontologies/2007/03/22/nco#publisher. 
     * An entity responsible for making the InformationElement available. 
     */
    void addPublisher(const QUrl& value) {
        addProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nco#publisher", QUrl::StrictMode), value);
    }

    /**
     * Get property http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#doi. 
     * The Digital object identifier. see http://www.doi.org/ for 
     * more information 
     */
    QString doi() const {
        QString value;
        if(contains(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#doi", QUrl::StrictMode)))
            value = property(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#doi", QUrl::StrictMode)).first().value<QString>();
        return value;
    }

    /**
     * Set property http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#doi. 
     * The Digital object identifier. see http://www.doi.org/ for 
     * more information 
     */
    void setDoi(const QString& value) {
        QVariantList values;
        values << value;
        setProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#doi", QUrl::StrictMode), values);
    }

    /**
     * Add value to property http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#doi. 
     * The Digital object identifier. see http://www.doi.org/ for 
     * more information 
     */
    void addDoi(const QString& value) {
        addProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#doi", QUrl::StrictMode), value);
    }

    /**
     * Get property http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#rearChannels. 
     * Number of rear channels. 
     */
    qint64 rearChannels() const {
        qint64 value;
        if(contains(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#rearChannels", QUrl::StrictMode)))
            value = property(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#rearChannels", QUrl::StrictMode)).first().value<qint64>();
        return value;
    }

    /**
     * Set property http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#rearChannels. 
     * Number of rear channels. 
     */
    void setRearChannels(const qint64& value) {
        QVariantList values;
        values << value;
        setProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#rearChannels", QUrl::StrictMode), values);
    }

    /**
     * Add value to property http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#rearChannels. 
     * Number of rear channels. 
     */
    void addRearChannels(const qint64& value) {
        addProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#rearChannels", QUrl::StrictMode), value);
    }

    /**
     * Get property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#version. 
     * The current version of the given data object. Exact semantics 
     * is unspecified at this level. Use more specific subproperties 
     * if needed. 
     */
    QStringList versions() const {
        QStringList value;
        foreach(const QVariant& v, property(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#version", QUrl::StrictMode)))
            value << v.value<QString>();
        return value;
    }

    /**
     * Set property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#version. 
     * The current version of the given data object. Exact semantics 
     * is unspecified at this level. Use more specific subproperties 
     * if needed. 
     */
    void setVersions(const QStringList& value) {
        QVariantList values;
        foreach(const QString& v, value)
            values << v;
        setProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#version", QUrl::StrictMode), values);
    }

    /**
     * Add value to property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#version. 
     * The current version of the given data object. Exact semantics 
     * is unspecified at this level. Use more specific subproperties 
     * if needed. 
     */
    void addVersion(const QString& value) {
        addProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#version", QUrl::StrictMode), value);
    }

    /**
     * Get property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#licenseType. 
     * The type of the license. Possible values for this field may include 
     * "GPL", "BSD", "Creative Commons" etc. 
     */
    QStringList licenseTypes() const {
        QStringList value;
        foreach(const QVariant& v, property(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#licenseType", QUrl::StrictMode)))
            value << v.value<QString>();
        return value;
    }

    /**
     * Set property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#licenseType. 
     * The type of the license. Possible values for this field may include 
     * "GPL", "BSD", "Creative Commons" etc. 
     */
    void setLicenseTypes(const QStringList& value) {
        QVariantList values;
        foreach(const QString& v, value)
            values << v;
        setProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#licenseType", QUrl::StrictMode), values);
    }

    /**
     * Add value to property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#licenseType. 
     * The type of the license. Possible values for this field may include 
     * "GPL", "BSD", "Creative Commons" etc. 
     */
    void addLicenseType(const QString& value) {
        addProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#licenseType", QUrl::StrictMode), value);
    }

    /**
     * Get property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#contentModified. 
     * The date of a modification of the original content (not its corresponding 
     * DataObject or local copy). Compare with nie:modified. 
     */
    QList<QDateTime> contentModifieds() const {
        QList<QDateTime> value;
        foreach(const QVariant& v, property(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#contentModified", QUrl::StrictMode)))
            value << v.value<QDateTime>();
        return value;
    }

    /**
     * Set property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#contentModified. 
     * The date of a modification of the original content (not its corresponding 
     * DataObject or local copy). Compare with nie:modified. 
     */
    void setContentModifieds(const QList<QDateTime>& value) {
        QVariantList values;
        foreach(const QDateTime& v, value)
            values << v;
        setProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#contentModified", QUrl::StrictMode), values);
    }

    /**
     * Add value to property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#contentModified. 
     * The date of a modification of the original content (not its corresponding 
     * DataObject or local copy). Compare with nie:modified. 
     */
    void addContentModified(const QDateTime& value) {
        addProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#contentModified", QUrl::StrictMode), value);
    }

    /**
     * Get property http://www.semanticdesktop.org/ontologies/2007/03/22/nco#creator. 
     * Creator of an information element, an entity primarily responsible 
     * for the creation of the content of the data object. 
     */
    QList<QUrl> ncoCreators() const {
        QList<QUrl> value;
        foreach(const QVariant& v, property(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nco#creator", QUrl::StrictMode)))
            value << v.value<QUrl>();
        return value;
    }

    /**
     * Set property http://www.semanticdesktop.org/ontologies/2007/03/22/nco#creator. 
     * Creator of an information element, an entity primarily responsible 
     * for the creation of the content of the data object. 
     */
    void setNcoCreators(const QList<QUrl>& value) {
        QVariantList values;
        foreach(const QUrl& v, value)
            values << v;
        setProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nco#creator", QUrl::StrictMode), values);
    }

    /**
     * Add value to property http://www.semanticdesktop.org/ontologies/2007/03/22/nco#creator. 
     * Creator of an information element, an entity primarily responsible 
     * for the creation of the content of the data object. 
     */
    void addNcoCreator(const QUrl& value) {
        addProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nco#creator", QUrl::StrictMode), value);
    }

    /**
     * Get property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#isLogicalPartOf. 
     * Generic property used to express 'logical' containment relationships 
     * between DataObjects. NIE extensions are encouraged to provide 
     * more specific subproperties of this one. It is advisable for 
     * actual instances of InformationElement to use those specific 
     * subproperties. Note the difference between 'physical' containment 
     * (isPartOf) and logical containment (isLogicalPartOf) 
     */
    QList<QUrl> isLogicalPartOfs() const {
        QList<QUrl> value;
        foreach(const QVariant& v, property(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#isLogicalPartOf", QUrl::StrictMode)))
            value << v.value<QUrl>();
        return value;
    }

    /**
     * Set property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#isLogicalPartOf. 
     * Generic property used to express 'logical' containment relationships 
     * between DataObjects. NIE extensions are encouraged to provide 
     * more specific subproperties of this one. It is advisable for 
     * actual instances of InformationElement to use those specific 
     * subproperties. Note the difference between 'physical' containment 
     * (isPartOf) and logical containment (isLogicalPartOf) 
     */
    void setIsLogicalPartOfs(const QList<QUrl>& value) {
        QVariantList values;
        foreach(const QUrl& v, value)
            values << v;
        setProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#isLogicalPartOf", QUrl::StrictMode), values);
    }

    /**
     * Add value to property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#isLogicalPartOf. 
     * Generic property used to express 'logical' containment relationships 
     * between DataObjects. NIE extensions are encouraged to provide 
     * more specific subproperties of this one. It is advisable for 
     * actual instances of InformationElement to use those specific 
     * subproperties. Note the difference between 'physical' containment 
     * (isPartOf) and logical containment (isLogicalPartOf) 
     */
    void addIsLogicalPartOf(const QUrl& value) {
        addProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#isLogicalPartOf", QUrl::StrictMode), value);
    }

    /**
     * Get property http://www.semanticdesktop.org/ontologies/2007/08/15/nao#isTopicOf. 
     * Defines a relationship between two resources, where the subject 
     * is a topic of the object 
     */
    QList<QUrl> isTopicOfs() const {
        QList<QUrl> value;
        foreach(const QVariant& v, property(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#isTopicOf", QUrl::StrictMode)))
            value << v.value<QUrl>();
        return value;
    }

    /**
     * Set property http://www.semanticdesktop.org/ontologies/2007/08/15/nao#isTopicOf. 
     * Defines a relationship between two resources, where the subject 
     * is a topic of the object 
     */
    void setIsTopicOfs(const QList<QUrl>& value) {
        QVariantList values;
        foreach(const QUrl& v, value)
            values << v;
        setProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#isTopicOf", QUrl::StrictMode), values);
    }

    /**
     * Add value to property http://www.semanticdesktop.org/ontologies/2007/08/15/nao#isTopicOf. 
     * Defines a relationship between two resources, where the subject 
     * is a topic of the object 
     */
    void addIsTopicOf(const QUrl& value) {
        addProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#isTopicOf", QUrl::StrictMode), value);
    }

    /**
     * Get property http://www.semanticdesktop.org/ontologies/2007/08/15/nao#modified. 
     * States the modification time for a resource 
     */
    QList<QDateTime> modifieds() const {
        QList<QDateTime> value;
        foreach(const QVariant& v, property(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#modified", QUrl::StrictMode)))
            value << v.value<QDateTime>();
        return value;
    }

    /**
     * Set property http://www.semanticdesktop.org/ontologies/2007/08/15/nao#modified. 
     * States the modification time for a resource 
     */
    void setModifieds(const QList<QDateTime>& value) {
        QVariantList values;
        foreach(const QDateTime& v, value)
            values << v;
        setProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#modified", QUrl::StrictMode), values);
    }

    /**
     * Add value to property http://www.semanticdesktop.org/ontologies/2007/08/15/nao#modified. 
     * States the modification time for a resource 
     */
    void addModified(const QDateTime& value) {
        addProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#modified", QUrl::StrictMode), value);
    }

    /**
     * Get property http://www.semanticdesktop.org/ontologies/2007/03/22/nco#contributor. 
     * An entity responsible for making contributions to the content 
     * of the InformationElement. 
     */
    QList<QUrl> ncoContributors() const {
        QList<QUrl> value;
        foreach(const QVariant& v, property(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nco#contributor", QUrl::StrictMode)))
            value << v.value<QUrl>();
        return value;
    }

    /**
     * Set property http://www.semanticdesktop.org/ontologies/2007/03/22/nco#contributor. 
     * An entity responsible for making contributions to the content 
     * of the InformationElement. 
     */
    void setNcoContributors(const QList<QUrl>& value) {
        QVariantList values;
        foreach(const QUrl& v, value)
            values << v;
        setProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nco#contributor", QUrl::StrictMode), values);
    }

    /**
     * Add value to property http://www.semanticdesktop.org/ontologies/2007/03/22/nco#contributor. 
     * An entity responsible for making contributions to the content 
     * of the InformationElement. 
     */
    void addNcoContributor(const QUrl& value) {
        addProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nco#contributor", QUrl::StrictMode), value);
    }

    /**
     * Get property http://www.semanticdesktop.org/ontologies/2007/08/15/nao#created. 
     * States the creation, or first modification time for a resource 
     */
    QDateTime created() const {
        QDateTime value;
        if(contains(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#created", QUrl::StrictMode)))
            value = property(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#created", QUrl::StrictMode)).first().value<QDateTime>();
        return value;
    }

    /**
     * Set property http://www.semanticdesktop.org/ontologies/2007/08/15/nao#created. 
     * States the creation, or first modification time for a resource 
     */
    void setCreated(const QDateTime& value) {
        QVariantList values;
        values << value;
        setProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#created", QUrl::StrictMode), values);
    }

    /**
     * Add value to property http://www.semanticdesktop.org/ontologies/2007/08/15/nao#created. 
     * States the creation, or first modification time for a resource 
     */
    void addCreated(const QDateTime& value) {
        addProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#created", QUrl::StrictMode), value);
    }

    /**
     * Get property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#mimeType. 
     * The mime type of the resource, if available. Example: "text/plain". 
     * See http://www.iana.org/assignments/media-types/. This 
     * property applies to data objects that can be described with 
     * one mime type. In cases where the object as a whole has one mime 
     * type, while it's parts have other mime types, or there is no mime 
     * type that can be applied to the object as a whole, but some parts 
     * of the content have mime types - use more specific properties. 
     */
    QString mimeType() const {
        QString value;
        if(contains(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#mimeType", QUrl::StrictMode)))
            value = property(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#mimeType", QUrl::StrictMode)).first().value<QString>();
        return value;
    }

    /**
     * Set property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#mimeType. 
     * The mime type of the resource, if available. Example: "text/plain". 
     * See http://www.iana.org/assignments/media-types/. This 
     * property applies to data objects that can be described with 
     * one mime type. In cases where the object as a whole has one mime 
     * type, while it's parts have other mime types, or there is no mime 
     * type that can be applied to the object as a whole, but some parts 
     * of the content have mime types - use more specific properties. 
     */
    void setMimeType(const QString& value) {
        QVariantList values;
        values << value;
        setProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#mimeType", QUrl::StrictMode), values);
    }

    /**
     * Add value to property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#mimeType. 
     * The mime type of the resource, if available. Example: "text/plain". 
     * See http://www.iana.org/assignments/media-types/. This 
     * property applies to data objects that can be described with 
     * one mime type. In cases where the object as a whole has one mime 
     * type, while it's parts have other mime types, or there is no mime 
     * type that can be applied to the object as a whole, but some parts 
     * of the content have mime types - use more specific properties. 
     */
    void addMimeType(const QString& value) {
        addProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#mimeType", QUrl::StrictMode), value);
    }

    /**
     * Get property http://www.semanticdesktop.org/ontologies/2007/08/15/nao#contributor. 
     * Refers to a single or a group of individuals that contributed 
     * to a resource 
     */
    QList<QUrl> naoContributors() const {
        QList<QUrl> value;
        foreach(const QVariant& v, property(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#contributor", QUrl::StrictMode)))
            value << v.value<QUrl>();
        return value;
    }

    /**
     * Set property http://www.semanticdesktop.org/ontologies/2007/08/15/nao#contributor. 
     * Refers to a single or a group of individuals that contributed 
     * to a resource 
     */
    void setNaoContributors(const QList<QUrl>& value) {
        QVariantList values;
        foreach(const QUrl& v, value)
            values << v;
        setProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#contributor", QUrl::StrictMode), values);
    }

    /**
     * Add value to property http://www.semanticdesktop.org/ontologies/2007/08/15/nao#contributor. 
     * Refers to a single or a group of individuals that contributed 
     * to a resource 
     */
    void addNaoContributor(const QUrl& value) {
        addProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#contributor", QUrl::StrictMode), value);
    }

    /**
     * Get property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#title. 
     * Name given to an InformationElement 
     */
    QString title() const {
        QString value;
        if(contains(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#title", QUrl::StrictMode)))
            value = property(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#title", QUrl::StrictMode)).first().value<QString>();
        return value;
    }

    /**
     * Set property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#title. 
     * Name given to an InformationElement 
     */
    void setTitle(const QString& value) {
        QVariantList values;
        values << value;
        setProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#title", QUrl::StrictMode), values);
    }

    /**
     * Add value to property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#title. 
     * Name given to an InformationElement 
     */
    void addTitle(const QString& value) {
        addProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#title", QUrl::StrictMode), value);
    }

    /**
     * Get property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#characterSet. 
     * Characterset in which the content of the InformationElement 
     * was created. Example: ISO-8859-1, UTF-8. One of the registered 
     * character sets at http://www.iana.org/assignments/character-sets. 
     * This characterSet is used to interpret any textual parts of 
     * the content. If more than one characterSet is used within one 
     * data object, use more specific properties. 
     */
    QString characterSet() const {
        QString value;
        if(contains(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#characterSet", QUrl::StrictMode)))
            value = property(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#characterSet", QUrl::StrictMode)).first().value<QString>();
        return value;
    }

    /**
     * Set property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#characterSet. 
     * Characterset in which the content of the InformationElement 
     * was created. Example: ISO-8859-1, UTF-8. One of the registered 
     * character sets at http://www.iana.org/assignments/character-sets. 
     * This characterSet is used to interpret any textual parts of 
     * the content. If more than one characterSet is used within one 
     * data object, use more specific properties. 
     */
    void setCharacterSet(const QString& value) {
        QVariantList values;
        values << value;
        setProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#characterSet", QUrl::StrictMode), values);
    }

    /**
     * Add value to property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#characterSet. 
     * Characterset in which the content of the InformationElement 
     * was created. Example: ISO-8859-1, UTF-8. One of the registered 
     * character sets at http://www.iana.org/assignments/character-sets. 
     * This characterSet is used to interpret any textual parts of 
     * the content. If more than one characterSet is used within one 
     * data object, use more specific properties. 
     */
    void addCharacterSet(const QString& value) {
        addProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#characterSet", QUrl::StrictMode), value);
    }

    /**
     * Get property http://www.semanticdesktop.org/ontologies/2007/08/15/nao#hasTag. 
     * Defines an existing tag for a resource 
     */
    QList<QUrl> tags() const {
        QList<QUrl> value;
        foreach(const QVariant& v, property(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#hasTag", QUrl::StrictMode)))
            value << v.value<QUrl>();
        return value;
    }

    /**
     * Set property http://www.semanticdesktop.org/ontologies/2007/08/15/nao#hasTag. 
     * Defines an existing tag for a resource 
     */
    void setTags(const QList<QUrl>& value) {
        QVariantList values;
        foreach(const QUrl& v, value)
            values << v;
        setProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#hasTag", QUrl::StrictMode), values);
    }

    /**
     * Add value to property http://www.semanticdesktop.org/ontologies/2007/08/15/nao#hasTag. 
     * Defines an existing tag for a resource 
     */
    void addTag(const QUrl& value) {
        addProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#hasTag", QUrl::StrictMode), value);
    }

    /**
     * Get property http://www.semanticdesktop.org/ontologies/2007/08/15/nao#scoreParameter. 
     * A marker property to mark selected properties which are input 
     * to a mathematical algorithm to generate scores for resources. 
     * Properties are marked by being defined as subproperties of 
     * this property 
     */
    QList<double> scoreParameters() const {
        QList<double> value;
        foreach(const QVariant& v, property(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#scoreParameter", QUrl::StrictMode)))
            value << v.value<double>();
        return value;
    }

    /**
     * Set property http://www.semanticdesktop.org/ontologies/2007/08/15/nao#scoreParameter. 
     * A marker property to mark selected properties which are input 
     * to a mathematical algorithm to generate scores for resources. 
     * Properties are marked by being defined as subproperties of 
     * this property 
     */
    void setScoreParameters(const QList<double>& value) {
        QVariantList values;
        foreach(const double& v, value)
            values << v;
        setProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#scoreParameter", QUrl::StrictMode), values);
    }

    /**
     * Add value to property http://www.semanticdesktop.org/ontologies/2007/08/15/nao#scoreParameter. 
     * A marker property to mark selected properties which are input 
     * to a mathematical algorithm to generate scores for resources. 
     * Properties are marked by being defined as subproperties of 
     * this property 
     */
    void addScoreParameter(const double& value) {
        addProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#scoreParameter", QUrl::StrictMode), value);
    }

    /**
     * Get property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#abstract. 
     * An abstract of the work. 
     */
    QString abstract() const {
        QString value;
        if(contains(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#abstract", QUrl::StrictMode)))
            value = property(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#abstract", QUrl::StrictMode)).first().value<QString>();
        return value;
    }

    /**
     * Set property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#abstract. 
     * An abstract of the work. 
     */
    void setAbstract(const QString& value) {
        QVariantList values;
        values << value;
        setProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#abstract", QUrl::StrictMode), values);
    }

    /**
     * Add value to property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#abstract. 
     * An abstract of the work. 
     */
    void addAbstract(const QString& value) {
        addProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#abstract", QUrl::StrictMode), value);
    }

    /**
     * Get property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#language. 
     * Language the InformationElement is expressed in. This property 
     * applies to the data object in its entirety. If the data object 
     * is divisible into parts expressed in multiple languages - more 
     * specific properties should be used. Users are encouraged to 
     * use the two-letter code specified in the RFC 3066 
     */
    QString language() const {
        QString value;
        if(contains(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#language", QUrl::StrictMode)))
            value = property(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#language", QUrl::StrictMode)).first().value<QString>();
        return value;
    }

    /**
     * Set property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#language. 
     * Language the InformationElement is expressed in. This property 
     * applies to the data object in its entirety. If the data object 
     * is divisible into parts expressed in multiple languages - more 
     * specific properties should be used. Users are encouraged to 
     * use the two-letter code specified in the RFC 3066 
     */
    void setLanguage(const QString& value) {
        QVariantList values;
        values << value;
        setProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#language", QUrl::StrictMode), values);
    }

    /**
     * Add value to property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#language. 
     * Language the InformationElement is expressed in. This property 
     * applies to the data object in its entirety. If the data object 
     * is divisible into parts expressed in multiple languages - more 
     * specific properties should be used. Users are encouraged to 
     * use the two-letter code specified in the RFC 3066 
     */
    void addLanguage(const QString& value) {
        addProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#language", QUrl::StrictMode), value);
    }

    /**
     * Get property http://www.semanticdesktop.org/ontologies/2007/08/15/nao#maintainedBy. 
     * The agent that maintains this resource, ie. created it and knows 
     * what to do with it. 
     */
    QList<QUrl> maintainedBys() const {
        QList<QUrl> value;
        foreach(const QVariant& v, property(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#maintainedBy", QUrl::StrictMode)))
            value << v.value<QUrl>();
        return value;
    }

    /**
     * Set property http://www.semanticdesktop.org/ontologies/2007/08/15/nao#maintainedBy. 
     * The agent that maintains this resource, ie. created it and knows 
     * what to do with it. 
     */
    void setMaintainedBys(const QList<QUrl>& value) {
        QVariantList values;
        foreach(const QUrl& v, value)
            values << v;
        setProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#maintainedBy", QUrl::StrictMode), values);
    }

    /**
     * Add value to property http://www.semanticdesktop.org/ontologies/2007/08/15/nao#maintainedBy. 
     * The agent that maintains this resource, ie. created it and knows 
     * what to do with it. 
     */
    void addMaintainedBy(const QUrl& value) {
        addProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#maintainedBy", QUrl::StrictMode), value);
    }

    /**
     * Get property http://www.semanticdesktop.org/ontologies/2007/03/22/nco#editor. 
     * The name(s) of the editor(s). 
     */
    QList<QUrl> editors() const {
        QList<QUrl> value;
        foreach(const QVariant& v, property(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nco#editor", QUrl::StrictMode)))
            value << v.value<QUrl>();
        return value;
    }

    /**
     * Set property http://www.semanticdesktop.org/ontologies/2007/03/22/nco#editor. 
     * The name(s) of the editor(s). 
     */
    void setEditors(const QList<QUrl>& value) {
        QVariantList values;
        foreach(const QUrl& v, value)
            values << v;
        setProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nco#editor", QUrl::StrictMode), values);
    }

    /**
     * Add value to property http://www.semanticdesktop.org/ontologies/2007/03/22/nco#editor. 
     * The name(s) of the editor(s). 
     */
    void addEditor(const QUrl& value) {
        addProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nco#editor", QUrl::StrictMode), value);
    }

    /**
     * Get property http://www.example.com/nbib#citedReference. 
     * Links all cited references from one publication. Good way to 
     * find new sources to read 
     */
    QList<QUrl> citedReferences() const {
        QList<QUrl> value;
        foreach(const QVariant& v, property(QUrl::fromEncoded("http://www.example.com/nbib#citedReference", QUrl::StrictMode)))
            value << v.value<QUrl>();
        return value;
    }

    /**
     * Set property http://www.example.com/nbib#citedReference. 
     * Links all cited references from one publication. Good way to 
     * find new sources to read 
     */
    void setCitedReferences(const QList<QUrl>& value) {
        QVariantList values;
        foreach(const QUrl& v, value)
            values << v;
        setProperty(QUrl::fromEncoded("http://www.example.com/nbib#citedReference", QUrl::StrictMode), values);
    }

    /**
     * Add value to property http://www.example.com/nbib#citedReference. 
     * Links all cited references from one publication. Good way to 
     * find new sources to read 
     */
    void addCitedReference(const QUrl& value) {
        addProperty(QUrl::fromEncoded("http://www.example.com/nbib#citedReference", QUrl::StrictMode), value);
    }

    /**
     * Get property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#identifier. 
     * An unambiguous reference to the InformationElement within 
     * a given context. Recommended best practice is to identify the 
     * resource by means of a string conforming to a formal identification 
     * system. 
     */
    QStringList identifiers() const {
        QStringList value;
        foreach(const QVariant& v, property(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#identifier", QUrl::StrictMode)))
            value << v.value<QString>();
        return value;
    }

    /**
     * Set property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#identifier. 
     * An unambiguous reference to the InformationElement within 
     * a given context. Recommended best practice is to identify the 
     * resource by means of a string conforming to a formal identification 
     * system. 
     */
    void setIdentifiers(const QStringList& value) {
        QVariantList values;
        foreach(const QString& v, value)
            values << v;
        setProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#identifier", QUrl::StrictMode), values);
    }

    /**
     * Add value to property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#identifier. 
     * An unambiguous reference to the InformationElement within 
     * a given context. Recommended best practice is to identify the 
     * resource by means of a string conforming to a formal identification 
     * system. 
     */
    void addIdentifier(const QString& value) {
        addProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#identifier", QUrl::StrictMode), value);
    }

    /**
     * Get property http://www.w3.org/2003/01/geo/wgs84_pos#alt. 
     */
    double alt() const {
        double value;
        if(contains(QUrl::fromEncoded("http://www.w3.org/2003/01/geo/wgs84_pos#alt", QUrl::StrictMode)))
            value = property(QUrl::fromEncoded("http://www.w3.org/2003/01/geo/wgs84_pos#alt", QUrl::StrictMode)).first().value<double>();
        return value;
    }

    /**
     * Set property http://www.w3.org/2003/01/geo/wgs84_pos#alt. 
     */
    void setAlt(const double& value) {
        QVariantList values;
        values << value;
        setProperty(QUrl::fromEncoded("http://www.w3.org/2003/01/geo/wgs84_pos#alt", QUrl::StrictMode), values);
    }

    /**
     * Add value to property http://www.w3.org/2003/01/geo/wgs84_pos#alt. 
     */
    void addAlt(const double& value) {
        addProperty(QUrl::fromEncoded("http://www.w3.org/2003/01/geo/wgs84_pos#alt", QUrl::StrictMode), value);
    }

    /**
     * Get property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#plainTextContent. 
     * Plain-text representation of the content of a InformationElement 
     * with all markup removed. The main purpose of this property is 
     * full-text indexing and search. Its exact content is considered 
     * application-specific. The user can make no assumptions about 
     * what is and what is not contained within. Applications should 
     * use more specific properties wherever possible. 
     */
    QString plainTextContent() const {
        QString value;
        if(contains(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#plainTextContent", QUrl::StrictMode)))
            value = property(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#plainTextContent", QUrl::StrictMode)).first().value<QString>();
        return value;
    }

    /**
     * Set property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#plainTextContent. 
     * Plain-text representation of the content of a InformationElement 
     * with all markup removed. The main purpose of this property is 
     * full-text indexing and search. Its exact content is considered 
     * application-specific. The user can make no assumptions about 
     * what is and what is not contained within. Applications should 
     * use more specific properties wherever possible. 
     */
    void setPlainTextContent(const QString& value) {
        QVariantList values;
        values << value;
        setProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#plainTextContent", QUrl::StrictMode), values);
    }

    /**
     * Add value to property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#plainTextContent. 
     * Plain-text representation of the content of a InformationElement 
     * with all markup removed. The main purpose of this property is 
     * full-text indexing and search. Its exact content is considered 
     * application-specific. The user can make no assumptions about 
     * what is and what is not contained within. Applications should 
     * use more specific properties wherever possible. 
     */
    void addPlainTextContent(const QString& value) {
        addProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#plainTextContent", QUrl::StrictMode), value);
    }

    /**
     * Get property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#contentSize. 
     * The size of the content. This property can be used whenever the 
     * size of the content of an InformationElement differs from the 
     * size of the DataObject. (e.g. because of compression, encoding, 
     * encryption or any other representation issues). The contentSize 
     * in expressed in bytes. 
     */
    qint64 contentSize() const {
        qint64 value;
        if(contains(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#contentSize", QUrl::StrictMode)))
            value = property(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#contentSize", QUrl::StrictMode)).first().value<qint64>();
        return value;
    }

    /**
     * Set property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#contentSize. 
     * The size of the content. This property can be used whenever the 
     * size of the content of an InformationElement differs from the 
     * size of the DataObject. (e.g. because of compression, encoding, 
     * encryption or any other representation issues). The contentSize 
     * in expressed in bytes. 
     */
    void setContentSize(const qint64& value) {
        QVariantList values;
        values << value;
        setProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#contentSize", QUrl::StrictMode), values);
    }

    /**
     * Add value to property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#contentSize. 
     * The size of the content. This property can be used whenever the 
     * size of the content of an InformationElement differs from the 
     * size of the DataObject. (e.g. because of compression, encoding, 
     * encryption or any other representation issues). The contentSize 
     * in expressed in bytes. 
     */
    void addContentSize(const qint64& value) {
        addProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#contentSize", QUrl::StrictMode), value);
    }

    /**
     * Get property http://www.semanticdesktop.org/ontologies/2007/08/15/nao#hasSuperResource. 
     * Defines a relationship between a resource and one or more super 
     * resources 
     */
    QList<QUrl> superResources() const {
        QList<QUrl> value;
        foreach(const QVariant& v, property(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#hasSuperResource", QUrl::StrictMode)))
            value << v.value<QUrl>();
        return value;
    }

    /**
     * Set property http://www.semanticdesktop.org/ontologies/2007/08/15/nao#hasSuperResource. 
     * Defines a relationship between a resource and one or more super 
     * resources 
     */
    void setSuperResources(const QList<QUrl>& value) {
        QVariantList values;
        foreach(const QUrl& v, value)
            values << v;
        setProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#hasSuperResource", QUrl::StrictMode), values);
    }

    /**
     * Add value to property http://www.semanticdesktop.org/ontologies/2007/08/15/nao#hasSuperResource. 
     * Defines a relationship between a resource and one or more super 
     * resources 
     */
    void addSuperResource(const QUrl& value) {
        addProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#hasSuperResource", QUrl::StrictMode), value);
    }

    /**
     * Get property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#contentCreated. 
     * The date of the content creation. This may not necessarily be 
     * equal to the date when the DataObject (i.e. the physical representation) 
     * itself was created. Compare with nie:created property. 
     */
    QDateTime contentCreated() const {
        QDateTime value;
        if(contains(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#contentCreated", QUrl::StrictMode)))
            value = property(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#contentCreated", QUrl::StrictMode)).first().value<QDateTime>();
        return value;
    }

    /**
     * Set property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#contentCreated. 
     * The date of the content creation. This may not necessarily be 
     * equal to the date when the DataObject (i.e. the physical representation) 
     * itself was created. Compare with nie:created property. 
     */
    void setContentCreated(const QDateTime& value) {
        QVariantList values;
        values << value;
        setProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#contentCreated", QUrl::StrictMode), values);
    }

    /**
     * Add value to property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#contentCreated. 
     * The date of the content creation. This may not necessarily be 
     * equal to the date when the DataObject (i.e. the physical representation) 
     * itself was created. Compare with nie:created property. 
     */
    void addContentCreated(const QDateTime& value) {
        addProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#contentCreated", QUrl::StrictMode), value);
    }

    /**
     * Get property http://www.semanticdesktop.org/ontologies/2007/08/15/nao#pluralPrefLabel. 
     * The plural form of the preferred label for a resource 
     */
    QString pluralPrefLabel() const {
        QString value;
        if(contains(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#pluralPrefLabel", QUrl::StrictMode)))
            value = property(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#pluralPrefLabel", QUrl::StrictMode)).first().value<QString>();
        return value;
    }

    /**
     * Set property http://www.semanticdesktop.org/ontologies/2007/08/15/nao#pluralPrefLabel. 
     * The plural form of the preferred label for a resource 
     */
    void setPluralPrefLabel(const QString& value) {
        QVariantList values;
        values << value;
        setProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#pluralPrefLabel", QUrl::StrictMode), values);
    }

    /**
     * Add value to property http://www.semanticdesktop.org/ontologies/2007/08/15/nao#pluralPrefLabel. 
     * The plural form of the preferred label for a resource 
     */
    void addPluralPrefLabel(const QString& value) {
        addProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#pluralPrefLabel", QUrl::StrictMode), value);
    }

    /**
     * Get property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#subject. 
     * An overall topic of the content of a InformationElement 
     */
    QStringList subjects() const {
        QStringList value;
        foreach(const QVariant& v, property(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#subject", QUrl::StrictMode)))
            value << v.value<QString>();
        return value;
    }

    /**
     * Set property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#subject. 
     * An overall topic of the content of a InformationElement 
     */
    void setSubjects(const QStringList& value) {
        QVariantList values;
        foreach(const QString& v, value)
            values << v;
        setProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#subject", QUrl::StrictMode), values);
    }

    /**
     * Add value to property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#subject. 
     * An overall topic of the content of a InformationElement 
     */
    void addSubject(const QString& value) {
        addProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#subject", QUrl::StrictMode), value);
    }

    /**
     * Get property http://www.semanticdesktop.org/ontologies/2007/08/15/nao#score. 
     * An authoritative score for an item valued between 0 and 1 
     */
    double score() const {
        double value;
        if(contains(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#score", QUrl::StrictMode)))
            value = property(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#score", QUrl::StrictMode)).first().value<double>();
        return value;
    }

    /**
     * Set property http://www.semanticdesktop.org/ontologies/2007/08/15/nao#score. 
     * An authoritative score for an item valued between 0 and 1 
     */
    void setScore(const double& value) {
        QVariantList values;
        values << value;
        setProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#score", QUrl::StrictMode), values);
    }

    /**
     * Add value to property http://www.semanticdesktop.org/ontologies/2007/08/15/nao#score. 
     * An authoritative score for an item valued between 0 and 1 
     */
    void addScore(const double& value) {
        addProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#score", QUrl::StrictMode), value);
    }

    /**
     * Get property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#rootElementOf. 
     * DataObjects extracted from a single data source are organized 
     * into a containment tree. This property links the root of that 
     * tree with the datasource it has been extracted from 
     */
    QList<QUrl> rootElementOfs() const {
        QList<QUrl> value;
        foreach(const QVariant& v, property(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#rootElementOf", QUrl::StrictMode)))
            value << v.value<QUrl>();
        return value;
    }

    /**
     * Set property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#rootElementOf. 
     * DataObjects extracted from a single data source are organized 
     * into a containment tree. This property links the root of that 
     * tree with the datasource it has been extracted from 
     */
    void setRootElementOfs(const QList<QUrl>& value) {
        QVariantList values;
        foreach(const QUrl& v, value)
            values << v;
        setProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#rootElementOf", QUrl::StrictMode), values);
    }

    /**
     * Add value to property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#rootElementOf. 
     * DataObjects extracted from a single data source are organized 
     * into a containment tree. This property links the root of that 
     * tree with the datasource it has been extracted from 
     */
    void addRootElementOf(const QUrl& value) {
        addProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#rootElementOf", QUrl::StrictMode), value);
    }

    /**
     * Get property http://www.semanticdesktop.org/ontologies/2007/08/15/nao#hasSymbol. 
     * Annotation for a resource in the form of a visual representation. 
     * Typically the symbol is a double-typed image file or a nao:FreeDesktopIcon. 
     */
    QList<QUrl> symbols() const {
        QList<QUrl> value;
        foreach(const QVariant& v, property(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#hasSymbol", QUrl::StrictMode)))
            value << v.value<QUrl>();
        return value;
    }

    /**
     * Set property http://www.semanticdesktop.org/ontologies/2007/08/15/nao#hasSymbol. 
     * Annotation for a resource in the form of a visual representation. 
     * Typically the symbol is a double-typed image file or a nao:FreeDesktopIcon. 
     */
    void setSymbols(const QList<QUrl>& value) {
        QVariantList values;
        foreach(const QUrl& v, value)
            values << v;
        setProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#hasSymbol", QUrl::StrictMode), values);
    }

    /**
     * Add value to property http://www.semanticdesktop.org/ontologies/2007/08/15/nao#hasSymbol. 
     * Annotation for a resource in the form of a visual representation. 
     * Typically the symbol is a double-typed image file or a nao:FreeDesktopIcon. 
     */
    void addSymbol(const QUrl& value) {
        addProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/08/15/nao#hasSymbol", QUrl::StrictMode), value);
    }

    /**
     * Get property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#informationElementDate. 
     * A point or period of time associated with an event in the lifecycle 
     * of an Information Element. A common superproperty for all date-related 
     * properties of InformationElements in the NIE Framework. 
     */
    QList<QDateTime> informationElementDates() const {
        QList<QDateTime> value;
        foreach(const QVariant& v, property(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#informationElementDate", QUrl::StrictMode)))
            value << v.value<QDateTime>();
        return value;
    }

    /**
     * Set property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#informationElementDate. 
     * A point or period of time associated with an event in the lifecycle 
     * of an Information Element. A common superproperty for all date-related 
     * properties of InformationElements in the NIE Framework. 
     */
    void setInformationElementDates(const QList<QDateTime>& value) {
        QVariantList values;
        foreach(const QDateTime& v, value)
            values << v;
        setProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#informationElementDate", QUrl::StrictMode), values);
    }

    /**
     * Add value to property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#informationElementDate. 
     * A point or period of time associated with an event in the lifecycle 
     * of an Information Element. A common superproperty for all date-related 
     * properties of InformationElements in the NIE Framework. 
     */
    void addInformationElementDate(const QDateTime& value) {
        addProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#informationElementDate", QUrl::StrictMode), value);
    }

    /**
     * Get property http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#classRole. 
     * Annotating abstract and concrete classes. Implementations 
     * may offer the feature to hide abstract classes. By default, 
     * classes are concrete. Classes can be declared abstract by setting 
     * their classRole to abstract. Instances should not have an abstract 
     * class as type (if not inferred). 
     */
    QUrl classRole() const {
        QUrl value;
        if(contains(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#classRole", QUrl::StrictMode)))
            value = property(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#classRole", QUrl::StrictMode)).first().value<QUrl>();
        return value;
    }

    /**
     * Set property http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#classRole. 
     * Annotating abstract and concrete classes. Implementations 
     * may offer the feature to hide abstract classes. By default, 
     * classes are concrete. Classes can be declared abstract by setting 
     * their classRole to abstract. Instances should not have an abstract 
     * class as type (if not inferred). 
     */
    void setClassRole(const QUrl& value) {
        QVariantList values;
        values << value;
        setProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#classRole", QUrl::StrictMode), values);
    }

    /**
     * Add value to property http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#classRole. 
     * Annotating abstract and concrete classes. Implementations 
     * may offer the feature to hide abstract classes. By default, 
     * classes are concrete. Classes can be declared abstract by setting 
     * their classRole to abstract. Instances should not have an abstract 
     * class as type (if not inferred). 
     */
    void addClassRole(const QUrl& value) {
        addProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/11/01/pimo#classRole", QUrl::StrictMode), value);
    }

    /**
     * Get property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#legal. 
     * A common superproperty for all properties that point at legal 
     * information about an Information Element 
     */
    QStringList legals() const {
        QStringList value;
        foreach(const QVariant& v, property(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#legal", QUrl::StrictMode)))
            value << v.value<QString>();
        return value;
    }

    /**
     * Set property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#legal. 
     * A common superproperty for all properties that point at legal 
     * information about an Information Element 
     */
    void setLegals(const QStringList& value) {
        QVariantList values;
        foreach(const QString& v, value)
            values << v;
        setProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#legal", QUrl::StrictMode), values);
    }

    /**
     * Add value to property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#legal. 
     * A common superproperty for all properties that point at legal 
     * information about an Information Element 
     */
    void addLegal(const QString& value) {
        addProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#legal", QUrl::StrictMode), value);
    }

    /**
     * Get property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#hasPart. 
     * Generic property used to express 'physical' containment relationships 
     * between DataObjects. NIE extensions are encouraged to provide 
     * more specific subproperties of this one. It is advisable for 
     * actual instances of DataObjects to use those specific subproperties. 
     * Note to the developers: Please be aware of the distinction between 
     * containment relation and provenance. The hasPart relation 
     * models physical containment, an InformationElement (a nmo:Message) 
     * can have a 'physical' part (an nfo:Attachment). Also, please 
     * note the difference between physical containment (hasPart) 
     * and logical containment (hasLogicalPart) the former has more 
     * strict meaning. They may occur independently of each other. 
     */
    QList<QUrl> parts() const {
        QList<QUrl> value;
        foreach(const QVariant& v, property(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#hasPart", QUrl::StrictMode)))
            value << v.value<QUrl>();
        return value;
    }

    /**
     * Set property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#hasPart. 
     * Generic property used to express 'physical' containment relationships 
     * between DataObjects. NIE extensions are encouraged to provide 
     * more specific subproperties of this one. It is advisable for 
     * actual instances of DataObjects to use those specific subproperties. 
     * Note to the developers: Please be aware of the distinction between 
     * containment relation and provenance. The hasPart relation 
     * models physical containment, an InformationElement (a nmo:Message) 
     * can have a 'physical' part (an nfo:Attachment). Also, please 
     * note the difference between physical containment (hasPart) 
     * and logical containment (hasLogicalPart) the former has more 
     * strict meaning. They may occur independently of each other. 
     */
    void setParts(const QList<QUrl>& value) {
        QVariantList values;
        foreach(const QUrl& v, value)
            values << v;
        setProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#hasPart", QUrl::StrictMode), values);
    }

    /**
     * Add value to property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#hasPart. 
     * Generic property used to express 'physical' containment relationships 
     * between DataObjects. NIE extensions are encouraged to provide 
     * more specific subproperties of this one. It is advisable for 
     * actual instances of DataObjects to use those specific subproperties. 
     * Note to the developers: Please be aware of the distinction between 
     * containment relation and provenance. The hasPart relation 
     * models physical containment, an InformationElement (a nmo:Message) 
     * can have a 'physical' part (an nfo:Attachment). Also, please 
     * note the difference between physical containment (hasPart) 
     * and logical containment (hasLogicalPart) the former has more 
     * strict meaning. They may occur independently of each other. 
     */
    void addPart(const QUrl& value) {
        addProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#hasPart", QUrl::StrictMode), value);
    }

    /**
     * Get property http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#encryptionStatus. 
     * The status of the encryption of the InformationElement. 
     */
    QUrl encryptionStatus() const {
        QUrl value;
        if(contains(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#encryptionStatus", QUrl::StrictMode)))
            value = property(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#encryptionStatus", QUrl::StrictMode)).first().value<QUrl>();
        return value;
    }

    /**
     * Set property http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#encryptionStatus. 
     * The status of the encryption of the InformationElement. 
     */
    void setEncryptionStatus(const QUrl& value) {
        QVariantList values;
        values << value;
        setProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#encryptionStatus", QUrl::StrictMode), values);
    }

    /**
     * Add value to property http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#encryptionStatus. 
     * The status of the encryption of the InformationElement. 
     */
    void addEncryptionStatus(const QUrl& value) {
        addProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#encryptionStatus", QUrl::StrictMode), value);
    }

    /**
     * Get property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#isStoredAs. 
     * Links the information element with the DataObject it is stored 
     * in. 
     */
    QList<QUrl> isStoredAses() const {
        QList<QUrl> value;
        foreach(const QVariant& v, property(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#isStoredAs", QUrl::StrictMode)))
            value << v.value<QUrl>();
        return value;
    }

    /**
     * Set property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#isStoredAs. 
     * Links the information element with the DataObject it is stored 
     * in. 
     */
    void setIsStoredAses(const QList<QUrl>& value) {
        QVariantList values;
        foreach(const QUrl& v, value)
            values << v;
        setProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#isStoredAs", QUrl::StrictMode), values);
    }

    /**
     * Add value to property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#isStoredAs. 
     * Links the information element with the DataObject it is stored 
     * in. 
     */
    void addIsStoredAs(const QUrl& value) {
        addProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#isStoredAs", QUrl::StrictMode), value);
    }

    /**
     * Get property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#description. 
     * A textual description of the resource. This property may be 
     * used for any metadata fields that provide some meta-information 
     * or comment about a resource in the form of a passage of text. This 
     * property is not to be confused with nie:plainTextContent. 
     * Use more specific subproperties wherever possible. 
     */
    QStringList nieDescriptions() const {
        QStringList value;
        foreach(const QVariant& v, property(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#description", QUrl::StrictMode)))
            value << v.value<QString>();
        return value;
    }

    /**
     * Set property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#description. 
     * A textual description of the resource. This property may be 
     * used for any metadata fields that provide some meta-information 
     * or comment about a resource in the form of a passage of text. This 
     * property is not to be confused with nie:plainTextContent. 
     * Use more specific subproperties wherever possible. 
     */
    void setNieDescriptions(const QStringList& value) {
        QVariantList values;
        foreach(const QString& v, value)
            values << v;
        setProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#description", QUrl::StrictMode), values);
    }

    /**
     * Add value to property http://www.semanticdesktop.org/ontologies/2007/01/19/nie#description. 
     * A textual description of the resource. This property may be 
     * used for any metadata fields that provide some meta-information 
     * or comment about a resource in the form of a passage of text. This 
     * property is not to be confused with nie:plainTextContent. 
     * Use more specific subproperties wherever possible. 
     */
    void addNieDescription(const QString& value) {
        addProperty(QUrl::fromEncoded("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#description", QUrl::StrictMode), value);
    }

protected:
    InformationElement(const QUrl& uri, const QUrl& type)
      : SimpleResource(uri) {
        addType(type);
    }
    InformationElement(const SimpleResource& res, const QUrl& type)
      : SimpleResource(res) {
        addType(type);
    }
};
}
}

#endif
