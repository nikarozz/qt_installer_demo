#pragma once

#include <QWidget>

class QLabel;
/**
 * @brief Стартовый экран мастера установки.
 *
 * Отображает имя кандидата и приветственный текст.
 */
class IntroWidget : public QWidget
{
    Q_OBJECT
public:
    explicit IntroWidget(QWidget* parent = nullptr);

    void setCandidateName(const QString& name);

private:
    QLabel* m_label_;
};
