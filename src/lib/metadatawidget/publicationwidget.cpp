/*
 * Copyright 2013 Jörg Ehrichs <joerg.ehrichs@gmx.de>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "publicationwidget.h"
#include "ui_publicationwidget.h"

#include <KDE/KDebug>

namespace NepomukWebMiner
{
namespace UI
{
class PublicationWidgetPrivate
{
public:
    Ui::PublicationWidget *ui;
    QVariantMap metadata;
};
}
}

NepomukWebMiner::UI::PublicationWidget::PublicationWidget(QWidget *parent)
    : MetaDataBaseWidget(parent)
    , d_ptr(new NepomukWebMiner::UI::PublicationWidgetPrivate)
{
    Q_D(PublicationWidget);
    d->ui = new Ui::PublicationWidget();
    d->ui->setupUi(this);

    //TODO: add publication date

    d->ui->editEntryType->addItem(i18n("Article"), QLatin1String("article"));
    d->ui->editEntryType->addItem(i18n("Book"), QLatin1String("book"));
    d->ui->editEntryType->addItem(i18n("Booklet"), QLatin1String("booklet"));
    d->ui->editEntryType->addItem(i18n("Collection"), QLatin1String("collection"));
    d->ui->editEntryType->addItem(i18n("Thesis"), QLatin1String("thesis"));
    d->ui->editEntryType->addItem(i18n("Report"), QLatin1String("report"));
    d->ui->editEntryType->addItem(i18n("Techreport"), QLatin1String("techreport"));
    d->ui->editEntryType->addItem(i18n("Website"), QLatin1String("website"));
    d->ui->editEntryType->addItem(i18n("Script"), QLatin1String("script"));
    d->ui->editEntryType->addItem(i18n("Presentation"), QLatin1String("presentation"));
    d->ui->editEntryType->addItem(i18n("Proceedings"), QLatin1String("proceedings"));
    d->ui->editEntryType->addItem(i18n("Manual"), QLatin1String("manual"));
    d->ui->editEntryType->addItem(i18n("Misc"), QLatin1String("publication"));
}

NepomukWebMiner::UI::PublicationWidget::~PublicationWidget()
{
    Q_D(PublicationWidget);
    delete d->ui;
}

void NepomukWebMiner::UI::PublicationWidget::setMetaData(const QVariantMap &publication)
{
    Q_D(PublicationWidget);
    d->metadata = publication;

    int index = d->ui->editEntryType->findData( publication.value(QLatin1String("bibtexentrytype")).toString().toLower() );
    if(index == -1) {
        index = d->ui->editEntryType->findData( QLatin1String("publication") );
    }
    d->ui->editEntryType->setCurrentIndex(index);

    //TODO: show different text for some fields (booktitle/title etc)
    d->ui->editTitle->setText( publication.value(QLatin1String("title")).toString() );
    d->ui->editCiteKey->setText( publication.value(QLatin1String("bibtexcitekey")).toString() );
    d->ui->editPages->setText( publication.value(QLatin1String("pages")).toString() );
    d->ui->editAuthors->setText( publication.value(QLatin1String("author")).toString() );
    d->ui->editEditors->setText( publication.value(QLatin1String("editor")).toString() );
    d->ui->editOrganization->setText( publication.value(QLatin1String("organization")).toString() );
    d->ui->editPublisher->setText( publication.value(QLatin1String("publisher")).toString() );
    d->ui->editTopics->setText( publication.value(QLatin1String("keywords")).toString() );
    d->ui->editAbstract->setText( publication.value(QLatin1String("abstract")).toString() );


    d->ui->editEvent->setText( publication.value(QLatin1String("event")).toString() );
    d->ui->editSeries->setText( publication.value(QLatin1String("series")).toString() );
    d->ui->editVolume->setText( publication.value(QLatin1String("volume")).toString() );
    d->ui->editNumber->setText( publication.value(QLatin1String("number")).toString() );
    d->ui->editEdition->setText( publication.value(QLatin1String("edition")).toString() );
    d->ui->editReportType->setText( publication.value(QLatin1String("type")).toString() );
    d->ui->editCopyright->setText( publication.value(QLatin1String("copyright")).toString() );
    d->ui->editLanguage->setText( publication.value(QLatin1String("language")).toString() );


    d->ui->editArchive->setText( publication.value(QLatin1String("archive")).toString() );
    d->ui->editArchiveLocation->setText( publication.value(QLatin1String("archivelocation")).toString() );
    d->ui->editEPrint->setText( publication.value(QLatin1String("eprint")).toString() );
    d->ui->editISBN->setText( publication.value(QLatin1String("isbn")).toString() );
    d->ui->editISSN->setText( publication.value(QLatin1String("issn")).toString() );
    d->ui->editLCCN->setText( publication.value(QLatin1String("lccn")).toString() );
    d->ui->editMRNumber->setText( publication.value(QLatin1String("mrnumber")).toString() );
    d->ui->editPubMed->setText( publication.value(QLatin1String("pubmed")).toString() );
    d->ui->editDOI->setText( publication.value(QLatin1String("doi")).toString() );

    //TODO: fill references
}

QVariantMap NepomukWebMiner::UI::PublicationWidget::metaData() const
{

    Q_D(const PublicationWidget);
    QVariantMap map = d->metadata;
    map.insert(QLatin1String("title"), d->ui->editTitle->text() );
    map.insert(QLatin1String("bibtexcitekey"), d->ui->editCiteKey->text() );
    map.insert(QLatin1String("pages"), d->ui->editPages->text() );
    map.insert(QLatin1String("author"), d->ui->editAuthors->text() );
    map.insert(QLatin1String("editor"), d->ui->editEditors->text() );
    map.insert(QLatin1String("organization"), d->ui->editOrganization->text() );
    map.insert(QLatin1String("publisher"), d->ui->editPublisher->text() );
    map.insert(QLatin1String("keywords"), d->ui->editTopics->text() );
    map.insert(QLatin1String("abstract"), d->ui->editAbstract->document()->toPlainText() );


    map.insert(QLatin1String("event"), d->ui->editEvent->text() );
    map.insert(QLatin1String("series"), d->ui->editSeries->text() );
    map.insert(QLatin1String("volume"), d->ui->editVolume->text() );
    map.insert(QLatin1String("number"), d->ui->editNumber->text() );
    map.insert(QLatin1String("edition"), d->ui->editEdition->text() );
    map.insert(QLatin1String("type"), d->ui->editReportType->text() );
    map.insert(QLatin1String("copyright"), d->ui->editCopyright->text() );
    map.insert(QLatin1String("language"), d->ui->editLanguage->text() );

    map.insert(QLatin1String("archive"), d->ui->editArchive->text() );
    map.insert(QLatin1String("archivelocation"), d->ui->editArchiveLocation->text() );
    map.insert(QLatin1String("eprint"), d->ui->editEPrint->text() );
    map.insert(QLatin1String("isbn"), d->ui->editISBN->text() );
    map.insert(QLatin1String("issn"), d->ui->editISSN->text() );
    map.insert(QLatin1String("lccn"), d->ui->editMRNumber->text() );
    map.insert(QLatin1String("mrnumber"), d->ui->editMRNumber->text() );
    map.insert(QLatin1String("pubmed"), d->ui->editPubMed->text() );
    map.insert(QLatin1String("doi"), d->ui->editDOI->text() );

    //TODO save changed publication refernces
    return map;
}

