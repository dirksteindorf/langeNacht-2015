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


void onNewPose(mira::ChannelRead<mira::Pose2> data)
{
   mira::Pose2 pose = data->value();
   fenster.setRobotPose(pose.x(), -pose.y(), pose.phi());  
   std::cout<<pose.x() << " " << pose.y() << " " <<pose.phi()<<std::endl;
}

void updateGui(const Timer& timer)
{
    if(fenster.isOpen()) {
        fenster.update(true);
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

    fenster.addArea("oTToCAR", sf::IntRect(5, 20, 192, 404), counter, "res/oTToCAR_Logo.png", "res/ottocar_CeBIT2015_03.jpg", 4.4, 10.4, M_PI * 0.25, true);
    fenster.addArea("robOTTO", sf::IntRect(204, 128, 167, 244), counter, "res/logo-robotto.png", "res/robotto.png", 7.0, 10.3, M_PI * 0.75, true);
    fenster.addArea("Finken Projekt", sf::IntRect(5, 436, 98, 142), counter, "res/red_logo.png", "res/finken.jpg", 3.5, 15.4, M_PI * 0.5, true);
    fenster.addArea("AG CSE", sf::IntRect(271, 436, 98, 142), counter, "res/red_logo.png", "res/missing_fig_groß.png", 8.2, 15.5, M_PI * 0.5, true);

    Framework framework(argc, argv, true);

    authority.checkin("/", "guiAuthority");
    authority.createTimer(Duration::milliseconds(50), &updateGui);
    authority.subscribe<mira::Pose2>("/robot/RobotFrame", &onNewPose);

    goalChannel = authority.publish<mira::Pose2>("goalChannel");

    authority.start();

    return framework.run();
}
