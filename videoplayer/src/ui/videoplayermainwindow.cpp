#include "videoplayermainwindow.h"
#include "videoplayercontroller.h"
#include "fileutil.h"

#include <QAction>
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDropEvent>
#include <QEvent>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QKeySequence>
#include <QLabel>
#include <QMimeData>
#include <QMouseEvent>
#include <QPushButton>
#include <QStatusBar>
#include <QStringList>
#include <QUrl>
#include <QVBoxLayout>
#include <QVideoWidget>

namespace
{
    QString getFirstSupportedVideoPath(const QMimeData* pMimeData)
    {
        if (!pMimeData || !pMimeData->hasUrls())
            return QString();

        for (const QUrl& url : pMimeData->urls())
        {
            if (!url.isLocalFile())
                continue;
            const QString filePath = url.toLocalFile();
            if (fileutils::isSupportedVideoFile(filePath))
                return filePath;
        }
        return QString();
    }
}

VideoPlayerMainWindow::VideoPlayerMainWindow(QWidget* pParent, VideoPlayerController* pController)
    : QMainWindow(pParent)
    , m_pController(pController)
    , m_isFullScreen(false)
{
    if (!m_pController)
        return;

    _initUI();

    m_pVideoWidget->installEventFilter(this);
    m_pController->setVideoOutput(m_pVideoWidget->videoSink());

    connect(m_pBtnOpenFile, &QPushButton::clicked,
        this, &VideoPlayerMainWindow::onOpenFile);
    connect(m_pBtnPlayPause, &QPushButton::clicked,
        m_pController, &VideoPlayerController::playPause);
    connect(m_pBtnStopPlay, &QPushButton::clicked,
        m_pController, &VideoPlayerController::stopPlay);
    connect(m_pController, &VideoPlayerController::showFileOpenedMessage,
        this, &VideoPlayerMainWindow::onShowFileOpenedMessage);
    connect(m_pController, &VideoPlayerController::playbackStateChanged,
        this, &VideoPlayerMainWindow::onPlaybackStateChanged);

    QAction* pPlayPauseAction = new QAction(this);
    pPlayPauseAction->setShortcut(QKeySequence(Qt::Key_Space));
    connect(pPlayPauseAction, &QAction::triggered,
        m_pController, &VideoPlayerController::playPause);
    addAction(pPlayPauseAction);
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

    m_pController->openFile(filePath);
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
    if (pWatched == m_pVideoWidget && pEvent->type() == QEvent::MouseButtonDblClick)
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
    QStringList patterns;
    for (const QString& suffix : fileutils::supportedSuffixes)
        patterns << QString("*.%1").arg(suffix);

    const QString filePath = QFileDialog::getOpenFileName(this, tr("Open Video"),
        QString(), tr("Video Files (%1);;All Files (*.*)").arg(patterns.join(" ")));
    m_pController->openFile(filePath);
}

void VideoPlayerMainWindow::onShowFileOpenedMessage(const QString& fileName, const QString& absolutePath)
{
    m_pFilePathLabel->setToolTip(absolutePath);
    m_pFilePathLabel->setText(tr("Current file: %1").arg(fileName));
    m_pBtnPlayPause->setEnabled(true);
}

void VideoPlayerMainWindow::onPlaybackStateChanged(QMediaPlayer::PlaybackState state)
{
    switch (state)
    {
    case QMediaPlayer::PlayingState:
    {
        m_pBtnStopPlay->setEnabled(true);
        m_pBtnPlayPause->setText(tr("Pause"));
    } break;
    case QMediaPlayer::PausedState:
        m_pBtnPlayPause->setText(tr("Play"));
        break;
    case QMediaPlayer::StoppedState:
    {
        m_pBtnStopPlay->setEnabled(false);
        m_pBtnPlayPause->setText(tr("Play"));
    } break;
    }
}

void VideoPlayerMainWindow::onToggleFullScreen()
{
    if (m_isFullScreen)
    {
        m_pControlPanel->show();
        setStyleSheet("");
        showNormal();
        statusBar()->show();
        m_isFullScreen = false;
    }
    else
    {
        m_pControlPanel->hide();
        setStyleSheet("QMainWindow { background-color: black; }");
        statusBar()->hide();
        showFullScreen();
        m_isFullScreen = true;
    }
}

void VideoPlayerMainWindow::_initUI()
{
    resize(960, 600);
    setWindowTitle(tr("VideoPlayerMainWindow"));

    QWidget* pCentral = new QWidget(this);
    setCentralWidget(pCentral);

    QVBoxLayout* pMainLayout = new QVBoxLayout(pCentral);
    pMainLayout->setContentsMargins(0, 0, 0, 0);

    m_pVideoWidget = new QVideoWidget(pCentral);
    m_pVideoWidget->setAspectRatioMode(Qt::KeepAspectRatioByExpanding);
    pMainLayout->addWidget(m_pVideoWidget);

    m_pControlPanel = new QWidget(pCentral);
    m_pControlPanel->setMaximumHeight(40);
    QHBoxLayout* pControlLayout = new QHBoxLayout(m_pControlPanel);

    m_pBtnOpenFile = new QPushButton(tr("Open File"), m_pControlPanel);
    m_pBtnPlayPause = new QPushButton(tr("Play"), m_pControlPanel);
    m_pBtnPlayPause->setEnabled(false);
    m_pBtnStopPlay = new QPushButton(tr("Stop"), m_pControlPanel);
    m_pBtnStopPlay->setEnabled(false);

    pControlLayout->addWidget(m_pBtnOpenFile);
    pControlLayout->addStretch();
    pControlLayout->addWidget(m_pBtnPlayPause);
    pControlLayout->addStretch();
    pControlLayout->addWidget(m_pBtnStopPlay);

    pMainLayout->addWidget(m_pControlPanel);

    m_pFilePathLabel = new QLabel(statusBar());
    statusBar()->addWidget(m_pFilePathLabel);
}
