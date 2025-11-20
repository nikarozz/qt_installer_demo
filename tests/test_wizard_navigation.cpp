#include <QtTest/QtTest>

#include "wizard_navigation.h"

class WizardNavigationTests : public QObject
{
    Q_OBJECT
private slots:
    void next_from_intro_goes_to_selectPackage()
    {
        WizardState s = WizardState::Intro;
        WizardState next = WizardNavigation::next(s);
        QCOMPARE(next, WizardState::SelectPackage);
    }

    void back_from_selectPackage_goes_to_intro()
    {
        WizardState s = WizardState::SelectPackage;
        WizardState back = WizardNavigation::back(s);
        QCOMPARE(back, WizardState::Intro);
    }

    void back_from_intro_stays_intro()
    {
        WizardState s = WizardState::Intro;
        WizardState back = WizardNavigation::back(s);
        QCOMPARE(back, WizardState::Intro);
    }

    void canGoNext_is_true_only_from_intro()
    {
        QVERIFY(WizardNavigation::canGoNext(WizardState::Intro));
        QVERIFY(!WizardNavigation::canGoNext(WizardState::SelectPackage));
    }

    void canGoBack_only_from_selectPackage()
    {
        QVERIFY(!WizardNavigation::canGoBack(WizardState::Intro));
        QVERIFY(WizardNavigation::canGoBack(WizardState::SelectPackage));
    }
};

QTEST_APPLESS_MAIN(WizardNavigationTests)
#include "test_wizard_navigation.moc"

