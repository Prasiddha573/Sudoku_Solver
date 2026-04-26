#include "homescreen.h"
#include <QPainter>
#include <QTimer>
#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QGraphicsDropShadowEffect>
#include <QPropertyAnimation>
#include <QVBoxLayout>
#include <QHBoxLayout>

HomeScreen::HomeScreen(QWidget *parent)
    : QWidget(parent)
    , m_titleScale(1.0)
{
    // No fixed size – let it expand
    setupUI();
    createBackground();

    loadBestTimes();
    updateBestTimeDisplay();

    m_titleAnimation = new QPropertyAnimation(this, "titleScale");
    m_titleAnimation->setDuration(2000);
    m_titleAnimation->setKeyValueAt(0, 1.0);
    m_titleAnimation->setKeyValueAt(0.5, 1.1);
    m_titleAnimation->setKeyValueAt(1, 1.0);
    m_titleAnimation->setEasingCurve(QEasingCurve::InOutSine);
    m_titleAnimation->setLoopCount(-1);
    m_titleAnimation->start();
}

void HomeScreen::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(40, 30, 40, 30);

    // Title
    m_titleLabel = new QLabel("SUDOKU PUZZLE");
    m_titleLabel->setAlignment(Qt::AlignCenter);
    m_titleLabel->setStyleSheet(
        "color: white;"
        "font-size: 52px;"
        "font-weight: bold;"
        "font-family: 'Arial Black';"
        "background: transparent;"
        "letter-spacing: 4px;"
        "margin-bottom: 10px;"
        );
    addNeonEffect(m_titleLabel);
    mainLayout->addWidget(m_titleLabel);

    // Subtitle
    m_subtitleLabel = new QLabel("Classic Puzzle Game");
    m_subtitleLabel->setAlignment(Qt::AlignCenter);
    m_subtitleLabel->setStyleSheet(
        "color: #3498db;"
        "font-size: 18px;"
        "letter-spacing: 2px;"
        "font-family: 'Arial';"
        "background: transparent;"
        "margin-bottom: 20px;"
        );
    mainLayout->addWidget(m_subtitleLabel);

    // Best Time with trophy
    QHBoxLayout *bestTimeLayout = new QHBoxLayout();
    bestTimeLayout->setAlignment(Qt::AlignCenter);

    QLabel *trophyLabel = new QLabel("🏆");
    trophyLabel->setStyleSheet("font-size: 32px; background: transparent;");
    bestTimeLayout->addWidget(trophyLabel);

    m_bestTimeTitle = new QLabel("BEST TIME");
    m_bestTimeTitle->setStyleSheet(
        "color: #f39c12;"
        "font-size: 24px;"
        "font-weight: bold;"
        "background: transparent;"
        );
    bestTimeLayout->addWidget(m_bestTimeTitle);

    mainLayout->addLayout(bestTimeLayout);

    // Best time value
    m_bestTimeValue = new QLabel("00:00");
    m_bestTimeValue->setAlignment(Qt::AlignCenter);
    m_bestTimeValue->setStyleSheet(
        "color: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #ffd700, stop:1 #ffa500);"
        "font-size: 48px;"
        "font-weight: 900;"
        "background: transparent;"
        "margin-bottom: 20px;"
        );
    mainLayout->addWidget(m_bestTimeValue);

    // Difficulty selection prompt
    QLabel *difficultyPrompt = new QLabel("SELECT DIFFICULTY");
    difficultyPrompt->setAlignment(Qt::AlignCenter);
    difficultyPrompt->setStyleSheet(
        "color: #3498db;"
        "font-size: 18px;"
        "font-weight: bold;"
        "letter-spacing: 2px;"
        "margin-top: 20px;"
        "margin-bottom: 10px;"
        "background: transparent;"
        );
    mainLayout->addWidget(difficultyPrompt);

    // Difficulty radio buttons (horizontal)
    QHBoxLayout *difficultyLayout = new QHBoxLayout();
    difficultyLayout->setSpacing(30);
    difficultyLayout->setAlignment(Qt::AlignCenter);

    m_difficultyGroup = new QButtonGroup(this);

    auto setupRadio = [&](QRadioButton* rb, const QString &color, int id) {
        rb->setStyleSheet(QString(
                              "QRadioButton { color: %1; font-size: 16px; font-weight: bold; spacing: 10px; background: transparent; }"
                              "QRadioButton::indicator { width: 18px; height: 18px; }"
                              "QRadioButton::indicator:checked { background-color: %1; border-radius: 9px; }"
                              ).arg(color));
        m_difficultyGroup->addButton(rb, id);
        difficultyLayout->addWidget(rb);
    };

    m_easyRadio = new QRadioButton("EASY");
    m_mediumRadio = new QRadioButton("MEDIUM");
    m_hardRadio = new QRadioButton("HARD");

    setupRadio(m_easyRadio, "#2ecc71", 0);
    setupRadio(m_mediumRadio, "#f39c12", 1);
    setupRadio(m_hardRadio, "#e74c3c", 2);
    m_mediumRadio->setChecked(true);

    mainLayout->addLayout(difficultyLayout);
    connect(m_difficultyGroup, QOverload<int>::of(&QButtonGroup::idClicked),
            this, &HomeScreen::onDifficultyChanged);

    mainLayout->addStretch();

    // Start button
    m_startBtn = new QPushButton("START GAME");
    m_startBtn->setCursor(Qt::PointingHandCursor);
    m_startBtn->setFixedHeight(60);
    m_startBtn->setMinimumWidth(280);
    m_startBtn->setStyleSheet(
        "QPushButton {"
        "   background-color: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #2ecc71, stop:1 #27ae60);"
        "   color: white;"
        "   border-radius: 30px;"
        "   font-size: 22px;"
        "   font-weight: bold;"
        "   border: none;"
        "}"
        "QPushButton:hover {"
        "   background-color: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #27ae60, stop:1 #2ecc71);"
        "}"
        "QPushButton:pressed {"
        "   background-color: #1e8449;"
        "}"
        );
    mainLayout->addWidget(m_startBtn, 0, Qt::AlignCenter);
    connect(m_startBtn, &QPushButton::clicked, this, &HomeScreen::onStartClicked);

    // Exit button
    m_exitBtn = new QPushButton("EXIT");
    m_exitBtn->setCursor(Qt::PointingHandCursor);
    m_exitBtn->setFixedHeight(50);
    m_exitBtn->setMinimumWidth(200);
    m_exitBtn->setStyleSheet(
        "QPushButton {"
        "   background-color: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #ff6b6b, stop:1 #ff4757);"
        "   color: white;"
        "   border-radius: 25px;"
        "   font-size: 18px;"
        "   font-weight: bold;"
        "   border: none;"
        "}"
        "QPushButton:hover {"
        "   background-color: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #ff4757, stop:1 #ff6b6b);"
        "}"
        "QPushButton:pressed {"
        "   background-color: #ff2222;"
        "}"
        );
    mainLayout->addWidget(m_exitBtn, 0, Qt::AlignCenter);
    connect(m_exitBtn, &QPushButton::clicked, this, &HomeScreen::onExitClicked);

    // Developer credit
    m_creditLabel = new QLabel("Developed by Prasiddha Timalsina");
    m_creditLabel->setAlignment(Qt::AlignCenter);
    m_creditLabel->setStyleSheet(
        "color: rgba(255, 255, 255, 150);"
        "font-size: 14px;"
        "font-style: italic;"
        "margin-top: 20px;"
        "background: transparent;"
        );
    mainLayout->addWidget(m_creditLabel);
}

