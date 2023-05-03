/*
 **********************************************************************************************************************
 *
 * This file is part of the rfid-controller project.
 *
 * Copyright (c) 2023 Ivan Odinets <i_odinets@protonmail.com>
 *
 * rfid-controller is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * rfid-controller is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with rfid-controller. If not, see <https://www.gnu.org/licenses/>.
 *
 */

#ifndef LOGWIDGET_H
#define LOGWIDGET_H

#include <QWidget>

class QCheckBox;
class QLineEdit;
class QPlainTextEdit;

#include <QFileInfo>

/*!
 *  @class LogWidget widgets/LogWidget.h
 *  @brief This widget is used to display logged events as well as control some functions of Logger objects.
 */

class LogWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LogWidget(QWidget *parent = nullptr);
    ~LogWidget() {}

signals:
    /*! @brief This signal is emitted when user has requested to start new log file, or overwrite existing log file. */
    void newLogFileRequested(const QString& filePath);

    /*! @brief This signal is emitted when user has requested to append new events to existing log file. */
    void appendLogFileRequested(const QString& filePath);

    /*! @brief This signal is emitted when user has requested to close current log file. */
    void logFileClosureRequested();

public slots:
    /*! @brief This slot should be invoked when some new line was attached to log. Just displaying it. */
    void appendLine(const QString logLine);

    /*! @brief This slot should be invoked when log file has changed (e.g. name was changed). */
    void logFileNameChaged(const QFileInfo& newLogFileInfo);

    /*! @brief This slot is used to clear log display widget from old events. New events will be appended to empty widget. */
    void clearLogWidget();

private:
    void _newLogFileButtonPressed();
    void _appendlogFileButtonPressed();

private:
    inline void     _setupUi();
    QPlainTextEdit* w_textEdit;
    QLineEdit*      w_logFilePath;
    QCheckBox*      w_logToWidgetCheckBox;

    QFileInfo       m_logFileInfo;
};

#endif // LOGWIDGET_H
