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

#include "InputEvent.h"

#if defined(Q_OS_LINUX) && !defined(Q_OS_ANDROID)
//
// Linux-specific part of InputEvnt
//

InputEvent::InputEvent(const input_event& event)
{
    m_type   = _typeFromInt(event.type);
    m_code   = event.code;
    m_value  = event.value;
}

InputEvent::Type InputEvent::_typeFromInt(quint8 value)
{
    switch (value) {
    case EV_SYN:
        return Syn;
    case EV_KEY:
        return Key;
    case EV_REL:
        return Rel;
    case EV_ABS:
        return Abs;
    case EV_MSC:
        return Msc;
    case EV_SW:
        return Sw;
    case EV_LED:
        return Led;
    case EV_SND:
        return Snd;
    case EV_REP:
        return Rep;
    case EV_FF:
        return Ff;
    case EV_PWR:
        return Power;
    case EV_FF_STATUS:
        return Status;
    case EV_MAX:
        return Max;
    case EV_CNT:
        return CNT;
    }
    Q_ASSERT(false);
    return Syn;
}

char InputEvent::_codeToChar(quint8 code)
{
    switch (code) {
    case KEY_1:
        return '1';
    case KEY_2:
        return '2';
    case KEY_3:
        return '3';
    case KEY_4:
        return '4';
    case KEY_5:
        return '5';
    case KEY_6:
        return '6';
    case KEY_7:
        return '7';
    case KEY_8:
        return '8';
    case KEY_9:
        return '9';
    case KEY_0:
        return '0';
    case KEY_MINUS:
        return '-';
    case KEY_EQUAL:
        return '=';
    case KEY_Q:
        return 'q';
    case KEY_W:
        return 'w';
    case KEY_E:
        return 'e';
    case KEY_R:
        return 'r';
    case KEY_T:
        return 't';
    case KEY_Y:
        return 'y';
    case KEY_U:
        return 'u';
    case KEY_I:
        return 'i';
    case KEY_O:
        return 'o';
    case KEY_P:
        return 'p';
    case KEY_A:
        return 'a';
    case KEY_S:
        return 's';
    case KEY_D:
        return 'd';
    case KEY_F:
        return 'f';
    case KEY_G:
        return 'g';
    case KEY_H:
        return 'h';
    case KEY_J:
        return 'j';
    case KEY_K:
        return 'k';
    case KEY_L:
        return 'l';
    case KEY_Z:
        return 'z';
    case KEY_X:
        return 'x';
    case KEY_C:
        return 'c';
    case KEY_V:
        return 'v';
    case KEY_B:
        return 'b';
    case KEY_N:
        return 'n';
    case KEY_M:
        return 'm';
    case KEY_TAB:
    case KEY_BACKSPACE:
    case KEY_LEFTBRACE:
    case KEY_RIGHTBRACE:
    case KEY_ENTER:
    case KEY_LEFTCTRL:
        return '\0';
    }
    return '\0';
}

#elif defined(Q_OS_LINUX) && defined(Q_OS_ANDROID)
    #error("Android builds currently not supported")
#elif defined(Q_OS_WINDOWS)
    #error("Windows builds currently not supported")
#else
    #error("Builds for other platforms are not supported")
#endif //PLATFORM SPECIFIC
