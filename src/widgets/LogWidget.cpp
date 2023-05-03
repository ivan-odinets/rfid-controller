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

#include "LogWidget.h"

#include <QApplication>
#include <QButtonGroup>
#include <QCheckBox>
#include <QDebug>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QVBoxLayout>

#include "appconfig/LoggerWidgetSettings.h"

LogWidget::LogWidget(QWidget *parent)
    : QWidget{parent}
{
    _setupUi();

    w_logToWidgetCheckBox->setChecked(loggerWidgetSettings->logToWidget());
    w_textEdit->setEnabled(loggerWidgetSettings->logToWidget());
}

void LogWidget::appendLine(const QString logLine)
{
    if (!w_logToWidgetCheckBox->isChecked())
        return;

    w_textEdit->appendPlainText(logLine);
}

void LogWidget::logFileNameChaged(const QFileInfo& newLogFileInfo)
{
    w_logFilePath->setText(newLogFileInfo.absoluteFilePath());
    m_logFileInfo = newLogFileInfo;
}

void LogWidget::clearLogWidget()
{
    w_textEdit->clear();
}

void LogWidget::_newLogFileButtonPressed()
{
    QString oldLogDirectory = (w_logFilePath->text().isEmpty()) ? QDir::homePath() : w_logFilePath->text();

    QString fileName = QFileDialog::getSaveFileName(this,tr("Select log file"),
                                 oldLogDirectory,tr("log (*.log);; All files (*.*)"));

    if (fileName.isEmpty())
        return;

    emit newLogFileRequested(fileName);
}

void LogWidget::_appendlogFileButtonPressed()
{
    QString oldLogDirectory = (w_logFilePath->text().isEmpty()) ? QDir::homePath() : w_logFilePath->text();

    QString fileName = QFileDialog::getOpenFileName(this,tr("Select log file"),
                                 oldLogDirectory,tr("log (*.log);; All files (*.*)"));

    if (fileName.isEmpty())
        return;

    emit appendLogFileRequested(fileName);
}

void LogWidget::_setupUi()
{
    QVBoxLayout* mainLayout = new QVBoxLayout;

    QHBoxLayout* logFileLayout = new QHBoxLayout;
    logFileLayout->addWidget(new QLabel(tr("Log file:")));
    w_logFilePath = new QLineEdit;
    w_logFilePath->setReadOnly(true);
    logFileLayout->addWidget(w_logFilePath);
    mainLayout->addLayout(logFileLayout);

    w_textEdit = new QPlainTextEdit;
    w_textEdit->setReadOnly(true);
    mainLayout->addWidget(w_textEdit);

    QHBoxLayout* controlButtonsLayout = new QHBoxLayout;
    QPushButton* newLogFileButton = new QPushButton(tr("New log file"));
    connect(newLogFileButton,&QPushButton::clicked,this,&LogWidget::_newLogFileButtonPressed);
    controlButtonsLayout->addWidget(newLogFileButton);
    QPushButton* openExistingLogButton = new QPushButton(tr("Open existing log file"));
    connect(openExistingLogButton,&QPushButton::clicked,this,&LogWidget::_appendlogFileButtonPressed);
    controlButtonsLayout->addWidget(openExistingLogButton);
    QPushButton* closeLogFileButtin = new QPushButton(tr("Close current log file"));
    connect(closeLogFileButtin,&QPushButton::clicked,this,&LogWidget::logFileClosureRequested);
    controlButtonsLayout->addWidget(closeLogFileButtin);
    QPushButton* clearLogWidgetButton = new QPushButton(tr("Clear widget"));
    connect(clearLogWidgetButton,&QPushButton::clicked,this,&LogWidget::clearLogWidget);
    controlButtonsLayout->addWidget(clearLogWidgetButton);
    w_logToWidgetCheckBox = new QCheckBox(tr("Display log events"));
    connect(w_logToWidgetCheckBox,&QCheckBox::toggled,[this](bool state){
        w_textEdit->setEnabled(state);
        loggerWidgetSettings->setLogToWidget(state);
    });
    controlButtonsLayout->addWidget(w_logToWidgetCheckBox);
    mainLayout->addLayout(controlButtonsLayout);

    setLayout(mainLayout);
}
