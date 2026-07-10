#include "videoplayermainwindow.h"
#include "ui_videoplayermainwindow.h"

#include <QMediaPlayer>
#include <QAudioOutput>
#include <QFileDialog>
#include <QDropEvent>
#include <QMimeData>

namespace
{
    bool isSupportedVideoFile(const QString& filePath)
    {
        static const QStringList supportedSuffixes = 
        {
            "mp4", "mkv", "avi", "mov", "wmv", "flv"
        };
        return supportedSuffixes.contains(QFileInfo(filePath).suffix().toLower());
    }

    QString getFirstSupportedVideoPath(const QMimeData* pMimeData)
    {
        if (!pMimeData || !pMimeData->hasUrls())
            return {};
        for (const QUrl& url : pMimeData->urls())
        {
            if (!url.isLocalFile())
                continue;
            const QString filePath = url.toLocalFile();
            if (isSupportedVideoFile(filePath))
                return filePath;
        }
        return {};
    }
}

VideoPlayerMainWindow::VideoPlayerMainWindow(QWidget* pParent)
    : QMainWindow(pParent)
    , m_pUi(new Ui::VideoPlayerMainWindow)
    , m_pPlayer(new QMediaPlayer(this))
    , m_pAudioOutput(new QAudioOutput(this))
    , m_isFullScreen(false)
{
    m_pUi->setupUi(this);
    m_pUi->videoWidget->installEventFilter(this);
    m_pPlayer->setAudioOutput(m_pAudioOutput);
    m_pPlayer->setVideoOutput(m_pUi->videoWidget);

    connect(m_pUi->btnOpenFile, &QPushButton::clicked,
        this, &VideoPlayerMainWindow::onOpenFile);
    connect(m_pUi->btnPlayPause, &QPushButton::clicked,
        this, &VideoPlayerMainWindow::onPlayPause);
    connect(m_pUi->btnStopPlay, &QPushButton::clicked,
        this, &VideoPlayerMainWindow::onStopPlay); 
    connect(m_pPlayer, &QMediaPlayer::playbackStateChanged,
        this, &VideoPlayerMainWindow::onPlaybackStateChanged);

    QAction* pPlayPauseAction = new QAction(this);
    pPlayPauseAction->setShortcut(QKeySequence(Qt::Key_Space));
    connect(pPlayPauseAction, &QAction::triggered,
            this, &VideoPlayerMainWindow::onPlayPause);
    addAction(pPlayPauseAction);
    QAction* pExitFullScreenAction = new QAction(this);
    pExitFullScreenAction->setShortcut(QKeySequence(Qt::Key_Escape));
    connect(pExitFullScreenAction, &QAction::triggered,
        this, &VideoPlayerMainWindow::onExitFullScreen);
    addAction(pExitFullScreenAction);
    QAction* pToggleFullScreenAction = new QAction(this);
    pToggleFullScreenAction->setShortcut(QKeySequence(Qt::Key_F));
    connect(pToggleFullScreenAction, &QAction::triggered,
        this, &VideoPlayerMainWindow::onToggleFullScreen);
    addAction(pToggleFullScreenAction);
}

VideoPlayerMainWindow::~VideoPlayerMainWindow()
{
    delete m_pUi;
}

void VideoPlayerMainWindow::dragEnterEvent(QDragEnterEvent* pEvent)
{
    if (getFirstSupportedVideoPath(pEvent->mimeData()).isEmpty())
        pEvent->ignore();
    else
    {
        setStyleSheet("QMainWindow { border: 2px dashed red; }");
        pEvent->acceptProposedAction();
    }
}

void VideoPlayerMainWindow::dropEvent(QDropEvent* pEvent)
{
    const QString filePath = getFirstSupportedVideoPath(pEvent->mimeData());
    if (filePath.isEmpty())
    {
        pEvent->ignore();
        return;
    }

    _openVideoFile(filePath);
    pEvent->acceptProposedAction();
    setStyleSheet("");
}

void VideoPlayerMainWindow::dragLeaveEvent(QDragLeaveEvent* pEvent)
{
    QMainWindow::dragLeaveEvent(pEvent);
    setStyleSheet("");
}

bool VideoPlayerMainWindow::eventFilter(QObject* pWatched, QEvent* pEvent)
{
    if (pWatched == m_pUi->videoWidget && pEvent->type() == QEvent::MouseButtonDblClick)
    {
        QMouseEvent* pMouseEvent = static_cast<QMouseEvent*>(pEvent);
        if (pMouseEvent->button() == Qt::LeftButton)
        {
            onToggleFullScreen();
            return true;
        }
    }
    return QMainWindow::eventFilter(pWatched, pEvent);
}

void VideoPlayerMainWindow::onOpenFile()
{
    const QString filePath = QFileDialog::getOpenFileName(this, tr("Open Video"), QString(),
        tr("Video Files (*.mp4 *.mkv *.avi *.mov *.wmv *.flv);;All Files (*.*)"));
    _openVideoFile(filePath);
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

void VideoPlayerMainWindow::onStopPlay()
{
    m_pPlayer->stop();
}

void VideoPlayerMainWindow::onPlaybackStateChanged()
{
    switch (m_pPlayer->playbackState())
    {
    case QMediaPlayer::PlayingState:
    {
        m_pUi->btnStopPlay->setEnabled(true);
        m_pUi->btnPlayPause->setText(tr("Pause"));
    } break;
    case QMediaPlayer::PausedState:
        m_pUi->btnPlayPause->setText(tr("Play"));
        break;    
    case QMediaPlayer::StoppedState:
    {
        m_pUi->btnStopPlay->setEnabled(false);
        m_pUi->btnPlayPause->setText(tr("Play"));
    } break;
    }
}

void VideoPlayerMainWindow::onExitFullScreen()
{
    m_pUi->controlPanel->show();
    setStyleSheet("");
    showNormal();
    statusBar()->show();
    m_isFullScreen = false;
}

void VideoPlayerMainWindow::onToggleFullScreen()
{
    if (m_isFullScreen)
    {
        m_pUi->controlPanel->show();
        setStyleSheet("");
        showNormal();
        statusBar()->show();
        m_isFullScreen = false;
    }
    else
    {
        m_pUi->controlPanel->hide();
        setStyleSheet("QMainWindow { background-color: black; }");
        statusBar()->hide();
        showFullScreen();
        m_isFullScreen = true;
    }
}

void VideoPlayerMainWindow::_openVideoFile(const QString& filePath)
{
    if (filePath.isEmpty() || !isSupportedVideoFile(filePath))
        return;

    m_pPlayer->setSource(QUrl::fromLocalFile(filePath));
    const QFileInfo fileInfo(filePath);
    const QString displayText = fileInfo.absoluteFilePath();
    m_pUi->filePathLabel->setToolTip(displayText);
    m_pUi->filePathLabel->setText(tr("Current file: %1").arg(fileInfo.fileName()));
    m_pUi->btnPlayPause->setEnabled(true);
}
