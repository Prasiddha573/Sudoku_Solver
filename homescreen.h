#ifndef HOMESCREEN_H
#define HOMESCREEN_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QButtonGroup>
#include <QRadioButton>
#include <QPropertyAnimation>
#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include <QKeyEvent>
#include <QResizeEvent>
#include <QMap>

struct BestTimeEntry {
    int seconds;
    QString date;
};

class HomeScreen : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(float titleScale READ titleScale WRITE setTitleScale)

public:
    explicit HomeScreen(QWidget *parent = nullptr);

    float titleScale() const { return m_titleScale; }
    void setTitleScale(float scale) { m_titleScale = scale; update(); }

    QString selectedDifficulty() const;
    void updateBestTime(const QString &difficulty, int seconds);

signals:
    void startGameClicked();
    void exitClicked();

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void onStartClicked();
    void onExitClicked();
    void onDifficultyChanged(int id);

private:
    void setupUI();
    void createBackground();
    void addNeonEffect(QWidget *widget);
    void loadBestTimes();
    void saveBestTimes();
    void updateBestTimeDisplay();

    QLabel *m_titleLabel;
    QLabel *m_subtitleLabel;
    QLabel *m_bestTimeTitle;
    QLabel *m_bestTimeValue;
    QButtonGroup *m_difficultyGroup;
    QRadioButton *m_easyRadio;
    QRadioButton *m_mediumRadio;
    QRadioButton *m_hardRadio;
    QPushButton *m_startBtn;
    QPushButton *m_exitBtn;
    QLabel *m_creditLabel;

    QPropertyAnimation *m_titleAnimation;
    float m_titleScale;

    QLinearGradient m_backgroundGradient;
    QMap<QString, BestTimeEntry> m_bestTimes;
};

#endif // HOMESCREEN_H
