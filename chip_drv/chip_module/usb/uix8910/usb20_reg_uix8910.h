
#ifndef _USB20_REG_UIX8909_HEADER_
#define _USB20_REG_UIX8909_HEADER_

//**************** Core Global CSR ***************************
#define USB_GOTGCTL                     (USB_REG_BASE + 0x0000)
#define USB_GOTGINT                     (USB_REG_BASE + 0x0004)
#define USB_GAHBCFG                     (USB_REG_BASE + 0x0008)
#define USB_GUSBCFG                     (USB_REG_BASE + 0x000c)
#define USB_GRSTCTL                     (USB_REG_BASE + 0x0010)
#define USB_GINTSTS                     (USB_REG_BASE + 0x0014)
#define USB_GINTMSK                     (USB_REG_BASE + 0x0018)
#define USB_GRXSTSR                     (USB_REG_BASE + 0x001c)
#define USB_GRXSTSP                     (USB_REG_BASE + 0x0020)
#define USB_GRXFSIZ                     (USB_REG_BASE + 0x0024)
#define USB_GNPTXFSIZ                   (USB_REG_BASE + 0x0028)
#define USB_GNPTXSTS                    (USB_REG_BASE + 0x002c)

#if 0
#define USB_GI2CCTL                     (USB_REG_BASE + 0x0030)
#define USB_GPVNDCTL                    (USB_REG_BASE + 0x0034)
#endif

#define USB_GGPIO                       (USB_REG_BASE + 0x0038)
#define USB_GUID                        (USB_REG_BASE + 0x003c)
#define USB_GSNPSID                     (USB_REG_BASE + 0x0040)
#define USB_GHWCFG1                     (USB_REG_BASE + 0x0044)
#define USB_GHWCFG2                     (USB_REG_BASE + 0x0048)
#define USB_GHWCFG3                     (USB_REG_BASE + 0x004c)
#define USB_GHWCFG4                     (USB_REG_BASE + 0x0050)

#if 0
#define USB_GLPMCFG                     (USB_REG_BASE + 0x0054)
#endif

#define USB_HPTXFSIZ                    (USB_REG_BASE + 0x0100)

#if 0
#define USB_DPTXFSIZ(n)                 (USB_REG_BASE + 0x0104+(n-1)*0x04)   // n(1-15)
#endif

#define USB_DIEPTXF(n)                  (USB_REG_BASE + 0x0104+(n-1)*0x04)   // n(1-15)

//**************** Power and Clock geting  CSR ****************
#define USB_PCGCR                       (USB_REG_BASE + 0x0E00)

//**************** Device CSR *********************************
#define USB_DCFG                        (USB_REG_BASE + 0x0800)
#define USB_DCTL                        (USB_REG_BASE + 0x0804)
#define USB_DSTS                        (USB_REG_BASE + 0x0808)
#define USB_DIEPMSK                     (USB_REG_BASE + 0x0810)
#define USB_DOEPMSK                     (USB_REG_BASE + 0x0814)
#define USB_DAINT                       (USB_REG_BASE + 0x0818)
#define USB_DAINTMSK                    (USB_REG_BASE + 0x081c)

#if 0
#define USB_DTKNQR1                     (USB_REG_BASE + 0x0820)
#define USB_DTKNQR2                     (USB_REG_BASE + 0x0824)
#define USB_DTKNQR3                     (USB_REG_BASE + 0x0830)
#define USB_DTKNQR4                     (USB_REG_BASE + 0x0834)
#endif

#define USB_DVBUSDIS                    (USB_REG_BASE + 0x0828)
#define USB_DVBUSPULSE                  (USB_REG_BASE + 0x082c)
#define USB_DTHRCTL                     (USB_REG_BASE + 0x0830)
#define USB_DIEPEMPMSK                  (USB_REG_BASE + 0x0834)

#if 0
#define USB_DEACHINT                    (USB_REG_BASE + 0x0838)
#define USB_DEACHINTMSK                 (USB_REG_BASE + 0x083c)
#define USB_DIACHINTMSK(n)              (USB_REG_BASE + 0x0840+(n)*0x04)  // n(0-15)
#define USB_DOACHINTMSK(n)              (USB_REG_BASE + 0x0880+(n)*0x04)  // n(0-15)
#endif

#define USB_DIEP0CTL                    (USB_REG_BASE + 0x0900)
#define USB_DIEPCTL(n)                  (USB_REG_BASE + 0x0900+(n)*0x20)  // n(1-15)
#define USB_DIEPINT(n)                  (USB_REG_BASE + 0x0908+(n)*0x20)  // n(0-15)
#define USB_DIEP0TSIZ                   (USB_REG_BASE + 0x0910)
#define USB_DIEPTSIZ(n)                 (USB_REG_BASE + 0x0910+(n)*0x20)  // n(1-15)
#define USB_DIEPDMA(n)                  (USB_REG_BASE + 0x0914+(n)*0x20)  // n(0-15)
#define USB_DTXFSTS(n)                  (USB_REG_BASE + 0x0918+(n)*0x20)  // n(0-15)
#define USB_DIEPDMAB(n)                 (USB_REG_BASE + 0x091c+(n)*0x20)  // n(0-15)
#define USB_DOEP0CTL                    (USB_REG_BASE + 0x0b00)
#define USB_DOEPCTL(n)                  (USB_REG_BASE + 0x0b00+(n)*0x20)  // n(1-15)
#define USB_DOEPINT(n)                  (USB_REG_BASE + 0x0b08+(n)*0x20)  // n(0-15)
#define USB_DOEP0TSIZ                   (USB_REG_BASE + 0x0b10)
#define USB_DOEPTSIZ(n)                 (USB_REG_BASE + 0x0b10+(n)*0x20)  // n(1-15)
#define USB_DOEPDMA(n)                  (USB_REG_BASE + 0x0b14+(n)*0x20)  // n(0-15)
#define USB_DOEPDMAB(n)                 (USB_REG_BASE + 0x0b1c+(n)*0x20)  // n(0-15)

