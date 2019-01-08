#include "HID.h"

volatile uint16_t led = 0;
volatile long lastUpdate = 0;

HID_& HID()
{
    static HID_ obj;
    return obj;
}

int HID_::getInterface(uint8_t* interfaceCount)
{
    *interfaceCount += 1; // uses 1
    HIDDescriptor hidInterface = {
        D_INTERFACE(pluggedInterface, 1, USB_DEVICE_CLASS_HUMAN_INTERFACE, HID_SUBCLASS_NONE, HID_PROTOCOL_NONE),
        D_HIDREPORT(descriptorSize),
        D_ENDPOINT(USB_ENDPOINT_IN(pluggedEndpoint), USB_ENDPOINT_TYPE_INTERRUPT, USB_EP_SIZE, 0x01)
    };
    return USB_SendControl(0, &hidInterface, sizeof(hidInterface));
}

int HID_::getDescriptor(USBSetup& setup)
{
    if (setup.bmRequestType != REQUEST_DEVICETOHOST_STANDARD_INTERFACE) { return 0; }
    if (setup.wValueH != HID_REPORT_DESCRIPTOR_TYPE) { return 0; }
    
    if (setup.wIndex != pluggedInterface) { return 0; }

    int total = 0;
    HIDSubDescriptor* node;
    for (node = rootNode; node; node = node->next) {
        int res = USB_SendControl(TRANSFER_PGM, node->data, node->length);
        if (res == -1)
            return -1;
        total += res;
    }
    
    protocol = HID_REPORT_PROTOCOL;
    
    return total;
}

uint8_t HID_::getShortName(char *name)
{
    name[0] = 'H';
    name[1] = 'I';
    name[2] = 'D';
    name[3] = 'A' + (descriptorSize & 0x0F);
    name[4] = 'A' + ((descriptorSize >> 4) & 0x0F);
    return 5;
}

void HID_::AppendDescriptor(HIDSubDescriptor *node)
{
    if (!rootNode) 
    {
        rootNode = node;
    } else 
    {
        HIDSubDescriptor *current = rootNode;
        while (current->next) 
        {
            current = current->next;
        }
        current->next = node;
    }
    descriptorSize += node->length;
}

int HID_::SendReport(uint8_t id, const void* data, int len)
{
    auto ret = USB_Send(pluggedEndpoint, &id, 1);
    if (ret < 0) return ret;
    auto ret2 = USB_Send(pluggedEndpoint | TRANSFER_RELEASE, data, len);
    if (ret2 < 0) return ret2;
    return ret + ret2;
}

bool HID_::setup(USBSetup& setup)
{
    if (pluggedInterface != setup.wIndex) 
    {
        return false;
    }

    uint8_t request = setup.bRequest;
    uint8_t requestType = setup.bmRequestType;

    if (requestType == REQUEST_DEVICETOHOST_CLASS_INTERFACE)
    {
        if (request == HID_GET_REPORT) 
        {
            return true;
        }
        if (request == HID_GET_PROTOCOL) 
        {
            return true;
        }
    }

    if (requestType == REQUEST_HOSTTODEVICE_CLASS_INTERFACE)
    {
        if (request == HID_SET_PROTOCOL) 
        {
            protocol = setup.wValueL;
            return true;
        }
        if (request == HID_SET_IDLE) 
        {
            idle = setup.wValueL;
            return true;
        }
        if (request == HID_SET_REPORT)
        {
            if(setup.wLength == 3)
            {
                uint8_t data[3];
                if (3 == USB_RecvControl(data, 3))
                {
                    led = (uint16_t) (data[2] << 8) + data[1];
                    lastUpdate = millis();
                }
                return true;
            }
        }
    }

    return false;
}

HID_::HID_(void) : PluggableUSBModule(1, 1, epType),
                   rootNode(NULL), descriptorSize(0),
                   protocol(HID_REPORT_PROTOCOL), idle(1)
{
    epType[0] = EP_TYPE_INTERRUPT_IN;
    PluggableUSB().plug(this);
}

int HID_::begin(void)
{
    return 0;
}
