#include "intro_widget.h"

#include <QVBoxLayout>
#include <QLabel>

IntroWidget::IntroWidget(QWidget* parent)
    : QWidget(parent)
    , m_label_(new QLabel(this))
{
    auto* layout = new QVBoxLayout(this);
    layout->addStretch();
    layout->addWidget(m_label_, 0, Qt::AlignCenter);
    layout->addStretch();

    m_label_->setText(QStringLiteral("Ермаков"));
    m_label_->setAlignment(Qt::AlignCenter);
}

void IntroWidget::setCandidateName(const QString& name)
{
    m_label_->setText(name);
}