#define USB_PCGCCTL                     (USB_REG_BASE + 0xE00)

#if 0
//**************** Device EP FIFO base address  *********************************
#define USB_DEPFIFOBA(n)                (USB_REG_BASE + (n+1)*0x1000)   // n(0-15)
#endif

#if 0
#define USB_CTRL_END                    0x203FFFFF
#define USB_CLK_12M                     12000000
#define USB_CLK_48M                     48000000
#endif


#define USB_DEVICE                      0
#define USB_INTERFACE                   1
#define USB_ENDPOINT                    2
#define USB_RECIPIENT_OTHER             3

#define  ENPOINT_HALT                   0

#define  USB_CONNECTTED                  0
#define  USB_POWERED                     1
#define  USB_DEFAULT                     2
#define  USB_ADDRESSED                   3
#define  USB_CONFIGED                    4


#define USB_EP_DIR_OUT                  1
#define USB_EP_DIR_IN                   0


#define USB_PACKET_16           0x10
#define USB_PACKET_32           0x20
#define USB_PACKET_64           0x40
#define USB_PACKET_128          0x80
#define USB_PACKET_256          0x100
#define USB_PACKET_512          0x200
#define USB_PACKET_1024         0x400

// USB endpoind type
typedef enum
{
    USB_EP_TYPE_CONTROL   = 0,
    USB_EP_TYPE_ISO          ,
    USB_EP_TYPE_BULK         ,
    USB_EP_TYPE_INTERRUPT    ,
    USB_EP_TYPE_MAX
} USB_EP_TYPE_E;

#undef USB_EP1
#undef USB_EP2
#undef USB_EP3
#undef USB_EP4
#undef USB_EP5
#undef USB_EP6

// USB EPx ID
typedef enum
{
    USB_EP0 = 0,
    USB_EP1,
    USB_EP2,
    USB_EP3,
    USB_EP4,
    USB_EP5,
    USB_EP6,
    USB_EP7,
    USB_EP8,
    USB_EP9,
    USB_EP10,
    USB_EP11,
    USB_EP12,
    USB_EP_NUMBER
} USB_EP_NUM_E;

#if 0
#define USB_INT_SOF                     (BIT_0)
#define USB_INT_SPEND                   (BIT_1)
#define USB_INT_RESUM                   (BIT_2)
#define USB_INT_RST                     (BIT_3)
#define USB_INT_UNPID                   (BIT_4)
#define USB_INT_EPID                    (BIT_5)
#define USB_INT_CHANGE                  (BIT_6)
#define USB_INT_DEVICE                  (BIT_7)
#define USB_INT_EP0                     (BIT_8)
#define USB_INT_EP1                     (BIT_9)
#define USB_INT_EP2                     (BIT_10)
#define USB_INT_EP3                     (BIT_11)
#define USB_INT_EP4                     (BIT_12)
#endif

/**----------------------------------------------------------------------------*
**                             Data Prototype                                 **
**----------------------------------------------------------------------------*/

//************************** Core Global Register Prototype *******************/

typedef union usb_gahbcfg_tag
{
    struct usb_gahbcfg_map
    {
        volatile unsigned int   global_int_mask     :1;  //global_interrupt_mask
        volatile unsigned int   burst_length        :4;  //burst_length
        volatile unsigned int   dma_enable          :1;  //dma_enable
        volatile unsigned int   reserved_1          :1;
        volatile unsigned int   nptxfemplvl         :1;  //non_periodic_txfifo_empty_level
        volatile unsigned int   ptxfemplvl          :1;  //periodic_txfifo_empty_level
        volatile unsigned int   reserved_2          :12;
        volatile unsigned int   remmemsupp          :1;
        volatile unsigned int   notialldmawrit      :1;
        volatile unsigned int   ahbsingle           :1;
        volatile unsigned int   invdescendianess    :1;
        volatile unsigned int   reserved_3          :7;
    } mBits;
    volatile unsigned int dwValue ;
} USB_GAHBCFG_U;

typedef union usb_gusbcfg_tag
{
    struct usb_gusbcfg_map
    {
        volatile unsigned int   toutcal                 :3;  //HS/FS time out calibration
        volatile unsigned int   phyintf                 :1;  //PHY interface
        volatile unsigned int   ulpiutmisel             :1;  //ULPI or UTMI+ select
        volatile unsigned int   fsintf                  :1;  //FULL speed serial interface select
        volatile unsigned int   physel                  :1;  //USB 2.0 high speed PHY or USB 1.1 FULL speed serial transceiver select
        volatile unsigned int   reserved_1              :1;  //ULPI DDR select
        volatile unsigned int   srpcap                  :1;  //SRP capable
        volatile unsigned int   hnpcap                  :1;  //HNP capable
        volatile unsigned int   usbtrdtim               :4;  //USB turnaroud time
        volatile unsigned int   reserved_2              :1;
        volatile unsigned int   phylpwrclksel           :1;  //PHY low power clock select
        volatile unsigned int   reserved_3              :6;
        volatile unsigned int   temseldlpulse           :1;  //Termsel Dline PUlsing Sleection
        volatile unsigned int   reserved_4              :3;
        volatile unsigned int   ic_usb_cap              :1;  //IC USB capable
        volatile unsigned int   reserved_5              :1;
        volatile unsigned int   tx_end_delay            :1;  //TX end delay
        volatile unsigned int   force_hst_mod           :1;  //Force host mode
        volatile unsigned int   force_dev_mod           :1;  //Force device mode
        volatile unsigned int   corrupt_tx_packet       :1;  //Corrupt TX packet
    } mBits ;
    volatile unsigned int dwValue ;
} USB_GUSBCFG_U;

