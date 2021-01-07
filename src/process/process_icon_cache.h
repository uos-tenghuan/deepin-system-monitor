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
#ifndef PROCESS_ICON_CACHE_H
#define PROCESS_ICON_CACHE_H

#include <QCache>
#include <QThread>

namespace core {
namespace process {

class ProcessIcon;
class ProcessIconCache : QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(ProcessIconCache)

public:
    static ProcessIconCache *instance();

    ProcessIcon *getProcessIcon(pid_t pid) const;
    void addProcessIcon(pid_t pid, ProcessIcon *procIcon);
    void removeProcessIcon(pid_t pid);
    bool contains(pid_t pid) const;
    void clear();
    void setMaxCost(int cost);

private:
    explicit ProcessIconCache(QObject *parent = nullptr);

private:
    QCache<pid_t, ProcessIcon> m_cache {1024};

    static ProcessIconCache *m_instance;
};

inline ProcessIconCache *ProcessIconCache::instance()
{
    if (!m_instance)
        m_instance = new ProcessIconCache();

    return m_instance;
}

inline ProcessIcon *ProcessIconCache::getProcessIcon(pid_t pid) const
{
    if (m_cache.contains(pid))
        return m_cache[pid];

    return nullptr;
}

inline void ProcessIconCache::addProcessIcon(pid_t pid, ProcessIcon *procIcon)
{
    m_cache.insert(pid, procIcon);
}

inline void ProcessIconCache::removeProcessIcon(pid_t pid)
{
    m_cache.remove(pid);
}

inline bool ProcessIconCache::contains(pid_t pid) const
{
    return m_cache.contains(pid);
}

inline void ProcessIconCache::clear()
{
    m_cache.clear();
}

inline void ProcessIconCache::setMaxCost(int cost)
{
    m_cache.setMaxCost(cost);
}

} // namespace process
} // namespace core

#endif // PROCESS_ICON_CACHE_H
