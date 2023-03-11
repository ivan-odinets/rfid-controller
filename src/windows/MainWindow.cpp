/*
 **********************************************************************************************************************
 *
 * This file is part of RFID Controller.
 *
 * RFID Controller is free software: you can redistribute it and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * RFID Controller is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with RFID Controller. If not, see
 * <https://www.gnu.org/licenses/>.
 *
 */

#include "MainWindow.h"

#include <QActionGroup>
#include <QApplication>
#include <QCloseEvent>
#include <QDebug>
#include <QFileDialog>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QStyle>
#include <QWindowStateChangeEvent>

#include "appconfig/MainWindowSettings.h"
#include "core/commands/CommandList.h"
#include "core/RfidController.h"
#include "widgets/MonitorWidget.h"

#ifdef HID
    #include "menus/InputDeviceSelectorMenu.h"
    #include "windows/InputFilterEditDialog.h"
#endif //HID

#ifdef SERIAL
    #include "menus/SerialDeviceSelectorMenu.h"
    #include "windows/SerialPortConfiguratorDialog.h"
    #include "windows/SerialFilterEditDialog.h"
#endif //SERIAL

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),p_windowSettings(MainWindowSettings::get()),
      p_controller(RfidController::get())
{
    _setupUi();
    _loadSettings();
    _connectController();
}

MainWindow::~MainWindow()
{
    _saveSettings();
}

void MainWindow::displayLastKey(const QString& key)
{
    w_centralWidget->displayLastKey(key);
}

void MainWindow::showErrorMessageBox(const QString& errorMessage)
{
    QMessageBox::critical(this,tr("Error - %1")
                          .arg(qApp->applicationName()),errorMessage);
}

void MainWindow::showInfoMessageBox(const QString& infoMessage)
{
    QMessageBox::information(this,tr("Info - %1").arg(qApp->applicationName()),
                             infoMessage);
}

void MainWindow::commandsFileNameChanged(const QString& newFileName)
{
    if (newFileName.isEmpty()) {
        setWindowTitle(QString("%1[*]").arg(qApp->applicationName()));
        return;
    }

    setWindowTitle(QString("%1 - %2[*]").arg(qApp->applicationName()).arg(newFileName));
    if (newFileName == p_controller->defaultNewFileName())
        return;

    if (m_recentFiles.contains(newFileName))
        return;

    m_recentFiles.append(newFileName);
    _addRecentFileAction(newFileName);
}

void MainWindow::commandsFileChanged()
{
    const QMessageBox::StandardButton result = QMessageBox::question(this,qApp->applicationName(),
        tr("Current commands file edited on disk by external program. Reload file?"),
        QMessageBox::Yes|QMessageBox::No);

    if (result == QMessageBox::Yes) {
        p_controller->reloadCurrentCommandFile();
    } else {
        setWindowModified(true);
    }
}

void MainWindow::commandsFileRemoved()
{
    const QMessageBox::StandardButton result = QMessageBox::question(this,qApp->applicationName(),
        tr("Current commands file was removed from disk by external program. Save file?"),
        QMessageBox::Yes|QMessageBox::No);

    if (result == QMessageBox::Yes)
        _saveCommandsFileAs();
}

void MainWindow::setCommandList(CommandList* commandsList)
{
    w_centralWidget->setCommandList(commandsList);
    setWindowModified(false);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (w_minimizeOnClose->isChecked()) {
        QMainWindow::setWindowState(Qt::WindowMinimized);
        event->ignore();
        return;
    }

    if (!_maybeSave()) {
        event->ignore();
        return;
    }

#ifndef QT_NO_SYSTEMTRAYICON
    //If the tray icon is still visible, after main window closed - application is still running
    //Hide the tray - kill the app
    if (w_trayIcon != nullptr)
        w_trayIcon->hide();
#endif //QT_NO_SYSTEMTRAYICON

    QMainWindow::closeEvent(event);
}

