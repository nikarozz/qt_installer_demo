#include "intro_widget.h"

#include <QVBoxLayout>
#include <QLabel>

IntroWidget::IntroWidget(QWidget* parent)
    : QWidget(parent)
    , m_label_(new QLabel(this))
{
    auto* rootLayout = new QHBoxLayout(this);
    rootLayout->setContentsMargins(0, 0, 0, 0);
    rootLayout->setSpacing(0);

    auto* leftPane = new QWidget(this);
    leftPane->setMinimumWidth(200);
    leftPane->setStyleSheet("background-color: white;");

    auto* rightPane = new QWidget(this);
    rightPane->setStyleSheet("background-color: #002542;");

    rootLayout->addWidget(leftPane, 1);
    rootLayout->addWidget(rightPane, 2);

    auto* rightLayout = new QVBoxLayout(rightPane);
    rightLayout->setContentsMargins(40, 40, 40, 40);
    rightLayout->setSpacing(16);

    auto* titleLabel = new QLabel(tr("Александр\nЕрмаков"), rightPane);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(45);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setStyleSheet("color: #13a538;");

    auto* subtitleLabel = new QLabel(
        tr("Добро пожаловать в мастер установки!\n"
           "Чтобы продолжить, нажмите кнопку \"Далее\"."),
        rightPane);
    subtitleLabel->setWordWrap(true);
    QFont subtitleFont = subtitleLabel->font();
    subtitleFont.setPointSize(18);
    subtitleLabel->setFont(subtitleFont);
    subtitleLabel->setStyleSheet("color: white;");

    rightLayout->addWidget(titleLabel);
    rightLayout->addWidget(subtitleLabel);
    rightLayout->addStretch();
}


void IntroWidget::setCandidateName(const QString& name)
{
    m_label_->setText(name);
}
