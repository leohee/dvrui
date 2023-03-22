/*----------------------------------------------------------------------------
 Defines referenced header files
-----------------------------------------------------------------------------*/
#include "dvr_defines.h"
#include "dvr_setup.h"
#include "ui_dvr_setup.h"
#include "dvr_main.h"
/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
extern dvrmain *pMainClass;
/*----------------------------------------------------------------------------
 local functions
-----------------------------------------------------------------------------*/
/*!network tab의 control들을 초기화 */
void dvr_setup::InitSetupNetworkControls()
{
    connect(ui->networktype1comboBox, SIGNAL(currentIndexChanged (int)), this, SLOT(ChangeNetworkType1SelectSlot(int)));
    connect(ui->networkipaddress1virtualkeypushButton, SIGNAL(clicked()), this, SLOT(NetworkIpaddress1VirtualKeyboardBtnSlot()));
    connect(ui->networksubnetmask1virtualkeypushButton, SIGNAL(clicked()), this, SLOT(NetworkSubnetmask1VirtualKeyboardBtnSlot()));
    connect(ui->networkgateway1virtualkeypushButton, SIGNAL(clicked()), this, SLOT(NetworkGateway1VirtualKeyboardBtnSlot()));

    connect(ui->networktype2comboBox, SIGNAL(currentIndexChanged (int)), this, SLOT(ChangeNetworkType2SelectSlot(int)));
    connect(ui->networkipaddress2virtualkeypushButton, SIGNAL(clicked()), this, SLOT(NetworkIpaddress2VirtualKeyboardBtnSlot()));
    connect(ui->networksubnetmask2virtualkeypushButton, SIGNAL(clicked()), this, SLOT(NetworkSubnetmask2VirtualKeyboardBtnSlot()));
    connect(ui->networkgateway2virtualkeypushButton, SIGNAL(clicked()), this, SLOT(NetworkGateway2VirtualKeyboardBtnSlot()));
}
/*!profile data struct에 따라서 ui에 value setting */
void dvr_setup::SetNetworkdata()
{
    ui->networktype1comboBox->setCurrentIndex(dlgdata.net_info1.type);
    ui->networkipaddress1lineEdit->setText(QString::fromUtf8(dlgdata.net_info1.ipaddr));
    ui->networksubnetmask1lineEdit->setText(QString::fromUtf8(dlgdata.net_info1.netmask));
    ui->networkgateway1lineEdit->setText(QString::fromUtf8(dlgdata.net_info1.gateway));

    ui->networktype2comboBox->setCurrentIndex(dlgdata.net_info2.type);
    ui->networkipaddress2lineEdit->setText(QString::fromUtf8(dlgdata.net_info2.ipaddr));
    ui->networksubnetmask2lineEdit->setText(QString::fromUtf8(dlgdata.net_info2.netmask));
    ui->networkgateway2lineEdit->setText(QString::fromUtf8(dlgdata.net_info2.gateway));
}
/*!ui control의 value를 읽고 해당 기능을 적용시킴 */
void dvr_setup::GetNetworkdata(void)
{
    dvr_net_info_t inet;

    read_ui_network_data(NETWORK_DEV1_NO, &inet);
    //nfs가 아닌경우만 eth0 network setting
    //eth0 network setting변경시 RTSP stop/start 시킴
    if(g_rfs_type!=RFS_NFS)
    {
        if(inet.type != profile.net_info1.type) {
            profile.net_info1.type = inet.type;
            if(inet.type == NETWORK_TYPE_DHCP) {
#if DVR_API_ENABLE
                LIB816x_stopRTSP();
#endif
                set_network_info(NETWORK_DEV1_NO);
                //RTSP Start
#if DVR_API_ENABLE
                LIB816x_startRTSP();
#endif
            }
        }

        if(inet.type == NETWORK_TYPE_STATIC)
        {
            if((strcmp(inet.ip, profile.net_info1.ipaddr)!=0 || strcmp(inet.mask, profile.net_info1.netmask)!=0
                            || strcmp(inet.gate, profile.net_info1.gateway)!=0) && strcmp(inet.ip, NETWORK_IPADDRESS_DEFAULT1) )
            {
                strcpy(profile.net_info1.ipaddr, inet.ip);
                strcpy(profile.net_info1.netmask, inet.mask);
                strcpy(profile.net_info1.gateway, inet.gate);
#if DVR_API_ENABLE
                LIB816x_stopRTSP();
#endif
                set_network_info(NETWORK_DEV1_NO);
                //RTSP Start
#if DVR_API_ENABLE
                LIB816x_startRTSP();
#endif
            }
        }
    }

    read_ui_network_data(NETWORK_DEV2_NO, &inet);
    if(inet.type != profile.net_info2.type) {
        profile.net_info2.type = inet.type;
        if(inet.type == NETWORK_TYPE_DHCP) {
            set_network_info(NETWORK_DEV2_NO);
        }
    }

    if(inet.type == NETWORK_TYPE_STATIC)
    {
        if((strcmp(inet.ip, profile.net_info2.ipaddr)!=0 || strcmp(inet.mask, profile.net_info2.netmask)!=0
                        || strcmp(inet.gate, profile.net_info2.gateway)!=0) && strcmp(inet.ip, NETWORK_IPADDRESS_DEFAULT2) )
        {
            strcpy(profile.net_info2.ipaddr, inet.ip);
            strcpy(profile.net_info2.netmask, inet.mask);
            strcpy(profile.net_info2.gateway, inet.gate);
            set_network_info(NETWORK_DEV2_NO);
        }
    }
}
/*!cancel시 network관련 변경적용된 내역을 복원시킴 */
void dvr_setup::ResotreNetwork()
{
	//nfs가 아닌경우만 eth0 network setting
    //eth0 network setting변경시 RTSP stop/start 시킴
    if(g_rfs_type!=RFS_NFS)
    {
        if(dlgdata.net_info1.type != profile.net_info1.type) {
            profile.net_info1.type = dlgdata.net_info1.type;
            if(profile.net_info1.type == NETWORK_TYPE_DHCP) {
#if DVR_API_ENABLE
                LIB816x_stopRTSP();
#endif
                set_network_info(NETWORK_DEV1_NO);
                //RTSP Start
#if DVR_API_ENABLE
                LIB816x_startRTSP();
#endif
            }
        }

        if(profile.net_info1.type == NETWORK_TYPE_STATIC)
        {
            if((strcmp(dlgdata.net_info1.ipaddr, profile.net_info1.ipaddr)!=0 || strcmp(dlgdata.net_info1.netmask, profile.net_info1.netmask)!=0
                            || strcmp(dlgdata.net_info1.gateway, profile.net_info1.gateway)!=0) && strcmp(dlgdata.net_info1.ipaddr, NETWORK_IPADDRESS_DEFAULT1) )
            {
                strcpy(profile.net_info1.ipaddr, dlgdata.net_info1.ipaddr);
                strcpy(profile.net_info1.netmask, dlgdata.net_info1.netmask);
                strcpy(profile.net_info1.gateway, dlgdata.net_info1.gateway);
#if DVR_API_ENABLE
                LIB816x_stopRTSP();
#endif
                set_network_info(NETWORK_DEV1_NO);
                //RTSP Start
#if DVR_API_ENABLE
                LIB816x_startRTSP();
#endif
            }
        }
    }

    if(dlgdata.net_info2.type != profile.net_info2.type) {
        profile.net_info2.type = dlgdata.net_info2.type;
        if(profile.net_info2.type == NETWORK_TYPE_DHCP) {
            set_network_info(NETWORK_DEV2_NO);
        }
    }

    if(profile.net_info2.type == NETWORK_TYPE_STATIC)
    {
        if((strcmp(dlgdata.net_info2.ipaddr, profile.net_info2.ipaddr)!=0 || strcmp(dlgdata.net_info2.netmask, profile.net_info2.netmask)!=0
                        || strcmp(dlgdata.net_info2.gateway, profile.net_info2.gateway)!=0) && strcmp(dlgdata.net_info2.ipaddr, NETWORK_IPADDRESS_DEFAULT2) )
        {
            strcpy(profile.net_info2.ipaddr, dlgdata.net_info2.ipaddr);
            strcpy(profile.net_info2.netmask, dlgdata.net_info2.netmask);
            strcpy(profile.net_info2.gateway, dlgdata.net_info2.gateway);
            set_network_info(NETWORK_DEV2_NO);
        }
    }
}
/*!eth0/eth1에대한 network setting 수행 */
int dvr_setup::set_network_info(int devno)
{
    int ret=0;
#if DVR_API_ENABLE

    dvr_net_info_t inet;
    if(devno==NETWORK_DEV1_NO) {
        inet.type = profile.net_info1.type;
        if(inet.type == NETWORK_TYPE_STATIC) {
            strcpy(inet.ip, profile.net_info1.ipaddr);
            strcpy(inet.mask, profile.net_info1.netmask);
            strcpy(inet.gate, profile.net_info1.gateway);
        }
        ret = LIB816x_net_info(1, devno, (void *)&inet);
    }
    else if(devno==NETWORK_DEV2_NO) {
        inet.type = profile.net_info2.type;
        if(inet.type == NETWORK_TYPE_STATIC) {
            strcpy(inet.ip, profile.net_info2.ipaddr);
            strcpy(inet.mask, profile.net_info2.netmask);
            strcpy(inet.gate, profile.net_info2.gateway);
        }
        ret = LIB816x_net_info(1, devno, (void *)&inet);
    }
#endif
    get_network_info();

    return ret;
}
/*!eth0 eth1의 network 정보를 read */
int dvr_setup::get_network_info(void)
{
#if DVR_API_ENABLE
    dvr_net_info_t inet;

    LIB816x_net_info(0, NETWORK_DEV1_NO, (void *)&inet);

    profile.net_info1.enable = inet.state;
    strcpy(profile.net_info1.ipaddr, inet.ip);
    strcpy(profile.net_info1.netmask, inet.mask);
    strcpy(profile.net_info1.gateway, inet.gate);

    LIB816x_net_info(0, NETWORK_DEV2_NO, (void *)&inet);

    profile.net_info2.enable = inet.state;
    strcpy(profile.net_info2.ipaddr, inet.ip);
    strcpy(profile.net_info2.netmask, inet.mask);
    strcpy(profile.net_info2.gateway, inet.gate);
#else
    profile.net_info1.enable = 1;
    strcpy(profile.net_info1.ipaddr, "192.168.1.200");
    strcpy(profile.net_info1.netmask, NETWORK_SUBNETMASK_DEFAULT);
    strcpy(profile.net_info1.gateway, NETWORK_GATEWAY_DEFAULT);
        profile.net_info2.enable = 0;
    strcpy(profile.net_info2.ipaddr, "192.168.1.201");
    strcpy(profile.net_info2.netmask, NETWORK_SUBNETMASK_DEFAULT);
    strcpy(profile.net_info2.gateway, NETWORK_GATEWAY_DEFAULT);
#endif

    return 0;
}
/*!ui에 setting된 network data를 읽음 */
void dvr_setup::read_ui_network_data(int devno, dvr_net_info_t *inet)
{
    if(devno == NETWORK_DEV1_NO) {
        inet->type = ui->networktype1comboBox->currentIndex();
        strcpy(inet->ip, QStringToStr(ui->networkipaddress1lineEdit->text()));
        strcpy(inet->mask, QStringToStr(ui->networksubnetmask1lineEdit->text()));
        strcpy(inet->gate, QStringToStr(ui->networkgateway1lineEdit->text()));
    }
    else if(devno == NETWORK_DEV2_NO) {
            inet->type = ui->networktype2comboBox->currentIndex();
        strcpy(inet->ip, QStringToStr(ui->networkipaddress2lineEdit->text()));
        strcpy(inet->mask, QStringToStr(ui->networksubnetmask2lineEdit->text()));
        strcpy(inet->gate, QStringToStr(ui->networkgateway2lineEdit->text()));
    }
}
/*!eth0의 ip address virtual keyboad icon click하면 호출 */
void dvr_setup::NetworkIpaddress1VirtualKeyboardBtnSlot()
{
    if(pMainClass->ShowVirtualKeyboard())
        ui->networkipaddress1lineEdit->setFocus();
}
/*!eth0의 subnetmask virtual keyboard icon click하면 호출 */
void dvr_setup::NetworkSubnetmask1VirtualKeyboardBtnSlot()
{
    if(pMainClass->ShowVirtualKeyboard())
        ui->networksubnetmask1lineEdit->setFocus();
}
/*!eth0 gateway virtual keyboard icon click하면 호출 */
void dvr_setup::NetworkGateway1VirtualKeyboardBtnSlot()
{
    if(pMainClass->ShowVirtualKeyboard())
        ui->networkgateway1lineEdit->setFocus();
}
/*!eth1 ip address virtual keyboard icon click하면 호출 */
void dvr_setup::NetworkIpaddress2VirtualKeyboardBtnSlot()
{
    if(pMainClass->ShowVirtualKeyboard())
        ui->networkipaddress2lineEdit->setFocus();
}
/*!eth1 subnet mask virtual keyboard icon click하면 호출 */
void dvr_setup::NetworkSubnetmask2VirtualKeyboardBtnSlot()
{
    if(pMainClass->ShowVirtualKeyboard())
        ui->networksubnetmask2lineEdit->setFocus();
}
/*!eth1 gateway virtual keyboard icon click하면 호출 */
void dvr_setup::NetworkGateway2VirtualKeyboardBtnSlot()
{
    if(pMainClass->ShowVirtualKeyboard())
        ui->networkgateway2lineEdit->setFocus();
}
/*!eth0 network type combo box 값이 변경되면 호출 */
void dvr_setup::ChangeNetworkType1SelectSlot(int index)
{
	//eth0는 DHCP 혹은 file system이 nfs인 경우 disable
    if(index==NETWORK_TYPE_DHCP || g_rfs_type==RFS_NFS)
    {
        ui->networkipaddress1lineEdit->setEnabled(false);
        ui->networkipaddress1virtualkeypushButton->setEnabled(false);
        ui->networksubnetmask1lineEdit->setEnabled(false);
        ui->networksubnetmask1virtualkeypushButton->setEnabled(false);
        ui->networkgateway1lineEdit->setEnabled(false);
        ui->networkgateway1virtualkeypushButton->setEnabled(false);
    }
    else if(index==NETWORK_TYPE_STATIC)
    {
        ui->networkipaddress1lineEdit->setEnabled(true);
        ui->networkipaddress1virtualkeypushButton->setEnabled(true);
        ui->networksubnetmask1lineEdit->setEnabled(true);
        ui->networksubnetmask1virtualkeypushButton->setEnabled(true);
        ui->networkgateway1lineEdit->setEnabled(true);
        ui->networkgateway1virtualkeypushButton->setEnabled(true);
    }
}
/*!eth1 network type combo box 값이 변경되면 호출 */
void dvr_setup::ChangeNetworkType2SelectSlot(int index)
{
	//eth1은 DHCP인 경우 disable
    if(index==NETWORK_TYPE_DHCP)
    {
        ui->networkipaddress2lineEdit->setEnabled(false);
        ui->networkipaddress2virtualkeypushButton->setEnabled(false);
        ui->networksubnetmask2lineEdit->setEnabled(false);
        ui->networksubnetmask2virtualkeypushButton->setEnabled(false);
        ui->networkgateway2lineEdit->setEnabled(false);
        ui->networkgateway2virtualkeypushButton->setEnabled(false);
    }
    else if(index==NETWORK_TYPE_STATIC)
    {
         ui->networkipaddress2lineEdit->setEnabled(true);
         ui->networkipaddress2virtualkeypushButton->setEnabled(true);
         ui->networksubnetmask2lineEdit->setEnabled(true);
         ui->networksubnetmask2virtualkeypushButton->setEnabled(true);
         ui->networkgateway2lineEdit->setEnabled(true);
         ui->networkgateway2virtualkeypushButton->setEnabled(true);
    }
}
