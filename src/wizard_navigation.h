#pragma once

enum class WizardState {
    Intro,
    SelectPackage,
    // Outro,
};

class WizardNavigation
{
public:
    static WizardState next(WizardState current);
    static WizardState back(WizardState current);

    static bool canGoNext(WizardState current);
    static bool canGoBack(WizardState current);
};