QString HomeScreen::selectedDifficulty() const
{
    int id = m_difficultyGroup->checkedId();
    if (id == 0) return "easy";
    if (id == 1) return "medium";
    return "hard";
}

void HomeScreen::updateBestTime(const QString &difficulty, int seconds)
{
    if (!m_bestTimes.contains(difficulty) || seconds < m_bestTimes[difficulty].seconds) {
        BestTimeEntry entry;
        entry.seconds = seconds;
        entry.date = QDateTime::currentDateTime().toString("dd/MM/yy");
        m_bestTimes[difficulty] = entry;
        saveBestTimes();
        updateBestTimeDisplay();
    }
}

void HomeScreen::loadBestTimes()
{
    QFile file("besttimes.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList parts = line.split(",");
            if (parts.size() == 3) {
                QString diff = parts[0];
                BestTimeEntry entry;
                entry.seconds = parts[1].toInt();
                entry.date = parts[2];
                m_bestTimes[diff] = entry;
            }
        }
        file.close();
    }
}

void HomeScreen::saveBestTimes()
{
    QFile file("besttimes.txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        for (auto it = m_bestTimes.begin(); it != m_bestTimes.end(); ++it) {
            out << it.key() << "," << it.value().seconds << "," << it.value().date << "\n";
        }
        file.close();
    }
}

