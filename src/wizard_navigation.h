#pragma once

/**
 * @brief Состояния шага мастера установки.
 */
enum class WizardState {
    Intro,
    SelectPackage,
    // Outro,
};

/**
 * @brief Класс для навигации между шагами мастера.
 */
class WizardNavigation
{
public:
    static WizardState next(WizardState current);
    static WizardState back(WizardState current);

    static bool canGoNext(WizardState current);
    static bool canGoBack(WizardState current);
};
