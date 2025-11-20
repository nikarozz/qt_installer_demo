# Qt Installer Demo

Графический инсталлятор `.deb`-пакетов на Qt5.
Все пакеты встроены в бинарник через Qt Resource System и устанавливаются локально через `dpkg -i`.
Тестирование инсталлятора проводилось на Linux Mint 22.2.

## Поддерживаемые пакеты

- **greetings** — простейший deb-пакет, созданный вручную для демонстрации сборки
- **sl** — лёгкая консольная утилита без зависимостей
- **slapd** — пакет с несколькими зависимостями (`slapd`, `ldap-utils`, `libodbc2`)

## Сборка

```bash
cmake -S . -B build -DENABLE_TESTS=ON
cmake --build build
```

## Запуск

```bash
cd build
sudo ./installer_app
```

## Тесты

```bash
ctest --test-dir build --verbose
```
## Документация

```bash
doxygen Doxyfile
```

