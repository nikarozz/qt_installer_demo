#include "package_selection_widget.h"

#include <QVBoxLayout>
#include <QComboBox>
#include <QLabel>

PackageSelectionWidget::PackageSelectionWidget(QWidget* parent)
    : QWidget(parent)
    , m_label_(new QLabel(tr("Выберите пакет для установки"), this))
    , m_combo_(new QComboBox(this))
    , m_log_edit_(new QTextEdit(this))
    , m_progress_bar_(new QProgressBar(this))
{
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(24, 24, 24, 24);
    layout->setSpacing(16);

    QFont titleFont = m_label_->font();
    titleFont.setPointSize(16);
    titleFont.setBold(true);
    m_label_->setFont(titleFont);

    m_log_edit_->setReadOnly(true);
    m_log_edit_->setMinimumHeight(180);
    m_log_edit_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    m_progress_bar_->setMinimum(0);
    m_progress_bar_->setMaximum(100);
    m_progress_bar_->setValue(0);
    m_progress_bar_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    layout->addWidget(m_label_);
    layout->addWidget(m_combo_);
    layout->addWidget(m_log_edit_);
    layout->addWidget(m_progress_bar_);

    this->setStyleSheet(
        "color: #002542;"
        "QComboBox { background-color: white; color: black; border: 1px solid #134f7a;}"
        "QTextEdit { background-color: #002542; color: #f0f0f0; border: 1px solid #134f7a; }"
        "QProgressBar { background: #002542; border: 1px solid #134f7a; }"
        "QProgressBar::chunk { background-color: #13a538; }"
        );

    connect(m_combo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &PackageSelectionWidget::currentPackageChanged);
}


void PackageSelectionWidget::setPackages(const std::vector<PackageDescriptor>& packages)
{
    m_combo_->clear();
    for (const auto& pkg : packages) {
        m_combo_->addItem(pkg.displayName);
    }
}

int PackageSelectionWidget::currentIndex() const
{
    return m_combo_->currentIndex();
}

void PackageSelectionWidget::appendLogLine(const QString& line)
{
    m_log_edit_->append(line);
}

void PackageSelectionWidget::clearLog()
{
    m_log_edit_->clear();
}


void PackageSelectionWidget::setProgressValue(int value)
{
    m_progress_bar_->setMinimum(0);
    m_progress_bar_->setMaximum(100);
    m_progress_bar_->setValue(value);
}