void MainWindow::changeEvent(QEvent *event)
{
#ifndef QT_NO_SYSTEMTRAYICON
    if (event->type() == QEvent::WindowStateChange) {
        if ( MainWindow::isMinimized() && w_minimizeToTray->isChecked() ) {
            MainWindow::hide();
            event->ignore();
            return;
        }
    }
#endif //QT_NO_SYSTEMTRAYICON

    QMainWindow::changeEvent(event);
}

/*
 **********************************************************************************************************************
 * Menu entries - File Menu
 */

void MainWindow::_newCommandsFile()
{
    if (!_maybeSave())
        return;

    p_controller->newCommandList();
}

void MainWindow::_openCommandsFile()
{
    if (!_maybeSave())
        return;

    QFileInfo currentFileInfo(p_controller->currentFileName());
    QDir searchDir = (currentFileInfo.exists()) ? currentFileInfo.dir() : QDir::home();

    QString fileName = QFileDialog::getOpenFileName(this,tr("Open commands file - %1").arg(qApp->applicationName()),
                searchDir.path(),tr("cmds (*.cmds);; All files (*.*)"));

    if (fileName.isEmpty())
        return;

    p_controller->openCommandListFile(fileName);
}

void MainWindow::_saveCommandsFile()
{
    QString fileName = p_controller->currentFileName();
    if (fileName.isEmpty()) {
        _saveCommandsFileAs();
        return;
    }

    p_controller->saveCurrentCommandsListAs(fileName);
    setWindowModified(false);
}

void MainWindow::_saveCommandsFileAs()
{
    QFileInfo currentFileInfo(p_controller->currentFileName());
    QDir searchDir = (currentFileInfo.exists()) ? currentFileInfo.dir() : QDir::home();

    QString fileName = QFileDialog::getSaveFileName(this,tr("Save commands file - %1").arg(qApp->applicationName()),
                searchDir.path(),tr("cmds (*.cmds);; All files (*.*)"));

    if (!fileName.isEmpty())
        p_controller->saveCurrentCommandsListAs(fileName);
}

void MainWindow::_closeCommandsFile()
{
    if (!_maybeSave())
        return;

    p_controller->closeCurrentFile();
}

/*
 **********************************************************************************************************************
 * Recent Files
 */

void MainWindow::_openRecentFile(QAction* fileAction)
{
    if (!_maybeSave())
        return;

    p_controller->openCommandListFile(fileAction->text());
}

void MainWindow::_clearRecent()
{
    m_recentFiles.clear();
    foreach (QAction* action, w_recentFilesActions->actions()) {
        w_recentFilesMenu->removeAction(action);
        action->deleteLater();
    }
}

void MainWindow::_exitApplication()
{
    if (_maybeSave()) {
        qApp->exit();
    }
}

/*
 **********************************************************************************************************************
 * Menu entries - Settings Menu
 */

void MainWindow::_toggleStartHiddenOption(bool state)
{
    p_windowSettings->setStartHidden(state);
}

#define NO_NOTIFY_LITERAL    QStringLiteral("none")
#define MSG_NOTIFY_LITERAL   QStringLiteral("msg")

#ifndef QT_NO_SYSTEMTRAYICON
    #define TRAY_NOTIFY_LITERAL  QStringLiteral("tray")
#endif //QT_NO_SYSTEMTRAYICON

void MainWindow::_toggleOpenedDeviceNotifyStatus(QAction* action)
{
    switch (action->data().toInt()) {
    case NoNotify:
        p_windowSettings->setNotifyAboutOpenedDevices(NO_NOTIFY_LITERAL);
        m_connectedDeviceNotification = NoNotify;
        return;
    case MsgNotify:
        p_windowSettings->setNotifyAboutOpenedDevices(MSG_NOTIFY_LITERAL);
        m_connectedDeviceNotification = MsgNotify;
        return;
#ifndef QT_NO_SYSTEMTRAYICON
    case TrayNotify:
        p_windowSettings->setNotifyAboutOpenedDevices(TRAY_NOTIFY_LITERAL);
        m_connectedDeviceNotification = TrayNotify;
        return;
#endif //QT_NO_SYSTEMTRAYICON
    }

    m_connectedDeviceNotification = UnknownNotify;
}

