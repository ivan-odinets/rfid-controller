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

#include "Hex16BitSpinBox.h"

#include <limits>

Hex16BitSpinBox::Hex16BitSpinBox(QWidget *parent)
    : QSpinBox(parent)
{
    setMinimum(std::numeric_limits<quint16>::min());
    setMaximum(std::numeric_limits<quint16>::max());
    setDisplayIntegerBase(16);
    setPrefix(QStringLiteral("0x"));
}

QString Hex16BitSpinBox::textFromValue(int value) const
{
    QString result = QString::number(value,16);
    while (result.size() < 4)
        result.push_front(QChar('0'));

    return result;
}

int Hex16BitSpinBox::valueFromText(const QString &text) const
{
    return text.toUInt(0, 16);
}

QValidator::State Hex16BitSpinBox::validate(QString &input, int &pos) const
{
    QString copy(input);
    if (copy.startsWith(QStringLiteral("0x")))
        copy.remove(0, 2);
    pos -= copy.size() - copy.trimmed().size();
    copy = copy.trimmed();

    if (copy.isEmpty())
        return QValidator::Intermediate;

    input = QString(QStringLiteral("0x")) + copy.toUpper();
    bool okay;
    unsigned int val = copy.toUInt(&okay, 16);
    if (!okay || (val > 0xFFFF))
        return QValidator::Invalid;

    return QValidator::Acceptable;
}
