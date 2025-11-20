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
    , m_back_button_(new QPushButton(QStringLiteral("Назад"), this))
    , m_next_button_(new QPushButton(QStringLiteral("Далее"), this))
    , m_installer_(new InstallerService(this))
{
    m_packages_ = PackageRepository::availablePackages();

    setupUi();

    m_selection_widget_->setPackages(m_packages_);

    connect(m_back_button_, &QPushButton::clicked,
            this, &MainWindow::onBackClicked);
    connect(m_next_button_, &QPushButton::clicked,
            this, &MainWindow::onNextClicked);

    connect(m_installer_, &InstallerService::installationStarted,
            this, &MainWindow::onInstallationStarted);
    connect(m_installer_, &InstallerService::installationFinished,
            this, &MainWindow::onInstallationFinished);
    connect(m_installer_, &InstallerService::installationOutput,
            this, &MainWindow::onInstallationOutput);
    //connect(m_installer_, &InstallerService::dependencyProblemsDetected,
    //        this, &MainWindow::onDependencyProblemsDetected);


    updateUiForState();
}

MainWindow::~MainWindow() = default;

void MainWindow::setupUi()
{
    auto* central = new QWidget(this);
    setCentralWidget(central);

    auto* mainLayout = new QVBoxLayout(central);

    m_stack_->addWidget(m_intro_widget_);       // index 0
    m_stack_->addWidget(m_selection_widget_);   // index 1

    mainLayout->addWidget(m_stack_);

    auto* buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_back_button_);
    buttonLayout->addWidget(m_next_button_);

    mainLayout->addLayout(buttonLayout);

    setWindowTitle(QStringLiteral("Demo Installer"));
    resize(500, 300);
}

void MainWindow::updateUiForState()
{
    switch (m_state_) {
    case WizardState::Intro:
        m_stack_->setCurrentWidget(m_intro_widget_);
        m_back_button_->setEnabled(false);
        m_back_button_->setVisible(false);
        m_next_button_->setText(QStringLiteral("Далее"));
        m_next_button_->setEnabled(WizardNavigation::canGoNext(m_state_));
        break;
    case WizardState::SelectPackage:
        m_stack_->setCurrentWidget(m_selection_widget_);
        m_back_button_->setVisible(true);
        m_back_button_->setEnabled(WizardNavigation::canGoBack(m_state_));
        m_next_button_->setText(QStringLiteral("Установить"));
        m_next_button_->setEnabled(WizardNavigation::canGoNext(m_state_));
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
}

void MainWindow::onInstallationFinished(const PackageDescriptor& pkg, bool success, const QString& errorMessage)
{
    Q_UNUSED(pkg);
    m_install_in_progress_ = false;
    setButtonsEnabled(true);

    if (success) {
        QMessageBox::information(this, QStringLiteral("Установка"),
                                 QStringLiteral("Пакет успешно установлен."));
    } else {
        QMessageBox::critical(this, QStringLiteral("Установка"),
                              QStringLiteral("Ошибка установки:\n%1")
                                  .arg(errorMessage));
    }
}

void MainWindow::onInstallationOutput(const QString& line)
{
    qDebug() << "OUTPUT:" << line;
}

/*void MainWindow::onDependencyProblemsDetected(const PackageDescriptor& pkg, const QString& details)
{
    m_install_in_progress_ = false;
    setButtonsEnabled(true);

    const QString text =
        QStringLiteral("При установке пакета '%1' обнаружены проблемы с зависимостями.\n\n"
                       "Сообщение dpkg:\n%2\n\n"
                       "Запустить 'apt-get -f install' для попытки автоматической установки "
                       "недостающих зависимостей?")
            .arg(pkg.displayName, details);

    auto reply = QMessageBox::question(this,
                                       QStringLiteral("Проблемы с зависимостями"),
                                       text,
                                       QMessageBox::Yes | QMessageBox::No,
                                       QMessageBox::Yes);

    if (reply == QMessageBox::Yes) {
        m_install_in_progress_ = true;
        setButtonsEnabled(false);
        //m_installer_->fixDependencies(pkg);
    } else {
        QMessageBox::warning(this,
                             QStringLiteral("Установка прервана"),
                             QStringLiteral("Установка пакета '%1' прервана из-за "
                                            "неудовлетворённых зависимостей.")
                                 .arg(pkg.displayName));
    }
}*/