/*
 **********************************************************************************************************************
 * Menu entries - Help Menu
 */

#if defined(Q_OS_LINUX) && !defined(Q_OS_ANDROID)
    #define ABOUT_MESSAGE "<b>RFID Controller</b><br>" \
                          "Version: " APP_VERSION "<br><br>" \
                          "RFID Controller - small application to use with usb-based RFID readers, which allow executing shell commands based on what key was read.<br>" \
                          "Can be used together with any other devices, which is recognized as a keyboard and producing /dev/input/* events.<br><br>" \
                          "The program is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.<br><br>" \
                          "Source code available here: <a href=https://github.com/ivan-odinets/rfid-controller>github</a><br>" \
                          "<a href=\"https://www.flaticon.com/free-icons/rfid\" title=\"rfid icons\">Rfid icons created by Freepik - Flaticon</a>"
#endif //PLATFORM SPECIFIC

void MainWindow::_aboutRfidController()
{
    QMessageBox message;
    message.setWindowTitle(tr("About %1").arg(qApp->applicationName()));
    message.setIconPixmap(QPixmap(":/icons/128x128/rfid-controller.png"));
    message.setWindowIcon(windowIcon());
    message.setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);
    message.setTextFormat(Qt::RichText);
    message.setText(tr(ABOUT_MESSAGE));
    message.exec();
}

/*
 **********************************************************************************************************************
 * Private methods
 */

void MainWindow::_loadSettings()
{
    resize(p_windowSettings->windowSize());
    w_minimizeToTray->setChecked(p_windowSettings->minimizeToTray());
    w_minimizeOnClose->setChecked(p_windowSettings->minimizeOnClose());

    m_recentFiles = p_windowSettings->recentFiles();
    foreach (QString recentFile,m_recentFiles)
        _addRecentFileAction(recentFile);

    w_startHidden->setChecked(p_windowSettings->startHidden());

    QString notifyStatus = p_windowSettings->notifyAboutOpenedDevices();
    if (notifyStatus == NO_NOTIFY_LITERAL) {
        m_connectedDeviceNotification = NoNotify;
        w_noDeviceNotificationAction->setChecked(true);
    } else if (notifyStatus == MSG_NOTIFY_LITERAL) {
        m_connectedDeviceNotification = MsgNotify;
        w_msgDeviceNotificationAction->setChecked(true);
    }
#ifndef QT_NO_SYSTEMTRAYICON
    else if (notifyStatus == TRAY_NOTIFY_LITERAL) {
        m_connectedDeviceNotification = TrayNotify;
        w_trayDeviceNotificationAction->setChecked(true);
    }
#endif //QT_NO_SYSTEMTRAYICON

#ifdef HID
    w_autoconnectHidDevices->setChecked(p_controller->inputDeviceManager()->inputDeviceAutoconnection());
#endif //HID

#ifdef SERIAL
    w_autoconnectSerialDevices->setChecked(p_controller->serialDeviceManager()->serialDeviceAutoconnection());
#endif //SERIAL
}

void MainWindow::_saveSettings()
{
    p_windowSettings->setWindowSize(this->size());
    p_windowSettings->setMinimizeOnClose(w_minimizeOnClose->isChecked());
#ifndef QT_NO_SYSTEMTRAYICON
    p_windowSettings->setMinimizeToTray(w_minimizeToTray->isChecked());
#endif //QT_NO_SYSTEMTRAYICON

    p_windowSettings->setRecentFiles(this->m_recentFiles);
}

void MainWindow::_addRecentFileAction(const QString& fileName)
{
    QAction* recentFileAction = new QAction(fileName);
    w_recentFilesMenu->insertAction(w_recentFilesSeparator,recentFileAction);
    w_recentFilesActions->addAction(recentFileAction);
}

