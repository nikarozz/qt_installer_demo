#pragma once

#include <QMainWindow>
#include <vector>

#include "package_descriptor.h"
#include "wizard_navigation.h"

class IntroWidget;
class PackageSelectionWidget;
class InstallerService;
class QPushButton;
class QStackedWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

private slots:
    void onBackClicked();
    void onNextClicked();

    void onInstallationStarted(const PackageDescriptor& pkg);
    void onInstallationFinished(const PackageDescriptor& pkg, bool success, const QString& errorMessage);
    void onInstallationOutput(const QString& line);

private:
    void setupUi();
    void updateUiForState();
    PackageDescriptor currentSelectedPackage() const;
    void setButtonsEnabled(bool enabled);

    WizardState m_state_;
    IntroWidget* m_intro_widget_;
    PackageSelectionWidget* m_selection_widget_;
    QStackedWidget* m_stack_;
    QPushButton* m_back_button_;
    QPushButton* m_next_button_;
    QPushButton* m_cancel_button_;
    QPushButton* m_install_button_;
    QPushButton* m_finish_button_;

    std::vector<PackageDescriptor> m_packages_;
    InstallerService* m_installer_;
    bool m_install_in_progress_ = false;
    bool m_is_any_installed_ = false;
};
