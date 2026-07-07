#pragma once

#include <QMainWindow>

class QMediaPlayer;
class QAudioOutput;

namespace Ui
{
class VideoPlayerMainWindow;
}

class VideoPlayerMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit VideoPlayerMainWindow(QWidget *parent = nullptr);
    ~VideoPlayerMainWindow() override;

private slots:
    void onOpenFile();
    void onPlayPause();
    void onPlaybackStateChanged();

private:
    void updatePlayPauseButton();

    Ui::VideoPlayerMainWindow *m_pUi;
    QMediaPlayer *m_pPlayer;
    QAudioOutput *m_pAudioOutput;
};
