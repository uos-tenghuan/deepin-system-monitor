/*
* Copyright (C) 2019 ~ 2020 Uniontech Software Technology Co.,Ltd
*
* Author:      maojj <maojunjie@uniontech.com>
* Maintainer:  maojj <maojunjie@uniontech.com>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "process_controller.h"

#include "application.h"

#include <QFile>
#include <QProcess>

#define CMD_PKEXEC "/usr/bin/pkexec"
#define CMD_KILL "/usr/bin/kill"

// constructor
ProcessController::ProcessController(pid_t pid, int signal, QObject *parent)
    : QObject(parent)
    , m_pid(pid)
    , m_signal(signal)
{
    m_proc = new QProcess(this);
    // emit background task state changed signal when process about to start
    connect(m_proc, &QProcess::started, this, [=]() {
        Q_EMIT gApp->backgroundTaskStateChanged(Application::kTaskStarted);
    });
    // process finished signal
    connect(m_proc, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, [=](int rc, QProcess::ExitStatus) {
        // EINVAL means call with invalid signal
        Q_ASSERT(rc != EINVAL);
        if (rc == ESRCH)
            // binary non exists
            rc = ESRCH;
        else if (rc != 0) {
            // permission denied
            rc = EPERM;
        }
        Q_EMIT resultReady(rc);
        // emit background task state changed signal when finished
        Q_EMIT gApp->backgroundTaskStateChanged(Application::kTaskFinished);
        m_proc->deleteLater();
        Q_EMIT finished();
    });
}

// execute pkexec+kill
void ProcessController::execute()
{
    QStringList params;

    // check pkexec existance
    if (!QFile::exists({CMD_PKEXEC})) {
        Q_EMIT resultReady(ENOENT);
        exit(ENOENT);
    }
    // check kill existance
    if (!QFile::exists({CMD_KILL})) {
        Q_EMIT resultReady(ENOENT);
        exit(ENOENT);
    }

    // format: kill -{signal} {pid}
    params << QString(CMD_KILL) << QString("-%1").arg(m_signal) << QString("%1").arg(m_pid);

    // EINVAL, EPERM, ESRCH
    // -2: cant not be started; -1: crashed; other: exit code of pkexec
    // pkexec: 127: not auth/cant auth/error; 126: dialog dismiss
    m_proc->start({CMD_PKEXEC}, params);
}
