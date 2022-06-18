#include "fidousb.h"
#define EPNUM_FIDO 0x05
#if CFG_TUD_FIDO

FIDOusb::FIDOusb()
{
    enableFIDO = true;
    _EPNUM_FIDO = EPNUM_FIDO;
}

void FIDOusb::setBaseEP(uint8_t ep)
{
    _EPNUM_FIDO = ep;
}

bool FIDOusb::begin(char* str)
{
    // Interface number, string index, EP Out & EP In address, EP size
    // uint8_t fido[] = {TUD_FIDO_DESCRIPTOR(1, 8, _EPNUM_FIDO, (uint8_t)(0x80 | _EPNUM_FIDO), 64)};
    // memcpy(&desc_configuration[total], fido, sizeof(fido));
    // total += sizeof(fido);
    // count += 2;
    // return EspTinyUSB::begin(str, 8);
}

#endif
