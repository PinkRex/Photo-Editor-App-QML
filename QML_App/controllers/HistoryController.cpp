#include "HistoryController.h"
#include "globals/AppState.h"
#include "globals/Helper.h"
#include "controllers/ActionLogController.h"

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
        AppState::instance()->setEdittingBaseImage(Helper::CvMatToQPixmap(top));

        // Log action
        ActionLogController::instance()->pushAction(QString("Undo"));
        return;
    }
    AppState::instance()->setCurrentImage(currentImage);

    // Log action
    ActionLogController::instance()->pushAction(QString("Undo"));
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

        // Log action
        ActionLogController::instance()->pushAction(QString("Redo"));
        return;
    }
    AppState::instance()->setCurrentImage(currentImage);
    AppState::instance()->setEdittingBaseImage(currentImage);

    // Log action
    ActionLogController::instance()->pushAction(QString("Redo"));
}
