#include "videoplayercontroller.h"
#include "fileutil.h"

#include <QAudioOutput>
#include <QUrl>
#include <QVideoSink>

VideoPlayerController::VideoPlayerController(QObject* pParent)
    : QObject(pParent)
    , m_pPlayer(new QMediaPlayer(this))
    , m_pAudioOutput(new QAudioOutput(this))
{
    m_pPlayer->setAudioOutput(m_pAudioOutput);
    connect(m_pPlayer, &QMediaPlayer::playbackStateChanged,
        this, &VideoPlayerController::playbackStateChanged);
}

void VideoPlayerController::setVideoOutput(QVideoSink* pVideoSink)
{
    m_pPlayer->setVideoOutput(pVideoSink);
}

bool VideoPlayerController::openFile(const QString& filePath)
{
    if (filePath.isEmpty() || !fileutils::isSupportedVideoFile(filePath))
        return false;

    m_pPlayer->setSource(QUrl::fromLocalFile(filePath));

    const QFileInfo fileInfo(filePath);
    emit this->showFileOpenedMessage(fileInfo.fileName(), fileInfo.absoluteFilePath());
    return true;
}

void VideoPlayerController::playPause()
{
    if (m_pPlayer->playbackState() == QMediaPlayer::PlayingState)
        m_pPlayer->pause();
    else
        m_pPlayer->play();
}

void VideoPlayerController::stopPlay()
{
    m_pPlayer->stop();
}