void MainWindow::_notifyAboutDevice(const QString& messageText)
{
    if (m_connectedDeviceNotification == NoNotify)
        return;

    if (m_connectedDeviceNotification == MsgNotify) {
        showInfoMessageBox(messageText);
        return;
    }

#ifndef QT_NO_SYSTEMTRAYICON
    if (m_connectedDeviceNotification == TrayNotify) {
        if (w_trayIcon == nullptr)
            return;

        w_trayIcon->showMessage(qApp->applicationName(),messageText);
        return;
    }
#endif //QT_NO_SYSTEMTRAYICON
}

bool MainWindow::_maybeSave()
{
    if (!isWindowModified())
        return true;

    const QMessageBox::StandardButton ret
            = QMessageBox::warning(this, qApp->applicationName(),
                                   tr("The commands file has been modified.\n"
                                      "Do you want to save changes?"),
                                   QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    qDebug() << ret;
    switch (ret) {
    case QMessageBox::Save:
        _saveCommandsFile();
        return true;
    case QMessageBox::Cancel:
        return false;
    default:
        break;
    }

    return true;
}

/*
 **********************************************************************************************************************
 * This part is needed only if we have HID support enabled
 */

#ifdef HID

void MainWindow::_inputDeviceConnected(const InputDeviceInfo& deviceInfo)
{
#if defined(Q_OS_LINUX) && !defined(Q_OS_ANDROID)
    QString notificationText = tr("New HID device connected: %1\r\nVendorId=%2 ProductId=%3 Name=%4")
            .arg(deviceInfo.deviceFilePath())
            .arg(deviceInfo.vendorId())
            .arg(deviceInfo.productId())
            .arg(deviceInfo.deviceName());
#elif defined(Q_OS_WINDOWS)
    #error("Windows builds currently not supported")
#elif
    #error("Builds for other platforms are not supported")
#endif

    _notifyAboutDevice(notificationText);
}

void MainWindow::_configureInputFilter()
{
    InputFilterEditDialog dialog;
    dialog.setWindowIcon(windowIcon());
    dialog.setWindowTitle(tr("Configure HID device filter - %1").arg(qApp->applicationName()));
    dialog.displayInputFilter(p_controller->inputDeviceManager()->inputDeviceFilter());

    dialog.exec();

    if (dialog.result() != QDialog::Accepted)
        return;

    p_controller->inputDeviceManager()->setInputDeviceFilter(dialog.inputFilter());
}

#endif //HID

/*
 **********************************************************************************************************************
 * This part is needed only if we have Serial support enabled
 */

#ifdef SERIAL

void MainWindow::_serialDeviceConnected(const QSerialPortInfo& portInfo)
{
    if (m_connectedDeviceNotification == NoNotify)
        return;

    QString notificationText = tr("New Serial device connected\r\nVendorId=%1 ProductId=%2 Name=%3")
            .arg(portInfo.vendorIdentifier())
            .arg(portInfo.productIdentifier())
            .arg(portInfo.portName());

    _notifyAboutDevice(notificationText);
}

void MainWindow::_configureSerialParameters()
{
    SerialPortConfiguratorDialog dialog;
    dialog.setWindowIcon(windowIcon());
    dialog.setWindowTitle(tr("Configure default serial port parameters - %1").arg(qApp->applicationName()));
    dialog.displaySerialPortConfig(p_controller->serialDeviceManager()->defaultPortConfig());

    dialog.exec();

    if (dialog.result() != QDialog::Accepted)
        return;

    p_controller->serialDeviceManager()->setDefaultPortConfig(dialog.serialPortConfig());
}

void MainWindow::_configureSerialFilter()
{
    SerialFilterEditDialog dialog;
    dialog.setWindowIcon(windowIcon());
    dialog.setWindowTitle(tr("Configure serial device filter - %1").arg(qApp->applicationName()));
    dialog.displaySerialFilter(p_controller->serialDeviceManager()->serialDeviceFilter());

    dialog.exec();

    if (dialog.result() != QDialog::Accepted)
        return;

    p_controller->serialDeviceManager()->setSerialDeviceFilter(dialog.serialFilter());
}

#endif //SERIAL

/*
 **********************************************************************************************************************
 * This part is needed only if we have System Tray support enabled
 */

#ifndef QT_NO_SYSTEMTRAYICON

void MainWindow::_iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::DoubleClick:
    case QSystemTrayIcon::MiddleClick:
        _toggleWindowVisibility();
        break;
    default:;
    }
}