typedef union usb_grstctl_tag
{
    struct usb_grstctl_map
    {
        volatile unsigned int   csftrst                 :1;  //Core soft reset
        volatile unsigned int   fsftrst                 :1;  //Hclock soft reset
        volatile unsigned int   frmcntrrst              :1;  //host frame counter reset
        volatile unsigned int   reserved_1              :1;  //in token sequence learning queue flush
        volatile unsigned int   rxfflush                :1;  //RX fifo flush
        volatile unsigned int   txfflush                :1;  //TX fifo flush
        volatile unsigned int   txfnum                  :5;  //TX fifo number
        volatile unsigned int   reserved_2              :19;
        volatile unsigned int   dmareq                  :1;  //DMA request signal
        volatile unsigned int   ahbidle                 :1;  //AHB Master idle
    } mBits ;
    volatile unsigned int dwValue ;
} USB_GRSTCTL_U;

typedef union usb_gintsts_tag
{
    struct usb_gintsts_map
    {
        volatile unsigned int   curmod                  :1;  //current mode of opration
        volatile unsigned int   modemis                 :1;  //mode mismatch interrupt
        volatile unsigned int   otgint                  :1;  //otg interrupt
        volatile unsigned int   sof                     :1;  //start of frame
        volatile unsigned int   rxflvl                  :1;  //RX fifo non empty
        volatile unsigned int   nptxfemp                :1;  //non periodic nak effective
        volatile unsigned int   ginnakeff               :1;  //global in nak effective
        volatile unsigned int   goutnakeff              :1;  //global out nak effective
        volatile unsigned int   reserved_1              :2;
        volatile unsigned int   erlususp                :1;  //early suspend
        volatile unsigned int   usbsusp                 :1;  //usb suspend
        volatile unsigned int   usbrst                  :1;  //usb reset
        volatile unsigned int   enumdone                :1;  //enumeration done
        volatile unsigned int   isooutdrop              :1;  //iso out packet dropped interrupt
        volatile unsigned int   eopf                    :1;  //end of periodic frame interrupt
        volatile unsigned int   reserved_2              :1;
        volatile unsigned int   epmis                   :1;  //endpoint mismatch interrupt
        volatile unsigned int   iepint                  :1;  //in endpoint interrupt
        volatile unsigned int   oepint                  :1;  //out endpoint interrupt
        volatile unsigned int   incomplsoin             :1;  //incomplete iso in transfer
        volatile unsigned int   incomip                 :1;  //incompleteperiodic transfer
        volatile unsigned int   fetsusp                 :1;  //data fetch suspend
        volatile unsigned int   resetdet                :1;  //reset detected interrupt
        volatile unsigned int   prtint                  :1;
        volatile unsigned int   hchint                  :1;
        volatile unsigned int   ptxfemp                 :1;
        volatile unsigned int   reserved_3              :1;
        volatile unsigned int   conidstschng            :1;  //connecter id status change interupt
        volatile unsigned int   disconnect              :1;
        volatile unsigned int   sessreqint              :1;  //session request
        volatile unsigned int   wkupint                 :1;  //resume/remote wakeup dedected interrupt
    } mBits ;
    volatile unsigned int dwValue ;
} USB_GINTSTS_U;

typedef union usb_gintmsk_tag
{
    struct usb_gintmsk_map
    {
        volatile unsigned int   reserved_1                 :1;
        volatile unsigned int   modemismsk                 :1;  //mode mismatch interrupt
        volatile unsigned int   otgintmsk                  :1;  //otg interrupt
        volatile unsigned int   sofmsk                     :1;  //start of frame
        volatile unsigned int   rxflvlmsk                  :1;  //RX fifo non empty
        volatile unsigned int   nptxfempmsk                :1;  //non periodic nak effective
        volatile unsigned int   ginnakeffmsk               :1;  //global in nak effective
        volatile unsigned int   goutnakeffmsk              :1;  //global out nak effective
        volatile unsigned int   reserved_2                 :2;
        volatile unsigned int   erlususpmsk                :1;  //early suspend
        volatile unsigned int   usbsuspmsk                 :1;  //usb suspend
        volatile unsigned int   usbrstmsk                  :1;  //usb reset
        volatile unsigned int   enumdonemsk                :1;  //enumeration done
        volatile unsigned int   isooutdropmsk              :1;  //iso out packet dropped interrupt
        volatile unsigned int   eopfmsk                    :1;  //end of periodic frame interrupt
        volatile unsigned int   reserved_3                 :1;
        volatile unsigned int   epmismsk                   :1;  //endpoint mismatch interrupt
        volatile unsigned int   iepintmsk                  :1;  //in endpoint interrupt
        volatile unsigned int   oepintmsk                  :1;  //out endpoint interrupt
        volatile unsigned int   reserved_4                 :1;
        volatile unsigned int   incomipmsk                 :1;  //data fetch suspend
        volatile unsigned int   fetsuspmsk                 :1;  //data fetch suspend
        volatile unsigned int   resetdetmsk                :1;  //reset detected interrupt
        volatile unsigned int   prtintmsk                  :1;
        volatile unsigned int   hchintmsk                  :1;
        volatile unsigned int   ptxfempmsk                 :1;
        volatile unsigned int   reserved_5                 :1;
        volatile unsigned int   conidstschngmsk            :1;  //connecter id status change interupt
        volatile unsigned int   disconintmsk               :1;  //disconnect detected interrupt mask
        volatile unsigned int   sessreqintmsk              :1;  //session request
        volatile unsigned int   wkupintmsk                 :1;  //resume/remote wakeup dedected interrupt
    } mBits ;
    volatile unsigned int dwValue ;
} USB_GINTMSK_U;

typedef union usb_grxstsp_tag
{
    struct usb_grxstsp_map
    {
        volatile uint32  endpoint_number                 :4;  //endpoint nummber
        volatile uint32  byte_count                      :11; //byte count
        volatile uint32  data_pid                        :2;  //data PID
        volatile uint32  packet_status                   :4;  //packet status
        volatile uint32  frame_number                    :4;  //frame nummber
        volatile uint32  reserved                        :7;
    } mBits;
    volatile unsigned int dwValue ;
} USB_GRXSTSP_U;

