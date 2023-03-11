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

#ifdef GUI
    #include <QApplication>
#else
    #include <QCoreApplication>
#endif //GUI

#include <QDateTime>
#include <QDebug>

#include "./appconfig/CommandLineParser.h"
#include "./appconfig/Settings.h"
#include "./core/RfidController.h"

#ifdef GUI
    #include <QApplication>
    #include <QScopedPointer>

    #include "./windows/MainWindow.h"
#else
    #include <QCoreApplication>
#endif //GUI

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg);

int main(int argc, char *argv[])
{
    qInstallMessageHandler(myMessageOutput);

#ifdef GUI
    QApplication app(argc, argv);
#else
    QCoreApplication app(argc,argv);
#endif //GUI
    QCoreApplication::setApplicationVersion(QStringLiteral(APP_VERSION));
    QCoreApplication::setApplicationName(QStringLiteral(APP_NAME));
    QCoreApplication::setOrganizationName(QStringLiteral("OdinSoft"));

    CommandLineParser parser;
    parser.process(app);

    Settings* appSettings = Settings::getSettings();
    if (!parser.configFile().isEmpty())
        appSettings->setConfigFileName(parser.configFile());

    if (!appSettings->configParsed())
        qWarning() << QCoreApplication::translate("main","Error parsing config file %1. Using default parameters.").arg(appSettings->confiFileName());

    RfidController* controller = RfidController::get();

#ifdef GUI
    if (parser.startHidden())
        appSettings->setStartHidden(true);

    QScopedPointer<MainWindow> mainWindow;
    if (!parser.noGui()) {
        mainWindow.reset(new MainWindow);

        if (!appSettings->startHidden())
            mainWindow->show();
    }
#endif //GUI

    if (appSettings->openedCommandsFileName().isEmpty())
        controller->newCommandList();
    else
        controller->openCommandListFile(appSettings->openedCommandsFileName());
    controller->start();

    return app.exec();
}

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    const char *function = context.function ? context.function : "";
    switch (type) {
    case QtDebugMsg:
        fprintf(stderr, "[DEBUG]   : [%s] %s (Line: %i, Function: %s)\n", QDateTime::currentDateTime().toString().toLocal8Bit().constData(),
                localMsg.constData(), context.line, function);
        break;
    case QtInfoMsg:
        fprintf(stderr, "[INFO]    : %s\n", localMsg.constData());
        break;
    case QtWarningMsg:
        fprintf(stderr, "[WARNING] : %s\n", localMsg.constData());
        break;
    case QtCriticalMsg:
        fprintf(stderr, "[CRITICAL]: %s\n", localMsg.constData());
        break;
    case QtFatalMsg:
        fprintf(stderr, "[FATAL]   : %s\n", localMsg.constData());
        break;
    }
}
