#include "package_selection_widget.h"

#include <QVBoxLayout>
#include <QComboBox>
#include <QLabel>

PackageSelectionWidget::PackageSelectionWidget(QWidget* parent)
    : QWidget(parent)
    , m_label_(new QLabel(this))
    , m_combo_(new QComboBox(this))
{
    auto* layout = new QVBoxLayout(this);

    m_label_->setText(QStringLiteral("Выберите пакет для установки:"));
    layout->addWidget(m_label_);
    layout->addWidget(m_combo_);
    layout->addStretch();

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