void HomeScreen::updateBestTimeDisplay()
{
    QString diff = selectedDifficulty();
    if (m_bestTimes.contains(diff)) {
        int secs = m_bestTimes[diff].seconds;
        int minutes = secs / 60;
        int seconds = secs % 60;
        m_bestTimeValue->setText(QString("%1:%2")
                                     .arg(minutes, 2, 10, QChar('0'))
                                     .arg(seconds, 2, 10, QChar('0')));
    } else {
        m_bestTimeValue->setText("--:--");
    }
}

void HomeScreen::createBackground()
{
    // Gradient will be updated in resizeEvent
    m_backgroundGradient = QLinearGradient(0, 0, width(), height());
    m_backgroundGradient.setColorAt(0.0, QColor(20, 30, 40));
    m_backgroundGradient.setColorAt(0.3, QColor(30, 40, 60));
    m_backgroundGradient.setColorAt(0.6, QColor(40, 30, 60));
    m_backgroundGradient.setColorAt(1.0, QColor(20, 20, 40));
}

void HomeScreen::addNeonEffect(QWidget *widget)
{
    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect();
    effect->setBlurRadius(30);
    effect->setColor(QColor(0, 255, 255, 100));
    effect->setOffset(0);
    widget->setGraphicsEffect(effect);
}

void HomeScreen::onDifficultyChanged(int id)
{
    Q_UNUSED(id);
    updateBestTimeDisplay();

    QString diffText = (id == 0) ? "EASY" : (id == 1) ? "MEDIUM" : "HARD";
    m_subtitleLabel->setText(QString("DIFFICULTY: %1").arg(diffText));
    QTimer::singleShot(2000, [this]() {
        m_subtitleLabel->setText("classic puzzle game");
    });
}

void HomeScreen::onStartClicked()
{
    emit startGameClicked();
}

void HomeScreen::onExitClicked()
{
    emit exitClicked();
}

void HomeScreen::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(), m_backgroundGradient);

    // Floating particles
    painter.setPen(Qt::NoPen);
    for (int i = 0; i < 8; ++i) {
        int x = (i * 100 + QDateTime::currentMSecsSinceEpoch() / 20) % width();
        int y = (i * 70) % height();
        painter.setBrush(QColor(52, 152, 219, 20));
        painter.drawEllipse(x, y, 40, 40);
    }
}

void HomeScreen::resizeEvent(QResizeEvent *event)
{
    m_backgroundGradient.setFinalStop(width(), height());
    QWidget::resizeEvent(event);
}

void HomeScreen::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
        onStartClicked();
    } else if (event->key() == Qt::Key_Escape) {
        onExitClicked();
    }
    QWidget::keyPressEvent(event);
}
