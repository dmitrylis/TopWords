#include "TwFileWorker.h"

#include <QFile>
#include <QTextStream>
#include <QRegularExpression>
#include <QThread>
#include <QAbstractEventDispatcher>

namespace {

constexpr int CHUNK_SIZE {100};
constexpr int NOTIFY_FREQUENCY {33}; // in ms, ~30Hz

}

TwFileWorker::TwFileWorker(QObject *parent)
    : QObject{parent}
{
}

void TwFileWorker::readFile(const QUrl &fileUrl)
{
    emit started();

    QFile file {fileUrl.toLocalFile()};
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        emit errorOccured();
    }

    const qint64 size {file.size()};
    const QRegularExpression notAWord {"\\W+", QRegularExpression::UseUnicodePropertiesOption}; // Could be "[^\\p{L}\\p{N}_]+" maybe, depends on requirements
    QAbstractEventDispatcher * const eventDispatcher {QThread::currentThread()->eventDispatcher()};

    QStringList wordsChunk;
    int lineCounter {0};

    QTextStream stream {&file};
    while (!stream.atEnd())
    {
        switch (m_command)
        {
        case Command::Pause:
        {
            emit paused();
            eventDispatcher->processEvents(QEventLoop::WaitForMoreEvents);
            break;
        }
        case Command::Resume:
        {
            emit resumed();
            break;
        }
        case Command::Stop:
        {
            file.close();
            setCommand(Command::None);
            emit stopped();

            return; // important
        }
        default: break;
        }

        wordsChunk << stream.readLine().split(notAWord, Qt::SkipEmptyParts);
        lineCounter++;

        if (lineCounter == ::CHUNK_SIZE || stream.atEnd())
        {
            const float progress {static_cast<float>(size - file.bytesAvailable()) / static_cast<float>(size)};
            emit chunkRead(wordsChunk, progress);

            wordsChunk.clear();
            lineCounter = 0;

            QThread::msleep(::NOTIFY_FREQUENCY);
        }

        eventDispatcher->processEvents(QEventLoop::AllEvents);
    }

    file.close();
    setCommand(Command::None);

    emit finished();
}

void TwFileWorker::setCommand(Command command)
{
    // Since i communicate with this worker using only Qt's Signals/Slots (QueuedConnection) - I don't use QMutex/QMutexLocker. It's safe enough
    m_command = command;
}