typedef union usb_grxstsr_tag
{
    struct usb_grxstsr_map
    {
        volatile uint32  endpoint_number                 :4;  //endpoint nummber
        volatile uint32  byte_count                      :11; //byte count
        volatile uint32  data_pid                        :2;  //data PID
        volatile uint32  packet_status                   :4;  //packet status
        volatile uint32  frame_number                    :4;  //frame nummber
        volatile uint32  reserved                        :7;
    } mBits;
    volatile unsigned int dwValue ;
} USB_GRXSTSR_U;

typedef union usb_grxfsiz_tag
{
    struct usbgrxfsiz__map
    {
        volatile unsigned int   rxfdep                      :12;  //RX fifo deepth
        volatile unsigned int   reserved                    :20;
    } mBits ;
    volatile unsigned int dwValue ;
} USB_GRXFSIZ_U;

typedef union usb_gnptxfsiz_tag
{
    struct usb_gnptxfsiz_map
    {
        volatile unsigned int       nptxfstaddr           :12;  //non periodic transmit ram start address
        volatile unsigned int       reserved_1            :4;
        volatile unsigned int       nptxfdep              :12;  //non periodic TX fifo deepth
        volatile unsigned int       reserved_2            :4;
    } mBits ;
    volatile unsigned int dwValue ;
} USB_GNPTXFSIZ_U;

typedef union usb_gnptxsts_tag
{
    struct usb_gnptxsts_map
    {
        volatile unsigned int       nptxfsavail          :16;   //non periodic tx fifo  space available
        volatile unsigned int       nptxqspcavail        :8;    //non periodic transmit request queue space available
        volatile unsigned int       nptxqtop             :7;    //top of non periodic transmit request queue
        volatile unsigned int       reserved             :1;
    } mBits ;
    volatile unsigned int dwValue ;
} USB_GNPTXSTS_U;

typedef union usb_ggpio_tag
{
    struct usb_ggpio_map
    {
        volatile unsigned int       gpi               :16;  //general purpose input
        volatile unsigned int       gpo               :16;  //general purpose output
    } mBits ;
    volatile unsigned int dwValue ;
} USB_GGPIO_U;

typedef union usb_ghwcfg1_tag
{
    struct usb_ghwcfg1_map
    {
        volatile unsigned int    epfir         :32;  //endpoint dirction
    } mBits ;
    volatile unsigned int dwValue ;
} USB_GHWCFG1_U;

typedef union usb_ghwcfg2_tag
{
    struct usb_ghwcfg2_map
    {
        volatile unsigned int       otgmode             :3;  //mode of opration
        volatile unsigned int       architechure        :2;  //architechure
        volatile unsigned int       singpnt             :1;  //point to point
        volatile unsigned int       hsphytype           :2;  //HS PHY interface type
        volatile unsigned int       fsphytype           :2;  //FS PHY interface type
        volatile unsigned int       numdeveps           :4;  //nummber of device endpoint
        volatile unsigned int       numhstchnl          :4;  //nummber of host channel
        volatile unsigned int       periodicsupport     :1;  //periodic out channels support in host mode
        volatile unsigned int       dynfifosizing       :1;  //dynamic fifo size enable
        volatile unsigned int       multiprocintprt     :1;  //multi processor interrupt enable
        volatile unsigned int       reserved_1          :1;
        volatile unsigned int       nptxqdepth          :2;  //non periodic request queue deepth
        volatile unsigned int       ptxqdepth           :2;  //host mode periodic request queue deepth
        volatile unsigned int       tknqdepth           :5;  //device mode in taken sequence learning queue depth
        volatile unsigned int       reserved_2          :1;
    } mBits ;
    volatile unsigned int dwValue ;
} USB_GHWCFG2_U;

typedef union usb_ghwcfg3_tag
{
    struct usb_ghwcfg3_map
    {
        volatile unsigned int       xfersizewidth      :4;  //width of transfer size counter
        volatile unsigned int       pktsizewidth       :3;  //width of packet size counter
        volatile unsigned int       otgen              :1;  //
        volatile unsigned int       i2cselect          :1;  //
        volatile unsigned int       vndctlsupt         :1;  //
        volatile unsigned int       optfeature         :1;  //
        volatile unsigned int       rsttype            :1;  //
        volatile unsigned int       adpsupport         :1;  //
        volatile unsigned int       hsicmode           :1;  //
        volatile unsigned int       bcsupport          :1;  //
        volatile unsigned int       lpmmode            :1;  //
        volatile unsigned int       dfifodepth         :16; // D FIFO depth
    } mBits ;
    volatile unsigned int dwValue ;
} USB_GHWCFG3_U;

typedef union usb_ghwcfg4_tag
{
    struct usb_ghwcfg4_map
    {
        volatile unsigned int       numdevperioeps      :4;   //nummber of device mode periodic in endpoint
        volatile unsigned int       enablepwropt        :1;   //enable power optimization
        volatile unsigned int       ahbfreq             :1;   //minimum ahb frequency less then 60MHz
        volatile unsigned int       hibernation         :1;
        volatile unsigned int       extendhibernation   :1;
        volatile unsigned int       reserved            :4;
        volatile unsigned int       acgsupt             :1;
        volatile unsigned int       enhancedlpmsupt     :1;
        volatile unsigned int       phydatawidth        :2;   //UTMI+ PHY/ULPI to internal UTMI+ Wrapper data width
        volatile unsigned int       numctleps           :4;   //nummber of device mode control endpoint in addition to endpoint 0
        volatile unsigned int       iddgfltr            :1;
        volatile unsigned int       vbusvalidfltr       :1;
        volatile unsigned int       avalidfltr          :1;
        volatile unsigned int       bvalidfltr          :1;
        volatile unsigned int       sessendfltr         :1;
        volatile unsigned int       dedfifomod          :1;
        volatile unsigned int       ineps               :4;   //nummber of decice mode in endpoint include control endpoint
        volatile unsigned int       sgdmac              :1;
        volatile unsigned int       sgdma               :1;
    } mBits ;
    volatile unsigned int dwValue ;
} USB_GHWCFG4_U;

