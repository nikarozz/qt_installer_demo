#include "main_window.h"
#include "intro_widget.h"
#include "package_selection_widget.h"
#include "package_repository.h"
#include "installer_service.h"

#include <QStackedWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , m_state_(WizardState::Intro)
    , m_intro_widget_(new IntroWidget(this))
    , m_selection_widget_(new PackageSelectionWidget(this))
    , m_stack_(new QStackedWidget(this))
    , m_back_button_(new QPushButton(tr("Назад"), this))
    , m_next_button_(new QPushButton(tr("Далее"), this))
    , m_cancel_button_(new QPushButton(tr("Отмена"), this))
    , m_finish_button_(new QPushButton(tr("Завершить"), this))
    , m_install_button_(new QPushButton(tr("Установить"), this))
    , m_installer_(new InstallerService(this))
{
    m_packages_ = PackageRepository::availablePackages();
    m_selection_widget_->setPackages(m_packages_);

    setupUi();

    connect(m_back_button_, &QPushButton::clicked,
            this, &MainWindow::onBackClicked);
    connect(m_next_button_, &QPushButton::clicked,
            this, &MainWindow::onNextClicked);
    connect(m_install_button_, &QPushButton::clicked,
            this, &MainWindow::onNextClicked);
    connect(m_cancel_button_, &QPushButton::clicked,
            this, &MainWindow::close);
    connect(m_finish_button_, &QPushButton::clicked,
            this, &MainWindow::close);

    connect(m_installer_, &InstallerService::installationStarted,
            this, &MainWindow::onInstallationStarted);
    connect(m_installer_, &InstallerService::installationFinished,
            this, &MainWindow::onInstallationFinished);
    connect(m_installer_, &InstallerService::installationOutput,
            this, &MainWindow::onInstallationOutput);
    connect(m_installer_, &InstallerService::installationProgress,
            this, &MainWindow::onInstallationProgress);

    updateUiForState();
}

MainWindow::~MainWindow() = default;

void MainWindow::setupUi()
{
    auto* central = new QWidget(this);
    setCentralWidget(central);

    auto* mainLayout = new QVBoxLayout(central);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    mainLayout->addWidget(m_stack_, 1);

    auto* buttonsPanel = new QWidget(central);
    auto* buttonsLayout = new QHBoxLayout(buttonsPanel);
    buttonsLayout->setContentsMargins(12, 8, 12, 8);
    buttonsLayout->setSpacing(8);

    auto* leftButtons = new QHBoxLayout();
    auto* rightButtons = new QHBoxLayout();
    leftButtons->setSpacing(8);
    rightButtons->setSpacing(8);

    leftButtons->addWidget(m_cancel_button_);
    leftButtons->addWidget(m_back_button_);
    leftButtons->addStretch();

    rightButtons->addStretch();
    rightButtons->addWidget(m_install_button_);
    rightButtons->addWidget(m_finish_button_);
    rightButtons->addWidget(m_next_button_);

    buttonsLayout->addLayout(leftButtons);
    buttonsLayout->addLayout(rightButtons);

    mainLayout->addWidget(buttonsPanel, 0);

    resize(900, 600);
    setMinimumSize(800, 500);

    m_stack_->addWidget(m_intro_widget_);
    m_stack_->addWidget(m_selection_widget_);

    m_state_ = WizardState::Intro;
}

void MainWindow::updateUiForState()
{
    switch (m_state_) {
    case WizardState::Intro:
        m_stack_->setCurrentWidget(m_intro_widget_);
        m_cancel_button_->setVisible(true);
        m_cancel_button_->setEnabled(true);

        m_back_button_->setVisible(false);

        m_next_button_->setVisible(true);
        m_next_button_->setEnabled(true);

        m_install_button_->setVisible(false);
        m_finish_button_->setVisible(false);
        break;
    case WizardState::SelectPackage:
        m_stack_->setCurrentWidget(m_selection_widget_);
        m_cancel_button_->setVisible(false);

        m_back_button_->setVisible(true);
        m_back_button_->setEnabled(true);

        m_next_button_->setVisible(false);

        m_install_button_->setVisible(true);
        m_install_button_->setEnabled(true);

        m_finish_button_->setVisible(true);
        m_finish_button_->setEnabled(m_is_any_installed_);



        m_cancel_button_->setEnabled(m_is_any_installed_);


        break;
    }
}

void MainWindow::onBackClicked()
{
    if (m_install_in_progress_)
        return;

    m_state_ = WizardNavigation::back(m_state_);
    updateUiForState();
}

void MainWindow::onNextClicked()
{
    if (m_install_in_progress_)
        return;

    if (m_state_ == WizardState::Intro) {
        m_state_ = WizardNavigation::next(m_state_);
        updateUiForState();
        return;
    }

    if (m_state_ == WizardState::SelectPackage) {
        int index = m_selection_widget_->currentIndex();
        if (index < 0 || index >= static_cast<int>(m_packages_.size())) {
            QMessageBox::warning(this, QStringLiteral("Установка"),
                                 QStringLiteral("Не выбран пакет для установки."));
            return;
        }

        auto pkg = currentSelectedPackage();
        setButtonsEnabled(false);
        m_install_in_progress_ = true;
        m_installer_->installPackage(pkg);
    }
}

PackageDescriptor MainWindow::currentSelectedPackage() const
{
    int index = m_selection_widget_->currentIndex();
    if (index < 0 || index >= static_cast<int>(m_packages_.size())) {
        return m_packages_.front();
    }
    return m_packages_[static_cast<std::size_t>(index)];
}

void MainWindow::setButtonsEnabled(bool enabled)
{
    m_back_button_->setEnabled(enabled && WizardNavigation::canGoBack(m_state_));
    m_next_button_->setEnabled(enabled && WizardNavigation::canGoNext(m_state_));
}

void MainWindow::onInstallationStarted(const PackageDescriptor& pkg)
{
    qDebug() << "Установка начата для пакета:" << pkg.id;

    m_install_in_progress_ = true;
    setButtonsEnabled(false);

    if (m_selection_widget_) {
        m_selection_widget_->clearLog();
        m_selection_widget_->setProgressValue(0);
        m_selection_widget_->appendLogLine(
            QStringLiteral("Установка пакета '%1' начата...").arg(pkg.displayName));
    }
}

void MainWindow::onInstallationFinished(const PackageDescriptor& pkg, bool success, const QString& errorMessage)
{
    Q_UNUSED(pkg);
    m_install_in_progress_ = false;
    setButtonsEnabled(true);

    if (m_selection_widget_) {
        m_selection_widget_->setProgressValue(success ? 100 : 0);
    }

    if (success) {
        QMessageBox::information(this, QStringLiteral("Установка"),
                                 QStringLiteral("Пакет успешно установлен."));

        m_is_any_installed_ = true;
        if (m_state_ == WizardState::SelectPackage) {
            m_back_button_->setEnabled(false);
            m_finish_button_->setEnabled(true);
        }
    } else {
        QMessageBox::critical(this, QStringLiteral("Установка"),
                              QStringLiteral("Ошибка установки:\n%1")
                                  .arg(errorMessage));
    }
}

void MainWindow::onInstallationOutput(const QString& line)
{
    qDebug() << "OUTPUT:" << line;
    if (m_selection_widget_) {
        m_selection_widget_->appendLogLine(line);
    }
}

void MainWindow::onInstallationProgress(const PackageDescriptor& pkg, int percent)
{
    Q_UNUSED(pkg);
    if (m_selection_widget_) {
        m_selection_widget_->setProgressValue(percent);
    }
}
