#include "HistoryController.h"
#include "globals/AppState.h"
#include "globals/Helper.h"

std::deque<cv::Mat> HistoryController::undoStack;
std::deque<cv::Mat> HistoryController::redoStack;

HistoryController* HistoryController::instance() {
    static HistoryController instance;
    return &instance;
}

HistoryController::HistoryController(QObject *parent) : QObject(parent) {}

bool HistoryController::canUndo() {
    return !undoStack.empty();
}

bool HistoryController::canRedo() {
    return !redoStack.empty();
}

void HistoryController::clearRedo() {
    redoStack.clear();
}

void HistoryController::clearAll() {
    redoStack.clear();
    undoStack.clear();
}

void HistoryController::push(cv::Mat &image) {
    if (undoStack.size() == MAX_HISTORY) {
        undoStack.pop_front();
    }
    undoStack.push_back(image.clone());
    clearRedo();
}

void HistoryController::undo(QPixmap currentImage) {
    auto currentState = Helper::QPixmapToCvMat(currentImage);
    if (canUndo()) {
        if (redoStack.size() == MAX_HISTORY) {
            redoStack.pop_front();
        }
        redoStack.push_back(currentState.clone());

        cv::Mat top = undoStack.back();
        undoStack.pop_back();
        AppState::instance()->setCurrentImage(Helper::CvMatToQPixmap(top));
        return;
    }
    AppState::instance()->setCurrentImage(currentImage);
}

void HistoryController::redo(QPixmap currentImage) {
    auto currentState = Helper::QPixmapToCvMat(currentImage);
    if (canRedo()) {
        if (undoStack.size() == MAX_HISTORY) {
            undoStack.pop_front();
        }
        undoStack.push_back(currentState.clone());

        cv::Mat top = redoStack.back();
        redoStack.pop_back();
        AppState::instance()->setCurrentImage(Helper::CvMatToQPixmap(top));
        return;
    }
    AppState::instance()->setCurrentImage(currentImage);
}