typedef union usb_dptxfsiz_tag
{
    struct usb_dptxfsiz_map
    {
        volatile unsigned int            dptxfstraddr       :16;  //device periodic TX fifo ram start address
        volatile unsigned int            dptxfsize          :16;  //device periodic TX fifo size
    } mBits ;
    volatile unsigned int dwValue ;
} USB_DPTXFSIZ_U;

typedef union usb_dieptxf_tag
{
    struct usb_dieptxf_map
    {
        volatile unsigned int           ineptxfstraddr      :12;  //in endpoint fifo transmit ram start address
        volatile unsigned int           reserved_1          :4;
        volatile unsigned int           ineptxfsize         :12;  //in endpoint TX fifo depth
        volatile unsigned int           reserved_2          :4;
    } mBits ;
    volatile unsigned int dwValue ;
} USB_DIEPTXF_U;

/*********************Device Register Prototype****************/

typedef union usb_dcfg_tag
{
    struct usb_dcfg_map
    {
        volatile unsigned int       devspeed            :2;  //device speed
        volatile unsigned int       nzstsouthshk        :1;  //non zero length status out handshake
        volatile unsigned int       ena32khzsusp        :1;
        volatile unsigned int       devaddr             :7;  //device address
        volatile unsigned int       perfrint            :2;  //periodic frame interval
        volatile unsigned int       endevoutnak         :1;
        volatile unsigned int       xcvrdly             :1;
        volatile unsigned int       erraticintmsk       :1;
        volatile unsigned int       reserved            :7;
        volatile unsigned int       descdma             :1;
        volatile unsigned int       perschintvl         :2;
        volatile unsigned int       reserved_2          :6;
    } mBits ;
    volatile unsigned int dwValue ;
} USB_DCFG_U;

typedef union usb_dctl_tag
{
    struct usb_dctl_map
    {
        volatile unsigned int           rmtwkupsig        :1;  //remote wakeup signaling
        volatile unsigned int           sftdiscon         :1;  //soft disconect
        volatile unsigned int           gnpinnaksts       :1;  //global non periodic in nak status
        volatile unsigned int           goutnaksts        :1;  //global out nak status
        volatile unsigned int           tstctl            :3;  //test control
        volatile unsigned int           sgnpinnak         :1;  //set global non periodic in nak
        volatile unsigned int           cgnpinnak         :1;  //clear global non periodic in nak
        volatile unsigned int           sgoutnak          :1;  //set out nak
        volatile unsigned int           cgoutnak          :1;  //clear out nak
        volatile unsigned int           pwronprgdone      :1;  //power on programing done
        volatile unsigned int           reserved_1        :1;  //power on programing done
        volatile unsigned int           gmc               :2;  //global muti count
        volatile unsigned int           ignrfrmnum        :1;  //
        volatile unsigned int           setnak            :1;  //
        volatile unsigned int           encontonbna       :1;  //
        volatile unsigned int           reserved_2        :14;
    } mBits ;
    volatile unsigned int dwValue ;
} USB_DCTL_U;

typedef union usb_dsts_tag
{
    struct usb_dsts_map
    {
        volatile unsigned int           suspsts             :1;  // suspend status
        volatile unsigned int           enumspd             :2;  //enumeration speed
        volatile unsigned int           errticerr           :1;  //erratic error
        volatile unsigned int           reserved_1          :4;
        volatile unsigned int           soffn               :14;  //frame or microframe nummber of the recieved SOF
        volatile unsigned int           devlnsts            :2;
        volatile unsigned int           reserved_2          :8;
    } mBits ;
    volatile unsigned int dwValue ;
} USB_DSTS_U;

typedef union usb_diepmsk_tag
{
    struct usb_diepmsk_map
    {
        volatile unsigned int           xfercompimsk         :1;  //transfer completed interrupt mask
        volatile unsigned int           epdisbidmsk          :1;  //endpoint disabled interrupt mask
        volatile unsigned int           ahberrmsk            :1;  //ahb error mask
        volatile unsigned int           timeoutmsk           :1;  //time out mask
        volatile unsigned int           intkntxfempmsk       :1;  //in token recieved when tx fifo empty mask
        volatile unsigned int           intknepmismsk        :1;  //in token recieved with ep mismatch mask
        volatile unsigned int           inepnakeffmsk        :1;  //in endpoint nak effective mask
        volatile unsigned int           reserved_1           :1;
        volatile unsigned int           fifounmsk            :1;  //fifo underrun mask
        volatile unsigned int           bnakintmsk           :1;  //BNK interrupt mask
        volatile unsigned int           reserved_2           :3;
        volatile unsigned int           nakmsk               :1;  //NAK interrupt mask
        volatile unsigned int           reserved_3           :18;
    } mBits ;
    volatile unsigned int dwValue ;
} USB_DIEPMSK_U;

typedef union usb_doepmsk_tag
{
    struct usb_doepmsk_map
    {
        volatile unsigned int           xfercompmsk          :1;  //transfer completed interrupt mask
        volatile unsigned int           epdismsk             :1;  //endpopint disabled interrupt mask
        volatile unsigned int           ahberrmsk            :1;  //ahb error
        volatile unsigned int           setupmsk             :1;  //setup phase done interrupt
        volatile unsigned int           outtknepdismsk       :1;  //out token recieved when endpoint disabled mask
        volatile unsigned int           stsphsercvdmsk       :1;
        volatile unsigned int           b2bsetup             :1;  //back to back setup packet recieved mask
        volatile unsigned int           reserved_1           :1;
        volatile unsigned int           outperrmsk           :1;  //out packet error mask
        volatile unsigned int           bnkmsk               :1;  //BNK mask
        volatile unsigned int           reserved_2           :2;
        volatile unsigned int           bbleerrmsk           :1;  //Babble error mask
        volatile unsigned int           nakmsk               :1;  //NAK mask
        volatile unsigned int           nyetmsk              :1;  //NYET mask
        volatile unsigned int           reserved_3           :17;
    } mBits ;
    volatile unsigned int dwValue ;
} USB_DOEPMSK_U;

