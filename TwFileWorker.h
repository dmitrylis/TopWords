#ifndef TWFILEWORKER_H
#define TWFILEWORKER_H

#include <QObject>
#include <QUrl>

class TwFileWorker final : public QObject
{
    Q_OBJECT

public:
    enum Command
    {
        None = 0,
        Pause,
        Resume,
        Stop
    };

    explicit TwFileWorker(QObject *parent = nullptr);

public slots:
    void readFile(const QUrl &fileUrl);
    void setCommand(Command command);

signals:
    void chunkRead(const QStringList &words, float progress);

    void started();
    void paused();
    void resumed();
    void stopped();

    void finished();
    void errorOccured();

private:
    Command m_command {Command::None};
};

#endif // TWFILEWORKER_H
