#include "lndw_gui.hpp"

#include <fw/Framework.h>
#include <fw/ChannelReadWrite.h>
#include <transform/Pose.h>

using namespace mira;

Authority authority;

mira::Channel<Pose2> poseChannel;
mira::Channel<Pose2> goalChannel;

lndw::Gui fenster(sf::VideoMode(1024, 768));//, sf::Style::Fullscreen);


void onNewPose(mira::ChannelRead<mira::Pose2> data)
{
   mira::Pose2 pose = data->value();
   fenster.setRobotPose(pose.x(), pose.y(), pose.phi());  
}

void updateGui(const Timer& timer)
{
    if(fenster.isOpen()) {
        fenster.update();
    }
}


int main(int argc, char** argv)
{
    std::wstring counter = L"123456789 123456789 123456789 123456789 \n123456789 123456789 123456789 \n123456789 123456789 123456789 \n123456789 123456789 123456789 \n123456789 123456789 123456789 ";

    fenster.addArea("oTToCAR", sf::IntRect(-12, 65, 121, 380), counter, "res/oTToCAR_Logo.png", "res/ottocar_CeBIT2015_03.jpg");
    fenster.addArea("robOTTO", sf::IntRect(227, 174, 135, 407), counter, "res/logo-robotto.png", "res/robotto.png");
    fenster.addArea("Finken Projekt", sf::IntRect(37, 1, 205, 63), counter, "res/red_logo.png", "res/finken.jpg");
    fenster.addArea("AG CSE", sf::IntRect(11, 445, 137, 128), counter, "res/red_logo.png", "res/missing_fig_groß.png");


    Framework framework(argc, argv, true);

    authority.checkin("/", "guiAuthority");
    authority.createTimer(Duration::milliseconds(50), &updateGui);
    authority.subscribe<mira::Pose2>("/robot/Odometry", &onNewPose);

    authority.start();

    return framework.run();
}