typedef union usb_daint_tag
{
    struct usb_daint_map
    {
        volatile unsigned int            inepint_0         :1;  // in endpoint interrupt
        volatile unsigned int            inepint_1         :1;  // in endpoint interrupt
        volatile unsigned int            inepint_2         :1;  // in endpoint interrupt
        volatile unsigned int            inepint_3         :1;  // in endpoint interrupt
        volatile unsigned int            inepint_4         :1;  // in endpoint interrupt
        volatile unsigned int            inepint_5         :1;  // in endpoint interrupt
        volatile unsigned int            inepint_6         :1;  // in endpoint interrupt
        volatile unsigned int            inepint_7         :1;  // in endpoint interrupt
        volatile unsigned int            inepint_8         :1;  // in endpoint interrupt
        volatile unsigned int            inepint_9         :1;  // in endpoint interrupt
        volatile unsigned int            inepint_10        :1;  // in endpoint interrupt
        volatile unsigned int            inepint_11        :1;  // in endpoint interrupt
        volatile unsigned int            inepint_12        :1;  // in endpoint interrupt
        volatile unsigned int            reserved_1        :3;  // in endpoint interrupt
        volatile unsigned int            outepint_0        :1;  // in endpoint interrupt
        volatile unsigned int            outepint_1        :1;  // in endpoint interrupt
        volatile unsigned int            outepint_2        :1;  // in endpoint interrupt
        volatile unsigned int            outepint_3        :1;  // in endpoint interrupt
        volatile unsigned int            outepint_4        :1;  // in endpoint interrupt
        volatile unsigned int            outepint_5        :1;  // in endpoint interrupt
        volatile unsigned int            outepint_6        :1;  // in endpoint interrupt
        volatile unsigned int            outepint_7        :1;  // in endpoint interrupt
        volatile unsigned int            outepint_8        :1;  // in endpoint interrupt
        volatile unsigned int            outepint_9        :1;  // in endpoint interrupt
        volatile unsigned int            outepint_10       :1;  // in endpoint interrupt
        volatile unsigned int            outepint_11       :1;  // in endpoint interrupt
        volatile unsigned int            outepint_12       :1;  // in endpoint interrupt
        volatile unsigned int            reserved_2        :3;  // out endppint interrupt
    } mBits ;
    volatile unsigned int dwValue ;
} USB_DAINT_U;

typedef union usb_daintmsk_tag
{
    struct usb_daintmsk_map
    {
        volatile unsigned int           inepintmsk          :13;  //in endpoint interrupt mask
        volatile unsigned int           reserved_1          :3;
        volatile unsigned int           outepintmsk         :13;  //out endpoint interrupt mak
        volatile unsigned int           reserved_2          :3;
    } mBits ;
    volatile unsigned int dwValue ;
} USB_DAINTMSK_U;

#if 0
typedef union usb_dtknqr1_tag
{
    struct usb_dtknqr1_map
    {
        volatile unsigned int            intknwptr         :5;  //in token queue write pointer
        volatile unsigned int            reserved          :2;  //
        volatile unsigned int            wrapbit           :1;  //wrap bit
        volatile unsigned int            eptkn             :24; //endpoint token
    } mBits ;
    volatile unsigned int dwValue ;
} USB_DTKNQR1_U;

typedef union usb_dtknqr2_tag
{
    struct usb_dtknqr2_map
    {
        volatile unsigned int           eptoken            :32;  // endpoint token
    } mBits ;
    volatile unsigned int dwValue ;
} USB_DTKNQR2_U;

typedef union usb_dtknqr3_tag
{
    struct usb_dtknqr3_map
    {
        volatile unsigned int           eptoken            :32;  // endpoint token
    } mBits ;
    volatile unsigned int dwValue ;
} USB_DTKNQR3_U;

typedef union usb_dtknqr4_tag
{
    struct usb_dtknqr4_map
    {
        volatile unsigned int           eptoken            :32;  // endpoint token
    } mBits ;
    volatile unsigned int dwValue ;
} USB_DTKNQR4_U;
#endif

typedef union usb_dvbusdis_tag
{
    struct usb_dvbusdis_map
    {
        volatile unsigned int           dvbusdis          :16;  //device vbus discharge time
        volatile unsigned int           reserved          :16;
    } mBits ;
    volatile unsigned int dwValue ;
} USB_DVBUSDIS_U;

typedef union usb_dvbuspulse_tag
{
    struct usb_dvbuspulse_map
    {
        volatile unsigned int           dvbuspt           :12;  //device vbus pulse time
        volatile unsigned int           reserved          :20;
    } mBits ;
    volatile unsigned int dwValue ;
} USB_DVBUSPULSE_U;

typedef union usb_dthrctl_tag
{
    struct usb_dthrctl_map
    {
        volatile unsigned int           nonisothren       :1;  //non iso in endpoint threshold enable
        volatile unsigned int           isothren          :1;  //iso in endpoint threshold enable
        volatile unsigned int           txthrlen          :9;  //transmit threshold length
        volatile unsigned int           ahbthrratio       :2;
        volatile unsigned int           reserved_1        :3;
        volatile unsigned int           rxthren           :1;  //receive threshold enable
        volatile unsigned int           rxthrlen          :9;  //receive threshold length
        volatile unsigned int           reserved_2        :1;
        volatile unsigned int           arbprken          :1;
        volatile unsigned int           reserved_3        :4;
    } mBits ;
    volatile unsigned int dwValue ;
} USB_DTHRCTL_U;

