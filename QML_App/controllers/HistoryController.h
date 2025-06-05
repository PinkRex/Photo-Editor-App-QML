#ifndef HISTORYCONTROLLER_H
#define HISTORYCONTROLLER_H

#include <QObject>
#include <deque>
#include <QDebug>
#include <opencv2/core/core.hpp>

class HistoryController : public QObject {
    Q_OBJECT

public:
    explicit HistoryController(QObject *parent = nullptr);
    static HistoryController* instance();

    Q_INVOKABLE void undo(QPixmap currentImage);
    Q_INVOKABLE void redo(QPixmap currentImage);
    Q_INVOKABLE void clearAll();

    void push(cv::Mat &image);
    bool canUndo();
    bool canRedo();
    void clearRedo();

private:
    const size_t MAX_HISTORY = 20;
    static std::deque<cv::Mat> undoStack;
    static std::deque<cv::Mat> redoStack;
};

#endif // HISTORYCONTROLLER_H
