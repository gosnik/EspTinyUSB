#include "sdusb.h"

#ifdef CFG_TUD_MSC

class SDCallbacks : public MSCCallbacks {
    SDCard2USB* m_parent;
    char _vendor_id[8+1];
    char _product_id[16+1];
    char _product_rev[4+1];

public:
    SDCallbacks(SDCard2USB* ram) 
    { 
        m_parent = ram; 
        const char vid[] = "ESP32-S2";
        const char pid[] = "SD card";
        const char rev[] = "1.0";

        strcpy(_vendor_id  , vid);
        strcpy(_product_id , pid);
        strcpy(_product_rev, rev);
    }

    SDCallbacks(SDCard2USB* ram, const char* vid, const char* pid, const char* rev) 
    { 
        m_parent = ram; 
        memset(_vendor_id, 0, 8+1);
        strncpy(_vendor_id, vid, 8);

        memset(_product_id, 0, 16+1);
        strncpy(_product_id, pid, 16);

        memset(_product_rev, 0, 4+1);
        strncpy(_product_rev, rev, 4);
    }

    ~SDCallbacks() { }
    void onInquiry(uint8_t lun, uint8_t vendor_id[8], uint8_t product_id[16], uint8_t product_rev[4]) 
    {
        if (m_parent->m_private)
        {
            m_parent->m_private->onInquiry(lun, vendor_id, product_id, product_rev);
        } else {
            memcpy(vendor_id  , _vendor_id, strlen(_vendor_id));
            memcpy(product_id , _product_id, strlen(_product_id));
            memcpy(product_rev, _product_rev, strlen(_product_rev));
            //log_v("default onInquiry");
        }
    }
    bool onReady(uint8_t lun) {
        if (m_parent->m_private)
        {
            return m_parent->m_private->onReady(lun);
        } 
        else 
        {
            return m_parent->sdcardReady;
        }
    }
    void onCapacity(uint8_t lun, uint32_t* block_count, uint16_t* block_size)
    {
        (void) lun;
        *block_count = m_parent->block_count;
        *block_size = m_parent->block_size;
        log_v("disk block count: %d, block size: %d\n", *block_count, *block_size);
    }
    bool onStop(uint8_t lun, uint8_t power_condition, bool start, bool load_eject)
    {
        (void) lun;
        (void) power_condition;

        if ( load_eject )
        {
            if (start)
            {
                // load disk storage
                log_v("default start/stop load");
            }else
            {
                // unload disk storage
                log_v("default start/stop unload");
            }
        }

        return true;
    }
    int32_t onRead(uint8_t lun, uint32_t lba, uint32_t offset, void* buffer, uint32_t bufsize)
    {
        //log_v("default onread");
        (void) lun;
        
        if (!m_parent->sdcardReady)
            return 0;

        if (RES_OK == SD.sd_read((uint8_t*)buffer, lba, bufsize/m_parent->block_size))
            return bufsize;

        return 0;
    }
    int32_t onWrite(uint8_t lun, uint32_t lba, uint32_t offset, void* buffer, uint32_t bufsize)
    {
        //log_v("default onwrite");
        (void) lun;

        if (!m_parent->sdcardReady)
            return 0;

        if (RES_OK == SD.sd_write((const uint8_t*)buffer, lba, bufsize/m_parent->block_size))
            return bufsize;

        return 0;
    }
};

SDCard2USB::SDCard2USB( )
{
    MSCusb::setCallbacks(new SDCallbacks(this));
}

SDCard2USB::SDCard2USB(const char* vid, const char* pid, const char* rev)
{
    MSCusb::setCallbacks(new SDCallbacks(this, vid, pid, rev));
}

bool SDCard2USB::begin(char* str)
{
    //assert(block_count);
    //assert(block_size);

    return MSCusb::begin(str);
}

bool SDCard2USB::initSD(uint8_t ssPin, SPIClass &spi, uint32_t frequency, const char * mountpoint, uint8_t max_files, const ff_diskio_impl_t * impl)
{
    if(!SD.begin(ssPin, spi, frequency, mountpoint, max_files))
    {
        //Serial.println("Card Mount Failed");
        return false;
    }

    return true;
}

bool SDCard2USB::initSD(int8_t sck, int8_t miso, int8_t mosi, int8_t ss, const ff_diskio_impl_t * impl)
{

    static SPIClass* spi = NULL;
    spi = new SPIClass(FSPI);
    spi->begin(sck, miso, mosi, ss);
    if(!SD.begin(ss, *spi, 40000000, "/sd", 5, true, impl))
    {
        delete spi;
        spi = NULL;
        return false;
    }
    
    uint8_t cardType = SD.cardType();

    if(cardType == CARD_NONE) {
        return false;
    }

    block_count = SD.cardSize() / block_size;
    sdcardReady = true;
    return true;
}

void SDCard2USB::setCapacity(uint32_t count, uint32_t size)
{
    block_count = count;
    block_size = size;
}

void SDCard2USB::setCallbacks(MSCCallbacks* cb)
{
    m_private = cb;
}

void SDCard2USB::ready(bool ready)
{
    sdcardReady = ready;
}

bool SDCard2USB::isReady()
{
    return sdcardReady;
}

#endif
