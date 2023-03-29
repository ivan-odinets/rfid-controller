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
#include "menus/NotificationTypeMenu.h"
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

void MainWindow::showErrorMessage(const QString& errorMessage)
{
    if (p_windowSettings->notifyAboutErrors() == Notification::None)
        return;

    _showError(errorMessage,p_windowSettings->notifyAboutErrors());
}

void MainWindow::commandsFileInfoChanged(const QFileInfo& newFileInfo)
{
    if (newFileInfo.fileName().isEmpty()) {
        setWindowTitle(QString("%1[*]").arg(qApp->applicationName()));
        return;
    }

    setWindowTitle(QString("%1 - %2[*]").arg(qApp->applicationName()).arg(newFileInfo.fileName()));
    if (newFileInfo.fileName() == p_controller->defaultNewFileName())
        return;

    if (m_recentFiles.contains(newFileInfo.filePath()))
        return;

    m_recentFiles.append(newFileInfo.filePath());
    _addRecentFileAction(newFileInfo.filePath());
}

void MainWindow::commandsFileModified()
{
    const QMessageBox::StandardButton result = QMessageBox::question(this,qApp->applicationName(),
        tr("Current commands file edited on disk by external program. Reload file?"),
        QMessageBox::Yes|QMessageBox::No);

    switch (result) {
    case QMessageBox::Yes:
        p_controller->reloadCurrentCommandFile();
        return;
    case QMessageBox::No:
        setWindowModified(true);
        return;
    default:
        //This should not happen
        Q_ASSERT(false);
    }
}

void MainWindow::commandsFileRemoved()
{
    const QMessageBox::StandardButton result = QMessageBox::question(this,qApp->applicationName(),
        tr("Current commands file was removed from disk by external program. Save file?"),
        QMessageBox::Yes|QMessageBox::No);

    switch (result) {
    case QMessageBox::Yes:
        _saveCommandsFileAs();
        return;
    case QMessageBox::No:
        setWindowModified(true);
        return;
    default:
        Q_ASSERT(false);
    }
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

    QFileInfo currentFileInfo = p_controller->currentFileInfo();
    QDir searchDir = (currentFileInfo.exists()) ? currentFileInfo.dir() : QDir::home();

    QString fileName = QFileDialog::getOpenFileName(this,tr("Open commands file - %1").arg(qApp->applicationName()),
                searchDir.path(),tr("cmds (*.cmds);; All files (*.*)"));

    if (fileName.isEmpty())
        return;

    p_controller->openCommandListFile(fileName);
}

void MainWindow::_saveCommandsFile()
{
    QString fileName = p_controller->currentFileInfo().filePath();
    if (fileName.isEmpty()) {
        _saveCommandsFileAs();
        return;
    }

    p_controller->saveCurrentCommandsListAs(fileName);
    setWindowModified(false);
}

