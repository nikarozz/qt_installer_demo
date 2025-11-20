#include <QtTest/QtTest>

#include "package_repository.h"

class PackageRepositoryTests : public QObject
{
    Q_OBJECT
private slots:
    void returns_expected_packages()
    {
        auto pkgs = PackageRepository::availablePackages();
        QVERIFY(!pkgs.empty());

        bool foundGreetings = false;
        bool foundSl = false;
        bool foundSlapd = false;

        for (const auto& p : pkgs) {
            QVERIFY(!p.id.isEmpty());
            QVERIFY(!p.displayName.isEmpty());
            QVERIFY(!p.resourcePaths.isEmpty());

            for (const auto& r : p.resourcePaths) {
                QVERIFY(r.startsWith(":/packages/"));
            }

            if (p.id == "greetings") {
                foundGreetings = true;
            } else if (p.id == "sl") {
                foundSl = true;
            } else if (p.id == "slapd") {
                foundSlapd = true;
            }
        }

        QVERIFY(foundGreetings);
        QVERIFY(foundSl);
        QVERIFY(foundSlapd);
    }
};

QTEST_APPLESS_MAIN(PackageRepositoryTests)
#include "test_package_repository.moc"

