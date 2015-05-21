#include "lndw_gui.hpp"

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

void onNewPose(mira::ChannelRead<mira::Pose2> data)
{
   mira::Pose2 pose = authority.getTransform<mira::Pose2>( "/robot/RobotFrame", 
                                                            "/GlobalFrame",
                                                            data.getTimestamp());
   fenster.setRobotPose(pose.x(), -pose.y(), pose.phi());  
   std::cout<<pose.x() << " " << pose.y() << " " <<pose.phi()<<std::endl;
}

void updateGui(const Timer& timer)
{
    bool drawTargetArrowAndBorder = true;
    bool robotFollowsMouse = true;
    
    if(fenster.isOpen()) {
        fenster.update(drawTargetArrowAndBorder, robotFollowsMouse, debugMsg);
    }
}

void publishPose()
{
    goalChannel.post(mira::Pose2(   fenster.target.x, 
                                    -fenster.target.y, 
                                    fenster.target.theta));
}


int main(int argc, char** argv)
{
    std::wstring counter = L"123456789 123456789 123456789 123456789 \n123456789 123456789 123456789 \n123456789 123456789 123456789 \n123456789 123456789 123456789 \n123456789 123456789 123456789 ";
    //main-image: ca. 596 x 400 px

    fenster.addArea("oTToCAR", sf::IntRect(200, 15, 170, 370), counter, "res/oTToCAR_Logo.png", "res/ottocar_CeBIT2015_03.jpg", 8.0, 10.4, M_PI * 0.6, "Willkommen bei oTToCAR", debugMsg);
    fenster.addArea("robOTTO", sf::IntRect(10, 255, 120, 320), counter, "res/logo-robotto.png", "res/robotto.png", 4.4, 13.0, M_PI * 0.55, "Willkommen bei robOTTO", debugMsg);
    fenster.addArea("Finken Projekt", sf::IntRect(10, 130, 100, 124), counter, "res/finken-logo.png", "res/finken.png", 6.0, 15.1, M_PI * 0.75, "Willkommen beim Finken Projekt", debugMsg);   //demo_pose im Gang; org_pose: 3.6, 5.4, M_PI * 0.55
    fenster.addArea("AG CSE", sf::IntRect(250, 436, 120, 142), counter, "res/red_logo.png", "res/missing_fig_groß.png", 7.7, 14.7, M_PI * 0.45, "Willkommen bei der Arbeitsgruppe CSE", debugMsg);

    Framework framework(argc, argv, true);

    authority.checkin("/", "guiAuthority");
    authority.createTimer(Duration::milliseconds(50), &updateGui);
    authority.subscribe<mira::Pose2>("/robot/RobotFrame", &onNewPose);

    goalChannel = authority.publish<mira::Pose2>("goalChannel");

    authority.start();

    return framework.run();
}
