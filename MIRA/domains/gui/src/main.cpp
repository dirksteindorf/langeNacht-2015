#include "lndw_gui.hpp"
#include "arduino-serial-lib.h"

#include <unistd.h>   // for usleep()
#include <getopt.h>

#include <fw/Framework.h>
#include <fw/ChannelReadWrite.h>
#include <transform/Pose.h>

#include <iostream>


using namespace mira;

Authority authority;

mira::Channel<Pose2> poseChannel;
mira::Channel<Pose2> goalChannel;

lndw::Gui fenster(sf::VideoMode(1024, 768), sf::VideoMode(1024, 768));

bool debugMsg = false;

// arduino
int fd = -1;
const int buf_max = 256;
char buf[buf_max];
int timeout = 500;
char eolchar= '\n';


void onNewPose(mira::ChannelRead<mira::Pose2> data)
{
   mira::Pose2 pose = authority.getTransform<mira::Pose2>( "/robot/RobotFrame", 
                                                            "/GlobalFrame",
                                                            data.getTimestamp());
   fenster.setRobotPose(pose.x(), -pose.y(), pose.phi());  
   std::cout<<pose.x() << " " << pose.y() << " " <<pose.phi()<<std::endl;
}

void onNewPilotEvent(mira::ChannelRead<std::string> data)
{
    if(data->value() == "GoalReached") {
        fenster.setCurrentTargetReached();
    }
}

void updateGui(const Timer& timer)
{
    bool drawTargetArrowAndBorder = true;
    bool robotFollowsMouse = true;
    
    if(fenster.isOpen()) {
        fenster.update(drawTargetArrowAndBorder, robotFollowsMouse, debugMsg);
    }
}

void readArduino(const Timer& timer)
{
    if(fd == -1)
        std::cout << "serial port not opened" << std::endl;
    
    memset(buf, 0, buf_max);
    serialport_read_until(fd, buf, eolchar, buf_max, timeout);
    int i = atoi(buf);

    if (i!=0) {
        if(i > 220)
            fenster.setPersonPresent(true);
        else
            fenster.setPersonPresent(false);
            
         //std::cout<<i<<std::endl;
     }
}

void publishPose()
{
    goalChannel.post(mira::Pose2(   fenster.target.x, 
                                    -fenster.target.y, 
                                    fenster.target.theta));
}


int main(int argc, char** argv)
{//main-image: 597 x 400 px
    std::wstring ottocar_text = L"Das Studententeam entwickelt ein\nautonom fahrendes Modellauto,\ndass selbstständig fährt,\nüberholt und einparkt.\nBesuchen Sie uns!";
    fenster.addArea("oTToCAR", sf::IntRect(200, 9, 170, 375), ottocar_text, "res/oTToCAR_Logo.png", "res/ottocar_CeBIT2015_03.jpg", 8.0, 10.4, M_PI * 0.6, "res/ottocar.ogg", debugMsg);
    
    std::wstring robotto_text = L"Positionieren, Greifen und\nAblegen ist für einen Roboter\neine Herausforderung - die wir\nmeistern. Überzeugen Sie sich\nan unserem Stand!";
    fenster.addArea("robOTTO", sf::IntRect(10, 255, 120, 320), robotto_text, "res/logo-robotto.png", "res/robotto.png", 4.4, 13.0, M_PI * 0.55, "res/robotto.ogg", debugMsg);
    
    std::wstring finken_text = L"Schwarmrobotik mit Quadkoptern\nDie \"FINken\" sind autonome\nEinheiten die mit einander und\nihrer Umgebung interagieren.\nBesuchen Sie sie!";
    fenster.addArea("SwarmLab", sf::IntRect(10, 130, 100, 124), finken_text, "res/finken-logo.png", "res/finken.png", 6.0, 15.1, M_PI * 0.75, "res/finken.ogg", debugMsg);   //demo_pose im Gang; org_pose: 3.6, 5.4, M_PI * 0.55

    std::wstring cse_text = L"123456789 123456789 123456789 123456789 \n123456789 123456789 123456789 \n123456789 123456789 123456789 \n123456789 123456789 123456789 \n123456789 123456789 123456789 ";
    fenster.addArea("AG CSE", sf::IntRect(250, 430, 120, 142), cse_text, "res/red_logo.png", "res/missing_fig_groß.png", 7.7, 14.7, M_PI * 0.45, "res/cse.ogg", debugMsg);
    
    
    // arduino stuff
    int baudrate = 9600;
    char *portname = "/dev/ttyACM0";
    fd = serialport_init(portname, baudrate);
    
    if( fd==-1 ) 
        std::cout << "couldn't open port" << std::endl;
    
    // MIRA stuff
    Framework framework(argc, argv, true);

    authority.checkin("/", "guiAuthority");
    authority.createTimer(Duration::milliseconds(50), &updateGui);
    authority.createTimer(Duration::milliseconds(20), &readArduino);
    authority.subscribe<mira::Pose2>("/robot/RobotFrame", &onNewPose);
    authority.subscribe<std::string>("/navigation/PilotEvent", &onNewPilotEvent);

    goalChannel = authority.publish<mira::Pose2>("goalChannel");

    authority.start();

    return framework.run();
}
