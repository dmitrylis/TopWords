#include "TwWordsModel.h"

#include <queue>

namespace {

constexpr int WORDS_RENDER_LIMIT {15};

struct WordsComparator
{
    constexpr bool operator()(const TwWordsModel::Word &l, const TwWordsModel::Word &r) const noexcept
    {
        return l.second < r.second;
    }
};

}

TwWordsModel::TwWordsModel(QObject *parent)
    : QAbstractListModel {parent}
{
}

int TwWordsModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
    {
        return 0;
    }

    return m_topWords.size();
}

QVariant TwWordsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }

    const Word &word {m_topWords[index.row()]};
    switch (role)
    {
    case WordsRoles::WordRole:
        return word.first;
    case WordsRoles::CountRole:
        return word.second;
    default:
        break;
    }

    return QVariant();
}

QHash<int, QByteArray> TwWordsModel::roleNames() const
{
    return {
        {WordsRoles::WordRole, "WordRole"},
        {WordsRoles::CountRole, "CountRole"}
    };
}

int TwWordsModel::maxWordCount() const
{
    return m_maxWordCount;
}

void TwWordsModel::append(const QStringList &words)
{
    for (const QString &word : words)
    {
        ++m_wordsCounter[word.toLower()];
    }

    beginResetModel();

    std::priority_queue<Word, std::vector<Word>, WordsComparator> tempQueue {m_wordsCounter.begin(), m_wordsCounter.end()};
    m_topWords.clear();

    while (!tempQueue.empty() && m_topWords.size() < ::WORDS_RENDER_LIMIT)
    {
        m_topWords.emplace_back(tempQueue.top());
        tempQueue.pop();
    }

    endResetModel();

    // Important: I could find the diff between new and old m_topWords and call beginInsertRows/endInsertRows,
    // beginRemoveRows/endRemoveRows, beginMoveRows/endMoveRows and dataChanged.
    // But for now I decided to keep it a bit simpler (but not optimal) and just reset the model

    setMaxWordCount(m_topWords.front().second);
}

void TwWordsModel::clear()
{
    m_wordsCounter.clear();

    beginResetModel();
    m_topWords.clear();
    endResetModel();

    setMaxWordCount(0);
}

void TwWordsModel::setMaxWordCount(int maxWordCount)
{
    if (m_maxWordCount == maxWordCount)
    {
        return;
    }

    m_maxWordCount = maxWordCount;
    emit maxWordCountChanged(m_maxWordCount);
}
