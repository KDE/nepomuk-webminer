/*
 * Copyright 2012 Jörg Ehrichs <joerg.ehrichs@gmx.de>
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

#ifndef STARTUPDIALOG_H
#define STARTUPDIALOG_H

#include <QtGui/QDialog>
#include <KDE/KDialog>

#include <QtCore/QFuture>
#include <QtCore/QFutureWatcher>

namespace Ui {
class StartupDialog;
}

namespace NepomukWebMiner {
namespace UI {
    class Fetcher;
}
}

/**
 * @brief Simple dialog that shows the status of the file extarction on startup
 *
 * Tells the user that something is going one, when a large collection of files is
 * parsed.
 */
class StartupDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit StartupDialog(NepomukWebMiner::UI::Fetcher *f, QWidget *parent = 0);
    ~StartupDialog();
    
    void setFetchingUrl(const KUrl url);
    void cancel();

public slots:
    void addStatusOutput(const QString &message);
    void startFetching();

private slots:
    void fetchingDone();

private:
    Ui::StartupDialog *ui;
    NepomukWebMiner::UI::Fetcher *m_fetcher;
    KUrl m_url;

    QFuture<void> m_Future;
    QFutureWatcher<void> m_FutureWatcher;
};

#endif // STARTUPDIALOG_H
