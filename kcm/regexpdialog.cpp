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

#include "regexpdialog.h"
#include "kcm/ui_regexpdialog.h"

#include <KDE/KToolInvocation>

#include <QtGui/QListWidget>
#include <QtGui/QListWidgetItem>
#include <QtGui/QMessageBox>

NepomukWebMiner::RegExpDialog::RegExpDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::RegExpDialog)
    , title(0)
    , show(0)
    , season(0)
    , episode(0)
    , person(0)
    , album(0)
{
    ui->setupUi(this);

    setupUi();

    updateMatchExample();
}

NepomukWebMiner::RegExpDialog::~RegExpDialog()
{
    delete ui;

    delete title;
    delete show;
    delete season;
    delete episode;
    delete person;
    delete album;
    delete track;
    delete year;
}

void NepomukWebMiner::RegExpDialog::setRegExpData(const Extractor::RegExpData &data)
{
    ui->lineEdit->setText(data.regExp.pattern());
    ui->lineEditExample->setText(data.example);

    ui->cb_path->setChecked(data.useFolder);

    for(int i=0;i<data.matchList.size();i++) {
        switch(data.matchList.at(i)) {
        case Extractor::MATCH_TITLE:
            ui->availableGroups->availableListWidget()->takeItem( ui->availableGroups->availableListWidget()->row(title));
            ui->availableGroups->selectedListWidget()->addItem(title);
            break;
        case Extractor::MATCH_SHOW:
            ui->availableGroups->availableListWidget()->takeItem( ui->availableGroups->availableListWidget()->row(show));
            ui->availableGroups->selectedListWidget()->addItem(show);
            break;
        case Extractor::MATCH_SEASON:
            ui->availableGroups->availableListWidget()->takeItem( ui->availableGroups->availableListWidget()->row(season));
            ui->availableGroups->selectedListWidget()->addItem(season);
            break;
        case Extractor::MATCH_EPISODE:
            ui->availableGroups->availableListWidget()->takeItem( ui->availableGroups->availableListWidget()->row(episode));
            ui->availableGroups->selectedListWidget()->addItem(episode);
            break;
        case Extractor::MATCH_PERSON:
            ui->availableGroups->availableListWidget()->takeItem( ui->availableGroups->availableListWidget()->row(person));
            ui->availableGroups->selectedListWidget()->addItem(person);
            break;
        case Extractor::MATCH_ALBUM:
            ui->availableGroups->availableListWidget()->takeItem( ui->availableGroups->availableListWidget()->row(album));
            ui->availableGroups->selectedListWidget()->addItem(album);
            break;
        case Extractor::MATCH_YEAR:
            ui->availableGroups->availableListWidget()->takeItem( ui->availableGroups->availableListWidget()->row(year));
            ui->availableGroups->selectedListWidget()->addItem(year);
            break;
        case Extractor::MATCH_TRACK:
            ui->availableGroups->availableListWidget()->takeItem( ui->availableGroups->availableListWidget()->row(track));
            ui->availableGroups->selectedListWidget()->addItem(track);
            break;
        }
    }

    updateMatchExample();
}

NepomukWebMiner::Extractor::RegExpData NepomukWebMiner::RegExpDialog::regExpData() const
{
    Extractor::RegExpData newData;

    newData.regExp = QRegExp(ui->lineEdit->text());
    newData.example = ui->lineEditExample->text();
    newData.useFolder = ui->cb_path->isChecked();

    for(int i=0;i<ui->availableGroups->selectedListWidget()->count();i++) {
        newData.matchList << (Extractor::MatchTypes)ui->availableGroups->selectedListWidget()->item(i)->data(Qt::UserRole).toInt();
    }

    return newData;
}

void NepomukWebMiner::RegExpDialog::accept() {

    QRegExp tmpRegExp = QRegExp(ui->lineEdit->text());

    if( !tmpRegExp.isValid() || tmpRegExp.isEmpty()) {
        QMessageBox::warning(this, i18n("Nepomuk-WebMiner"),
                                   i18n("Invalid regular expression.\n\n"
                                        "Your regular expression is not valid and can not extract any information."),
                                       QMessageBox::Close, QMessageBox::Close);
        return;
    }

    int groups = tmpRegExp.captureCount();
    if(ui->availableGroups->selectedListWidget()->count() != groups) {

        QMessageBox::warning(this, i18n("Nepomuk-WebMiner"),
                                   i18n("Wrong number of matching groups selected.\n\n"
                                        "Your regular expression counts %1 capture groups for the filename check. You need to select also what all these groups represent.", groups),
                                       QMessageBox::Close, QMessageBox::Close);
        return;
    }

    // if we end up here, everything is correct
    QDialog::accept();
}