void MainWindow::_toggleWindowVisibility()
{
    //If we are minimized - restore and activate window
    if (MainWindow::isMinimized()) {
        MainWindow::showNormal();
        MainWindow::activateWindow();
        return;
    }

    //If we are visible, but not active - activate
    if (MainWindow::isVisible() && !MainWindow::isActiveWindow()) {
        MainWindow::activateWindow();
        return;
    }

    //If we are hidden - show back
    if (MainWindow::isHidden()) {
        MainWindow::showNormal();
        return;
    }

    //If we are visible and active - hide window or minimize
    if (MainWindow::isActiveWindow() && MainWindow::isVisible()) {
        MainWindow::hide();
        return;
    }

}

void MainWindow::_setupTray()
{
    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        qWarning() << tr("No system tray found!");
        return;
    }

    w_trayIcon = new QSystemTrayIcon(this);
    w_trayIcon->setIcon(QIcon(QStringLiteral(":/icons/24x24/rfid-controller.png")));

    connect(w_trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::_iconActivated);
    w_trayIcon->show();

    QMenu* trayMenu = new QMenu();

    QAction* openCommandsFileAction = trayMenu->addAction(tr("Open"));
    openCommandsFileAction->setIcon(QApplication::style()->standardIcon(QStyle::SP_DialogOpenButton));
    connect(openCommandsFileAction,&QAction::triggered,this,&MainWindow::_openCommandsFile);

    QAction* saveCommandsFileAction = trayMenu->addAction(tr("Save"));
    saveCommandsFileAction->setIcon(QApplication::style()->standardIcon(QStyle::SP_DialogSaveButton));
    connect(saveCommandsFileAction,&QAction::triggered,this,&MainWindow::_saveCommandsFile);

    QAction* exitAction = trayMenu->addAction(tr("Exit"));
    exitAction->setIcon(QApplication::style()->standardIcon(QStyle::SP_DialogCloseButton));
    connect(exitAction,&QAction::triggered,this,&MainWindow::_exitApplication);
    w_trayIcon->setContextMenu(trayMenu);
}

#endif

/*
 **********************************************************************************************************************
 * Setting up UI
 */

void MainWindow::_setupUi()
{
    setWindowTitle(QString("%1[*]").arg(qApp->applicationName()));
    setWindowIcon(QIcon(QStringLiteral(":/icons/64x64/rfid-controller.png")));

    w_centralWidget = new MonitorWidget;
    setCentralWidget(w_centralWidget);

    _setupMenus();

#ifndef QT_NO_SYSTEMTRAYICON
    _setupTray();
#endif //QT_NO_SYSTEMTRAYICON
}

