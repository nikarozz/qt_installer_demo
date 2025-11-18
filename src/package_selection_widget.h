#pragma once

#include <QWidget>
#include <vector>
#include "package_descriptor.h"

class QComboBox;
class QLabel;

class PackageSelectionWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PackageSelectionWidget(QWidget* parent = nullptr);

    void setPackages(const std::vector<PackageDescriptor>& packages);
    int currentIndex() const;

signals:
    void currentPackageChanged(int index);

private:
    QLabel* m_label_;
    QComboBox* m_combo_;
};
