#ifndef TWWORDSMODEL_H
#define TWWORDSMODEL_H

#include <QAbstractListModel>

#include <unordered_map>

class TwWordsModel final : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int maxWordCount READ maxWordCount NOTIFY maxWordCountChanged)

public:
    using Word = std::pair<QString, int>;

    enum WordsRoles
    {
        WordRole = Qt::UserRole + 1,
        CountRole
    };
    Q_ENUM(WordsRoles)

    explicit TwWordsModel(QObject *parent = nullptr);

    // overrides
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    // custom methods
    int maxWordCount() const;

    void append(const QStringList &words);
    void clear();

signals:
    void maxWordCountChanged(int maxWordCount);

protected:
    void setMaxWordCount(int maxWordCount);

private:
    std::unordered_map<QString, int> m_wordsCounter;
    std::vector<Word> m_topWords;

    int m_maxWordCount {0};
};

#endif // TWWORDSMODEL_H
