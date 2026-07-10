#pragma once

#include <QMainWindow>

class QMediaPlayer;
class QAudioOutput;
class QDragEnterEvent;
class QDropEvent;
class QDragLeaveEvent;

namespace Ui
{
    class VideoPlayerMainWindow;
}

class VideoPlayerMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit VideoPlayerMainWindow(QWidget* pParent = nullptr);
    ~VideoPlayerMainWindow() override;

protected:
    void dragEnterEvent(QDragEnterEvent* pEvent) override;
    void dropEvent(QDropEvent* pEvent) override;
    void dragLeaveEvent(QDragLeaveEvent* pEvent) override;
    bool eventFilter(QObject* pWatched, QEvent* pEvent) override;

private slots:
    void onOpenFile();
    void onPlayPause();
    void onStopPlay();
    void onPlaybackStateChanged();
    void onExitFullScreen();
    void onToggleFullScreen();

private:
    void _openVideoFile(const QString& filePath);

private:
    Ui::VideoPlayerMainWindow* m_pUi = nullptr;
    QMediaPlayer* m_pPlayer = nullptr;
    QAudioOutput* m_pAudioOutput = nullptr;
    bool m_isFullScreen = false;
};
