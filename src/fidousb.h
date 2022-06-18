
#include "esptinyusb.h"
#include "USBHID.h"

#pragma once

#ifdef CFG_TUD_FIDO

#define FIDO_HID_INTERFACE 0
#define FIDO_HID_REPORT_DESC_SIZE (sizeof (hid_report_desc))

static const uint8_t hid_report_desc[] = {
  0x06, 0xd0, 0xf1, /* USAGE_PAGE (FIDO Alliance) */
  0x09, 0x01,       /* USAGE (Keyboard) */

  0xa1, 0x01,       /* COLLECTION (Application) */

  0x09, 0x20,       /*   USAGE (Input report data) */
  0x15, 0x00,       /*   LOGICAL_MINIMUM (0) */
  0x26, 0xff, 0x00, /*   LOGICAL_MAXIMUM (255) */
  0x75, 0x08,       /*   REPORT_SIZE (8) */
  0x95, 0x40,       /*   REPORT_COUNT (64) */
  0x81, 0x02,       /*   INPUT (Data,Var,Abs); Modifier byte */

  0x09, 0x21,       /*   USAGE (Output report data) */
  0x15, 0x00,       /*   LOGICAL_MINIMUM (0) */
  0x26, 0xff, 0x00, /*   LOGICAL_MAXIMUM (255) */
  0x75, 0x08,       /*   REPORT_SIZE (8) */
  0x95, 0x40,       /*   REPORT_COUNT (64) */
  0x91, 0x02,       /*   OUTPUT (Data,Var,Abs); Modifier byte */

  0xc0              /* END_COLLECTION */
};

/* USB Device Descriptor */
static const uint8_t u2f_device_desc[18] = {
  18,                /* bLength */
  TUSB_DESC_DEVICE,  /* bDescriptorType */
  0x10, 0x01,        /* bcdUSB = 1.1 */
  0x00,              /* bDeviceClass (Unknown). */
  0x00,              /* bDeviceSubClass. */
  0x00,              /* bDeviceProtocol. */
  0x40,              /* bMaxPacketSize. */
  0xd0, 0x16,        /* idVendor */
  0x90, 0x0e,        /* idProduct */
  0x00, 0x01,        /* bcdDevice */
  1,                 /* iManufacturer. */
  2,                 /* iProduct. */
  3,                 /* iSerialNumber. */
  1                  /* bNumConfigurations. */
};

#define FEATURE_BUS_POWERED        0x80

/* Configuration Descriptor tree for a HID.*/
static const uint8_t u2f_config_desc[41] = {
  9,
  TUSB_DESC_CONFIGURATION,  /* bDescriptorType: Configuration */
  /* Configuration Descriptor.*/
  41, 0x00,                 /* wTotalLength. */
  0x01,                     /* bNumInterfaces. */
  0x01,                     /* bConfigurationValue. */
  0,                        /* iConfiguration. */
  FEATURE_BUS_POWERED,      /* bmAttributes. */
  50,                       /* bMaxPower (100mA). */

  /* Interface Descriptor.*/
  9,                    /* bLength: Interface Descriptor size */
  TUSB_DESC_INTERFACE,  /* bDescriptorType: Interface */
  FIDO_HID_INTERFACE,   /* bInterfaceNumber: Number of Interface */
  0x00,                 /* bAlternateSetting: Alternate setting */
  0x02,                 /* bNumEndpoints: Two endpoints used */
  0x03,                 /* bInterfaceClass: HID */
  0x00,                 /* bInterfaceSubClass: no boot */
  0x00,                 /* bInterfaceProtocol: 0=none */
  0x04,                 /* iInterface */

  /* HID Descriptor.*/
  9,                            /* bLength: HID Descriptor size */
  0x21,                         /* bDescriptorType: HID */
  0x10, 0x01,                   /* bcdHID: HID Class Spec release number */
  0x00,                         /* bCountryCode: Hardware target country */
  0x01,                         /* bNumDescriptors: Number of HID class descriptors to follow */
  0x22,                         /* bDescriptorType */
  FIDO_HID_REPORT_DESC_SIZE, 0, /* wItemLength: Total length of Report descriptor */

  /*Endpoint IN1 Descriptor*/
  7,                      /* bLength: Endpoint Descriptor size */
  TUSB_DESC_ENDPOINT,     /* bDescriptorType: Endpoint */
  0x81,                   /* bEndpointAddress: (IN1) */
  0x03,                   /* bmAttributes: Interrupt */
  0x40, 0x00,             /* wMaxPacketSize: 64 */
  0x05,                   /* bInterval (5ms) */

  /*Endpoint OUT1 Descriptor*/
  7,                      /* bLength: Endpoint Descriptor size */
  TUSB_DESC_ENDPOINT,     /* bDescriptorType: Endpoint */
  0x01,                   /* bEndpointAddress: (OUT1) */
  0x03,                   /* bmAttributes: Interrupt */
  0x40, 0x00,             /* wMaxPacketSize: 64 */
  0x05,                   /* bInterval (5ms) */
};


class FIDOusb : public EspTinyUSB
{
public:
    FIDOusb();
    bool begin(char* str = nullptr);
    void setBaseEP(uint8_t);
    // int available(void);
    // int peek(void) { return -1; }
    // int read(void) { return -1; }
    // size_t read(uint8_t *buffer, size_t size) { return 0; }
    // void flush(void) { return; }
    // size_t write(uint8_t) { return 0; }
    // size_t write(const uint8_t *buffer, size_t size) { return 0; }

private:
    size_t _len;
    uint8_t _EPNUM_FIDO;
};
#endif