void NepomukWebMiner::RegExpDialog::updateMatchExample()
{
    if(ui->lineEditExample->text().isEmpty()) {
        ui->matchExampleLabel->setText(i18n("Add example to check if your expression works."));
        return;
    }

    QRegExp tmpRegExp = QRegExp(ui->lineEdit->text());

    if(tmpRegExp.captureCount() == 0) {
        ui->matchExampleLabel->setText(i18n("No capture groups available that could match anything."));
    }
    else {
        if(!tmpRegExp.exactMatch(ui->lineEditExample->text())) {
            ui->matchExampleLabel->setText(i18n("No match found."));
        }
        else {
            QString matchedGroups;
            QStringList groups = tmpRegExp.capturedTexts();

            // ignore i=0 this is the full example text again
            for(int i=1;i<groups.size(); i++) {

                // show the user selected data text or just a general group number
                QListWidgetItem *item = ui->availableGroups->selectedListWidget()->item(i-1);
                if(item) {
                    matchedGroups.append(QLatin1String("<b>"));
                    matchedGroups.append(item->text());
                    matchedGroups.append(QLatin1String("</b>: "));
                }
                else {
                    matchedGroups.append(i18n("<font color=red><b>Group %1: </b></font>",i));
                }

                matchedGroups.append(groups.at(i));
                matchedGroups.append(QLatin1String(", "));
            }

            matchedGroups.chop(2);

            ui->matchExampleLabel->setText(matchedGroups);
        }
    }
}

void NepomukWebMiner::RegExpDialog::showHelp()
{
    KToolInvocation::invokeHelp(QString(), QString("kcontrol/nepomuk-webminer"));
}

void NepomukWebMiner::RegExpDialog::setupUi()
{
    title = new QListWidgetItem(i18n("Title"));
    title->setData(Qt::UserRole, Extractor::MATCH_TITLE);
    ui->availableGroups->availableListWidget()->addItem(title);

    show = new QListWidgetItem(i18n("Show"));
    show->setData(Qt::UserRole, Extractor::MATCH_SHOW);
    ui->availableGroups->availableListWidget()->addItem(show);

    season = new QListWidgetItem(i18n("Season"));
    season->setData(Qt::UserRole, Extractor::MATCH_SEASON);
    ui->availableGroups->availableListWidget()->addItem(season);

    episode = new QListWidgetItem(i18n("Episode"));
    episode->setData(Qt::UserRole, Extractor::MATCH_EPISODE);
    ui->availableGroups->availableListWidget()->addItem(episode);

    person = new QListWidgetItem(i18n("Person"));
    person->setData(Qt::UserRole, Extractor::MATCH_PERSON);
    ui->availableGroups->availableListWidget()->addItem(person);

    album = new QListWidgetItem(i18n("Album"));
    album->setData(Qt::UserRole, Extractor::MATCH_ALBUM);
    ui->availableGroups->availableListWidget()->addItem(album);

    year = new QListWidgetItem(i18n("Year"));
    year->setData(Qt::UserRole, Extractor::MATCH_YEAR);
    ui->availableGroups->availableListWidget()->addItem(year);


    track = new QListWidgetItem(i18n("Track"));
    track->setData(Qt::UserRole, Extractor::MATCH_TRACK);
    ui->availableGroups->availableListWidget()->addItem(track);

    connect(ui->lineEdit, SIGNAL(textChanged(QString)), this, SLOT(updateMatchExample()));
    connect(ui->lineEditExample, SIGNAL(textChanged(QString)), this, SLOT(updateMatchExample()));
    connect(ui->availableGroups, SIGNAL(added(QListWidgetItem*)), this, SLOT(updateMatchExample()));
    connect(ui->availableGroups, SIGNAL(removed(QListWidgetItem*)), this, SLOT(updateMatchExample()));
    connect(ui->buttonBox, SIGNAL(helpRequested()), this, SLOT(showHelp()));
}
