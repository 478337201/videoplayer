#pragma once

#include <QObject>
#include <QMediaPlayer>
#include <QString>

class QAudioOutput;
class QVideoSink;

class VideoPlayerController : public QObject
{
    Q_OBJECT

public:
    explicit VideoPlayerController(QObject* pParent);

    void setVideoOutput(QVideoSink* pVideoSink);
    bool openFile(const QString& filePath);
    void playPause();
    void stopPlay();

signals:
    void showFileOpenedMessage(const QString& fileName, const QString& absolutePath);
    void playbackStateChanged(QMediaPlayer::PlaybackState state);

private:
    QMediaPlayer* m_pPlayer = nullptr;
    QAudioOutput* m_pAudioOutput = nullptr;
};
