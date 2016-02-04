#ifndef LOGMANAGER_H
#define LOGMANAGER_H

#include <QtCore>

namespace Dtk {
namespace Log {

class ConsoleAppender;
class RollingFileAppender;

class DLogManager
{
public:
    static void registerConsoleAppender();
    static void registerFileAppender();
    static QString getlogFilePath();

signals:

public slots:

private:
    QString m_format;
    QString m_logPath;
    ConsoleAppender* m_consoleAppender;
    RollingFileAppender* m_rollingFileAppender;

    void initConsoleAppender();
    void initRollingFileAppender();
    QString joinPath(const QString& path, const QString& fileName);

    inline static DLogManager* instance(){
        static DLogManager instance;
        return &instance;
    }

    explicit DLogManager();
    ~DLogManager();
    DLogManager(const DLogManager &);
    DLogManager & operator = (const DLogManager &);
};

}}

#endif // LOGMANAGER_H