typedef union usb_diepint_tag
{
    struct usb_diepint_map
    {
        volatile unsigned int           transfer_com    :1;
        volatile unsigned int           epdis           :1;
        volatile unsigned int           ahb_error       :1;
        volatile unsigned int           timeout_condi   :1;
        volatile unsigned int           intokenfifoemp  :1;
        volatile unsigned int           intokenepmis    :1;
        volatile unsigned int           inepnakef       :1;
        volatile unsigned int           tfifoemp        :1;
        volatile unsigned int           fifo_underrun   :1;
        volatile unsigned int           bna             :1;
        volatile unsigned int           reserved_1      :1;
        volatile unsigned int           pktdrpsts       :1;
        volatile unsigned int           bbleerr         :1;
        volatile unsigned int           nak             :1;
        volatile unsigned int           nyet            :1;
        volatile unsigned int           reserved_2      :17;
    } mBits ;
    volatile unsigned int dwValue ;
} USB_DIEPINT_U;

typedef union usb_doepint_tag
{
    struct usb_doepint_map
    {
        volatile unsigned int           transfer_com    :1; // [0]
        volatile unsigned int           epdis           :1; // [1]
        volatile unsigned int           ahb_error       :1; // [2]
        volatile unsigned int           timeout_condi   :1; // [3] (for ep0 setup)
        volatile unsigned int           outtokenfifoemp :1; // [4]
        volatile unsigned int           stsphsercvd     :1; // [5]
        volatile unsigned int           back2backsetup  :1; // [6]
        volatile unsigned int           reserved_1      :1; // [7]
        volatile unsigned int           outpkterr       :1; // [8]
        volatile unsigned int           bna             :1; // [9]
        volatile unsigned int           reserved_2      :1;
        volatile unsigned int           pktdrpsts       :1; // [11]
        volatile unsigned int           bbleerr         :1; // [12]
        volatile unsigned int           nak             :1; // [13]
        volatile unsigned int           nyet            :1; // [14]
        volatile unsigned int           stuppktrcvd     :1; // [15]
        volatile unsigned int           reserved_3      :16;
    } mBits ;
    volatile unsigned int dwValue ;
} USB_DOEPINT_U;

typedef union usb_dieptsiz_tag
{
    struct usb_dieptsiz_map
    {
        volatile unsigned int           transfer_size   :19;
        volatile unsigned int           packet_count    :10;
        volatile unsigned int           mult_count      :2;
        volatile unsigned int           reserved        :1;
    } mBits ;
    volatile unsigned int dwValue ;
} USB_DIEPTSIZ_U;

typedef union usb_doeptsiz_tag
{
    struct usb_doeptsiz_map
    {
        volatile unsigned int           transfer_size   :19;
        volatile unsigned int           packet_count    :10;
        volatile unsigned int           mult_count      :2;
        volatile unsigned int           reserved        :1;
    } mBits;
    volatile unsigned int dwValue ;
} USB_DOEPTSIZ_U;

typedef union usb_doeptsiz0_tag
{
    struct usb_doeptsiz0_map
    {
        volatile unsigned int xfersize : 7; // [6:0]
        volatile unsigned int __18_7 : 12;
        volatile unsigned int pktcnt : 1; // [19]
        volatile unsigned int __28_20 : 9;
        volatile unsigned int supcnt : 2; // [30:29]
        volatile unsigned int __31_31 : 1;
    } mBits;
    volatile unsigned int dwValue;
} USB_DOEPTSIZ0_U;

typedef union usb_diepctl0_tag
{
    struct usb_diepctl0_map
    {
        volatile unsigned int mps : 2; // [1:0]
        volatile unsigned int __14_2 : 13;
        volatile unsigned int usbactep : 1; // [15], read only
        volatile unsigned int __16_16 : 1;
        volatile unsigned int naksts : 1; // [17], read only
        volatile unsigned int eptype : 2; // [19:18], read only
        volatile unsigned int __20_20 : 1;
        volatile unsigned int stall : 1;  // [21]
        volatile unsigned int txfnum : 4; // [25:22]
        volatile unsigned int cnak : 1;   // [26]
        volatile unsigned int snak : 1;   // [27]
        volatile unsigned int __29_28 : 2;
        volatile unsigned int epdis : 1; // [30]
        volatile unsigned int epena : 1; // [31]
    } mBits;
    volatile unsigned int dwValue;
} USB_DIEPCTL0_U;

typedef union usb_dieptsiz0_tag
{
    struct usb_dieptsiz0_map
    {
        volatile unsigned int xfersize : 7; // [6:0]
        volatile unsigned int __18_7 : 12;
        volatile unsigned int pktcnt : 2; // [20:19]
        volatile unsigned int __31_21 : 11;
    } mBits;
    volatile unsigned int dwValue;
} USB_DIEPTSIZ0_U;

typedef union usb_diepctl_tag
{
    struct usb_diepctl_map
    {
        volatile unsigned int       mps             :11;
        volatile unsigned int       reserved_1      :4;
        volatile unsigned int       active_ep       :1;
        volatile unsigned int       ep_data_pid     :1;
        volatile unsigned int       nak_status      :1;
        volatile unsigned int       ep_type         :2;
        volatile unsigned int       reserved_2      :1;
        volatile unsigned int       stall_handshak  :1;
        volatile unsigned int       tx_fifo_number  :4;
        volatile unsigned int       clear_nak       :1;
        volatile unsigned int       set_nak         :1;
        volatile unsigned int       set_data0_pid   :1;
        volatile unsigned int       set_data1_pid   :1;
        volatile unsigned int       ep_disable      :1;
        volatile unsigned int       ep_enable       :1;
    } mBits;
    volatile unsigned int dwValue ;
} USB_DIEPCTL_U;