void MainWindow::_setupMenus()
{
    QMenuBar* menuBar = new QMenuBar;

    //File Menu
    QMenu* fileMenu = new QMenu(tr("File"));
    QAction* newFileAction = fileMenu->addAction(tr("New"));
    newFileAction->setIcon(QApplication::style()->standardIcon(QStyle::SP_FileIcon));
    newFileAction->setShortcut(tr("Ctrl+N"));
    connect(newFileAction,&QAction::triggered,this,&MainWindow::_newCommandsFile);
    QAction* openCommandsFileAction = fileMenu->addAction(tr("Open"));
    openCommandsFileAction->setShortcut(tr("Ctrl+O"));
    openCommandsFileAction->setIcon(QApplication::style()->standardIcon(QStyle::SP_DialogOpenButton));
    connect(openCommandsFileAction,&QAction::triggered,this,&MainWindow::_openCommandsFile);
    QAction* saveCommandsFileAction = fileMenu->addAction(tr("Save"));
    saveCommandsFileAction->setIcon(QApplication::style()->standardIcon(QStyle::SP_DialogSaveButton));
    saveCommandsFileAction->setShortcut(tr("Ctrl+S"));
    connect(saveCommandsFileAction,&QAction::triggered,this,&MainWindow::_saveCommandsFile);
    QAction* saveCommandsFileAsAction = fileMenu->addAction(tr("Save As"));
    connect(saveCommandsFileAsAction,&QAction::triggered,this,&MainWindow::_saveCommandsFileAs);

    w_recentFilesMenu = fileMenu->addMenu(tr("Recent"));
    w_recentFilesActions = new QActionGroup(w_recentFilesMenu);
    connect(w_recentFilesActions,&QActionGroup::triggered,this,&MainWindow::_openRecentFile);
    w_recentFilesSeparator = w_recentFilesMenu->addSeparator();
    QAction* clearRecent = w_recentFilesMenu->addAction(tr("Clear Menu"));
    connect(clearRecent,&QAction::triggered,this,&MainWindow::_clearRecent);

    QAction* closeCommandsFileAction = fileMenu->addAction(tr("Close file"));
    connect(closeCommandsFileAction,&QAction::triggered,this,&MainWindow::_closeCommandsFile);

    fileMenu->addSeparator();

    QAction* exitAction = fileMenu->addAction(tr("Exit"));
    exitAction->setShortcut(tr("Ctrl+Q"));
    exitAction->setIcon(QApplication::style()->standardIcon(QStyle::SP_DialogCloseButton));
    connect(exitAction,&QAction::triggered,this,&MainWindow::_exitApplication);
    menuBar->addMenu(fileMenu);

    //Devices menu
    QMenu* devicesMenu = menuBar->addMenu(tr("Open Device..."));

#ifdef HID
    //Input device selection menu
    w_inputDeviceSelectorMenu = new InputDeviceSelectorMenu(tr("HID"));
    w_inputDeviceSelectorMenu->allowMultipleSelection(true);
    devicesMenu->addMenu(w_inputDeviceSelectorMenu);
#endif //HID

#ifdef SERIAL
    //Serial device selection menu
    w_serialDeviceSelectorMenu = new SerialDeviceSelectorMenu(tr("Serial"));
    w_serialDeviceSelectorMenu->allowMultipleSelection(true);
    devicesMenu->addMenu(w_serialDeviceSelectorMenu);
#endif //SERIAL

    //Settings menu
    QMenu* settingsMenu = new QMenu(tr("Settings"));
    settingsMenu->setToolTipsVisible(true);

    //Some UI related menu entries
    w_minimizeOnClose = settingsMenu->addAction(tr("Minimize on close"));
    w_minimizeOnClose->setCheckable(true);

    w_startHidden = settingsMenu->addAction(tr("Start hidden"));
    w_startHidden->setCheckable(true);
    connect(w_startHidden,&QAction::triggered,this,&MainWindow::_toggleStartHiddenOption);

    QMenu* deviceNotificationMenu = new QMenu(tr("Inform about connected devices"));
    QActionGroup* w_deviceNotificationActionGroup = new QActionGroup(this);
    w_deviceNotificationActionGroup->setExclusive(true);
    w_noDeviceNotificationAction = deviceNotificationMenu->addAction(tr("No notification"));
    w_noDeviceNotificationAction->setData(NoNotify);
    w_noDeviceNotificationAction->setCheckable(true);
    w_deviceNotificationActionGroup->addAction(w_noDeviceNotificationAction);
    w_msgDeviceNotificationAction = deviceNotificationMenu->addAction(tr("Messagebox notification"));
    w_msgDeviceNotificationAction->setData(MsgNotify);
    w_msgDeviceNotificationAction->setCheckable(true);
    w_deviceNotificationActionGroup->addAction(w_msgDeviceNotificationAction);
#ifndef QT_NO_SYSTEMTRAYICON
    w_trayDeviceNotificationAction = deviceNotificationMenu->addAction(tr("Systray notification"));
    w_trayDeviceNotificationAction->setData(TrayNotify);
    w_trayDeviceNotificationAction->setCheckable(true);
    w_deviceNotificationActionGroup->addAction(w_trayDeviceNotificationAction);
#endif //QT_NO_SYSTEMTRAYICON
    connect(w_deviceNotificationActionGroup,&QActionGroup::triggered,this,&MainWindow::_toggleOpenedDeviceNotifyStatus);
    settingsMenu->addMenu(deviceNotificationMenu);

#ifndef QT_NO_SYSTEMTRAYICON
    // No tray - no need to have option "minimize to tray"
    w_minimizeToTray = settingsMenu->addAction(tr("Minimize to tray"));
    w_minimizeToTray->setCheckable(true);
#endif //QT_NO_SYSTEMTRAYICON
    settingsMenu->addSeparator();

#ifdef HID
    w_autoconnectHidDevices = settingsMenu->addAction(tr("HID Autoconnect"));
    w_autoconnectHidDevices->setToolTip(tr("Automatically connect HID device, which is matching filtering criteria"));
    w_autoconnectHidDevices->setCheckable(true);

    QAction* specifyHidFilter = settingsMenu->addAction(tr("Specify HID devices filter"));
    connect(specifyHidFilter,&QAction::triggered,this,&MainWindow::_configureInputFilter);
    settingsMenu->addSeparator();
#endif //HID

#ifdef SERIAL
    w_autoconnectSerialDevices = settingsMenu->addAction(tr("Serial autoconnect"));
    w_autoconnectSerialDevices->setToolTip(tr("Automatically connect serial device, which is matching filtering criteria"));
    w_autoconnectSerialDevices->setCheckable(true);

    QAction* specifySerialFilter = settingsMenu->addAction(tr("Specify serial devices filter"));
    connect(specifySerialFilter,&QAction::triggered,this,&MainWindow::_configureSerialFilter);

    QAction* specifySerialParameters = settingsMenu->addAction(tr("Configure default serial port parameters"));
    connect(specifySerialParameters,&QAction::triggered,this,&MainWindow::_configureSerialParameters);
    settingsMenu->addSeparator();
#endif //SERIAL

    menuBar->addMenu(settingsMenu);

    QMenu* helpMenu = new QMenu(tr("Help"));
    QAction* aboutAction = helpMenu->addAction(tr("About"),this,&MainWindow::_aboutRfidController);
    aboutAction->setIcon(QApplication::style()->standardIcon(QStyle::SP_DialogHelpButton));
    helpMenu->addAction(tr("About Qt"),qApp,&QApplication::aboutQt);
    menuBar->addMenu(helpMenu);

    setMenuBar(menuBar);
}

