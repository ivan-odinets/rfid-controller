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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QFileInfo>

class QActionGroup;

#include "core/NotificationType.h"

class CommandList;
class RfidController;
class MainWindowSettings;
class MonitorWidget;
class NotificationMenu;

#ifndef QT_NO_SYSTEMTRAYICON
    #include <QSystemTrayIcon>
#endif //QT_NO_SYSTEMTRAYICON

#ifdef HID
    class InputDeviceInfo;
    class InputDeviceSelectorMenu;
#endif //HID

#ifdef SERIAL
    class QSerialPortInfo;

    class SerialDeviceSelectorMenu;
#endif //SERIAL

/*!
 *  @class MainWindow windows/MainWindow.h
 *  @brief This class represents main window of the application
 */

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void displayLastKey(const QString& key);
    void showErrorMessage(const QString& errorMessage);

    void commandsFileInfoChanged(const QFileInfo& newFileInfo);
    void commandsFileModified();
    void commandsFileRemoved();
    void setCommandList(CommandList* commandsList);

protected:
    void closeEvent(QCloseEvent *event) override;
    void changeEvent(QEvent* event) override;

private slots:
    //Menu - File
    void _newCommandsFile();
    void _openCommandsFile();
    void _saveCommandsFile();
    void _saveCommandsFileAs();
    void _closeCommandsFile();

    //Recent files
    void _openRecentFile(QAction* fileAction);
    void _clearRecent();

    void _exitApplication();

    //Menu - settings
    void _toggleStartHiddenOption(bool state);
    void _attachedDeviceNotifyStatusChanged(Notification::Type newStatus);
    void _detachedDeviceNotifyStatusChanged(Notification::Type newStatus);
    void _openedDeviceNotifyStatusChanged(Notification::Type newStatus);
    void _closedDeviceNotifyStatusChanged(Notification::Type newStatus);
    void _errorNotificationStatusChanged(Notification::Type newStatus);

    //Menu - Help
    void _aboutRfidController();

private:
    void _loadSettings();
    void _saveSettings();
    MainWindowSettings*           p_windowSettings;
    RfidController*               p_controller;

    void _addRecentFileAction(const QString& fileName);
    QStringList                   m_recentFiles;

    void _showNotification(const QString& messageText,Notification::Type type);
    void _showError(const QString& messageText,Notification::Type type);

    bool _maybeSave();

    //UI-Related
    void _setupUi();
    void _setupMenus();
    void _connectController();

    QActionGroup*                 w_recentFilesActions;
    QAction*                      w_recentFilesSeparator;
    QMenu*                        w_recentFilesMenu;

    MonitorWidget*                w_centralWidget;
    QAction*                      w_minimizeOnClose;
    QAction*                      w_startHidden;

    NotificationMenu*             w_attachedDeviceNotificationMenu;
    NotificationMenu*             w_detachedDeviceNotificationMenu;
    NotificationMenu*             w_openedDeviceNotificationMenu;
    NotificationMenu*             w_closedDeviceNotificationMenu;
    NotificationMenu*             w_errorMessage;

#ifdef HID
//
// This part is needed only if we have HID support enabled
//

private slots:
    void _inputDeviceAttached(const InputDeviceInfo& deviceInfo);
    void _inputDeviceDetached(const InputDeviceInfo& deviceInfo);
    void _inputDeviceOpened(const InputDeviceInfo& deviceInfo);
    void _inputDeviceClosed(const InputDeviceInfo& deviceInfo);

    //Settings menu entry
    void _configureInputFilter();

private:
    InputDeviceSelectorMenu*      w_inputDeviceSelectorMenu;
    QAction*                      w_autoconnectHidDevices;
    QAction*                      w_hideSomeInputDevices;
#endif //HID

#ifdef SERIAL
//
// This part is needed only if we have Serial support enabled
//

private slots:
    void _serialDeviceAttached(const QSerialPortInfo& portInfo);
    void _serialDeviceDetached(const QSerialPortInfo& portInfo);
    void _serialDeviceOpened(const QSerialPortInfo& portInfo);
    void _serialDeviceClosed(const QSerialPortInfo& portInfo);

    //Settings menu entries
    void _configureSerialParameters();
    void _configureSerialFilter();

private:
    SerialDeviceSelectorMenu*     w_serialDeviceSelectorMenu;
    QAction*                      w_autoconnectSerialDevices;
#endif //SERIAL

#ifndef QT_NO_SYSTEMTRAYICON
//
// This part is needed only if we have SystemTray support enabled
//

private:
    void _toggleWindowVisibility();
    void _setupTray();
    QSystemTrayIcon*              w_trayIcon;
    QAction*                      w_minimizeToTray;

private slots:
    void _iconActivated(QSystemTrayIcon::ActivationReason reason);
#endif //QT_NO_SYSTEMTRAYICON
};

#endif // MAINWINDOW_H
