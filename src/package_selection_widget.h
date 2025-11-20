#pragma once

#include <QProgressBar>
#include <QWidget>
#include <QTextEdit>
#include <vector>
#include "package_descriptor.h"

class QComboBox;
class QLabel;

/**
 * @brief Экран выбора пакета и отображения хода установки.
 *
 * Содержит:
 *  - выпадающий список доступных пакетов;
 *  - окно логов установки;
 *  - индикатор прогресса.
 */
class PackageSelectionWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PackageSelectionWidget(QWidget* parent = nullptr);

    void setPackages(const std::vector<PackageDescriptor>& packages);
    int currentIndex() const;

    void appendLogLine(const QString& line);
    void clearLog();

    void setProgressValue(int value);

signals:
    void currentPackageChanged(int index);

private:
    QLabel* m_label_ = nullptr;
    QComboBox* m_combo_ = nullptr;
    QTextEdit* m_log_edit_ = nullptr;
    QProgressBar* m_progress_bar_ = nullptr;
};