typedef union usb_doepctl0_tag
{
    struct usb_doepctl0_map
    {
        volatile unsigned int mps : 2; // [1:0], read only
        volatile unsigned int __14_2 : 13;
        volatile unsigned int usbactep : 1; // [15], read only
        volatile unsigned int __16_16 : 1;
        volatile unsigned int naksts : 1; // [17], read only
        volatile unsigned int eptype : 2; // [19:18], read only
        volatile unsigned int snp : 1;    // [20]
        volatile unsigned int stall : 1;  // [21]
        volatile unsigned int __25_22 : 4;
        volatile unsigned int cnak : 1; // [26]
        volatile unsigned int snak : 1; // [27]
        volatile unsigned int __29_28 : 2;
        volatile unsigned int epdis : 1; // [30], read only
        volatile unsigned int epena : 1; // [31]
    } mBits;
    volatile unsigned int dwValue;
} USB_DOEPCTL0_U;

typedef union usb_doepctl_tag
{
    struct usb_doepctl_map
    {
        volatile unsigned int       mps             :11;
        volatile unsigned int       reserved_1      :4;
        volatile unsigned int       active_ep       :1;
        volatile unsigned int       ep_data_pid     :1;
        volatile unsigned int       nak_status      :1;
        volatile unsigned int       ep_type         :2;
        volatile unsigned int       snoop_mode      :1;
        volatile unsigned int       stall_handshak  :1;
        volatile unsigned int       reserved_2      :4;
        volatile unsigned int       clear_nak       :1;
        volatile unsigned int       set_nak         :1;
        volatile unsigned int       set_data0_pid   :1;
        volatile unsigned int       set_data1_pid   :1;
        volatile unsigned int       ep_disable      :1;
        volatile unsigned int       ep_enable       :1;
    } mBits;
    volatile unsigned int dwValue;
} USB_DOEPCTL_U;

typedef union usb_diep0tsiz_tag
{
    struct usb_diep0tsiz_map
    {
        volatile unsigned int       transfer_size  :7;
        volatile unsigned int       reserved_1     :12;
        volatile unsigned int       packet_count   :2;
        volatile unsigned int       reserved_2     :11;
    } mBits;
    volatile unsigned int dwValue ;
} USB_DIEP0TSIZ_U;

typedef union usb_doep0tsiz_tag
{
    struct usb_doep0tsiz_map
    {
        volatile unsigned int       transfer_size       :7;
        volatile unsigned int       reserved_1          :12;
        volatile unsigned int       packet_count        :1;
        volatile unsigned int       reserved_2          :9;
        volatile unsigned int       setup_packet_count  :2;
        volatile unsigned int       reserved_3          :1;
    } mBits;
    volatile unsigned int dwValue ;
} USB_DOEP0TSIZ_U;

typedef union usb_intsts_tag
{
    struct usb_intsts_map
    {
        volatile unsigned int curmod            :1; // [0]
        volatile unsigned int modemis           :1; // [1]
        volatile unsigned int otgint            :1; // [2]
        volatile unsigned int sof               :1; // [3]
        volatile unsigned int rxflvl            :1; // [4]
        volatile unsigned int nptxfemp          :1; // [5]
        volatile unsigned int ginnakeff         :1; // [6]
        volatile unsigned int goutnakeff        :1; // [7]
        volatile unsigned int reserved_1        :2;
        volatile unsigned int erlysusp          :1; // [10]
        volatile unsigned int usbsusp           :1; // [11]
        volatile unsigned int usbrst            :1; // [12]
        volatile unsigned int enumdone          :1; // [13]
        volatile unsigned int isooutdrop        :1; // [14]
        volatile unsigned int eopf              :1; // [15]
        volatile unsigned int reserved_2        :1;
        volatile unsigned int epmis             :1; // [17]
        volatile unsigned int iepint            :1; // [18]
        volatile unsigned int oepint            :1; // [19]
        volatile unsigned int incompisoin       :1; // [20]
        volatile unsigned int incompip          :1; // [21]
        volatile unsigned int fetsusp           :1; // [22]
        volatile unsigned int resetdet          :1; // [23]
        volatile unsigned int prtint            :1; // [24]
        volatile unsigned int hchint            :1; // [25]
        volatile unsigned int ptxfemp           :1; // [26]
        volatile unsigned int reserved_3        :1;
        volatile unsigned int conidstschng      :1; // [28]
        volatile unsigned int discoonint        :1; // [29]
        volatile unsigned int sessreqint        :1; // [30]
        volatile unsigned int wkupint           :1; // [31]
    } mBits ;
    volatile unsigned int dwValue ;
} USB_INTSTS_U;

typedef union usb_pcgcctl_tag
{
    struct usb_pcgcctl_map
    {
        volatile unsigned int   StopPclk     :1;  //global_interrupt_mask
        volatile unsigned int   reserved_1   :2;
        volatile unsigned int   RstPdwnModule :1;  //burst_length
        volatile unsigned int   reserved_2   :2;
        volatile unsigned int   PhySleep         :1;  //non_periodic_txfifo_empty_level
        volatile unsigned int   L1_Suspended     :1;  //periodic_txfifo_empty_level
        volatile unsigned int   Reserved     :24;
    } mBits ;
    volatile unsigned int dwValue ;
} USB_PCGCCTL_U;

/********************  USB2.0 Protocol Prototype  ****************/

typedef union usb_request_1_tag
{
    struct usb_request_1_map
    {
        volatile unsigned int       recipient     :5;
        volatile unsigned int       type          :2;
        volatile unsigned int       direction     :1;
        volatile unsigned int       brequest      :8;
        volatile unsigned int       value_l       :8;
        volatile unsigned int       value_m       :8;
    } mBits ;
    volatile unsigned int dwValue ;
} USB_REQUEST_1_U;

typedef union usb_request_2_tag
{
    struct usb_request_2_map
    {

        volatile unsigned int       index_l       :8;
        volatile unsigned int       index_m       :8;
        volatile unsigned int       length_l      :8;
        volatile unsigned int       length_m      :8;
    } mBits ;
    volatile unsigned int dwValue ;
} USB_REQUEST_2_U;

#define USB_EPx_NUMBER 12

#endif // _USB20_REG_UIX8909_HEADER_

