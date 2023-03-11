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

#ifndef HEX16BITSPINBOX_H
#define HEX16BITSPINBOX_H

#include <QSpinBox>

/*!
 *  @class Hex16BitSpinBox widgets/Hex16BitSpinBox.h
 *  @brief This is just a slight modification of QSpinBox widget to display 16-bit unsigned integers in hexadecimal system
 */

class Hex16BitSpinBox : public QSpinBox
{
    Q_OBJECT
public:
    explicit Hex16BitSpinBox(QWidget *parent = nullptr);
    ~Hex16BitSpinBox() {}

protected:
    QString              textFromValue(int value) const override;
    int                  valueFromText(const QString &text) const override;
    QValidator::State    validate(QString &input, int &pos) const override;
};

#endif // HEX16BITSPINBOX_H
