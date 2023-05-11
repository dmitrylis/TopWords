#ifndef TWCONTROLLER_H
#define TWCONTROLLER_H

#include "TwTypes.h"

#include "TwFileWorker.h"

#include <QObject>
#include <QThread>
#include <QUrl>

class TwWordsModel;

class TwController final : public QObject
{
    Q_OBJECT
    Q_PROPERTY(TwWordsModel* wordsModel READ wordsModel CONSTANT)
    Q_PROPERTY(QUrl fileUrl READ fileUrl WRITE setFileUrl NOTIFY fileUrlChanged)
    Q_PROPERTY(TwTypes::ReadingStatus readingStatus READ readingStatus NOTIFY readingStatusChanged)
    Q_PROPERTY(float readingProgress READ readingProgress NOTIFY readingProgressChanged)

public:
    explicit TwController(QObject *parent = nullptr);
    ~TwController();

    TwWordsModel *wordsModel() const;

    QUrl fileUrl() const;
    TwTypes::ReadingStatus readingStatus() const;
    float readingProgress() const;

    Q_INVOKABLE void start();
    Q_INVOKABLE void pause();
    Q_INVOKABLE void resume();
    Q_INVOKABLE void stop();
    Q_INVOKABLE void clear();

    Q_INVOKABLE QString fileName(const QUrl &fielUrl) const;

signals:
    void fileUrlChanged(const QUrl &fielUrl);
    void readingStatusChanged(TwTypes::ReadingStatus readingStatus);
    void readingProgressChanged(float readingProgress);

    void postCommand(TwFileWorker::Command command);

protected:
    void setFileUrl(const QUrl &newFileUrl);
    void setReadingStatus(TwTypes::ReadingStatus readingStatus);
    void setReadingProgress(float readingProgress);

private:
    QThread m_workerThread;
    TwFileWorker m_fileWorker;

    TwWordsModel *m_wordsModel {nullptr};

    QUrl m_fileUrl;
    TwTypes::ReadingStatus m_readingStatus {TwTypes::ReadingStatus::Idle};
    float m_readingProgress {0.f};
};

#endif // TWCONTROLLER_H
