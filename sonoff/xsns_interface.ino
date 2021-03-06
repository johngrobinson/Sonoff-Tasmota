/*
  xsns_interface.ino - Sensor interface support for Sonoff-Tasmota

  Copyright (C) 2018  Theo Arends inspired by ESPEasy

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

boolean (* const xsns_func_ptr[])(byte) PROGMEM = {  // Sensor Function Pointers for simple implementation of sensors
#ifdef XSNS_01
  &Xsns01,
#endif

#ifdef XSNS_02
  &Xsns02,
#endif

#ifdef XSNS_03
  &Xsns03,
#endif

#ifdef XSNS_04
  &Xsns04,
#endif

#ifdef XSNS_05
  &Xsns05,
#endif

#ifdef XSNS_06
  &Xsns06,
#endif

#ifdef XSNS_07
  &Xsns07,
#endif

#ifdef XSNS_08
  &Xsns08,
#endif

#ifdef XSNS_09
  &Xsns09,
#endif

#ifdef XSNS_10
  &Xsns10,
#endif

#ifdef XSNS_11
  &Xsns11,
#endif

#ifdef XSNS_12
  &Xsns12,
#endif

#ifdef XSNS_13
  &Xsns13,
#endif

#ifdef XSNS_14
  &Xsns14,
#endif

#ifdef XSNS_15
  &Xsns15,
#endif

#ifdef XSNS_16
  &Xsns16,
#endif

#ifdef XSNS_17
  &Xsns17,
#endif

#ifdef XSNS_18
  &Xsns18,
#endif

#ifdef XSNS_19
  &Xsns19,
#endif

#ifdef XSNS_20
  &Xsns20,
#endif
};

const uint8_t xsns_present = sizeof(xsns_func_ptr) / sizeof(xsns_func_ptr[0]);  // Number of External Sensors found

/*********************************************************************************************\
 * Function call to all xsns
 *
 * FUNC_INIT
 * FUNC_PREP_BEFORE_TELEPERIOD
 * FUNC_SAVE_BEFORE_RESTART
 * FUNC_JSON_APPEND
 * FUNC_WEB_APPEND
 * return FUNC_COMMAND
 * FUNC_EVERY_SECOND
 * FUNC_EVERY_50_MSECOND
\*********************************************************************************************/

boolean XsnsCall(byte Function)
{
  boolean result = false;

  for (byte x = 0; x < xsns_present; x++) {
#ifdef USE_WEBSERVER
    if (FUNC_WEB_APPEND == Function) {
      mqtt_data[0] = '\0';
    }
#endif  // USE_WEBSERVER
    result = xsns_func_ptr[x](Function);
    if (result) {
      break;
    }
#ifdef USE_WEBSERVER
    if ((FUNC_WEB_APPEND == Function) && strlen(mqtt_data)) {
      if (D_DECIMAL_SEPARATOR[0] != '.') {
        for (int i = 0; i < strlen(mqtt_data); i++) {
          if ('.' == mqtt_data[i]) {
            mqtt_data[i] = D_DECIMAL_SEPARATOR[0];
          }
        }
      }
      WebServer->sendContent(mqtt_data);
    }
#endif  // USE_WEBSERVER
  }

  return result;
}
