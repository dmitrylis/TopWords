#include "TwController.h"

#include "TwFileWorker.h"
#include "TwWordsModel.h"

#include <QQmlEngine>

TwController::TwController(QObject *parent)
    : QObject {parent}
{
    qRegisterMetaType<TwWordsModel*>("TwWordsModel*");
    qRegisterMetaType<TwFileWorker::Command>("TwFileWorker::Command");
    qmlRegisterUncreatableType<TwTypes>("com.dln.twtypes", 1, 0, "TwTypes", "TwTypes cannot be created from QML");

    m_fileWorker.moveToThread(&m_workerThread);
    m_wordsModel = new TwWordsModel(this);

    connect(&m_workerThread, &QThread::started, &m_fileWorker, [this](){ m_fileWorker.readFile(m_fileUrl); });

    connect(this, &TwController::postCommand, &m_fileWorker, &TwFileWorker::setCommand);

    connect(&m_fileWorker, &TwFileWorker::chunkRead, this,
            [this](const QStringList &words, float progress)
            {
                m_wordsModel->append(words);
                setReadingProgress(progress);
            });

    connect(&m_fileWorker, &TwFileWorker::errorOccured, this,
            []()
            {
                /* handle error somehow */
            });

    connect(&m_fileWorker, &TwFileWorker::started, this,
            [this]()
            {
                m_wordsModel->clear();
                setReadingStatus(TwTypes::ReadingStatus::InProgress);
                setReadingProgress(0.f);
            });

    connect(&m_fileWorker, &TwFileWorker::paused, this,
            [this]()
            {
                setReadingStatus(TwTypes::ReadingStatus::Paused);
            });

    connect(&m_fileWorker, &TwFileWorker::resumed, this,
            [this]()
            {
                setReadingStatus(TwTypes::ReadingStatus::InProgress);
            });

    connect(&m_fileWorker, &TwFileWorker::stopped, this,
            [this]()
            {
                m_wordsModel->clear();
                setReadingStatus(TwTypes::ReadingStatus::Idle);
                setReadingProgress(0.f);

                m_workerThread.quit();
                m_workerThread.wait();
            });

    connect(&m_fileWorker, &TwFileWorker::finished, this,
            [this]()
            {
                setReadingStatus(TwTypes::ReadingStatus::Finished);

                m_workerThread.quit();
                m_workerThread.wait();
            });
}

TwController::~TwController()
{
    m_workerThread.quit();
    m_workerThread.wait();
}

TwWordsModel *TwController::wordsModel() const
{
    return m_wordsModel;
}

QUrl TwController::fileUrl() const
{
    return m_fileUrl;
}

TwTypes::ReadingStatus TwController::readingStatus() const
{
    return m_readingStatus;
}

float TwController::readingProgress() const
{
    return m_readingProgress;
}

void TwController::start()
{
    if (!m_workerThread.isRunning())
    {
        m_workerThread.start();
    }
}

void TwController::pause()
{
    if (m_workerThread.isRunning())
    {
        emit postCommand(TwFileWorker::Command::Pause);
    }
}

void TwController::resume()
{
    if (m_workerThread.isRunning())
    {
        emit postCommand(TwFileWorker::Command::Resume);
    }
}

void TwController::stop()
{
    if (m_workerThread.isRunning())
    {
        emit postCommand(TwFileWorker::Command::Stop);
    }
}

void TwController::clear()
{
    if (!m_workerThread.isRunning())
    {
        m_wordsModel->clear();
        setReadingStatus(TwTypes::ReadingStatus::Idle);
        setReadingProgress(0.f);
    }
}

QString TwController::fileName(const QUrl &fielUrl) const
{
    return fielUrl.fileName();
}

void TwController::setFileUrl(const QUrl &newFileUrl)
{
    if (m_fileUrl == newFileUrl)
    {
        return;
    }

    m_fileUrl = newFileUrl;
    emit fileUrlChanged(m_fileUrl);
}

void TwController::setReadingStatus(TwTypes::ReadingStatus readingStatus)
{
    if (m_readingStatus == readingStatus)
    {
        return;
    }

    m_readingStatus = readingStatus;
    emit readingStatusChanged(m_readingStatus);
}

void TwController::setReadingProgress(float readingProgress)
{
    if (m_readingProgress == readingProgress)
    {
        return;
    }

    m_readingProgress = readingProgress;
    emit readingProgressChanged(m_readingProgress);
}
