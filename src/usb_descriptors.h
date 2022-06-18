// Copyright 2020 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#define _PID_MAP(itf, n) ((CFG_TUD_##itf) << (n))

#define USB_ESPRESSIF_VID 0x303A

#define USB_STRING_DESCRIPTOR_ARRAY_SIZE 10
typedef char *tusb_desc_strarray_device_t[USB_STRING_DESCRIPTOR_ARRAY_SIZE];

#define TUD_FIDO_DESC_HEAD_LEN (9 + 9 + 9 + 7)
#define TUD_FIDO_DESC_HEAD(_itfnum,  _stridx, _numcables) \
  /* Audio Control (AC) Interface */\
  9, TUSB_DESC_INTERFACE, _itfnum, 0, 0, TUSB_CLASS_AUDIO, AUDIO_SUBCLASS_CONTROL, AUDIO_FUNC_PROTOCOL_CODE_UNDEF, _stridx,\
  /* AC Header */\
  9, TUSB_DESC_CS_INTERFACE, AUDIO_CS_AC_INTERFACE_HEADER, U16_TO_U8S_LE(0x0100), U16_TO_U8S_LE(0x0009), 1, (uint8_t)((_itfnum) + 1),\
  /* FIDO Streaming (MS) Interface */\
  9, TUSB_DESC_INTERFACE, (uint8_t)((_itfnum) + 1), 0, 2, TUSB_CLASS_AUDIO, AUDIO_SUBCLASS_FIDO_STREAMING, AUDIO_FUNC_PROTOCOL_CODE_UNDEF, 0,\
  /* MS Header */\
  7, TUSB_DESC_CS_INTERFACE, FIDO_CS_INTERFACE_HEADER, U16_TO_U8S_LE(0x0100), U16_TO_U8S_LE(7 + (_numcables) * TUD_FIDO_DESC_JACK_LEN)

#define TUD_FIDO_JACKID_IN_EMB(_cablenum) \
  (uint8_t)(((_cablenum) - 1) * 4 + 1)

#define TUD_FIDO_JACKID_IN_EXT(_cablenum) \
  (uint8_t)(((_cablenum) - 1) * 4 + 2)

#define TUD_FIDO_JACKID_OUT_EMB(_cablenum) \
  (uint8_t)(((_cablenum) - 1) * 4 + 3)

#define TUD_FIDO_JACKID_OUT_EXT(_cablenum) \
  (uint8_t)(((_cablenum) - 1) * 4 + 4)

#define TUD_FIDO_DESC_JACK_LEN (6 + 6 + 9 + 9)
#define TUD_FIDO_DESC_JACK(_cablenum) \
  /* MS In Jack (Embedded) */\
  6, TUSB_DESC_CS_INTERFACE, FIDO_CS_INTERFACE_IN_JACK, FIDO_JACK_EMBEDDED, TUD_FIDO_JACKID_IN_EMB(_cablenum), 0,\
  /* MS In Jack (External) */\
  6, TUSB_DESC_CS_INTERFACE, FIDO_CS_INTERFACE_IN_JACK, FIDO_JACK_EXTERNAL, TUD_FIDO_JACKID_IN_EXT(_cablenum), 0,\
  /* MS Out Jack (Embedded), connected to In Jack External */\
  9, TUSB_DESC_CS_INTERFACE, FIDO_CS_INTERFACE_OUT_JACK, FIDO_JACK_EMBEDDED, TUD_FIDO_JACKID_OUT_EMB(_cablenum), 1, TUD_FIDO_JACKID_IN_EXT(_cablenum), 1, 0,\
  /* MS Out Jack (External), connected to In Jack Embedded */\
  9, TUSB_DESC_CS_INTERFACE, FIDO_CS_INTERFACE_OUT_JACK, FIDO_JACK_EXTERNAL, TUD_FIDO_JACKID_OUT_EXT(_cablenum), 1, TUD_FIDO_JACKID_IN_EMB(_cablenum), 1, 0

#define TUD_FIDO_DESC_EP_LEN(_numcables) (9 + 4 + (_numcables))
#define TUD_FIDO_DESC_EP(_epout, _epsize, _numcables) \
  /* Endpoint: Note Audio v1.0's endpoint has 9 bytes instead of 7 */\
  9, TUSB_DESC_ENDPOINT, _epout, TUSB_XFER_BULK, U16_TO_U8S_LE(_epsize), 0, 0, 0, \
  /* MS Endpoint (connected to embedded jack) */\
  (uint8_t)(4 + (_numcables)), TUSB_DESC_CS_ENDPOINT, FIDO_CS_ENDPOINT_GENERAL, _numcables

// Length of template descriptor (88 bytes)
#define TUD_FIDO_DESC_LEN (TUD_FIDO_DESC_HEAD_LEN + TUD_FIDO_DESC_JACK_LEN + TUD_FIDO_DESC_EP_LEN(1) * 2)