void MainWindow::_saveCommandsFileAs()
{
    QFileInfo currentFileInfo = p_controller->currentFileInfo();
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

void MainWindow::_attachedDeviceNotifyStatusChanged(Notification::Type newStatus)
{
    p_windowSettings->setNotifyAboutAttachedDevices(newStatus);
}

void MainWindow::_detachedDeviceNotifyStatusChanged(Notification::Type newStatus)
{
    p_windowSettings->setNotifyAboutDetachedDevices(newStatus);
}

void MainWindow::_openedDeviceNotifyStatusChanged(Notification::Type newType)
{
    p_windowSettings->setNotifyAboutOpenedDevices(newType);
}

void MainWindow::_closedDeviceNotifyStatusChanged(Notification::Type newType)
{
    p_windowSettings->setNotifyAboutClosedDevices(newType);
}

void MainWindow::_errorNotificationStatusChanged(Notification::Type newType)
{
    p_windowSettings->setNotifyAboutErrors(newType);
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

    w_attachedDeviceNotificationMenu->setNotificationType(p_windowSettings->notifyAboutAttachedDevices());
    w_detachedDeviceNotificationMenu->setNotificationType(p_windowSettings->notifyAboutDetachedDevices());
    w_openedDeviceNotificationMenu->setNotificationType(p_windowSettings->notifyAboutOpenedDevices());
    w_closedDeviceNotificationMenu->setNotificationType(p_windowSettings->notifyAboutClosedDevices());
    w_errorMessage->setNotificationType(p_windowSettings->notifyAboutErrors());

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

void MainWindow::_addRecentFileAction(const QString& filePath)
{
    QAction* recentFileAction = new QAction(filePath);
    w_recentFilesMenu->insertAction(w_recentFilesSeparator,recentFileAction);
    w_recentFilesActions->addAction(recentFileAction);
}

void MainWindow::_showNotification(const QString& messageText,Notification::Type type)
{
    if (type == Notification::None)
        return;

    if (type == Notification::MessageBox) {
        QMessageBox::information(this,tr("Info - %1").arg(qApp->applicationName()),
                                 messageText);
        return;
    }

#ifndef QT_NO_SYSTEMTRAYICON
    if (type == Notification::Systemtray) {
        if (w_trayIcon == nullptr)
            return;

        w_trayIcon->showMessage(qApp->applicationName(),messageText);
        return;
    }
#endif //QT_NO_SYSTEMTRAYICON
}

void MainWindow::_showError(const QString& messageText,Notification::Type type)
{
    if (type == Notification::None)
        return;

    if (type == Notification::MessageBox) {
        QMessageBox::critical(this,tr("Error - %1").arg(qApp->applicationName()),
                              messageText);
        return;
    }

#ifndef QT_NO_SYSTEMTRAYICON
    if (type == Notification::Systemtray) {
        if (w_trayIcon == nullptr)
            return;

        w_trayIcon->showMessage(qApp->applicationName(),messageText,QSystemTrayIcon::Warning);
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

void MainWindow::_inputDeviceAttached(const InputDeviceInfo& deviceInfo)
{
    if (p_windowSettings->notifyAboutAttachedDevices() == Notification::None)
        return;

    QString notificationText = tr("New HID device attached: %1\r\nVendorId=%2 ProductId=%3 Name=%4")
            .arg(deviceInfo.deviceFilePath())
            .arg(deviceInfo.vendorId())
            .arg(deviceInfo.productId())
            .arg(deviceInfo.deviceName());

    _showNotification(notificationText,p_windowSettings->notifyAboutAttachedDevices());
}

void MainWindow::_inputDeviceDetached(const InputDeviceInfo& deviceInfo)
{
    if (p_windowSettings->notifyAboutAttachedDevices() == Notification::None)
        return;

    QString notificationText = tr("HID device detached: %1\r\nVendorId=%2 ProductId=%3 Name=%4")
            .arg(deviceInfo.deviceFilePath())
            .arg(deviceInfo.vendorId())
            .arg(deviceInfo.productId())
            .arg(deviceInfo.deviceName());

    _showNotification(notificationText,p_windowSettings->notifyAboutDetachedDevices());
}

void MainWindow::_inputDeviceOpened(const InputDeviceInfo& deviceInfo)
{
    QString notificationText = tr("HID device opened: %1\r\nVendorId=%2 ProductId=%3 Name=%4")
            .arg(deviceInfo.deviceFilePath())
            .arg(deviceInfo.vendorId())
            .arg(deviceInfo.productId())
            .arg(deviceInfo.deviceName());

    _showNotification(notificationText,p_windowSettings->notifyAboutOpenedDevices());
}

void MainWindow::_inputDeviceClosed(const InputDeviceInfo& deviceInfo)
{
    QString notificationText = tr("HID device closed: %1\r\nVendorId=%2 ProductId=%3 Name=%4")
            .arg(deviceInfo.deviceFilePath())
            .arg(deviceInfo.vendorId())
            .arg(deviceInfo.productId())
            .arg(deviceInfo.deviceName());

    _showNotification(notificationText,p_windowSettings->notifyAboutClosedDevices());
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

void MainWindow::_serialDeviceAttached(const QSerialPortInfo& portInfo)
{
    if (p_windowSettings->notifyAboutAttachedDevices() == Notification::None)
        return;

    QString notificationText = tr("New serial device attached\r\nVendorId=%1 ProductId=%2 Name=%3")
            .arg(QString::number(portInfo.vendorIdentifier(),16))
            .arg(QString::number(portInfo.productIdentifier(),16))
            .arg(portInfo.portName());

    _showNotification(notificationText,p_windowSettings->notifyAboutAttachedDevices());
}

void MainWindow::_serialDeviceDetached(const QSerialPortInfo& portInfo)
{
    if (p_windowSettings->notifyAboutDetachedDevices() == Notification::None)
        return;

    QString notificationText = tr("Serial device detached\r\nVendorId=%1 ProductId=%2 Name=%3")
            .arg(QString::number(portInfo.vendorIdentifier(),16))
            .arg(QString::number(portInfo.productIdentifier(),16))
            .arg(portInfo.portName());

    _showNotification(notificationText,p_windowSettings->notifyAboutDetachedDevices());
}

void MainWindow::_serialDeviceOpened(const QSerialPortInfo& portInfo)
{
    QString notificationText = tr("Serial device opened\r\nVendorId=%1 ProductId=%2 Name=%3")
            .arg(QString::number(portInfo.vendorIdentifier(),16))
            .arg(QString::number(portInfo.productIdentifier(),16))
            .arg(portInfo.portName());

    _showNotification(notificationText,p_windowSettings->notifyAboutOpenedDevices());
}

void MainWindow::_serialDeviceClosed(const QSerialPortInfo& portInfo)
{
    QString notificationText = tr("Serial device closed\r\nVendorId=%1 ProductId=%2 Name=%3")
            .arg(QString::number(portInfo.vendorIdentifier(),16))
            .arg(QString::number(portInfo.productIdentifier(),16))
            .arg(portInfo.portName());

    _showNotification(notificationText,p_windowSettings->notifyAboutClosedDevices());
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

    QMenu* notificationMenu = new QMenu(tr("Notifications"));

    w_attachedDeviceNotificationMenu = new NotificationMenu(tr("Attached devices"));
    connect(w_attachedDeviceNotificationMenu,&NotificationMenu::notificationTypeChanged,this,&MainWindow::_attachedDeviceNotifyStatusChanged);
    notificationMenu->addMenu(w_attachedDeviceNotificationMenu);
    w_detachedDeviceNotificationMenu = new NotificationMenu(tr("Detached devices"));
    connect(w_detachedDeviceNotificationMenu,&NotificationMenu::notificationTypeChanged,this,&MainWindow::_detachedDeviceNotifyStatusChanged);
    notificationMenu->addMenu(w_detachedDeviceNotificationMenu);

    w_openedDeviceNotificationMenu = new NotificationMenu(tr("Opened devices"));
    connect(w_openedDeviceNotificationMenu,&NotificationMenu::notificationTypeChanged,this,&MainWindow::_openedDeviceNotifyStatusChanged);
    notificationMenu->addMenu(w_openedDeviceNotificationMenu);
    w_closedDeviceNotificationMenu = new NotificationMenu(tr("Closed devices"));
    connect(w_closedDeviceNotificationMenu,&NotificationMenu::notificationTypeChanged,this,&MainWindow::_closedDeviceNotifyStatusChanged);
    notificationMenu->addMenu(w_closedDeviceNotificationMenu);
    w_errorMessage = new NotificationMenu(tr("Error messages"));
    connect(w_errorMessage,&NotificationMenu::notificationTypeChanged,this,&MainWindow::_errorNotificationStatusChanged);
    notificationMenu->addMenu(w_errorMessage);

    settingsMenu->addMenu(notificationMenu);

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
    connect(p_controller,&RfidController::errorMessage,this,&MainWindow::showErrorMessage);

    //New key discovery
    connect(p_controller,&RfidController::keyFound,this,&MainWindow::displayLastKey);

    //New CommandList
    connect(p_controller,&RfidController::commandListChanged,this,&MainWindow::setCommandList);

    //Handling changes in CommandFile name and state
    connect(p_controller,&RfidController::commandsFileRemoved,this,&MainWindow::commandsFileRemoved);
    connect(p_controller,&RfidController::commandFileNameChanged,this,&MainWindow::commandsFileInfoChanged);
    connect(p_controller,&RfidController::commandsFileModified,this,&MainWindow::commandsFileModified);

#ifdef HID
    //
    //Input devices
    //

    //Add devices, which are already attached to the system
    w_inputDeviceSelectorMenu->setCurrentInputDeviceList(p_controller->inputDeviceManager()->availableInputDevices());

    //Refreshing
    connect(w_inputDeviceSelectorMenu,&InputDeviceSelectorMenu::updateRequested,p_controller->inputDeviceManager(),&InputDeviceManager::updateInputDeviceList);

    //Handle attached input devices
    connect(p_controller->inputDeviceManager(),&InputDeviceManager::inputDeviceWasAttached,w_inputDeviceSelectorMenu,&InputDeviceSelectorMenu::deviceWasAttached);
    connect(p_controller->inputDeviceManager(),&InputDeviceManager::inputDeviceWasAttached,this,&MainWindow::_inputDeviceAttached);

    //Handle detached input devices
    connect(p_controller->inputDeviceManager(),&InputDeviceManager::inputDeviceWasDetached,w_inputDeviceSelectorMenu,&InputDeviceSelectorMenu::deviceWasDetached);
    connect(p_controller->inputDeviceManager(),&InputDeviceManager::inputDeviceWasDetached,this,&MainWindow::_inputDeviceDetached);

    //Handle opened input devices
    connect(p_controller->inputDeviceManager(),&InputDeviceManager::inputDeviceWasOpened,w_inputDeviceSelectorMenu,&InputDeviceSelectorMenu::deviceWasOpened);
    connect(p_controller->inputDeviceManager(),&InputDeviceManager::inputDeviceWasOpened,this,&MainWindow::_inputDeviceOpened);

    //Handle closed input devices
    connect(p_controller->inputDeviceManager(),&InputDeviceManager::inputDeviceWasClosed,w_inputDeviceSelectorMenu,&InputDeviceSelectorMenu::deviceWasClosed);
    connect(p_controller->inputDeviceManager(),&InputDeviceManager::inputDeviceWasClosed,this,&MainWindow::_inputDeviceClosed);

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
    w_serialDeviceSelectorMenu->setCurrentSerialDeviceList(p_controller->serialDeviceManager()->availableDevices());

    //Refreshing
    connect(w_serialDeviceSelectorMenu,&SerialDeviceSelectorMenu::updateRequested,p_controller->serialDeviceManager(),&SerialDeviceManager::updateSerialDeviceList);

    //Handle attached serial devices
    connect(p_controller->serialDeviceManager(),&SerialDeviceManager::serialDeviceWasAttached,w_serialDeviceSelectorMenu,&SerialDeviceSelectorMenu::deviceWasAttached);
    connect(p_controller->serialDeviceManager(),&SerialDeviceManager::serialDeviceWasAttached,this,&MainWindow::_serialDeviceAttached);

    //Handle detached serial devices
    connect(p_controller->serialDeviceManager(),&SerialDeviceManager::serialDeviceWasDetached,w_serialDeviceSelectorMenu,&SerialDeviceSelectorMenu::deviceWasDetached);
    connect(p_controller->serialDeviceManager(),&SerialDeviceManager::serialDeviceWasDetached,this,&MainWindow::_serialDeviceDetached);

    //Handle opened serial devices
    connect(p_controller->serialDeviceManager(),&SerialDeviceManager::serialDeviceWasOpened,w_serialDeviceSelectorMenu,&SerialDeviceSelectorMenu::deviceWasOpened);
    connect(p_controller->serialDeviceManager(),&SerialDeviceManager::serialDeviceWasOpened,this,&MainWindow::_serialDeviceOpened);

    //Handle closed serial devices
    connect(p_controller->serialDeviceManager(),&SerialDeviceManager::serialDeviceWasClosed,w_serialDeviceSelectorMenu,&SerialDeviceSelectorMenu::deviceWasClosed);
    connect(p_controller->serialDeviceManager(),&SerialDeviceManager::serialDeviceWasClosed,this,&MainWindow::_serialDeviceClosed);

    //Handle opening and closing of serial devices by user
    connect(w_serialDeviceSelectorMenu,&SerialDeviceSelectorMenu::deviceOpeningRequested,p_controller->serialDeviceManager(),&SerialDeviceManager::serialDeviceOpeningRequested);
    connect(w_serialDeviceSelectorMenu,&SerialDeviceSelectorMenu::deviceClosureRequested,p_controller->serialDeviceManager(),&SerialDeviceManager::serialDeviceClosureRequested);

    //Handle serial autoconnection
    connect(w_autoconnectSerialDevices,&QAction::triggered,p_controller->serialDeviceManager(),&SerialDeviceManager::setSerialDeviceAutoconnection);
#endif //SERIAL
}


