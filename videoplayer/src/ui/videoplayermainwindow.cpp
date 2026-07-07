#include "videoplayermainwindow.h"
#include "ui_videoplayermainwindow.h"

#include <QMediaPlayer>
#include <QAudioOutput>
#include <QFileDialog>
#include <QAction>

VideoPlayerMainWindow::VideoPlayerMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_pUi(new Ui::VideoPlayerMainWindow)
    , m_pPlayer(new QMediaPlayer(this))
    , m_pAudioOutput(new QAudioOutput(this))
{
    m_pUi->setupUi(this);

    m_pPlayer->setAudioOutput(m_pAudioOutput);
    m_pPlayer->setVideoOutput(m_pUi->videoWidget);

    connect(m_pUi->btnOpenFile, &QPushButton::clicked,
        this, &VideoPlayerMainWindow::onOpenFile);
    connect(m_pUi->btnPlayPause, &QPushButton::clicked,
        this, &VideoPlayerMainWindow::onPlayPause);
    connect(m_pPlayer, &QMediaPlayer::playbackStateChanged,
        this, &VideoPlayerMainWindow::onPlaybackStateChanged);

    QAction *pPlayPauseAction = new QAction(this);
    pPlayPauseAction->setShortcut(QKeySequence(Qt::Key_Space));
    connect(pPlayPauseAction, &QAction::triggered,
            this, &VideoPlayerMainWindow::onPlayPause);
    addAction(pPlayPauseAction);
}

VideoPlayerMainWindow::~VideoPlayerMainWindow()
{
    delete m_pUi;
}

void VideoPlayerMainWindow::onOpenFile()
{
    const QString filePath = QFileDialog::getOpenFileName(this, tr("Open Video"), QString(),
        tr("Video Files (*.mp4 *.mkv *.avi *.mov *.wmv *.flv);;All Files (*.*)"));
    if (filePath.isEmpty())
        return;

    m_pPlayer->setSource(QUrl::fromLocalFile(filePath));

    m_pUi->btnPlayPause->setEnabled(true);
    m_pUi->btnPlayPause->setText(tr("Play"));
}

void VideoPlayerMainWindow::onPlayPause()
{
    if (!m_pUi->btnPlayPause->isEnabled())
        return;
    
    if (m_pPlayer->playbackState() == QMediaPlayer::PlayingState)
        m_pPlayer->pause();
    else
        m_pPlayer->play();
}

void VideoPlayerMainWindow::onPlaybackStateChanged()
{
    updatePlayPauseButton();
}

void VideoPlayerMainWindow::updatePlayPauseButton()
{
    if (m_pPlayer->playbackState() == QMediaPlayer::PlayingState)
        m_pUi->btnPlayPause->setText(tr("Pause"));
    else
        m_pUi->btnPlayPause->setText(tr("Play"));
}