void MainWindow::_connectController()
{
    //Error handling
    connect(p_controller,&RfidController::errorMessage,this,&MainWindow::showErrorMessageBox);

    //New key discovery
    connect(p_controller,&RfidController::keyFound,this,&MainWindow::displayLastKey);

    //New CommandList
    connect(p_controller,&RfidController::commandListChanged,this,&MainWindow::setCommandList);

    //Handling changes in CommandFile name and state
    connect(p_controller,&RfidController::commandsFileRemoved,this,&MainWindow::commandsFileRemoved);
    connect(p_controller,&RfidController::commandsFileNameChanged,this,&MainWindow::commandsFileNameChanged);
    connect(p_controller,&RfidController::commandsFileContentChanged,this,&MainWindow::commandsFileChanged);

#ifdef HID
    //
    //Input devices
    //

    //Refreshing
    connect(w_inputDeviceSelectorMenu,&InputDeviceSelectorMenu::updateRequested,p_controller->inputDeviceManager(),&InputDeviceManager::updateInputDeviceList);

    //Handle attached input devices
    connect(p_controller->inputDeviceManager(),&InputDeviceManager::inputDeviceWasAttached,w_inputDeviceSelectorMenu,&InputDeviceSelectorMenu::deviceWasAttached);

    //Handle detached input devices
    connect(p_controller->inputDeviceManager(),&InputDeviceManager::inputDeviceWasDetached,w_inputDeviceSelectorMenu,&InputDeviceSelectorMenu::deviceWasDetached);

    //Handle opened input devices
    connect(p_controller->inputDeviceManager(),&InputDeviceManager::inputDeviceWasOpened,w_inputDeviceSelectorMenu,&InputDeviceSelectorMenu::deviceWasOpened);
    connect(p_controller->inputDeviceManager(),&InputDeviceManager::inputDeviceWasOpened,this,&MainWindow::_inputDeviceConnected);

    //Handle closed input devices
    connect(p_controller->inputDeviceManager(),&InputDeviceManager::inputDeviceWasClosed,w_inputDeviceSelectorMenu,&InputDeviceSelectorMenu::deviceWasClosed);

    //Handle opening and closure of input devices by user
    connect(w_inputDeviceSelectorMenu,&InputDeviceSelectorMenu::deviceOpeningRequested,p_controller->inputDeviceManager(),&InputDeviceManager::inputDeviceOpeningRequested);
    connect(w_inputDeviceSelectorMenu,&InputDeviceSelectorMenu::deviceClosureRequested,p_controller->inputDeviceManager(),&InputDeviceManager::inputDeviceClosureRequested);

    //Handle input autoconnection
    connect(w_autoconnectHidDevices,&QAction::triggered,p_controller->inputDeviceManager(),&InputDeviceManager::setInputDeviceAutoconnection);
#endif //HID

#ifdef SERIAL
    //
    //Serial devices
    //

    //Refreshing
    connect(w_serialDeviceSelectorMenu,&SerialDeviceSelectorMenu::updateRequested,p_controller->serialDeviceManager(),&SerialDeviceManager::updateSerialDeviceList);

    //Handle attached serial devices
    connect(p_controller->serialDeviceManager(),&SerialDeviceManager::serialDeviceWasAttached,w_serialDeviceSelectorMenu,&SerialDeviceSelectorMenu::deviceWasAttached);

    //Handle detached serial devices
    connect(p_controller->serialDeviceManager(),&SerialDeviceManager::serialDeviceWasDetached,w_serialDeviceSelectorMenu,&SerialDeviceSelectorMenu::deviceWasDetached);

    //Handle opened serial devices
    connect(p_controller->serialDeviceManager(),&SerialDeviceManager::serialDeviceWasOpened,w_serialDeviceSelectorMenu,&SerialDeviceSelectorMenu::deviceWasOpened);
    connect(p_controller->serialDeviceManager(),&SerialDeviceManager::serialDeviceWasOpened,this,&MainWindow::_serialDeviceConnected);

    //Handle closed serial devices
    connect(p_controller->serialDeviceManager(),&SerialDeviceManager::serialDeviceWasClosed,w_serialDeviceSelectorMenu,&SerialDeviceSelectorMenu::deviceWasClosed);

    //Handle opening and closing of serial devices by user
    connect(w_serialDeviceSelectorMenu,&SerialDeviceSelectorMenu::deviceOpeningRequested,p_controller->serialDeviceManager(),&SerialDeviceManager::serialDeviceOpeningRequested);
    connect(w_serialDeviceSelectorMenu,&SerialDeviceSelectorMenu::deviceClosureRequested,p_controller->serialDeviceManager(),&SerialDeviceManager::serialDeviceClosureRequested);

    //Handle serial autoconnection
    connect(w_autoconnectSerialDevices,&QAction::triggered,p_controller->serialDeviceManager(),&SerialDeviceManager::setSerialDeviceAutoconnection);
#endif //SERIAL
}


