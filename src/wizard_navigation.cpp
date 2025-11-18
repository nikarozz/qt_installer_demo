#include "wizard_navigation.h"

WizardState WizardNavigation::next(WizardState current)
{
    switch (current) {
    case WizardState::Intro:
        return WizardState::SelectPackage;
    case WizardState::SelectPackage:
        return WizardState::SelectPackage;
    }
    return current;
}

WizardState WizardNavigation::back(WizardState current)
{
    switch (current) {
    case WizardState::Intro:
        return WizardState::Intro;
    case WizardState::SelectPackage:
        return WizardState::Intro;
    }
    return current;
}

bool WizardNavigation::canGoNext(WizardState current)
{
    switch (current) {
    case WizardState::Intro:
        return true;
    case WizardState::SelectPackage:
        return true;
    }
    return false;
}

bool WizardNavigation::canGoBack(WizardState current)
{
    switch (current) {
    case WizardState::Intro:
        return false;
    case WizardState::SelectPackage:
        return true;
    }
    return false;
}
