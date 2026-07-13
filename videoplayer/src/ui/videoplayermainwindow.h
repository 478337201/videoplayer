#pragma once

#include <QMainWindow>
#include <QMediaPlayer>
#include <QString>

class QDragEnterEvent;
class QDropEvent;
class QDragLeaveEvent;
class QEvent;
class QLabel;
class QPushButton;
class QVideoWidget;
class QWidget;
class VideoPlayerController;

class VideoPlayerMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    VideoPlayerMainWindow(QWidget* pParent, VideoPlayerController* pController);
    ~VideoPlayerMainWindow() override = default;

protected:
    void dragEnterEvent(QDragEnterEvent* pEvent) override;
    void dropEvent(QDropEvent* pEvent) override;
    void dragLeaveEvent(QDragLeaveEvent* pEvent) override;
    bool eventFilter(QObject* pWatched, QEvent* pEvent) override;

private slots:
    void onOpenFile();
    void onShowFileOpenedMessage(const QString& fileName, const QString& absolutePath);
    void onPlaybackStateChanged(QMediaPlayer::PlaybackState state);
    void onToggleFullScreen();

private:
    void _initUI();

private:
    VideoPlayerController* m_pController = nullptr;
    QVideoWidget* m_pVideoWidget = nullptr;
    QWidget* m_pControlPanel = nullptr;
    QPushButton* m_pBtnOpenFile = nullptr;
    QPushButton* m_pBtnPlayPause = nullptr;
    QPushButton* m_pBtnStopPlay = nullptr;
    QLabel* m_pFilePathLabel = nullptr;
    bool m_isFullScreen = false;
};
