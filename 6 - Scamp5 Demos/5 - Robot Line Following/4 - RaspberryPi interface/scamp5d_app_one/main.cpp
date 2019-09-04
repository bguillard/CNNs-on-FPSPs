
#include <unistd.h>
#include <cstdlib>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include "scamp5d_usb.h"
#include "vs_packet_decoder.hpp"
#include "debug_functions.h"


scamp5d_interface*box;
vs_packet_decoder*packet_switch;

std::atomic<bool> quit(false);

int packet_count, ones_count, twos_count, threes_count;

void setup_packet_switch(){
    // text are from the 'scamp5_send text' function
    // the messages are displayed, each on a new line
    packet_switch->case_text([&](const char*text,size_t length){
        switch(text[0]){
            case '1':
                if(packet_count > 30){
                    ones_count ++;
                }
                packet_count ++;
                break;
            case '2':
                if(packet_count > 30){
                    twos_count ++;
                }
                packet_count ++;
                break;
            case '3':
                if(packet_count > 30){
                    threes_count ++;
                }
                packet_count ++;
                break;
            default:
                std::cout << text << std::endl << std::flush;
                break;
        }

        if(packet_count >= 36){
            if(ones_count >= twos_count && ones_count >= threes_count){
                std::cout << '1' << std::endl << std::flush;
            } else if(twos_count >= threes_count){
                std::cout << '2' << std::endl << std::flush;
            } else {
                std::cout << '3' << std::endl << std::flush;
            }
            packet_count=0; ones_count=0; twos_count=0; threes_count=0;
        }
    });
}


void input_loop(){

    std::cout << "<press Q to quit>" << std::endl;

    while(quit==false){
        char c = conio_getch();
        switch(c){

        case 'q':
            quit = true;
            std::cout << "quit" << std::endl;
            break;

        case 's':// for diagnostic purpose
            std::cout << "signature_counter: " << box->get_signature_counter() <<
                ", packet_counter: " << box->get_packet_counter() << std::endl;
            break;

        }
    }
}


int main(int argc,char*argv[]){
    int r;

    packet_count=0; ones_count=0; twos_count=0; threes_count=0;

    std::cout << "Scamp5d Application Example" << std::endl;

    // a thread to handle console keyboard input
    std::thread input_thread(input_loop);

    // 'scamp5d_packet_switch' the object to decode the incoming packets and pass them
    // to the corresponding handler function registered.
    packet_switch = new vs_packet_decoder;

    setup_packet_switch();

    /// Initialize the device interface
    box = new scamp5d_usb();
    r = box->open("",0);

    if(r){
        std::cout << "<Error> failed to open device!" << std::endl;
        exit(-1);
    }else{
        std::cout << "<Device Ready>\n------------------------------------------------" << std::endl;
    }

    /// Setup how to handle incoming packets
    box->on_receive_packet([&](const uint8_t*packet,size_t packet_size){
        packet_switch->decode_packet(packet,packet_size,box->get_packet_counter());
    });

    /// Main loop
    while(quit==false){
        box->routine();
    }

    box->close();
    delete box;

    delete packet_switch;

    input_thread.join();

    return 0;
}
